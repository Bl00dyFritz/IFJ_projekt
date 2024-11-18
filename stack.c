/**
 * @file stack.c
 * @brief implementace ADT zasobnik pro precedencni analyzu
 * @author Nikola Jordanov xjordan00
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "scanner.h"
#include "error.h"

void InitTStack (tTokenStack *stack){
	if (!stack) exit(99);
	stack->top = NULL;
}

void PushTStack (tTokenStack *stack, tToken token){
	if (!stack) exit(99);
	tTokenStackElem *new = (tTokenStackElem*) malloc(sizeof(tTokenStackElem));
	if (!new) exit(99);
	new->token = token;
	new->next = stack->top;
	stack->top = new;
}

void PopTStack (tTokenStack *stack){
	if(!stack) exit(99);
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
