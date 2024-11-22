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

void ExamineSemantics (tAstNode *node){
	if(!node) return;
	switch (node->type){
		case STATEMENT:
			ExamineSemantics(node->structure.statement.function);
			ExamineSemantics(node->structure.statement.next_statement);
			break;
		case CODE:
			ExamineSemantics(node->structure.code.operation);
			ExamineSemantics(node->structure.code.next_code);
			break;
		case VAR:
			break;
		case VAL:
			break;
		case WHILE:
			ExamineSemantics(node->structure.while_loop.expr);
			ExamineSemantics(node->structure.while_loop.nn_id);
			ExamineSemantics(node->structure.while_loop.code);
			break;
		case IF:
			ExamineSemantics(node->structure.if_block.expr);
			ExamineSemantics(node->structure.if_block.nn_id);
			ExamineSemantics(node->structure.if_block.if_code);
			ExamineSemantics(node->structure.if_block.else_code);
			break;
		case BIN_OP:
			ExamineSemantics(node->structure.bin_op.op1);
			ExamineSemantics(node->structure.bin_op.op2);
			break;
		case ASSIGN:
			ExamineSemantics(node->structure.assign.dst);
			ExamineSemantics(node->structure.assign.src);
			break;
		case CONST_DECL:
			break;
		case VAR_DECL:
			break;
		case FUNC_CALL:
			break;
		case FUNC_DEF:
			ExamineSemantics(node->structure.func_def.code);
			break;
	}
}
