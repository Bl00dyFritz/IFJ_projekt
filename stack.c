/**
 * @file stack.c
 * @brief ADT stack implementation for precedent analysis
 * @author Nikola Jordanov xjordan00
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "scanner.h"
#include "error.h"

void InitTStack (tTokenStack *stack){
	if (!stack) exit(INTERNAL_COMP_ERROR);
	stack->top = NULL;
}

void PushTStack (tTokenStack *stack, tToken token){
	if (!stack) exit(INTERNAL_COMP_ERROR);
	tTokenStackElem *new = (tTokenStackElem*) malloc(sizeof(tTokenStackElem));
	if (!new) exit(INTERNAL_COMP_ERROR);
	new->token = token;
	new->next = stack->top;
	stack->top = new;
}

void PopTStack (tTokenStack *stack){
	if(!stack) exit(INTERNAL_COMP_ERROR);
	tTokenStackElem *elemP;
	if(stack->top){
		elemP = stack->top;
		stack->top = stack->top->next;
		free(elemP);
	}
}

tToken TopTStack (tTokenStack *stack){
	return stack->top->token;
}

bool StackIsEmpty (tTokenStack *stack){
	return !stack->top;
}
