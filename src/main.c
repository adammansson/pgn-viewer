#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pgn_tag {
  char *name, *value;
};

struct pgn_context {
  struct pgn_tag *tags;
  size_t nbr_tags;
};

void read_until(FILE *fp, char until_c) {
  while (fgetc(fp) != until_c)
    ;
}

void fill_buffer_until(FILE *fp, char until_c, char *buffer,
                       size_t buffer_size) {
  int i;
  char c;

  i = 0;
  while (i < (buffer_size - 1) && (c = fgetc(fp)) != until_c)
    buffer[i++] = c;
  buffer[i] = '\0';
}

void parse_tags(struct pgn_tag **tags, size_t *nbr_tags, FILE *fp) {
  char name_buffer[256], value_buffer[256];
  char *name_ptr, *value_ptr;
  int i;

  *tags = NULL;
  i = 0;
  while (fgetc(fp) == '[') {
    fill_buffer_until(fp, ' ', name_buffer, 256);

    read_until(fp, '"');
    fill_buffer_until(fp, '"', value_buffer, 256);

    name_ptr = malloc((strlen(name_buffer) + 1));
    value_ptr = malloc((strlen(value_buffer) + 1));
    strcpy(name_ptr, name_buffer);
    strcpy(value_ptr, value_buffer);

    *tags = realloc(*tags, (i + 1) * sizeof(struct pgn_tag));
    (*tags)[i] = (struct pgn_tag){name_ptr, value_ptr};

    i++;
    read_until(fp, '\n');
  }
  *nbr_tags = i;
}

void print_tag(struct pgn_tag *tag) {
  printf("%s - %s\n", tag->name, tag->value);
}

void print_tags(struct pgn_tag *tags, size_t nbr_tags) {
  int i;

  printf("Tags:\n");
  for (i = 0; i < nbr_tags; i++)
    print_tag(&tags[i]);
}

int main(int argc, char *argv[]) {
  char *file_path;
  FILE *fp;
  struct pgn_tag *tags;
  size_t nbr_tags;

  if (argc < 2) {
    printf("No filepath provided, using default [example.pgn]\n");
    file_path = "example.pgn";
  } else {
    printf("Filepath provided: %s\n", argv[1]);
    file_path = argv[1];
  }

  if ((fp = fopen(file_path, "r")) == NULL) {
    printf("File not found, exiting\n");
    return -1;
  }

  parse_tags(&tags, &nbr_tags, fp);
  print_tags(tags, nbr_tags);
}
