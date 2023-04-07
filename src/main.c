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

  char **moves;
  size_t nbr_moves;

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

  char starting_board[BOARD_SIZE + 1] =
      "rnbqkbnrpppppppp                                PPPPPPPPRNBQKBNR";

  preview_moves(starting_board, moves, nbr_moves, nbr_moves);
}
