#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum token_type {
	TOKEN_EOF = 0,
	TOKEN_STRING,
	TOKEN_SYMBOL,
	TOKEN_PERIOD,
	TOKEN_ASTERISK,
	TOKEN_LBRACKET,
	TOKEN_RBRACKET,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_LANGLE,
	TOKEN_RANGLE
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

typedef struct parser {
	tokenizer_t *tokenizer;
	token_t *tokens;
	int pos;
} parser_t;

typedef struct tag_pair {
	char *name;
	char *value;
} tag_pair_t;

typedef struct game {
	tag_pair_t *tags;
	int tags_size;
	int tags_allocated;

	char **moves;
	int moves_size;
	int moves_allocated;
} game_t;

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
	buffer = malloc(allocated * sizeof *buffer);

	while ((c = getc(fp)) != EOF) {
		buffer[pos] = c;
		pos++;

		if (pos >= allocated) {
			allocated += 64;
			buffer = realloc(buffer, allocated * sizeof *buffer);
		}
	}
	buffer[pos] = '\0';

	return buffer;
}

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
	else if (c == '*') {
		token.type = TOKEN_ASTERISK;
	}
	else if (c == '[') {
		token.type = TOKEN_LBRACKET;
	}
	else if (c == ']') {
		token.type = TOKEN_RBRACKET;
	}
	else if (c == '(') {
		token.type = TOKEN_LPAREN;
	}
	else if (c == ')') {
		token.type = TOKEN_RPAREN;
	}
	else if (c == '<') {
		token.type = TOKEN_LANGLE;
	}
	else if (c == '>') {
		token.type = TOKEN_RANGLE;
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

token_t *tokenize(tokenizer_t *t)
{
	int pos;
	int allocated;
	token_t *buffer;
	token_t token;

	pos = 0;
	allocated = 64;
	buffer = malloc(allocated * sizeof *buffer);

	while (true) {
		token = tokenizer_next(t);
		buffer[pos] = token;
		pos++;

		if (pos >= allocated) {
			allocated += 64;
			buffer = realloc(buffer, allocated * sizeof *buffer);
		}

		if (token.type == TOKEN_EOF)
			return buffer;
	}
}

void game_addtag(game_t *g, tokenizer_t *t, token_t name_token, token_t value_token)
{
	int i;
	tag_pair_t tag_pair;

	if (g->tags == NULL) {
		g->tags_allocated = 7;
		g->tags = malloc(g->tags_allocated * (sizeof *g->tags));
	}

	tag_pair.name = malloc((name_token.length + 1) * (sizeof *tag_pair.name));
	for (i = 0; i < name_token.length; i++) {
		tag_pair.name[i] = t->text[name_token.start + i];
	}
	tag_pair.name[name_token.length] = '\0';

	tag_pair.value = malloc((value_token.length + 1) * (sizeof *tag_pair.value));
	for (i = 0; i < value_token.length; i++) {
		tag_pair.value[i] = t->text[value_token.start + i];
	}
	tag_pair.value[value_token.length] = '\0';

	g->tags[g->tags_size] = tag_pair;
	g->tags_size++;

	if (g->tags_allocated >= g->tags_size) {
		g->tags_allocated += 7;
		g->tags = realloc(g->tags, g->tags_allocated * (sizeof *g->tags));
	}
}

void game_addmove(game_t *g, tokenizer_t *t, token_t move_token)
{
	int i;
	char *move;

	if (g->moves == NULL) {
		g->moves_allocated = 16;
		g->moves = malloc(g->moves_allocated * (sizeof *g->moves));
	}

	move = malloc((move_token.length + 1) * (sizeof *move));
	for (i = 0; i < move_token.length; i++) {
		move[i] = t->text[move_token.start + i];
	}
	move[move_token.length] = '\0';

	g->moves[g->moves_size] = move;
	g->moves_size++;

	if (g->moves_size >= g->moves_allocated) {
		g->moves_allocated += 16;
		g->moves = realloc(g->moves, g->moves_allocated * (sizeof *g->moves));
	}
}

void game_print(game_t *g)
{
	int i;

	printf("TAGS:\n");
	for (i = 0; i < g->tags_size; i++) {
		printf("%s -> %s\n", g->tags[i].name, g->tags[i].value);
	}

	printf("\nMOVES:\n");
	for (i = 0; i < g->moves_size; i++) {
		printf("%s\n", g->moves[i]);
	}
}

void parser_error(const char *text)
{
	fprintf(stderr, "parser: %s\n", text);
	exit(EXIT_FAILURE);
}

token_t parser_peek(parser_t *p)
{
	return p->tokens[p->pos];
}

token_t parser_consume(parser_t *p)
{
	p->pos++;
	return p->tokens[p->pos - 1];
}

token_t parser_match(parser_t *p, token_type_t type)
{
	if (p->tokens[p->pos].type == type) {
		p->pos++;
		return p->tokens[p->pos - 1];
	} else {
		parser_error("parser_match");
		exit(EXIT_FAILURE);
	}
}

void parse_tags(parser_t *p, game_t *g)
{
	token_t maybe_name;
	token_t maybe_value;

	switch (parser_peek(p).type)
	{
	case TOKEN_LBRACKET:
		parser_consume(p);

		maybe_name = parser_match(p, TOKEN_SYMBOL);
		maybe_value = parser_match(p, TOKEN_STRING);

		parser_match(p, TOKEN_RBRACKET);

		game_addtag(g, p->tokenizer, maybe_name, maybe_value);

		parse_tags(p, g);
		break;
	
	default:
		break;
	}
}

void parse_moves(parser_t *p, game_t *g)
{
	token_t maybe_white;
	token_t maybe_black;

	switch (parser_peek(p).type)
	{
	case TOKEN_SYMBOL:
		maybe_white = parser_consume(p);
		if (parser_peek(p).type == TOKEN_PERIOD) {
			parser_consume(p);
			maybe_white = parser_match(p, TOKEN_SYMBOL);
		}

		maybe_black = parser_match(p, TOKEN_SYMBOL);

		game_addmove(g, p->tokenizer, maybe_white);
		game_addmove(g, p->tokenizer, maybe_black);

		parse_moves(p, g);
		break;
	
	default:
		break;
	}
}

void parse_game(parser_t *p, game_t *g)
{
	parse_tags(p, g);
	parse_moves(p, g);
}

int main(void)
{
	tokenizer_t tokenizer;
	game_t game;
	parser_t parser;

	printf("pgn-viewer\n\n");
	tokenizer.text = import("examples/0.pgn");
	tokenizer.pos = 0;

	parser.tokenizer = &tokenizer;
	parser.tokens = tokenize(&tokenizer);
	parser.pos = 0;

	game.tags = NULL;
	game.tags_size = 0;
	game.tags_allocated = 0;
	game.moves = NULL;
	game.moves_size = 0;
	game.moves_allocated = 0;

	parse_game(&parser, &game);
	game_print(&game);

	return 0;
}
