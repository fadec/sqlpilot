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

typedef enum { SUCCESS, WARNING, ERROR } Status;
typedef enum { FILL, OVERWRITE, REPLACE } WriteStrategy;

char csv_buf[CSV_BUFSIZE];
char *csv_row[CSV_COLS];

DBStatement *flights_ins;
DBStatement *flights_update;
DBStatement *aircraft_ins;
DBStatement *flights_where_date_fltno_depiata_arriata;
DBStatement *flights_where_date_fltno_depicao_arricao;
DBStatement *airport_tz_by_iata;
DBStatement *airport_tz_by_icao;

typedef struct InCSV InCSV;
struct InCSV {
  FILE *fh;
  int date,
    leg,
    fltno,
    tail,
    fleetno,
    role,
    model,
    dep,
    arr,
    aout,
    ain,
    sout,
    sin,
    dur,
    night,
    inst,
    aprch,
    hold,
    dland,
    nland,
    xc,
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
  csv->fleetno = i++;
  csv->model = i++;
  csv->dep = i++;
  csv->arr = i++;
  csv->aout = i++;
  csv->ain = i++;
  csv->sout = i++;
  csv->sin = i++;
  csv->dur = i++;
  csv->night = i++;
  csv->inst = i++;
  csv->aprch = i++;
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

time_t incsv_mktime(DB *db, const char *airport, InCSVTimebase timebase, struct tm *tm, Status *stat)
{
  time_t t;
  char tz[BUF_TZ] = "\0";

  *stat = SUCCESS;
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
      printf("    Warning: No timezone for airport %s. Assuming machine local time.\n", airport);
      t = mktime(tm);
      *stat = WARNING;
    }
    break;
  }
  return t;
}

Status report_airport(const char *position, const char *code)
{
  int slen = strlen(EMPTY_IF_NULL(code));
  if (slen == 0) {
    printf("    Warning: %s airport blank\n", position);
    return WARNING;
    
  } else if (slen < 3 || slen > 4) {
    printf("    Error: %s airport identifier \"%s\" does not look like ICAO or IATA\n", position, code);
    return ERROR;
  }
  return SUCCESS;
}

Status incsv_import(InCSV *incsv, DB *db, int write_strategy)
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
  Status s;
  int warnings = 0;
  DBStatement *existing, *stmt; /* Existing row, insert or update statement */
  int update = 0;               /* nonzero if stmt is an update */
  int overwrite = 0;

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
	printf("Error: missing column %d in row %d\n", ncol+1, nrow);
	return ERROR;
      }
    }

    *date = *sout = *soututc = *sin = *sinutc = *sdur = *aout = *aoututc = *ain = *ainutc = *dur = *night = *inst = '\0';
    fnight = finst = 0;
    dland = nland = 0;
    leg = 0;
    aout_t = ain_t = sout_t = sin_t = -1;

    /* Reformat date to yyyy-mm-dd */
    strncpy(date, csv_row[incsv->date], sizeof(date));
    for (ptr = date; ptr < (date + sizeof(date)); ptr++) {
      if ('/' == *ptr) { *ptr = '-'; };
    }
    date[sizeof(date) - 1] = '\0';

