/*
 * Notes:
 *
 * Not yet supporting the following:
 * - compilation to bytecode.
 * - non-integer numbers
 * - long-form strings
 * - comments
 */

#include "../include/lexer.h"
#include <stdint.h>
#include <stdlib.h>

/*
	chunk ::= block

	block ::= {stat} [retstat]

	stat ::=  ‘;’ | 
		 varlist ‘=’ explist | 
		 functioncall | 
		 label | 
		 break | 
		 goto Name | 
		 do block end | 
		 while exp do block end | 
		 repeat block until exp | 
		 if exp then block {elseif exp then block} [else block] end | 
		 for Name ‘=’ exp ‘,’ exp [‘,’ exp] do block end | 
		 for namelist in explist do block end | 
		 function funcname funcbody | 
		 local function Name funcbody | 
		 local namelist [‘=’ explist] 

	retstat ::= return [explist] [‘;’]

	label ::= ‘::’ Name ‘::’

	funcname ::= Name {‘.’ Name} [‘:’ Name]

	varlist ::= var {‘,’ var}

	var ::= Name | prefixexp ‘[’ exp ‘]’ | prefixexp ‘.’ Name 

	namelist ::= Name {‘,’ Name}

	explist ::= exp {‘,’ exp}

	exp ::=  nil | false | true | Numeral | LiteralString | ‘...’ | functiondef | 
		 prefixexp | tableconstructor | exp binop exp | unop exp 

	prefixexp ::= var | functioncall | ‘(’ exp ‘)’

	functioncall ::=  prefixexp args | prefixexp ‘:’ Name args 

	args ::=  ‘(’ [explist] ‘)’ | tableconstructor | LiteralString 

	functiondef ::= function funcbody

	funcbody ::= ‘(’ [parlist] ‘)’ block end

	parlist ::= namelist [‘,’ ‘...’] | ‘...’

	tableconstructor ::= ‘{’ [fieldlist] ‘}’

	fieldlist ::= field {fieldsep field} [fieldsep]

	field ::= ‘[’ exp ‘]’ ‘=’ exp | Name ‘=’ exp | exp

	fieldsep ::= ‘,’ | ‘;’

	binop ::=  ‘+’ | ‘-’ | ‘*’ | ‘/’ | ‘//’ | ‘^’ | ‘%’ | 
		 ‘&’ | ‘~’ | ‘|’ | ‘>>’ | ‘<<’ | ‘..’ | 
		 ‘<’ | ‘<=’ | ‘>’ | ‘>=’ | ‘==’ | ‘~=’ | 
		 and | or

	unop ::= ‘-’ | not | ‘#’ | ‘~’
*/

struct l_interpreter {
	struct l_token** tokens;
	uint32_t n_tokens;
	uint32_t pos;
};

struct l_interpreter* l_interpreter_new(struct l_token** tokens, uint32_t n_tokens) {
	struct l_interpreter* parser = (struct l_interpreter*) malloc(sizeof(struct l_interpreter));
	parser->tokens = tokens;
	parser->n_tokens = n_tokens;
	parser->pos = 0;

	return parser;
}

void l_interpreter_free(struct l_interpreter* parser) {
	for (uint32_t i = 0; i < parser->n_tokens; i++) {
		l_token_free(parser->tokens[i]);
	}

	free(parser->tokens);
	free(parser);
}

#define INTERPRETER_EOS(parser) ((0 > parser->pos) || (parser->pos >= parser->n_tokens))

void interpret_statement(struct l_interpreter* parser) {
	// TODO
}

void interpret(struct l_interpreter* parser) {
	while (!INTERPRETER_EOS(parser)) {
		interpret_statement(parser);
	}
}

int main(int argc, char** argv) {
	struct l_token** tokens;
	uint32_t n_tokens;

	char* program = ""
		"function fact (n)\n"
		"if n == 0 then\n"
		"return 1\n"
		"else\n"
		"return n * fact(n-1)\n"
		"end\n"
		"end\n"
		"print(\"Factorial starting now...\")\n"
		"print(fact(10))\n";

	tokenize(program, &tokens, &n_tokens);
	struct l_interpreter* parser = l_interpreter_new(tokens, n_tokens);
	// TODO: interpret
	l_interpreter_free(parser);

	return 0;
}
