
#include "tmz.h"

typedef struct Blocktime Blocktime;
struct Blocktime {
  tmz_date_str date;
  tmz_date_str tripdate;
  Tmz sout;
  Tmz sin;
  Tmz aout;
  Tmz ain;
};

void blocktime_to_airport_local(Blocktime *bt)
{

}
