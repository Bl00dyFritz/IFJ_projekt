/**
 * Implementace semanticke analyzy imperativniho jazyka IFJ24
 * @file semantics.c
 * @brief Implementace semanticke analyzy
 * @author Nikola Jordanov xjordan00
 */

#include <stdio.h>
#include <stdlib.h>
#include "semantics.h"
#include "ast.h"
#include "symtable.h"
#include "error.h"

/**
 * @brief Funkce inicializujici seznam vyhledavacich tabulek
 * @param list Ukazatel na seznam ktery se ma nainicializovat
 */
void InitSymtableList (tSymtableList *list){
	list->first = NULL;
}


/**
 * @brief Funkce, ktera pridava dalsi prvek do zacatku seznamu
 * @param list Ukazatel na seznam do ktereho se ma pridat prvek
 * @param symtree ukazatel na korenovy uzel vyhledavaci tabulky ktera se ma pridat
 */
void SymtableListAdd (tSymtableList *list, tBstNode *symtree){
	tSymtableListElem *tmp = (tSymtableListElem *) malloc(sizeof(tSymtableListElem));
	if (!tmp) exit (99);
	tmp->root_ptr = symtree;
	tmp->next = list->first;
	list->first = tmp;
}

/**
 * @brief Funkce, ktera odstranuje prvni prvek ze seznamu
 * @param list Ukazatel na seznam ze ktereho se ma odebrat prvek
 */
void SymtableListRemove (tSymtableList *list){
	tSymtableListElem *tmp = list->first;
	list->first = list->first->next;
	BstDispose(&tmp->root_ptr);
	free(tmp);
}
