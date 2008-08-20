/************************************************************************/
/* Copyright (C) 2008  Sam Danielson                                    */
/*                                                                      */
/* This file is part of Sqlpilot.                                       */
/*                                                                      */
/* Sqlpilot is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* Sqlpilot is distributed in the hope that it will be useful,          */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    */
/************************************************************************/

#include "lib/csv.h"
#include "db/db.h"
#include "sqlpilot.h"
#include "logbook.h"
#include "flights.h"
#include "aircraft.h"
#include "airports.h"
#include "util.h"

#define CSV_BUFSIZE 4096
#define CSV_COLS 128

typedef enum {
  INCSV_TIMEBASE_LOCAL,
  INCSV_TIMEBASE_UTC,
  INCSV_TIMEBASE_AIRPORT,
  INCSV_TIMEBASE_UTCOFFSET
} InCSVTimebase;

char csv_buf[CSV_BUFSIZE];
char *csv_row[CSV_COLS];

DBStatement *flights_ins;
DBStatement *aircraft_ins;
DBStatement *airport_tz_by_iata;
DBStatement *airport_tz_by_icao;

typedef struct InCSV InCSV;
struct InCSV {
  FILE *fh;
  int date,
    leg,
    fltno,
    aircraft,
    type,
    dep,
    arr,
    aout,
    ain,
    sout,
    sin,
    dur,
    night,
    inst,
    apprch,
    xc,
    role,
    dland,
    nland,
    crew,
    notes,
    trip,
    tripdate,
    numcol;
  int sep;
  int timebase;
};


static int incsv_init(InCSV *csv, const char *filename)
{
  int i = 0;
  csv->fh = stdin;

  csv->sep = ',';
  csv->timebase = INCSV_TIMEBASE_AIRPORT;
  csv->date = i++;
  csv->leg = i++;
  csv->fltno = i++;
  csv->aircraft = i++;
  csv->type = i++;
  csv->dep = i++;
  csv->arr = i++;
  csv->aout = i++;
  csv->ain = i++;
  csv->sout = i++;
  csv->sin = i++;
  csv->dur = i++;
  csv->night = i++;
  csv->inst = i++;
  csv->apprch = i++;
  csv->xc = i++;
  csv->role = i++;
  csv->dland = i++;
  csv->nland = i++;
  csv->crew = i++;
  csv->notes = i++;
  csv->trip = i++;
  csv->tripdate = i++;
  csv->numcol = i;
  return 0;
}

static void incsv_finalize(InCSV *incsv)
{
  fclose(incsv->fh);
}

time_t incsv_mktime(DB *db, const char *airport, InCSVTimebase timebase, struct tm *tm)
{
  time_t t;
  char tz[BUF_TZ] = "\0";

  switch (timebase) {
  default:
    fprintf(stderr, "timebase not set\n");
    exit(1);
    break;
  case INCSV_TIMEBASE_LOCAL:
    t = mktime(tm);
    break;
  case INCSV_TIMEBASE_UTC:
    t = tmtz_mktime(tm, "UTC");
    break;
  case INCSV_TIMEBASE_AIRPORT:
    if (tz_of_airport(airport_tz_by_iata, airport, tz, BUF_TZ) && strlen(tz)) {
      t = tmtz_mktime(tm, tz);
    } else {
      fprintf(stderr, "Warning: No timezone for airport %s. Assuming machine local time.\n", airport);
      t = mktime(tm);
    }
    break;
  }
  return t;
}

