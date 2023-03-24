#ifndef _PGN_IO_H_
#define _PGN_IO_H_

#include "stdio.h"

void read_to(char to_c, FILE *fp);

void read_until(char until_c, FILE *fp);

void read_until_digit(FILE *fp);

int read_int(FILE *fp);

#endif /* _PGN_IO_H */
