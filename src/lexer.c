#include "../include/lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* KEYWORDS[] = {
	"break", "goto", "do", "end",
	"while", "repeat", "until",
	"elseif", "else", "if", "then",
	"for", "in", "local", "function",
	"return", "nil", "false", "true",
};

const uint32_t N_KEYWORDS = 19;

const char* PUNCTUATION[] = {
	";", ",", "::", "...", ":",
	".." // Note: this is technically an operator
	"[", "]", ".", "(", ")",
	"{", "}"
};

const uint32_t N_PUNCTUATION = 12;

const char*	OPERATORS[] = {
	// Binary operators
	"+", "-", "*", "//", "/",
	"^", "%", "&", "~=", "|",
	"<<", ">>", "==", "<=", ">=",
	"~", "<", ">", "and", "or",

	// Unary operators
	"not", "#"
};

const uint32_t N_OPERATORS = 22;

struct l_token* l_token_new(enum l_token_type type, char* lexeme) {
	struct l_token* tok = (struct l_token*) malloc(sizeof(struct l_token));
	tok->type = type;
	tok->lexeme = strdup(lexeme);

	return tok;
}

void l_token_free(struct l_token* tok) {
	free(tok->lexeme);
	free(tok);
}

void tokenize(char* program, struct l_token*** tokens_ptr, uint32_t* n_tokens_ptr) {
	struct l_token** tokens = NULL;
	uint32_t n_tokens = 0;
	bool found_match;

	for (uint32_t i = 0; i < strlen(program); i++) {
		found_match = false;
		for (uint32_t j = 0; j < N_KEYWORDS; j++) {
			if (strncmp(&program[i], KEYWORDS[j], strlen(KEYWORDS[j])) == 0) {
				tokens = realloc(tokens, sizeof(struct l_token*) * (n_tokens + 1));
				tokens[n_tokens++] = l_token_new(L_KEYWORD, KEYWORDS[j]);
				i += strlen(KEYWORDS[j]) - 1;
				found_match = true;
				break;
			}
		}

		if (!found_match) {
			for (uint32_t j = 0; j < N_PUNCTUATION; j++) {
				if (strncmp(&program[i], PUNCTUATION[j], strlen(PUNCTUATION[j])) == 0) {
					tokens = realloc(tokens, sizeof(struct l_token*) * (n_tokens + 1));
					tokens[n_tokens++] = l_token_new(L_PUNC, PUNCTUATION[j]);
					i += strlen(PUNCTUATION[j]) - 1;
					found_match = true;
					break;
				}
			}
		}

		if (!found_match) {
			for (uint32_t j = 0; j < N_OPERATORS; j++) {
				if (strncmp(&program[i], OPERATORS[j], strlen(OPERATORS[j])) == 0) {
					tokens = realloc(tokens, sizeof(struct l_token*) * (n_tokens + 1));
					tokens[n_tokens++] = l_token_new(L_OPERATOR, OPERATORS[j]);
					i += strlen(OPERATORS[j]) - 1;
					found_match = true;
					break;
				}
			}
		}

		if (found_match) {
			continue;
		}

		if (isdigit(program[i])) {
			char* lexeme = malloc(sizeof(char) * 2);
			lexeme[0] = program[i++];
			uint32_t n_lexeme = 1;

			while (i < strlen(program) && isdigit(program[i])) {
				lexeme = realloc(lexeme, sizeof(char) * (n_lexeme + 2));
				lexeme[n_lexeme++] = program[i++];
			}

			if (i < strlen(program) && (isalpha(program[i]) || program[i] == '_')) {
				fprintf(stderr, "error: character after numeral was alphabetic or an underscore.\n");
			}

			lexeme[n_lexeme] = 0;
			tokens = realloc(tokens, sizeof(struct l_token*) * (n_tokens + 1));
			tokens[n_tokens++] = l_token_new(L_NUMERAL, lexeme);
			free(lexeme);

			i -= 1;
		} else if (program[i] == '\'' || program[i] == '"') {
			char end = program[i++];
			char* lexeme = NULL;
			uint32_t n_lexeme = 0;

			while (i < strlen(program) && (program[i] != end)) {
				if (program[i] == '\\') {
					if (i + 1 >= strlen(program)) {
						fprintf(stderr, "error: no characters after backslash.\n");
					}
					else {
						lexeme = realloc(lexeme, sizeof(char) * (n_lexeme + 2));
						lexeme[n_lexeme++] = program[i++];
					}
				}

				lexeme = realloc(lexeme, sizeof(char) * (n_lexeme + 2));
				lexeme[n_lexeme++] = program[i++];
			}

			lexeme[n_lexeme] = 0;
			tokens = realloc(tokens, sizeof(struct l_token*) * (n_tokens + 1));
			tokens[n_tokens++] = l_token_new(L_LITERAL_STRING, lexeme);
			free(lexeme);
		} else if (isalpha(program[i]) || program[i] == '_') {
			char* lexeme = (char*) malloc(sizeof(char) * 2);
			lexeme[0] = program[i++];
			uint32_t n_lexeme = 1;

			while (i < strlen(program) && (isalnum(program[i]) || program[i] == '_')) {
				lexeme = realloc(lexeme, sizeof(char) * (n_lexeme + 2));
				lexeme[n_lexeme++] = program[i++];
			}

			lexeme[n_lexeme] = 0;
			tokens = realloc(tokens, sizeof(struct l_token*) * (n_tokens + 1));
			tokens[n_tokens++] = l_token_new(L_NAME, lexeme);
			free(lexeme);

			i -= 1;
		}
	}

	*tokens_ptr = tokens;
	*n_tokens_ptr = n_tokens;
}
