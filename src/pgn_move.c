#define POSIX_C_SOURCE 200809L

#include "pgn_move.h"
#include "pgn_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PGN_MOVE_BUFFER_SIZE 8

static void fill_move_buffer(char *move_buffer, size_t buffer_size, FILE *fp) {
  size_t i;
  char c;

  i = 0;
  while ((c = fgetc(fp)) != EOF && i < (buffer_size - 1) && c != ' ' &&
         c != '\n')
    move_buffer[i++] = c;
  move_buffer[i] = '\0';
  ungetc(c, fp);
}

void parse_moves(struct pgn_move **moves, size_t *nbr_moves, FILE *fp) {
  char move_buffer[PGN_MOVE_BUFFER_SIZE];
  int move_counter;
  char c;

  *moves = NULL;
  move_counter = 0;
  while ((c = fgetc(fp)) != EOF) {
    ungetc(c, fp);

    read_until_digit(fp);
    read_int(fp); /* Read turn number */
    fgetc(fp);    /* Skip '.' */
    fgetc(fp);    /* Skip ' ' */

    *moves = realloc(*moves, (move_counter + 2) * sizeof(struct pgn_move));

    fill_move_buffer(move_buffer, PGN_MOVE_BUFFER_SIZE, fp);
    (*moves)[move_counter] =
        (struct pgn_move){strndup(move_buffer, PGN_MOVE_BUFFER_SIZE)};

    fgetc(fp); /* Skip ' ' */

    fill_move_buffer(move_buffer, PGN_MOVE_BUFFER_SIZE, fp);
    (*moves)[move_counter + 1] =
        (struct pgn_move){strndup(move_buffer, PGN_MOVE_BUFFER_SIZE)};

    move_counter += 2;
  }
  *nbr_moves = move_counter;
}

void print_moves(struct pgn_move *moves, size_t nbr_moves) {
  size_t i;

  printf("\nMoves:\n");
  for (i = 0; i < nbr_moves; i += 2)
    printf("Turn %lu: %s, %s\n", i / 2 + 1, moves[i].san, moves[i + 1].san);
}

// void preview_moves(struct pgn_move *moves, size_t nbr_moves) {}
