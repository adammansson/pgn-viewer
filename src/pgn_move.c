#define POSIX_C_SOURCE 200809L

#include "pgn_move.h"
#include "pgn_io.h"
#include <ctype.h>
#include <stdbool.h>
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

void parse_moves(char ***moves, size_t *nbr_moves, FILE *fp) {
  char move_buffer[PGN_MOVE_BUFFER_SIZE];
  int move_counter;
  char c;

  *moves = NULL;
  move_counter = 0;
  while ((c = fgetc(fp)) != EOF) {
    ungetc(c, fp);

    read_until_digit(fp);
    read_int(fp); // Read turn number
    fgetc(fp);    // Skip '.'
    fgetc(fp);    // Skip ' '

    *moves = realloc(*moves, (move_counter + 2) * sizeof(char *));

    fill_move_buffer(move_buffer, PGN_MOVE_BUFFER_SIZE, fp);
    (*moves)[move_counter] = strndup(move_buffer, PGN_MOVE_BUFFER_SIZE);

    fgetc(fp); /* Skip ' ' */

    fill_move_buffer(move_buffer, PGN_MOVE_BUFFER_SIZE, fp);
    (*moves)[move_counter + 1] = strndup(move_buffer, PGN_MOVE_BUFFER_SIZE);

    move_counter += 2;
  }
  *nbr_moves = move_counter;
}

void print_moves(char **moves, size_t nbr_moves) {
  size_t i;

  printf("\nMoves:\n");
  for (i = 0; i < nbr_moves; i += 2)
    printf("Turn %lu: %s, %s\n", i / 2 + 1, moves[i], moves[i + 1]);
}

static void make_move(char *board, char *move, bool whites_turn) {
  int index;

  printf("Move: %s\n", move);
  if (islower(move[0])) { // A pawn move
    index = (move[0] - 'a') + 8 * (8 - (move[1] - '0'));
    printf("Index: %i\n", index);
    if (whites_turn)
      board[index] = 'P';
    else
      board[index] = 'p';
  } else if (isupper(move[0])) {
    index = (move[1] - 'a') + 8 * (8 - (move[2] - '0'));
    if (whites_turn)
      board[index] = move[0];
    else
      board[index] = move[0] - ('A' - 'a');
  }
  // TODO: add handling of all types of moves
}

void print_board(char *board) {
  int i;

  printf("Board:\n");
  for (i = 0; i < 64; i++) {
    if (i != 0 && i % 8 == 0)
      putc('\n', stdout);
    putc(board[i], stdout);
  }
  putc('\n', stdout);
}

void preview_moves(char *board, char **moves, size_t nbr_moves,
                   size_t moves_left) {
  char original_board[BOARD_SIZE + 1];
  char updated_board[BOARD_SIZE + 1];
  char c;

  memcpy(original_board, board, BOARD_SIZE + 1);
  memcpy(updated_board, board, BOARD_SIZE + 1);

  printf("\nMove preview:\n'n' or 'p' then enter\n");
  print_board(original_board);
  while (true) {
    c = fgetc(stdin);
    read_to('\n', stdin);

    if (c == 'q') {
      exit(0);
    } else if (c == 'n') {
      if (moves_left == 0)
        printf("No more moves left.\n");
      else {
        make_move(updated_board, moves[0], (nbr_moves - moves_left) % 2 == 0);
        preview_moves(updated_board, moves + 1, nbr_moves, moves_left - 1);
        print_board(original_board);
      }
    } else if (c == 'p') {
      if (moves_left == nbr_moves)
        printf("Already at first move.\n");
      else
        return;
    } else {
      printf("Unknown command '%c', try again.\n", c);
    }
  }
}
