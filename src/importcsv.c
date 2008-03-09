#include "lib/csv.h"
#include "db/db.h"
#include "sqlpilot.h"
#include "util.h"

#define CSV_BUFSIZE 1000
#define CSV_COLS 100

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
};


static int incsv_init(InCSV *csv, const char *filename)
{

  if ((csv->fh = fopen(filename, "rb")) == NULL) return 1;

  csv->sep = ',';
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
    inst[BUF_TIME];
  float fnight, finst;
  int dland, nland;
  char *ptr;
  int nrow;
  DBStatement *flights_ins = db_prep(db, FLIGHTS_INSERT);
  DBStatement *aircraft_ins = db_prep(db, AIRCRAFT_INSERT);
/*   DBStatement *types_ins = db_prep(db, TYPES_INSERT); */
/*   DBStatement *roles_ins = db_prep(db, ROLES_INSERT); */
/*   DBStatement *airports_ins = db_prep(db, AIRPORTS_INSERT); */

  nrow = 0;
  while (csv_row_fread(incsv->fh,
		       (unsigned char *)csv_buf,
		       CSV_BUFSIZE,
		       (unsigned char **)csv_row,
		       CSV_COLS,
		       incsv->sep,
		       0) > 0) {

    if (nrow == 0) { nrow++; continue; }	/* Skip CSV header */

    *date = *sout = *sin = *sdur = *aout = *ain = *dur = *night = *inst = '\0';
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
    m_to_strtime(daywrap_minutes(strtime_to_m(ain) - strtime_to_m(aout)), dur, BUF_TIME, '+');
    m_to_strtime(daywrap_minutes(strtime_to_m(sin) - strtime_to_m(sout)), sdur, BUF_TIME, '+');
    
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
    db_bind_text(flights_ins, FLIGHTS_WRITE_SOUT, sout);
    db_bind_text(flights_ins, FLIGHTS_WRITE_SIN, sin);
    db_bind_text(flights_ins, FLIGHTS_WRITE_SDUR, sdur);
    db_bind_text(flights_ins, FLIGHTS_WRITE_AOUT, aout);
    db_bind_text(flights_ins, FLIGHTS_WRITE_AIN, ain);
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
    fprintf(stderr, "%d\n", nrow);
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
