#include "compiler.h"


extern int match (TokenSet token);
extern void advance(void);
extern char* getLexeme(void);

#define POOL_SIZE 2048
enum {MV,PUSH,ADD,SUB,MUL,DIV,EXIT};
long *pc, *bp, *sp, ax, cycle; // virtual machine registers
long *text,*old_text,*stack; 
char *data;   
int pos=0;        

int execute()
{
	int op;
	
	while (1) 
	{
    	op = *pc++; // get next operation code	
    	if (op == MV) {ax = *pc++; printf("move :%ld\n",ax);}  
    	else if (op == PUSH) {*--sp = ax;printf("PUSH :%ld\n",ax);}
    	else if (op == ADD) {ax = *sp++ + ax;printf("ADD :%ld\n",ax);}
    	else if (op == MUL) {ax = *sp++ * ax;printf("MUL :%ld\n",ax);}
    	else if (op == SUB) {ax = *sp++ - ax;printf("SUB :%ld\n",ax);}
    	else if (op == DIV) {ax = *sp++ / ax;printf("DIV :%ld\n",ax);}
    	else if (op == EXIT) { printf("exit(%ld)\n", *sp); return *sp;}
    	else  return 0;
    }
}
	
/*int main(int argc,char *argv[])
{

	int i;

    if (!(text = old_text = malloc(POOL_SIZE))) {
        printf("could not malloc(%d) for text area\n", POOL_SIZE);
        return -1;
    }
    if (!(data = malloc(POOL_SIZE))) {
        printf("could not malloc(%d) for data area\n", POOL_SIZE);
        return -1;
    }
    if (!(stack = malloc(POOL_SIZE))) {
        printf("could not malloc(%d) for stack area\n", POOL_SIZE);
        return -1;
    }
    memset(text, 0, POOL_SIZE);
    memset(data, 0, POOL_SIZE);
    memset(stack, 0, POOL_SIZE);
    
    bp = sp = (long *)((long)stack + POOL_SIZE);
	ax = 0;
	
	//program
	i = 0;
    text[i++] = MV;
    text[i++] = 20;
    text[i++] = PUSH;
    text[i++] = MV;
    text[i++] = 10;
    text[i++] = DIV;
    text[i++] = PUSH;
    text[i++] = EXIT;
    pc=text;
    
    execute();
}*/

void statement(void);
void expr(void);
void expr_tail(void);
void term(void);
void term_tail(void);
void factor(void);

void statement(void)
{
    if (match(END)) {
        //advance();
        printf("return\n");
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
        //printf("ADDs: %s\n", getLexeme());
        //advance();
        term();
        expr_tail();
    }
    else if (match(SUBOP)){
        //printf("SUB: %s\n", getLexeme());
       // advance();
        term();
        expr_tail();
    } 
    else {
        // NIL
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
        //advance();
        factor();
        term_tail();
    }
    else if (match(DIVOP)) {
        printf("DIV: %s\n", getLexeme());
        //advance();
        factor();
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
        text[pos++] = PUSH;
        advance();
    } else if (match(ADDOP)) {
        printf("ADD: %s\n", getLexeme());
        advance();
        if (match(INT)) {
            printf("INT: %s\n", getLexeme());
            text[pos++] = MV;
    		text[pos++] = atoi(getLexeme());
    		text[pos++] = ADD;
    		text[pos++] = PUSH;
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
            text[pos++] = MV;
    		text[pos++] = atoi(getLexeme());
    		text[pos++] = SUB;
    		text[pos++] = PUSH;
            advance();
        } else if (match(ID)) {
            printf("ID: %s\n", getLexeme());
            advance();
        }
    }
     else if (match(MULOP)) {
       // printf("SUB: %s\n", getLexeme());
        advance();
        if (match(INT)) {
            printf("INT: %s\n", getLexeme());
            text[pos++] = MV;
    		text[pos++] = atoi(getLexeme());
    		text[pos++] = MUL;
    		text[pos++] = PUSH;
            advance();
        } else if (match(ID)) {
            printf("ID: %s\n", getLexeme());
            advance();
        }
    }
    else if (match(DIVOP)) {
        printf("SUB: %s\n", getLexeme());
        advance();
        if (match(INT)) {
            printf("INT: %s\n", getLexeme());
            text[pos++] = MV;
    		text[pos++] = atoi(getLexeme());
    		text[pos++] = DIV;
    		text[pos++] = PUSH;
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
    if (!(data = malloc(POOL_SIZE))) {
        printf("could not malloc(%d) for data area\n", POOL_SIZE);
        return -1;
    }
    if (!(stack = malloc(POOL_SIZE))) {
        printf("could not malloc(%d) for stack area\n", POOL_SIZE);
        return -1;
    }
    memset(text, 0, POOL_SIZE);
    memset(data, 0, POOL_SIZE);
    memset(stack, 0, POOL_SIZE);
    
    bp = sp = (long *)((long)stack + POOL_SIZE);
	ax = 0;
		
	statement();
	pc=text;
	printf("###########Assemblly############\n");
	execute();
    return 0;
}