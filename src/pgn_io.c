#include "pgn_io.h"
#include <ctype.h>

void read_to(char to_c, FILE *fp) {
  char c;

  while ((c = fgetc(fp)) != to_c && c != EOF)
    ;
}

void read_until(char until_c, FILE *fp) {
  char c;

  while ((c = fgetc(fp)) != until_c && c != EOF)
    ;
  ungetc(c, fp);
}

void read_until_digit(FILE *fp) {
  char c;

  while (!isdigit(c = fgetc(fp)) && c != EOF)
    ;
  ungetc(c, fp);
}

int read_int(FILE *fp) {
  int res;
  char c;

  res = 0;
  while (isdigit(c = fgetc(fp))) {
    res = res * 10 + c - '0';
  }
  ungetc(c, fp);
  return res;
}
