#ifndef _PGN_MOVE_H_
#define _PGN_MOVE_H_

#include <stdio.h>

#define BOARD_SIZE 64

void parse_moves(char ***moves, size_t *nbr_moves, FILE *fp);

void print_moves(char **moves, size_t nbr_moves);

void print_board(char *board);

void preview_moves(char *board, char **moves, size_t nbr_moves,
                   size_t moves_left);

#endif /* _PGN_MOVE_H */
