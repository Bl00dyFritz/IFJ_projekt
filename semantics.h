/**
 * Implementace semanticke analyzy imperativniho jazyka IFJ24
 * @file semantics.h
 * @brief Hlavickovy soubor pro semantickou analyzu
 * @author Nikola Jordanov xjordan00
 */

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symtable.h"
#include "error.h"

/**
 * @brief struktura prvku seznamu vyhledavacich tabulek
 */
typedef struct symtable_list_elem{
	tBstNode *root_ptr;
	struct symtable_list_elem *next;
}tSymtableListElem;

/**
 * @brief struktura seznamu vyhledavacich tabulek
 */
typedef struct symtable_list{
	tSymtableListElem *first;
}tSymtableList;

/**
 * @brief Funkce inicializujici seznam vyhledavacich tabulek
 * @param list Ukazatel na seznam ktery se ma nainicializovat
 */
void InitSymtableList (tSymtableList *list);

/**
 * @brief Funkce, ktera pridava dalsi prvek do zacatku seznamu
 * @param list Ukazatel na seznam do ktereho se ma pridat prvek
 * @param symtree ukazatel na korenovy uzel vyhledavaci tabulky ktera se ma pridat
 */
void SymtableListAdd (tSymtableList *list, tBstNode *symtree);

/**
 * @brief Funkce, ktera odstranuje prvni prvek ze seznamu
 * @param list Ukazatel na seznam ze ktereho se ma odebrat prvek
 */
void SymtableListRemove (tSymtableList *list);

void AssignVals(tVarVals **vals, void *in_val, tType in_type);
void AssignInt(tVarVals **vals, void *in_val);
void AssignDouble(tVarVals **vals, void *in_val);
void ExamineBuiltInFunc(tAstNode *node, tType *out_type);

void ExamineFunctionDef (tAstNode *node, tSymtableList *symlist);
void ExamineFunctionCall (tAstNode *node, tSymtableList *symlist, tType *out_type);
void ExamineVar (tAstNode *node, tSymtableList *symlist, void *in_val, tType in_type, void **out_val, tType *out_type);
void ExamineVal (tAstNode *node, void **out_val, tType *out_type);
void ExamineDecl (tAstNode *node, tSymtableList *symlist);
void ExamineWhile (tAstNode *node, tSymtableList *symlist);
void ExamineIf (tAstNode *node, tSymtableList *symlist);
void ExamineBinOp (tAstNode *node, tSymtableList *symlist);
void ExamineAssign (tAstNode *node, tSymtableList *symlist);
void ExamineSemantics (tAstNode *node, tSymtableList *symlist);
