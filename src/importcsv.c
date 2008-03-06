#include "lib/csv.h"
#include "db/db.h"
#include "sqlpilot.h"
#include "util.h"

#define CSV_BUFSIZE 1000
#define CSV_COLS 20

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
    crew,
    notes,
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
  csv->notes = 16;
  csv->crew = 17;
  csv->numcol = 18;

  return 0;
}

static void incsv_finalize(InCSV *incsv)
{
  fclose(incsv->fh);
}

void incsv_import(InCSV *incsv, DB *db)
{

  char date[11];		/* "2007-12-25\0" is 11 bytes */
  float fnight, finst;
  int
    dur,
    sdur,
    night,
    inst;
  char *ptr;
  int nrow;
  DBStatement *flights_ins = db_prep(db, FLIGHTS_INSERT);
  DBStatement *aircraft_ins = db_prep(db, AIRCRAFT_INSERT);
  DBStatement *types_ins = db_prep(db, TYPES_INSERT);
  DBStatement *roles_ins = db_prep(db, ROLES_INSERT);
  DBStatement *airports_ins = db_prep(db, AIRPORTS_INSERT);

  nrow = 0;
  while (csv_row_fread(incsv->fh,
		       (unsigned char *)csv_buf,
		       CSV_BUFSIZE,
		       (unsigned char **)csv_row,
		       CSV_COLS,
		       incsv->sep,
		       0) > 0) {

    strncpy(date, csv_row[incsv->date], sizeof(date));
    for (ptr = date; ptr < (date + sizeof(date)); ptr++) {
      if ('/' == *ptr) { *ptr = '-'; };
    }
    date[sizeof(date) - 1] = '\0';

    dur = parseB60(csv_row[incsv->ain]) - parseB60(csv_row[incsv->aout]);
    sdur = parseB60(csv_row[incsv->sin]) - parseB60(csv_row[incsv->sout]);
    
    sscanf(csv_row[incsv->night], "%f", &fnight);
    night = fnight * 60;

    sscanf(csv_row[incsv->inst], "%f", &finst);
    inst = finst * 60;

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
    db_bind_text(flights_ins, FLIGHTS_WRITE_SOUT, csv_row[incsv->sout]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_SIN, csv_row[incsv->sin]);
    db_bind_int(flights_ins, FLIGHTS_WRITE_SDUR, sdur);
    db_bind_int(flights_ins, FLIGHTS_WRITE_DUR, dur);
    db_bind_int(flights_ins, FLIGHTS_WRITE_NIGHT, night);
    db_bind_int(flights_ins, FLIGHTS_WRITE_INST, inst);
    db_bind_text(flights_ins, FLIGHTS_WRITE_APRCH, csv_row[incsv->apprch]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_CREW, csv_row[incsv->crew]);
    db_bind_text(flights_ins, FLIGHTS_WRITE_NOTES, csv_row[incsv->notes]);
    db_stp_res_clr(flights_ins);
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
