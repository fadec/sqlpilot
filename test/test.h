#include <stdlib.h>
#include <stdio.h>

#define PASS 1
#define FAIL 0

void test_init(void);
int test(void *test_function(void));
int assert_equal_int(int a, int b);
int assert_equal_str(const char *, const char *);
