#include "compiler.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

static TokenSet getToken(void);
static TokenSet lookahead = UNKNOWN;
static char lexeme[MAXLEN];
char input_s[MAXLEN];



TokenSet getToken()
{
    int i;
    char c;
	
    while ( (c = fgetc(stdin)) == ' ' || c== '\t' );  // c?2?aA�DO|r?�M

    if (isdigit(c)) {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while (isdigit(c) && i<MAXLEN) {
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return INT;
    } else if (c == '+') {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return ADDOP;
    } else if (c == '-') {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return SUBOP;
    }  
    else if (c == '/') {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return DIVOP;
    }else if (c == '*') {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return MULOP;
    }  
    else if (c == '\n') {
        lexeme[0] = '\0';
        //printf("return\n");
        return END;
    } else if (c == '=') {
        strcpy(lexeme, "=");
        return ASSIGN;
    } else if (c == '(') {
        strcpy(lexeme, "(");
        return LPAREN;
    } else if (c == ')') {
        strcpy(lexeme, ")");
        return RPAREN;
    } else if (isalpha(c) || c == '_') {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while (isalpha(c) || isdigit(c) || c == '_') {
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return ID;
    } else {
        return UNKNOWN;
    }
}

void advance(void)
{
    int i;
    
    lookahead = getToken();
}

int match(TokenSet token)
{
    if (lookahead == UNKNOWN) advance();
    //else if (lookahead == END) return END;
    return token == lookahead;
}

char* getLexeme(void)
{
    return lexeme;
}


