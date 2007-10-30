#include <test.h>

int ntests;
int nassertions;
int nfailures;


static int pass()
{
	nassertions++;
	fprintf(stderr, ".");
	return PASS;
}

static int fail()
{
	nassertions++;
	nfailures++;
	fprintf(stderr, "F");
	return FAIL;
}

void test_init()
{
	ntests = nassertions = nfailures = 0;
}

int test(void *test_function(void))
{
	ntests++;
	test_function();
}

int assert_equal_int(int a, int b)
{
	return (a == b) ? pass() : fail();
}

int assert_equal_str(const char *a, const char *b)
{
	return ((strlen(a) == strlen(b)) && (!strcmp(a, b))) ? pass() : fail();
}
