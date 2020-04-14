#include "compiler.h"

#define POOL_SIZE 2048

enum {MV,PUSH,ADD,EXIT};
long *pc, *bp, *sp, ax, cycle; // virtual machine registers
long *text,*old_text,*stack; 
char *data;           

int execute()
{
	int op;
	
	while (1) 
	{
    	op = *pc++; // get next operation code	
    	if (op == MV) {ax = *pc++;}  
    	else if (op == PUSH) {*--sp = ax;}
    	else if (op == ADD) ax = *sp++ + ax;
    	else if (op == EXIT) { printf("exit(%ld)", *sp); return *sp;}
    	else  return 0;
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
	
	//program
	i = 0;
    text[i++] = MV;
    text[i++] = 10;
    text[i++] = PUSH;
    text[i++] = MV;
    text[i++] = 20;
    text[i++] = ADD;
    text[i++] = PUSH;
    text[i++] = EXIT;
    pc=text;
    
    execute();
}