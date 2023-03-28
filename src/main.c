#include "pgn_board.h"
#include "pgn_move.h"
#include "pgn_tag.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *file_path;
  FILE *fp;

  struct pgn_tag *tags;
  size_t nbr_tags;

  struct pgn_move *moves;
  size_t nbr_moves;

  struct pgn_board *board;

  if (argc < 2) {
    file_path = "example.pgn";
    printf("No filepath provided, using default \"%s\"\n", file_path);
  } else {
    file_path = argv[1];
    printf("Filepath provided: %s\n", file_path);
  }

  if ((fp = fopen(file_path, "r")) == NULL) {
    printf("File not found, exiting...\n");
    return -1;
  }

  parse_tags(&tags, &nbr_tags, fp);
  print_tags(tags, nbr_tags);

  parse_moves(&moves, &nbr_moves, fp);
  print_moves(moves, nbr_moves);

  board = new_board();
  print_board(board);

  // preview_moves(moves, nbr_moves);
}
