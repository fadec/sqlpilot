#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PASS 1
#define FAIL 0

void test_init(void);
int test(void *test_function(void));
int assert(int);
int assert_equal_str(const char *, const char *);
