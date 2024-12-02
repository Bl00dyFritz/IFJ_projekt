/**
 * @file stack.h
 * @brief Header file for defining ADT stack for working with AST and precedent analysis
 * @author Nikola Jordanov xjordan00
 */

#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "error.h"

/**
 * @brief The token stack element structure
 */
typedef struct token_stack_elem{
	tToken token;					//Token containing element data
	struct token_stack_elem *next;	//Pointer to the next stack element
}tTokenStackElem;

/**
 * @brief Token stack structure
 */
typedef struct token_stack{
	tTokenStackElem *top;	//Pointer to the top of the stack
}tTokenStack;

/**
 * @brief A function that initializes the stack
 * @param stack A pointer to the initialized stack
 */
void InitTStack (tTokenStack *stack);

/**
 * @brief Function to add an element to the top of the stack
 * @param stack The stack in which the element is stored
 * @param token Token to be added to the new element
 */
void PushTStack (tTokenStack *stack, tToken token);

/**
 * @brief Function to remove an element from the top of the stack
 * @param stack The stack from which to remove the element
 */
void PopTStack (tTokenStack *stack);

/**
 * @brief A function that returns the value of the element from the top of the stack
 * @param stack The stack from which to return the vertex value
 * @return Returns the value stored at the top
 */
tToken TopTStack (tTokenStack *stack);

/**
 * @brief Function that checks if the stack is empty
 * @param stack The stack to check
 * @return True if the stack is empty, false otherwise
 */
bool StackIsEmpty (tTokenStack *stack);

#endif /** STACK_H **/
