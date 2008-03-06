#include <test.h>

void test_assert_equal_int()
{
	assert_equal_int(1, 1);
	assert_equal_int(0, 42);
}

void test_assert_equal_str()
{
	assert_equal_str("", "");
	assert_equal_str("a", "a");
	assert_equal_str("abc", "abcd");
	assert_equal_str("abcd", "abc");
	assert_equal_str("", "a");
	assert_equal_str("a", "");
}

void run(int argc, char **argv)
{
	test(&test_assert_equal_str);
}

int main(int argc, char **argv)
{
	test_init();
	test(&test_assert_equal_str);
	return 0;
}