/*     Set stmt to update or merge depending on whether or not a sufficient match is in the db */
    if (csv_row[incsv->dep] && (strlen(csv_row[incsv->dep]) == 3)) {
      existing = flights_where_date_fltno_depiata_arriata;
    } else {
      existing = flights_where_date_fltno_depicao_arricao;
    }
    db_bind_text(existing, 1, date);
    db_bind_text(existing, 2, csv_row[incsv->fltno]);
    db_bind_text(existing, 3, csv_row[incsv->dep]);
    db_bind_text(existing, 4, csv_row[incsv->arr]);
    
    switch(db_step(existing)) {
    case DB_ROW:
      stmt = flights_update;
      db_bind_int64(stmt, FLIGHTS_WRITE_ID, db_column_int64(existing, 0));
      update = 1;
      break;
    case DB_DONE:
      stmt = flights_ins;
      update = 0;
      break;
    default:
      printf("Database error\n");
      return ERROR;
      break;
    }
      

    printf("%s %s %s %s\n", update ? (overwrite ? "Overwrite" : "Update") : "Insert", date, csv_row[incsv->dep], csv_row[incsv->arr]);

    report_airport("Departure", csv_row[incsv->dep]);
    report_airport("Arrival", csv_row[incsv->arr]);

    sscanf(csv_row[incsv->leg], "%d", &leg);

    /* Compute in-out times and departure dates */
    if (strlen(csv_row[incsv->aout])) {
      format_time(csv_row[incsv->aout], aout, ':');
      tm_read_strdate(&tm, date);
      tm_read_strtime(&tm, aout);
      aout_t = incsv_mktime(db, csv_row[incsv->dep], incsv->timebase, &tm, &s);
      if (s == WARNING) { warnings = 1; }
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
      ain_t = incsv_mktime(db, csv_row[incsv->arr], incsv->timebase, &tm, &s);
      if (s == WARNING) { warnings = 1; }
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
      sout_t = incsv_mktime(db, csv_row[incsv->dep], incsv->timebase, &tm, &s);
      if (s == WARNING) { warnings = 1; }
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
      sin_t = incsv_mktime(db, csv_row[incsv->arr], incsv->timebase, &tm, &s);
      if (s == WARNING) { warnings = 1; }
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
    if (fnight) m_to_strtime(fnight, night, BUF_TIME, '+');

    /* Compute inst - also rounds cause why not */
    sscanf(csv_row[incsv->inst], "%f", &finst);
    finst *= 60;
    if (finst >= strtime_to_m(dur) - 3) finst = strtime_to_m(dur);
    if (finst) m_to_strtime(finst, inst, BUF_TIME, '+');

    /* Landings */
    sscanf(csv_row[incsv->dland], "%d", &dland);
    sscanf(csv_row[incsv->nland], "%d", &nland);
    
    /* Do DB writes */
    if (!overwrite && update && db_column_text(existing, FLIGHTS_COL_AIRCRAFT_ID)) {
      db_bind_int64(stmt, FLIGHTS_WRITE_AIRCRAFT, db_column_int64(existing, FLIGHTS_COL_AIRCRAFT_ID));
    } else if (csv_row[incsv->fleetno] && strlen(csv_row[incsv->fleetno])) {
      if (!row_exists(db, "aircraft", "fleetno", csv_row[incsv->fleetno])) {
	bind_id_of(aircraft_ins, AIRCRAFT_WRITE_MODEL, "models", "ident", csv_row[incsv->model]);
	db_bind_nonempty_text_else_null(aircraft_ins, AIRCRAFT_WRITE_FLEETNO, csv_row[incsv->fleetno]);
	db_stp_res_clr(aircraft_ins);
      }
      bind_id_of(stmt, FLIGHTS_WRITE_AIRCRAFT, "aircraft", "fleetno", csv_row[incsv->fleetno]);
    }

    if (!overwrite && update && db_column_text(existing, FLIGHTS_COL_ROLE_ID)) {
      db_bind_int64(stmt, FLIGHTS_WRITE_ROLE, db_column_int64(existing, FLIGHTS_COL_ROLE_ID));
    } else {
      bind_id_of(stmt, FLIGHTS_WRITE_ROLE, "roles", "ident", csv_row[incsv->role]);
    }
    if (!overwrite && update && db_column_text(existing, FLIGHTS_COL_DEP_ID)) {
      db_bind_int64(stmt, FLIGHTS_WRITE_DEP, db_column_int64(existing, FLIGHTS_COL_DEP_ID));
    } else {
      bind_id_of(stmt, FLIGHTS_WRITE_DEP, "airports", "iata", csv_row[incsv->dep]);
    }
    if (!overwrite && update && db_column_text(existing, FLIGHTS_COL_ARR_ID)) {
      db_bind_int64(stmt, FLIGHTS_WRITE_ARR, db_column_int64(existing, FLIGHTS_COL_ARR_ID));
    } else {
      bind_id_of(stmt, FLIGHTS_WRITE_ARR, "airports", "iata", csv_row[incsv->arr]);
    }
    /* These macros only change null values in db on update */
    #define BIND_TEXT(wcol, rcol, val)					\
      db_bind_nonempty_text_else_null(stmt, (wcol), (!overwrite && update && db_column_text(existing, (rcol))) ? (char*)db_column_text(existing, (rcol)) : (val))
    #define BIND_INT(wcol, rcol, val)					\
      db_bind_int(stmt, (wcol), (!overwrite && update && db_column_text(existing, (rcol))) ? db_column_int(existing, (rcol)) : (val))
    #define BIND_BOOL(wcol, rcol, val)					\
      db_bind_int(stmt, (wcol), (!overwrite && update && db_column_text(existing, (rcol))) ? str_bool((char*)db_column_text(existing, (rcol))) : (val))
    BIND_TEXT(FLIGHTS_WRITE_DATE, FLIGHTS_COL_DATE, date);
    BIND_INT(FLIGHTS_WRITE_LEG, FLIGHTS_COL_LEG, leg);
    BIND_TEXT(FLIGHTS_WRITE_FLTNO, FLIGHTS_COL_FLTNO, csv_row[incsv->fltno]);
    BIND_TEXT(FLIGHTS_WRITE_SOUT, FLIGHTS_COL_SOUT, sout);
    BIND_TEXT(FLIGHTS_WRITE_SOUTUTC, FLIGHTS_COL_SOUTUTC, soututc);
    BIND_TEXT(FLIGHTS_WRITE_SIN, FLIGHTS_COL_SIN, sin);
    BIND_TEXT(FLIGHTS_WRITE_SINUTC, FLIGHTS_COL_SINUTC, sinutc);
    BIND_TEXT(FLIGHTS_WRITE_SDUR, FLIGHTS_COL_SDUR, sdur);
    BIND_TEXT(FLIGHTS_WRITE_AOUT, FLIGHTS_COL_AOUT, aout);
    BIND_TEXT(FLIGHTS_WRITE_AOUTUTC, FLIGHTS_COL_AOUTUTC, aoututc);
    BIND_TEXT(FLIGHTS_WRITE_AIN, FLIGHTS_COL_AIN, ain);
    BIND_TEXT(FLIGHTS_WRITE_AOUTUTC, FLIGHTS_COL_AINUTC, ainutc);
    BIND_TEXT(FLIGHTS_WRITE_DUR, FLIGHTS_COL_DUR, dur);
    BIND_TEXT(FLIGHTS_WRITE_NIGHT, FLIGHTS_COL_NIGHT, night);
    BIND_TEXT(FLIGHTS_WRITE_INST, FLIGHTS_COL_INST, inst);
    BIND_TEXT(FLIGHTS_WRITE_SIMINST, FLIGHTS_COL_SIMINST, NULL);
    BIND_BOOL(FLIGHTS_WRITE_XC, FLIGHTS_COL_XC, str_bool(csv_row[incsv->xc]));
    BIND_BOOL(FLIGHTS_WRITE_HOLD, FLIGHTS_COL_HOLD, str_bool(csv_row[incsv->hold]));
    BIND_INT(FLIGHTS_WRITE_DLAND, FLIGHTS_COL_DLAND, dland);
    BIND_INT(FLIGHTS_WRITE_NLAND, FLIGHTS_COL_NLAND, nland);
    BIND_TEXT(FLIGHTS_WRITE_APRCH, FLIGHTS_COL_APRCH, csv_row[incsv->aprch]);
    BIND_TEXT(FLIGHTS_WRITE_CREW, FLIGHTS_COL_CREW, csv_row[incsv->crew]);
    BIND_TEXT(FLIGHTS_WRITE_NOTES, FLIGHTS_COL_NOTES, csv_row[incsv->notes]);
    BIND_TEXT(FLIGHTS_WRITE_TRIP, FLIGHTS_COL_TRIP, csv_row[incsv->trip]);
    BIND_TEXT(FLIGHTS_WRITE_TRIPDATE, FLIGHTS_COL_TRIPDATE, csv_row[incsv->tripdate]);
    #undef BIND_TEXT
    #undef BIND_INT
    #undef BIND_BOOL
    db_stp_res_clr(stmt);
    db_reset(existing);
    db_clear_bindings(existing);
    nrow++;
  }
  return warnings ? WARNING : SUCCESS;
}

