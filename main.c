#include "compiler.h"


extern int match (TokenSet token);
extern void advance(void);
extern char* getLexeme(void);

#define POOL_SIZE 2048
enum {MV,PUSH,ADD,SUB,MUL,DIV,EXIT};
long *pc, *sp, ax; // virtual machine registers
long *text,*old_text,*stack;    
int pos=0;        

int execute()
{
	int op;
	
	while (1) 
	{
    	op = *pc++; // get next operation code	
    	if (op==END) return 0;
    	else if (op == MV) {ax = *pc++; printf("move :%ld\n",ax);}  
    	else if (op == PUSH) {*--sp = ax;printf("PUSH :%ld\n",ax);}
    	else if (op == ADD) {printf("ADD :%ld\n",ax);ax = *sp++ + ax;}
    	else if (op == MUL) {printf("MUL :%ld\n",ax);ax = *sp++ * ax;}
    	else if (op == SUB) {printf("SUB :%ld\n",ax);ax = *sp++ - ax;}
    	else if (op == DIV) {printf("DIV :%ld\n",ax);ax = *sp++ / ax;}
    	else if (op == EXIT) { printf("exit(%ld)\n", *sp); return *sp;}
    	else  return 0;
    }
}
	
void statement(void);
void expr(void);
void expr_tail(void);
void term(void);
void term_tail(void);
void factor(void);

void statement(void)
{
    if (match(END)) {
        printf("return\n");
        text[pos++] = END;
        return;
        // nothing
    } else {
        expr();
        match(END);
        if (match(END)) return;
        advance();
        
    }
}
void expr(void)
{
    term();
    expr_tail();
}
void expr_tail(void)
{
    if (match(ADDOP)){
        printf("ADDs: %s\n", getLexeme());
    	text[pos++] = PUSH;
        advance();
        term();
        text[pos++] = ADD;
        expr_tail();
 
    }
    else if (match(SUBOP)){
        printf("SUB: %s\n", getLexeme());
        text[pos++] = PUSH;
        advance();
        term();
        text[pos++] = ADD;
        expr_tail();
    } 
    else {
        // NIL
        text[pos++] = PUSH;
        text[pos++] = EXIT;
    }
}
void term(void) 
{
    factor();
    term_tail();
}
void term_tail(void)
{
    if (match(MULOP)) {
        printf("MUL: %s\n", getLexeme());
		text[pos++] = PUSH;
        advance();
        factor();
        text[pos++] = MUL;
        term_tail();
    }
    else if (match(DIVOP)) {
        printf("DIV: %s\n", getLexeme());
        text[pos++] = PUSH;
        advance();
        factor();
        text[pos++] = DIV;
        term_tail();
    }  
    else {
        // NIL
         //text[pos++] = EXIT;
    }
}
void factor(void)
{
    if (match(INT)) {
        printf("INT: %d\n", atoi(getLexeme()));
        text[pos++] = MV;
        text[pos++] = atoi(getLexeme());
        advance();
    } else if (match(ADDOP)) {
        printf("ADD: %s\n", getLexeme());
        advance();
        if (match(INT)) {
            printf("INT: %s\n", getLexeme());
            advance();
        } else if (match(ID)) {
            printf("ID: %s\n", getLexeme());
            advance();
        }
    } else if (match(SUBOP)) {
        //printf("SUB: %s\n", getLexeme());
        advance();
        if (match(INT)) {
            printf("INT: %s\n", getLexeme());
            advance();
        } else if (match(ID)) {
            printf("ID: %s\n", getLexeme());
            advance();
        }
    }
     else if (match(ID)) {
        printf("ID: %s\n", getLexeme());
        advance();
        if (match(ASSIGN)) {
            printf("ASSIGN: %s\n", getLexeme());
            advance();
            expr();
        }
    } else if (match(LPAREN)) {
        printf("LPAREN: %s\n", getLexeme());
        advance();
        expr();
        if (match(RPAREN)) {
            printf("RPAREN: %s\n", getLexeme());
            advance();
        } else {
            printf("Parenthesis mismatched\n");
        }
    }
}


int main(int argc,char *argv[])
{
	int i;

    if (!(text = old_text = malloc(POOL_SIZE))) {
        printf("could not malloc(%d) for text area\n", POOL_SIZE);
        return -1;
    }
    if (!(stack = malloc(POOL_SIZE))) {
        printf("could not malloc(%d) for stack area\n", POOL_SIZE);
        return -1;
    }
    memset(text, 0, POOL_SIZE);
    memset(stack, 0, POOL_SIZE);
    
    sp = (long *)((long)stack + POOL_SIZE);
	ax = 0;
		
	statement();
	pc=text;
	printf("###########Assemblly############\n");
	execute();
    return 0;
}