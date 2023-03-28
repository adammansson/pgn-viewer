#ifndef _PGN_MOVE_H_
#define _PGN_MOVE_H_

#include <stdio.h>

struct pgn_move {
  char *san;
};

void parse_moves(struct pgn_move **moves, size_t *nbr_moves, FILE *fp);

void print_moves(struct pgn_move *moves, size_t nbr_moves);

void preview_moves(struct pgn_move *moves, size_t nbr_moves);

#endif /* _PGN_MOVE_H */
