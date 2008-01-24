

#define EXIT_SUCESS 0
#define EXIT_BARF  1

typedef struct Sqlpilot Sqlpilot;
struct Sqlpilot {
  Logbook *logbook;
};

Sqlpilot *sqlpilot_init(Sqlpilot **sqlpilot);

void sqlpilot_finalize(Sqlpilot *);

void barf(const char *message);

