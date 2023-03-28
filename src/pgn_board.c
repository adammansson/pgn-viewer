#include "pgn_board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pgn_board *new_board() {
  struct pgn_board *res;
  enum piece pieces[8][8] = {
      {W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK},
      {W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN, W_PAWN},
      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
      {B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN, B_PAWN},
      {B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK},
  };

  res = malloc(sizeof(struct pgn_board));
  res->pieces = malloc(64 * sizeof(enum piece));
  memcpy(res->pieces, pieces, 64 * sizeof(enum piece));

  return res;
}

enum piece piece_at(struct pgn_board *board, char file, int rank) {
  int index;

  index = rank * 8 + (file - 'a');
  return board->pieces[index];
}

char piece_to_char(enum piece piece) {
  char map[13] = {' ', 'P', 'p', 'N', 'n', 'B', 'b',
                  'R', 'r', 'Q', 'q', 'K', 'k'};

  return map[piece];
}

void print_board(struct pgn_board *board) {
  char file;
  int rank;

  printf("\nBoard:\n");
  for (rank = 7; rank >= 0; rank--) {
    for (file = 'a'; file <= 'h'; file++) {
      printf("%c ", piece_to_char(piece_at(board, file, rank)));
    }
    printf("\n");
  }
}
