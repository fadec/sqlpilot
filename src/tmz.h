#ifndef TIME_H
#define TIME_H

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef char tmz_tz_str[64];
typedef char tmz_date_str[sizeof("YYYY-MM-DD")];
typedef char tmz_time_str[sizeof("HH:MM:SS")];
typedef char tmz_datetime_str[sizeof("YYYY-MM-DD HH:MM:SS")];
typedef char tmz_datetimetz_str[sizeof(tmz_datetime_str) + sizeof(tmz_tz_str)];

typedef struct tmz Tmz;
struct tmz {
  struct tm tm;
  tmz_tz_str tz;
};

time_t tmz_set(Tmz *tmz, int year, int mon, int mday, int hour, int min, int sec, tmz_tz_str tz);
int tmz_move(Tmz *tmz, tmz_tz_str totz);
void tmz_datetimetz(Tmz *tmz, tmz_datetimetz_str datetimetz);
size_t tmz_strftime(Tmz *tmz, const char *format, char *s, size_t max);
int tmz_read_datetimetz(Tmz *tmz, const char *datetimetz);

#endif
