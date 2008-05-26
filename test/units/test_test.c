/************************************************************************/
/* Copyright (C) 2008  Sam Danielson                                    */
/*                                                                      */
/* This file is part of Sqlpilot.				        */
/* 								        */
/* Sqlpilot is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.				        */
/* 								        */
/* Sqlpilot is distributed in the hope that it will be useful,	        */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.			        */
/* 								        */
/* You should have received a copy of the GNU General Public License    */
/* along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    */
/************************************************************************/

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
