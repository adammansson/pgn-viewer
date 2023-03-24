#ifndef _PGN_TAG_H_
#define _PGN_TAG_H_

#include <stdio.h>

struct pgn_tag {
  char *name, *value;
};

void parse_tags(struct pgn_tag **tags, size_t *nbr_tags, FILE *fp);

void print_tags(struct pgn_tag *tags, size_t nbr_tags);

#endif /* _PGN_TAG_H */
