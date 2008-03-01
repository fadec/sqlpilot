#ifndef CSV_H
#define CSV_H

#include <stddef.h>
#include <stdio.h>

#define CSV_TRIM   0x01
#define CSV_QUOTES 0x02

int csv_row_parse_wcs(const wchar_t *src, size_t sn, wchar_t *buf, size_t bn, wchar_t *row[], int rn, int sep, int trim);

int csv_row_parse_str(const unsigned char *src, size_t sn, unsigned char *buf, size_t bn, unsigned char *row[], int rn, int sep, int trim);

int csv_row_fread(FILE *in, unsigned char *buf, size_t bn, unsigned char *row[], int numcols, int sep, int trim);

#endif


