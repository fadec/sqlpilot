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
}

void test_daywrap(void)
{
  assert(daywrap(24 * 3600) == 0);
  assert(daywrap(24 * 3600 + 1) == 1);
  assert(daywrap(-1) == (24 * 3600 - 1));
}

void test_daywrap_minutes(void)
{
  assert(daywrap_minutes(-1) == (24 * 60 - 1));
}

int main(int argc, char **argv)
{
  test_init();
  test(test_parseB60);
  test(test_parsetime);
  test(test_daywrap);
  test(test_daywrap_minutes);
  test_finalize();
  return 0;
}

