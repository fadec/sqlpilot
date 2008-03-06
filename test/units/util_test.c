#include <test.h>
#include <stdio.h>
#include "util.h"

void test_parseB60(void)
{
  assert(parseB60("") == 0);
  assert(parseB60("10:12") == 612);
  assert(parseB60("1.2:02") == 74);
  assert(parseB60("01:10.4") == 70);
  assert(parseB60("02:01.99") == 121);
  assert(parseB60("01.5:00.000") == 90);
  assert(parseB60("1:0:0") == 3600);
  assert(parseB60("1.9:00") == 114);
  assert(parseB60(".9:00") == 54);
  assert(parseB60(".95:0") == 57);
  assert(parseB60("1:1:1") == 3661);
  assert(parseB60("1.05") == 1);
  assert(parseB60("1.95") == 1);
  assert(parseB60("1.3:") == 78);
}

void test_parsetime(void)
{
  assert(parsetime("1+02", 2) == 62);
  assert(parsetime("1+02+05", 2) == 62);
  printf("%d\n", parsetime("1+02", 2));
}

int main(int argc, char **argv)
{
	test_init();
	test(test_parseB60);
       	test(test_parsetime);
	test_finalize();
	return 0;
}

