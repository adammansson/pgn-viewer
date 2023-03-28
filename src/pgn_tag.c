#define POSIX_C_SOURCE 200809L

#include "pgn_tag.h"
#include "pgn_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PGN_TAG_BUFFER_SIZE 256

static void fill_tag_buffer_until(char until_c, char *tag_buffer,
                                  size_t buffer_size, FILE *fp) {
  size_t i;
  char c;

  i = 0;
  while ((c = fgetc(fp)) != EOF && i < (buffer_size - 1) && c != until_c)
    tag_buffer[i++] = c;
  tag_buffer[i] = '\0';
  ungetc(c, fp);
}

void parse_tags(struct pgn_tag **tags, size_t *nbr_tags, FILE *fp) {
  char name_buffer[PGN_TAG_BUFFER_SIZE];
  char value_buffer[PGN_TAG_BUFFER_SIZE];
  int tag_counter;

  *tags = NULL;
  tag_counter = 0;
  while (fgetc(fp) == '[') {
    fill_tag_buffer_until(' ', name_buffer, PGN_TAG_BUFFER_SIZE, fp);

    read_to('"', fp);
    fill_tag_buffer_until('"', value_buffer, PGN_TAG_BUFFER_SIZE, fp);

    *tags = realloc(*tags, (tag_counter + 1) * sizeof(struct pgn_tag));

    (*tags)[tag_counter] =
        (struct pgn_tag){strndup(name_buffer, PGN_TAG_BUFFER_SIZE),
                         strndup(value_buffer, PGN_TAG_BUFFER_SIZE)};

    read_to('\n', fp);
    tag_counter++;
  }
  *nbr_tags = tag_counter;
}

static void print_tag(struct pgn_tag *tag) {
  printf("%s - %s\n", tag->name, tag->value);
}

void print_tags(struct pgn_tag *tags, size_t nbr_tags) {
  size_t i;

  printf("\nTags:\n");
  for (i = 0; i < nbr_tags; i++)
    print_tag(&tags[i]);
}