void incsv_import(InCSV *incsv, DB *db)
{
  char
    date[BUF_DATE],
    sout[BUF_TIME],
    sin[BUF_TIME],
    sdur[BUF_TIME],
    aout[BUF_TIME], 
    ain[BUF_TIME],
    dur[BUF_TIME],
    night[BUF_TIME],
    inst[BUF_TIME],
    soututc[BUF_TIME],
    sinutc[BUF_TIME],
    aoututc[BUF_TIME],
    ainutc[BUF_TIME],
    tz[BUF_TZ];
  struct tm tm;
  time_t
    sout_t,
    sin_t,
    aout_t,
    ain_t;
  float fnight, finst;
  int dland, nland, leg;
  char *ptr;
  int nrow, ncol;

  nrow = 0;
  while (csv_row_fread(incsv->fh,
		       (unsigned char *)csv_buf,
		       CSV_BUFSIZE,
		       (unsigned char **)csv_row,
		       CSV_COLS,
		       incsv->sep,
		       CSV_QUOTES) > 0) {

    /* CSV reading and checking */
    if (nrow == 0) { nrow++; continue; }	/* Skip CSV header */

    for (ncol=0; ncol<incsv->numcol; ncol++) {
      if (!csv_row[ncol]) {
	csv_row[ncol] = "\0";
	fprintf(stderr, "Warning: missing column %d in row %d\n", ncol+1, nrow);
      }
    }

    *date = *sout = *soututc = *sin = *sinutc = *sdur = *aout = *aoututc = *ain = *ainutc = *dur = *night = *inst = '\0';
    fnight = finst = 0;
    dland = nland = 0;
    leg = 0;
    aout_t = ain_t = sout_t = sin_t = -1;

    /* Copy and reformat certain fields to internal formats */
    strncpy(date, csv_row[incsv->date], sizeof(date));
    for (ptr = date; ptr < (date + sizeof(date)); ptr++) {
      if ('/' == *ptr) { *ptr = '-'; };
    }
    date[sizeof(date) - 1] = '\0';

    sscanf(csv_row[incsv->leg], "%d", &leg);

    /* Compute in-out times and departure dates */
    if (strlen(csv_row[incsv->aout])) {
      format_time(csv_row[incsv->aout], aout, ':');
      tm_read_strdate(&tm, date);
      tm_read_strtime(&tm, aout);
      aout_t = incsv_mktime(db, csv_row[incsv->dep], incsv->timebase, &tm);
      if (tz_of_airport(airport_tz_by_iata, csv_row[incsv->dep], tz, BUF_TZ)) {
	localtime_tz(&aout_t, tz, &tm);
      }
      strftime(aout, BUF_TIME, "%H:%M", &tm);
      gmtime_r(&aout_t, &tm);
      strftime(aoututc, BUF_TIME, "%H:%M", &tm);
    }
    if (strlen(csv_row[incsv->ain])) {
      format_time(csv_row[incsv->ain],  ain,  ':');
      tm_read_strdate(&tm, date);
      tm_read_strtime(&tm, ain);
      ain_t = incsv_mktime(db, csv_row[incsv->arr], incsv->timebase, &tm);
      if (tz_of_airport(airport_tz_by_iata, csv_row[incsv->arr], tz, BUF_TZ)) {
	localtime_tz(&ain_t, tz, &tm);
      }
      strftime(ain, BUF_TIME, "%H:%M", &tm);
      gmtime_r(&ain_t, &tm);
      strftime(ainutc, BUF_TIME, "%H:%M", &tm);
    }
    if (strlen(csv_row[incsv->sout])) {
      format_time(csv_row[incsv->sout], sout, ':');
      tm_read_strdate(&tm, date);
      tm_read_strtime(&tm, sout);
      sout_t = incsv_mktime(db, csv_row[incsv->dep], incsv->timebase, &tm);
      if (tz_of_airport(airport_tz_by_iata, csv_row[incsv->dep], tz, BUF_TZ)) {
	localtime_tz(&sout_t, tz, &tm);
      }
      strftime(sout, BUF_TIME, "%H:%M", &tm);
      gmtime_r(&sout_t, &tm);
      strftime(soututc, BUF_TIME, "%H:%M", &tm);
    }
    if (strlen(csv_row[incsv->sin])) {
      format_time(csv_row[incsv->sin],  sin, ':');
      tm_read_strdate(&tm, date);
      tm_read_strtime(&tm, sin);
      sin_t = incsv_mktime(db, csv_row[incsv->arr], incsv->timebase, &tm);
      if (tz_of_airport(airport_tz_by_iata, csv_row[incsv->arr], tz, BUF_TZ)) {
	localtime_tz(&sin_t, tz, &tm);
      }
      strftime(sin, BUF_TIME, "%H:%M", &tm);
      gmtime_r(&sin_t, &tm);
      strftime(sinutc, BUF_TIME, "%H:%M", &tm);
    }

    /* Compute actual and scheduled durations */
    /* We use the same date for in and out, later wraping to 24 hours */
    /* This scheme wont work if DST rules cause an airport time to map to */
    /* a different UTC for departure and arrival */
    if (aout_t != -1 && ain_t != -1) {
      m_to_strtime(daywrap_minutes((ain_t - aout_t) / 60), dur, BUF_TIME, '+');
    }
    if (sout_t != -1 && sin_t != -1) {
      m_to_strtime(daywrap_minutes((sin_t - sout_t) / 60), sdur, BUF_TIME, '+');
    }

    /* Compute night */
    /* Round times close to whole duration to equal duration e.g. 1.3 will become 1+20 if dur is 1+20 */
    sscanf(csv_row[incsv->night], "%f", &fnight);
    fnight *= 60;
    if (fnight >= strtime_to_m(dur) - 3) fnight = strtime_to_m(dur);
    m_to_strtime(fnight, night, BUF_TIME, '+');

    /* Compute inst - also rounds cause why not */
    sscanf(csv_row[incsv->inst], "%f", &finst);
    finst *= 60;
    if (finst >= strtime_to_m(dur) - 3) finst = strtime_to_m(dur);
    m_to_strtime(finst, inst, BUF_TIME, '+');

    /* Landings */
    sscanf(csv_row[incsv->dland], "%d", &dland);
    sscanf(csv_row[incsv->nland], "%d", &nland);

    /* Do DB inserts */
    if (!row_exists(db, "aircraft", "tail", csv_row[incsv->aircraft])) {
      bind_id_of(aircraft_ins, AIRCRAFT_WRITE_TYPE, "types", "ident", csv_row[incsv->type]);
      db_bind_text(aircraft_ins, AIRCRAFT_WRITE_TAIL, csv_row[incsv->aircraft]);
      db_stp_res_clr(aircraft_ins);
    }
    bind_id_of(flights_ins, FLIGHTS_WRITE_AIRCRAFT, "aircraft", "tail", csv_row[incsv->aircraft]);
    bind_id_of(flights_ins, FLIGHTS_WRITE_ROLE, "roles", "ident", csv_row[incsv->role]);
    bind_id_of(flights_ins, FLIGHTS_WRITE_DEP, "airports", "ident", csv_row[incsv->dep]);
    bind_id_of(flights_ins, FLIGHTS_WRITE_ARR, "airports", "ident", csv_row[incsv->arr]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_DATE, date);
    db_bind_int(flights_ins, FLIGHTS_WRITE_LEG, leg);
    db_bind_text(flights_ins, FLIGHTS_WRITE_FLTNO, csv_row[incsv->fltno]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_SOUT, sout);
    db_bind_text(flights_ins, FLIGHTS_WRITE_SOUTUTC, soututc);
    db_bind_text(flights_ins, FLIGHTS_WRITE_SIN, sin);
    db_bind_text(flights_ins, FLIGHTS_WRITE_SINUTC, sinutc);
    db_bind_text(flights_ins, FLIGHTS_WRITE_SDUR, sdur);
    db_bind_text(flights_ins, FLIGHTS_WRITE_AOUT, aout);
    db_bind_text(flights_ins, FLIGHTS_WRITE_AOUTUTC, aoututc);
    db_bind_text(flights_ins, FLIGHTS_WRITE_AIN, ain);
    db_bind_text(flights_ins, FLIGHTS_WRITE_AINUTC, ainutc);
    db_bind_text(flights_ins, FLIGHTS_WRITE_DUR, dur);
    db_bind_text(flights_ins, FLIGHTS_WRITE_NIGHT, night);
    db_bind_text(flights_ins, FLIGHTS_WRITE_INST, inst);
    db_bind_int(flights_ins, FLIGHTS_WRITE_XC, str_bool(csv_row[incsv->xc]));
    db_bind_int(flights_ins, FLIGHTS_WRITE_DLAND, dland);
    db_bind_int(flights_ins, FLIGHTS_WRITE_NLAND, nland);
    db_bind_text(flights_ins, FLIGHTS_WRITE_APRCH, csv_row[incsv->apprch]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_CREW, csv_row[incsv->crew]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_NOTES, csv_row[incsv->notes]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_TRIP, csv_row[incsv->trip]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_TRIPDATE, csv_row[incsv->tripdate]);
    db_stp_res_clr(flights_ins);
    fprintf(stderr, "%d: %s\n", nrow, date);
    nrow++;
  }
}

int main(int argc, char **argv)
{
  InCSV incsv;
  DB *db;

  if (argc < 2) {
    printf("Usage: cat my.csv | importcsv logbook.db\n");
    return 1;
  }

  flights_ins = db_prep(db, FLIGHTS_INSERT);
  aircraft_ins = db_prep(db, AIRCRAFT_INSERT);
  airport_tz_by_iata = db_prep(db, AIRPORTS_SELECT_TZONE_BY_IATA);
  airport_tz_by_icao = db_prep(db, AIRPORTS_SELECT_TZONE_BY_ICAO);

  if (incsv_init(&incsv, "")) {
    printf("Failed to init csv file\n");
    return 1;
  }

  if ((db = db_open(argv[1])) == NULL) {
    printf("Failed to open database\n");
    return 1;
  }

  incsv_import(&incsv, db);

  incsv_finalize(&incsv);
  return 0;
}
