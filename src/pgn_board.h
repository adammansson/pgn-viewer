#ifndef _PGN_BOARD_H_
#define _PGN_BOARD_H_

enum piece {
  EMPTY,
  W_PAWN,
  B_PAWN,
  W_KNIGHT,
  B_KNIGHT,
  W_BISHOP,
  B_BISHOP,
  W_ROOK,
  B_ROOK,
  W_QUEEN,
  B_QUEEN,
  W_KING,
  B_KING,
};

struct pgn_board {
  enum piece *pieces;
};

struct pgn_board *new_board();

enum piece piece_at(struct pgn_board *board, char file, int rank);

void print_board(struct pgn_board *board);

#endif /* _PGN_BOARD_H */
