#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/*
	This tokenizer struct is used to hold the current position in a string. It is necessary if the string contains non-alphanumeric values which are used
	to separate tokens when tokenizing.
*/
typedef struct Tokenizer_t {
    char *current;
} Tokenizer;

/*
	This function initializes a tokenizer struct with a given string.
*/
Tokenizer *TKCreate(char *tokenString) {
    Tokenizer *tok = malloc(sizeof(Tokenizer));
    tok->current = tokenString;
    return tok;
}

/*
	This function creates a token using the current position of the given tokenizer. It mallocs an initial size of 5 bytes which can be reallocated
	in 5 byte chunks to hold the final token. It continues adding the next character in the tokenizer as long as it is alphanumeric. At the end, it adds
	a string terminator character ('\0') and returns the token.
*/
static char *buildToken(Tokenizer *tk) {
	// Here, we use size_t because the size is guaranteed to never be negative.
    size_t size = 0;
    /*
        We allocate memory in chunks of 5 bytes so we're not constantly reallocating memory.
        This reduces the amount of function calls.
    */
    size_t capacity = 5;
    char *token = malloc(5);
    while(*tk->current != NULL && isalnum(*tk->current)) {
        size++;
        if(size >= capacity) {
            capacity += 5;
            token = realloc(token, capacity);
        }
        token[size - 1] = *tk->current++;
    }
    /*
        Reduce C string to appropriate size and terminate it with a '\0' character.
    */
    token = realloc(token, size + 1);
    token[size] = '\0';
    return token;
}

/*
	This function gets the next token in the given tokenizer. It first finds the next valid string that can be tokenized. It stops when it reaches an 
	alphabetical character because string tokens can't start with digits. Then it returns the token that can be built from that point. If there are no more tokens
	in the file, then it returns NULL.
*/
char *TKGetNextToken(Tokenizer * tk) {
    /* End of token string. There are no more tokens. */
    if(*tk->current == NULL) {
        return NULL;
    }
    /* Advance to next token */
    while(*tk->current != NULL && !isalpha(*tk->current)) {
        tk->current++;
    }
    /* Build and return the token */
    return buildToken(tk);
}