#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum token_type {
	TOKEN_EOF = 0,
	TOKEN_STRING,
	TOKEN_SYMBOL,
	TOKEN_PERIOD,
	TOKEN_LBRACKET,
	TOKEN_RBRACKET
} token_type_t;

typedef struct token {
	token_type_t type;
	int start;
	int length;
} token_t;

typedef struct tokenizer {
	char *text;
	int pos;
} tokenizer_t;

void tokenizer_print(tokenizer_t *t, token_t token)
{
	int i;
	printf("TOKEN (%d, %d, %d): '", token.type, token.start, token.length);

	for (i = 0; i < token.length; i++) {
		putchar(t->text[token.start + i]);
	}
	printf("'\n");
}

void tokenizer_error(const char *text)
{
	fprintf(stderr, "tokenizer: %s\n", text);
	exit(EXIT_FAILURE);
}

bool is_whitespace(char c)
{
	return c == ' ' || c == '\n' || c == '\t';
}

bool is_letter(char c)
{
	return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

bool is_digit(char c)
{
	return '0' <= c && c <= '9';
}

bool is_printing(char c)
{
	return 32 <= c && c <= 126;
}

bool is_symbol_start(char c)
{
	return is_letter(c) || is_digit(c);
}

bool is_symbol_continuation(char c)
{
	return is_symbol_start(c) || c == '_' || c == '+' || c == '#' || c == '=' || c == ',' || c == '-' || c == '/';
}

token_t tokenizer_next(tokenizer_t *t)
{
	token_t token;
	char c;

	c = t->text[t->pos];
	t->pos++;
	while (is_whitespace(c)) {
		c = t->text[t->pos];
		t->pos++;
	}
	token.start = t->pos - 1;

	if (c == '"') {
		token.type = TOKEN_STRING;

		t->pos++;
		while (true) {
			c = t->text[t->pos];
			t->pos++;
			if (!is_printing(c) || t->pos - token.start >= 255)
				tokenizer_error("string");

			if (c == '"')
				break;
		}
	}
	else if (is_symbol_start(c)) {
		token.type = TOKEN_SYMBOL;

		while (is_symbol_continuation(c)) {
			c = t->text[t->pos];
			t->pos++;
		}
		t->pos--;
	}
	else if (c == '.') {
		token.type = TOKEN_PERIOD;
	}
	else if (c == '[') {
		token.type = TOKEN_LBRACKET;
	}
	else if (c == ']') {
		token.type = TOKEN_RBRACKET;
	}
	else if (c == '\0') {
		token.type = TOKEN_EOF;
	}
	else {
		fprintf(stderr, "error at: %c\n", c);
		tokenizer_error("unknown token");
	}

	token.length = t->pos - token.start;

	return token;
}

char *import(const char *filename)
{
	FILE *fp;
	int pos;
	int allocated;
	char *buffer;
	int c;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		perror("import");
		exit(EXIT_FAILURE);
	}

	pos = 0;
	allocated = 64;
	buffer = malloc(allocated * sizeof(char));

	while ((c = getc(fp)) != EOF) {
		buffer[pos] = c;
		pos++;

		if (pos >= allocated) {
			allocated += 64;
			buffer = realloc(buffer, allocated * sizeof(char));
		}
	}
	buffer[pos] = '\0';

	return buffer;
}

int main(void)
{
	tokenizer_t tokenizer;
	token_t token;

	printf("pgn-viewer\n");
	tokenizer.text = import("example.pgn");
	tokenizer.pos = 0;

	while (true) {
		token = tokenizer_next(&tokenizer);
		tokenizer_print(&tokenizer, token);
		if (token.type == TOKEN_EOF)
			return 0;
	}
}
