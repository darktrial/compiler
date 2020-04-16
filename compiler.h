#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 256
typedef enum {UNKNOWN, END, INT, ID, ADDOP,SUBOP, MULOP,DIVOP, ASSIGN,
LPAREN, RPAREN} TokenSet;