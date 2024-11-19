/**
 * @file stack.h
 * @brief hlavickovy soubor na definovani ADT zasobnik pro prace s AST a precedencni analyzou
 * @author Nikola Jordanov xjordan00
 */

#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "error.h"

/**
 * @brief Struktura prvku zasobniku tokenu
 */
typedef struct token_stack_elem{
	tToken token;	//token obsahujici data prvku
	struct token_stack_elem *next;	//Ukazatel na dalsi prvek zasobniku
}tTokenStackElem;

/**
 * @brief Struktura zasobniku tokenu
 */
typedef struct token_stack{
	tTokenStackElem *top;	//Ukazatel na vrchol zasobniku
}tTokenStack;

/**
 * @brief Funkce, ktera inicializuje zasobnik
 * @param stack Ukazatel na inicializovany zasobnik
 */
void InitTStack (tTokenStack *stack);

/**
 * @brief Funkce na pridavani element na vrchol zasobniku
 * @param stack Zasobnik do ktereho se uklada prvek
 * @param token Token ktery se ulzi do noveho prvku
 */
void PushTStack (tTokenStack *stack, tToken token);

/**
 * @brief Funkce na odstraneni prvku z vrcholu zasobniku
 * @param stack Zasobnik ze kteryho se ma odstranit prvek
 */
void PopTStack (tTokenStack *stack);

/**
 * @brief Funkce, ktera vraci hodnotu prvku z vrcholu zasobniku
 * @param stack Zasobnik ze kteryho se ma vratit hodnota vrcholu
 * @return Vraci hodnotu ulozenou na vrcholu
 */
tToken TopTStack (tTokenStack *stack);

/**
 * @brief Funkce ktera kontroluje zda je zasobnik prazdny
 * @param stack Kontrolovany zasobnik
 * @return True pokud je zasobnik prazdny, jinka false
 */
bool StackIsEmpty (tTokenStack *stack);
#endif /** STACK_H **/
