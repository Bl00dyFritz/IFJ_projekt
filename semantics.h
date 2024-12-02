/**
 * Implementace semanticke analyzy imperativniho jazyka IFJ24
 * @file semantics.h
 * @brief Hlavickovy soubor pro semantickou analyzu
 * @author Nikola Jordanov xjordan00
 */

#ifndef SEMANTICS_H
#define SEMANTICS_H

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
 * @brief pomocna struktura na posilani dat mezi funkcema
 */
typedef struct com_link{
	void *val; //Ukazatel na hodnoty, urcena k zdileni
	tType type; //Typ dane hodndoty
	bool is_var; //true pokud jde o promenne nebo vyraz, false pokud jde o literal nebo konstanta se znamou hodnotu
}tComData;

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

void AssignVals(tVarVals **vals, tComData in_data);
void AssignInt(tVarVals **vals, void *in_val);
void AssignDouble(tVarVals **vals, void *in_val);
void ExamineBuiltInFunc(tAstNode *node, tType *out_type);
void AssignType(tVarVals **vals, tType in_type);
void CmpVars (tComData op1_data, tComData op2_data, tComData *out_data);
void CmpVarLit (tComData op1_data, tComData op2_data, tComData *out_data, bool is_div);
void BstCheckUse(tBstNode *tree);

void ExamineFunctionDef (tAstNode *node, tSymtableList *symlist);
void ExamineFunctionCall (tAstNode *node, tSymtableList *symlist, tType *out_type);
void ExamineVar (tAstNode *node, tSymtableList *symlist, tComData *in_data, tComData *out_data);
void ExamineVal (tAstNode *node, tComData *out_data);
void ExamineDecl (tAstNode *node, tSymtableList *symlist);
void ExamineWhile (tAstNode *node, tSymtableList *symlist, tTokenType *ret_type);
void ExamineIf (tAstNode *node, tSymtableList *symlist, tTokenType *ret_type);
void ExamineBinOp (tAstNode *node, tSymtableList *symlist, tComData *out_data);
void ExamineAssign (tAstNode *node, tSymtableList *symlist);
void ExamineRet (tAstNode *node, tSymtableList *symlist, tTokenType *ret_type);
void ExamineSemantics (tAstNode *node, tSymtableList *symlist, tComData *in_data, tComData *out_data, tTokenType *ret_type);

#endif /** SEMANTICS_H **/
