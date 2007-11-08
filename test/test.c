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

void test_finalize()
{
	printf("\n%d tests, %d assertions, %d failures\n", ntests, nassertions, nfailures);
}

int test(void *test_function(void))
{
	ntests++;
	test_function();
	return 0;
}

int assert(int perhaps)
{
	return perhaps ? pass() : fail();
}

int assert_equal_str(const char *a, const char *b)
{
	return assert((strlen(a) == strlen(b)) && (!strcmp(a, b)));
}
