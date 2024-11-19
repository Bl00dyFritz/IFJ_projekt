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

/**
 * @brief Funkce, ktera inicializuje zasobnik
 * @param stack Ukazatel na inicializovany zasobnik
 */
void InitTStack (tTokenStack *stack){
	if (!stack) exit(99);
	stack->top = NULL;
}

/**
 * @brief Funkce na pridavani element na vrchol zasobniku
 * @param stack Zasobnik do ktereho se uklada prvek
 * @param token Token ktery se ulzi do noveho prvku
 */
void PushTStack (tTokenStack *stack, tToken token){
	if (!stack) exit(99);
	tTokenStackElem *new = (tTokenStackElem*) malloc(sizeof(tTokenStackElem));
	if (!new) exit(99);
	new->token = token;
	new->next = stack->top;
	stack->top = new;
}

/**
 * @brief Funkce na odstraneni prvku z vrcholu zasobniku
 * @param stack Zasobnik ze kteryho se ma odstranit prvek
 */
void PopTStack (tTokenStack *stack){
	if(!stack) exit(99);
	tTokenStackElem *elemP;
	if(stack->top){
		elemP = stack->top;
		stack->top = stack->top->next;
		free(elemP);
	}
}

/**
 * @brief Funkce, ktera vraci hodnotu prvku z vrcholu zasobniku
 * @param stack Zasobnik ze kteryho se ma vratit hodnota vrcholu
 * @return Vraci hodnotu ulozenou na vrcholu
 */
tToken TopTStack (tTokenStack *stack){
	return stack->top->token;
}


/**
 * @brief Funkce ktera kontroluje zda je zasobnik prazdny
 * @param stack Kontrolovany zasobnik
 * @return True pokud je zasobnik prazdny, jinka false
 */
bool StackIsEmpty (tTokenStack *stack){
	return !stack->top;
}