int main(int argc, char **argv)
{
  char usage[] = "Usage: cat <CSV file> | importcsv <Database file>. Option -h for details.\n";
  InCSV incsv;
  DB *db;
  char *db_filename = NULL;
  int i;
  int force = 0;
  int dryrun = 0;
  WriteStrategy write_strategy = FILL;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(argv[i][1]) {
      case 'h':
	printf("SQLPilot (CSV -> Database) Importer\n");
	printf("%s\n", usage);
	printf("    -h    This message.\n");
	printf("    -o    Overwrite. Without this option only previously null fields are imported.\n");
	printf("    -r    Replace. Like overwrite but also imports blank fields. (not implemented)\n");
	printf("    -f    Force. Ignore warnings and modify database anyway.\n");
	printf("    -d    Dry run. Will not change data.\n");
	printf("\n");
	printf("Imported flights matching on date/fltno/dep/arr will update the existing record. See -o and -r.\n");
	printf("%s\n", SQLPILOT_CONSOLE_GREETING);
	return 0;
      case 'f':
	force = 1;
	break;
      case 'd':
	dryrun = 1;
	break;
      case 'o':
	write_strategy = OVERWRITE;
      case 'r':
	write_strategy = REPLACE;
	break;
      default:
	fprintf(stderr, "Invalid option. -h for help.\n");
	return 1;
      }
    } else {
      db_filename = argv[i];
    }
  }

  if (!db_filename) {
    printf("%s", usage);
    return 1;
  }

  if (incsv_init(&incsv, "")) {
    printf("Failed to open CSV file\n");
    return 1;
  }

  if ((db = db_open(db_filename)) == NULL) {
    printf("Failed to open database\n");
    return 1;
  }
  flights_ins = db_prep(db, FLIGHTS_INSERT);
  flights_update = db_prep(db, FLIGHTS_UPDATE);
  flights_where_date_fltno_depiata_arriata = db_prep(db, FLIGHTS_SELECT FLIGHTS_WHERE_DATE_FLTNO_DEPIATA_ARRIATA);
  flights_where_date_fltno_depicao_arricao = db_prep(db, FLIGHTS_SELECT FLIGHTS_WHERE_DATE_FLTNO_DEPICAO_ARRICAO);
  aircraft_ins = db_prep(db, AIRCRAFT_INSERT);
  airport_tz_by_iata = db_prep(db, AIRPORTS_SELECT_TZONE_BY_IATA);
  airport_tz_by_icao = db_prep(db, AIRPORTS_SELECT_TZONE_BY_ICAO);

  db_exec_simple(db, "BEGIN TRANSACTION;");
  switch (incsv_import(&incsv, db, write_strategy)) {
  case SUCCESS:
    if (dryrun) {
      db_exec_simple(db, "ROLLBACK;");
      printf("Dry run successful. Nothing changed.\n\n");
    } else {
      db_exec_simple(db, "COMMIT;");
      printf("Import successful. Database modified.\n\n");
    }
    break;
  case WARNING:
    if (force) {
      db_exec_simple(db, "COMMIT;");
      printf("Commit forced with warnings. Database modified.\n\n");
    } else {
      db_exec_simple(db, "ROLLBACK;");
      printf("Warnings. Nothing changed. Use -f to force.\n\n");
    }
    break;
  default:
  case ERROR:
    db_exec_simple(db, "ROLLBACK;");
    fprintf(stderr, "Transaction Failed.\n\n");
    return 1;
    break;
  }

  incsv_finalize(&incsv);
  return 0;
}
