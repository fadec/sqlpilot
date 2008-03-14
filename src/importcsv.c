#include "lib/csv.h"
#include "db/db.h"
#include "sqlpilot.h"
#include "util.h"

#define CSV_BUFSIZE 4096
#define CSV_COLS 128

typedef enum {
  INCSV_TIMEBASE_LOCAL,
  INCSV_TIMEBASE_UTC,
  INCSV_TIMEBASE_AIRPORT
} InCSVTimebase;

char csv_buf[CSV_BUFSIZE];
char *csv_row[CSV_COLS];

typedef struct InCSV InCSV;
struct InCSV {
  FILE *fh;
  int date,
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
    role,
    dland,
    nland,
    crew,
    notes,
    trip,
    numcol;
  int sep;
  int timebase;
};


static int incsv_init(InCSV *csv, const char *filename)
{

  if ((csv->fh = fopen(filename, "rb")) == NULL) return 1;

  csv->sep = ',';
  csv->timebase = INCSV_TIMEBASE_AIRPORT;
  csv->date = 0;
  csv->fltno = 1;
  csv->aircraft = 2;
  csv->type = 3;
  csv->dep = 4;
  csv->arr = 5;
  csv->aout = 6;
  csv->ain = 7;
  csv->sout = 8;
  csv->sin = 9;
  csv->dur = 10;
  csv->night = 11;
  csv->inst = 12;
  csv->apprch = 13;
  csv->role = 15;
  csv->dland = 16;
  csv->nland = 17;
  csv->notes = 18;
  csv->crew = 19;
  csv->trip = 20;
  csv->numcol = 21;
  return 0;
}

static void incsv_finalize(InCSV *incsv)
{
  fclose(incsv->fh);
}

