/**
 * @file stack.h
 * @brief hlavickovy soubor na definovani ADT zasobnik pro precedencni analyzu
 * @author Nikola Jordanov xjordan00
 */

#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "error.h"

typedef struct token_stack_elem{
	tToken token;
	struct token_stack_elem *next;
}tTokenStackElem;

typedef struct token_stack{
	tTokenStackElem *top;
}tTokenStack;

void InitTStack (tTokenStack *stack);
void PushTStack (tTokenStack *stack, tToken token);
void PopTStack (tTokenStack *stack);
tToken TopTStack (tTokenStack *stack);
bool StackIsEmpty (tTokenStack *stack);
#endif /** STACK_H **/
