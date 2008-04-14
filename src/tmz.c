
#include "tmz.h"

#define S(x) #x

#define TZ_STACK_MAX 16
tmz_tz_str tz_stack[TZ_STACK_MAX];
int tz_stack_top = 0;

static int tz_push(tmz_tz_str tz)
{
  char *oldtz;
  if (tz_stack_top < TZ_STACK_MAX) {
    oldtz = getenv("TZ");
    snprintf(tz_stack[tz_stack_top++], sizeof(tmz_tz_str), "%s", oldtz ? oldtz : "");
    if (setenv("TZ", tz, 1) == -1) {
      fprintf(stderr, "tz_push error setting TZ environment variable\n");
      return -1;
    }
  } else {
    fprintf(stderr, "tz_push stack overflow\n");
    return -1;
  }
  tzset();
  //fprintf(stderr, "    push to %s\n", getenv("TZ"));
  return 0;
}

static int tz_pop(void)
{
  char *tz;
  if (tz_stack_top) {
    tz = tz_stack[--tz_stack_top];
    if (strlen(tz)) {
      if (setenv("TZ", tz, 1) == -1) {
	fprintf(stderr, "tz_pop error setting TZ environment variable\n");
	return -1;
      }
    } else {
      if (unsetenv("TZ") == -1) {
	fprintf(stderr, "tz_pop error unsetting TZ\n");
	return -1;
      }
    }
  } else {
    fprintf(stderr, "tz_pop stack underflow\n");
    return -1;
  }
  tzset();
  //fprintf(stderr, "    pop to %s\n", getenv("TZ"));
  return 0;
}

static time_t tmz_mktime(Tmz *tmz)
{
  time_t t;
  tz_push(tmz->tz);
  tmz->tm.tm_isdst = -1;
  t = mktime(&tmz->tm);
  tz_pop();
  return t;
}

static void tmz_set_tz(Tmz *tmz, tmz_tz_str tz)
{
  memcpy(tmz->tz, tz, sizeof(tmz_tz_str));
}

static void tmz_set_tm(Tmz *tmz, int year, int mon, int mday, int hour, int min, int sec)
{
  if (year != -1) tmz->tm.tm_year = year - 1900;
  if (mon != -1) tmz->tm.tm_mon  = mon - 1;
  if (mday != -1) tmz->tm.tm_mday = mday;
  if (hour != -1) tmz->tm.tm_hour = hour;
  if (min != -1) tmz->tm.tm_min  = min;
  if (sec != -1) tmz->tm.tm_sec  = sec;
}

time_t tmz_set(Tmz *tmz, int year, int mon, int mday, int hour, int min, int sec, tmz_tz_str tz)
{
  tmz_set_tm(tmz, year, mon, mday, hour, min, sec);
  tmz_set_tz(tmz, tz);
  return tmz_mktime(tmz);
}

int tmz_move(Tmz *tmz, tmz_tz_str totz)
{
  time_t t;
  int err=0;

  tz_push(tmz->tz);
  if ((t = mktime(&tmz->tm)) == -1) {
    fprintf(stderr, "to_tz mktime error\n");
    err = -1;
  }
  tz_pop();
  tz_push(totz);
  if (localtime_r(&t, &tmz->tm) == NULL) {
    fprintf(stderr, "to_tz localtime error\n");
    err = -1;
  }
  tmz_set_tz(tmz, totz);
  tz_pop();
  return err;
}

void tmz_datetimetz(Tmz *tmz, tmz_datetimetz_str datetimetz)
{
  tmz_datetime_str datetime;
  tz_push(tmz->tz);
  strftime(datetime, sizeof(tmz_datetime_str), "%Y-%m-%d %H:%M:%S", &tmz->tm);
  snprintf(datetimetz, sizeof(tmz_datetimetz_str), "%s %s", datetime, tmz->tz);
  tz_pop();
}

size_t tmz_strftime(Tmz *tmz, const char *format, char *s, size_t max)
{
  tz_push(tmz->tz);
  return strftime(s, max, format, &tmz->tm);
}

int tmz_read_datetimetz(Tmz *tmz, const char *datetimetz)
{
  int year=-1, mon=-1, mday=-1, hour=-1, min=-1, sec=-1;
  char *p = (char*)datetimetz, *tzstart;

  sscanf(datetimetz, "%d-%d-%d %d:%d:%d", &year, &mon, &mday, &hour, &min, &sec);

  while((tzstart = p++) && *tzstart && !isalpha(*tzstart)) {};

  tmz_set(tmz, year, mon, mday, hour, min, sec, tzstart);

  return 0;
}

int tmz_read_date(Tmz *tmz, const char *date)
{
  int year=-1, mon=-1, mday=-1;
  
  scanf(date, "%d-%d-%d", &year, &mon, &mday);
  tmz_set_tm(tmz, year, mon, mday, -1, -1, -1);
  return 0;
}