time_t airport_mktime(DB *db, const char *airport, InCSVTimebase timebase, struct tm *tm)
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
    if (tz_of_airport_ident(db, airport, tz, BUF_TZ) && strlen(tz)) {
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
    sout_dt[BUF_DATETIME],
    sin_dt[BUF_DATETIME],
    aout_dt[BUF_DATETIME],
    ain_dt[BUF_DATETIME];
  struct tm
    sout_tm,
    sin_tm,
    aout_tm,
    ain_tm;
  time_t
    sout_t,
    sin_t,
    aout_t,
    ain_t;
  float fnight, finst;
  int dland, nland;
  char *ptr;
  int nrow, ncol;
  DBStatement *flights_ins = db_prep(db, FLIGHTS_INSERT);
  DBStatement *aircraft_ins = db_prep(db, AIRCRAFT_INSERT);

  nrow = 0;
  while (csv_row_fread(incsv->fh,
		       (unsigned char *)csv_buf,
		       CSV_BUFSIZE,
		       (unsigned char **)csv_row,
		       CSV_COLS,
		       incsv->sep,
		       0) > 0) {

    /* CSV reading and checking */
    if (nrow == 0) { nrow++; continue; }	/* Skip CSV header */
    for (ncol=0; ncol<incsv->numcol; ncol++) {
      if (!csv_row[ncol]) {
	csv_row[ncol] = "\0";
	fprintf(stderr, "Warning: missing column %d in row %d\n", ncol+1, nrow);
      }
    }

    /* Copy and reformat certain fields to internal formats */
    *date = *sout = *sout_dt = *sin = *sin_dt = *sdur = *aout = *aout_dt = *ain = *ain_dt = *dur = *night = *inst = '\0';
    fnight = finst = 0;
    dland = nland = 0;

    strncpy(date, csv_row[incsv->date], sizeof(date));
    for (ptr = date; ptr < (date + sizeof(date)); ptr++) {
      if ('/' == *ptr) { *ptr = '-'; };
    }
    date[sizeof(date) - 1] = '\0';

    format_time(csv_row[incsv->aout], aout, ':');
    format_time(csv_row[incsv->ain],  ain,  ':');
    format_time(csv_row[incsv->sout], sout, ':');
    format_time(csv_row[incsv->sin],  sin, ':');

    tm_read_strdate(&sout_tm, date);
    tm_read_strtime(&sout_tm, sout);
    tm_read_strdate(&sin_tm, date);
    tm_read_strtime(&sin_tm, sin);
    tm_read_strdate(&aout_tm, date);
    tm_read_strtime(&aout_tm, aout);
    tm_read_strdate(&ain_tm, date);
    tm_read_strtime(&ain_tm, ain);

    sout_t = airport_mktime(db, csv_row[incsv->dep], incsv->timebase, &sout_tm);
    sin_t = airport_mktime(db, csv_row[incsv->arr], incsv->timebase, &sin_tm);
    aout_t = airport_mktime(db, csv_row[incsv->dep], incsv->timebase, &aout_tm);
    ain_t = airport_mktime(db, csv_row[incsv->arr], incsv->timebase, &ain_tm);

    gmtime_r(&sout_t, &sout_tm);
    gmtime_r(&sin_t, &sin_tm);
    gmtime_r(&aout_t, &aout_tm);
    gmtime_r(&sin_t, &ain_tm);

    strftime(sout_dt, BUF_DATETIME, "%Y-%m-%d %H:%M", &sout_tm);
    strftime(sin_dt, BUF_DATETIME, "%Y-%m-%d %H:%M", &sin_tm);
    strftime(aout_dt, BUF_DATETIME, "%Y-%m-%d %H:%M", &aout_tm);
    strftime(ain_dt, BUF_DATETIME, "%Y-%m-%d %H:%M", &ain_tm);

/*     m_to_strtime(daywrap_minutes(strtime_to_m(ain) - strtime_to_m(aout)), dur, BUF_TIME, '+'); */
/*     m_to_strtime(daywrap_minutes(strtime_to_m(sin) - strtime_to_m(sout)), sdur, BUF_TIME, '+'); */
    strncpy(sdur, "00:00", BUF_TIME);
    strncpy(dur, "00:00", BUF_TIME);


    sscanf(csv_row[incsv->night], "%f", &fnight);
    fnight *= 60;
    if (fnight > strtime_to_m(dur)) fnight = strtime_to_m(dur);
    m_to_strtime(fnight, night, BUF_TIME, '+');

    sscanf(csv_row[incsv->inst], "%f", &finst);
    finst *= 60;
    if (finst > strtime_to_m(dur)) finst = strtime_to_m(dur);
    m_to_strtime(finst, inst, BUF_TIME, '+');

    sscanf(csv_row[incsv->dland], "%d", &dland);
    sscanf(csv_row[incsv->nland], "%d", &nland);

    /* Do DB inserts */
    if (!row_exists(db, "aircraft", "ident", csv_row[incsv->aircraft])) {
      bind_id_of(aircraft_ins, AIRCRAFT_WRITE_TYPE, "types", "ident", csv_row[incsv->type]);
      db_bind_text(aircraft_ins, AIRCRAFT_WRITE_IDENT, csv_row[incsv->aircraft]);
      db_stp_res_clr(aircraft_ins);
    }
    bind_id_of(flights_ins, FLIGHTS_WRITE_AIRCRAFT, "aircraft", "ident", csv_row[incsv->aircraft]);
    bind_id_of(flights_ins, FLIGHTS_WRITE_ROLE, "roles", "ident", csv_row[incsv->role]);
    bind_id_of(flights_ins, FLIGHTS_WRITE_DEP, "airports", "ident", csv_row[incsv->dep]);
    bind_id_of(flights_ins, FLIGHTS_WRITE_ARR, "airports", "ident", csv_row[incsv->arr]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_DATE, date);
    db_bind_text(flights_ins, FLIGHTS_WRITE_FLTNO, csv_row[incsv->fltno]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_SOUT, sout_dt);
    db_bind_text(flights_ins, FLIGHTS_WRITE_SIN, sin_dt);
    db_bind_text(flights_ins, FLIGHTS_WRITE_SDUR, sdur);
    db_bind_text(flights_ins, FLIGHTS_WRITE_AOUT, aout_dt);
    db_bind_text(flights_ins, FLIGHTS_WRITE_AIN, ain_dt);
    db_bind_text(flights_ins, FLIGHTS_WRITE_DUR, dur);
    db_bind_text(flights_ins, FLIGHTS_WRITE_NIGHT, night);
    db_bind_text(flights_ins, FLIGHTS_WRITE_INST, inst);
    db_bind_int(flights_ins, FLIGHTS_WRITE_DLAND, dland);
    db_bind_int(flights_ins, FLIGHTS_WRITE_NLAND, nland);
    db_bind_text(flights_ins, FLIGHTS_WRITE_APRCH, csv_row[incsv->apprch]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_CREW, csv_row[incsv->crew]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_NOTES, csv_row[incsv->notes]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_TRIP, csv_row[incsv->trip]);
    db_stp_res_clr(flights_ins);
    fprintf(stderr, "%d: %s\n", nrow, date);
    nrow++;
  }
}

int main(int argc, char **argv)
{
  InCSV incsv;
  DB *db;

  if (argc < 3) {
    printf("Usage: importcsv file.csv logbook.db\n");
    return 1;
  }

  if (incsv_init(&incsv, argv[1])) {
    printf("Failed to init csv file\n");
    return 1;
  }

  if ((db = db_open(argv[2])) == NULL) {
    printf("Failed to open database\n");
    return 1;
  }

  incsv_import(&incsv, db);

  incsv_finalize(&incsv);
  return 0;
}
