#ifndef __LEXER_H_
#define __LEXER_H_ 0

#include <stdint.h>

extern const char* KEYWORDS[];
extern const uint32_t N_KEYWORDS;
extern const char* PUNCTUATION[];
extern const uint32_t N_PUNCTUATION;
extern const char* OPERATORS[];
extern const uint32_t N_OPERATORS;

enum l_token_type {
	L_NAME = 0, L_NUMERAL, L_LITERAL_STRING,
	L_KEYWORD, L_PUNC, L_OPERATOR
};

#ifdef __CLUA_DEBUG_

const char* TOKEN_TYPE_STRING(enum l_token_type type) {
	switch (type) {
	case L_NAME:
		return "Name";
	case L_NUMERAL:
		return "Numeral";
	case L_LITERAL_STRING:
		return "Literal String";
	case L_KEYWORD:
		return "Keyword";
	case L_PUNC:
		return "Punctuation";
	case L_OPERATOR:
		return "Operator";
	}
	return "Unknown";
}

#endif // __CLUA_DEBUG_

struct l_token {
	enum l_token_type type;
	char* lexeme;
};

struct l_token* l_token_new(enum l_token_type, char*);
void l_token_free(struct l_token*);

void tokenize(char*, struct l_token***, uint32_t*);

#endif // __LEXER_H_
