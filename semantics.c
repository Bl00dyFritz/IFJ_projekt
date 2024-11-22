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


void ExamineVar (tAstNode *node, tSymtableList *symlist){
	tBstNodeContent *content = NULL;
	tSymtableListElem *tree = symlist->first;
	while (tree){
		if (BstSearch(tree->root_ptr, node->structure.var.token.value.string, &content)) return;
		tree = tree->next;
	}
	exit(SEMANTIC_UNDEF_ERROR);
}

void ExamineFunctionCall (tAstNode *node, tSymtableList *symlist){
	tBstNodeContent *content = NULL;
	tSymtableListElem *tree = symlist->first;
	tArgDef *symtable_data = NULL;
	tArgs *arg_data = NULL;
	while(tree){
		if (BstSearch(tree->root_ptr, node->structure.func_call.name_token.value.string, &content)) break;
		tree = tree->next;
	}
	if(!tree) exit(SEMANTIC_UNDEF_ERROR);
	if(node->structure.func_call.arg_cnt !=((tFunctionVals *)content->value)->paramCnt) 
		exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
	symtable_data = ((tFunctionVals *)content->value)->params;
	arg_data = node->structure.func_call.args;
	while(arg_data){
		if (arg_data->token.type == Token_Integer && symtable_data->type_token.type != Token_i32 
				&& symtable_data->type_token.type != Token_Ni32) 
			exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
		if (arg_data->token.type == Token_Float && symtable_data->type_token.type != Token_f64 
				&& symtable_data->type_token.type != Token_Nf64) 
			exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
		if (arg_data->token.type == Token_FuncID){
			tree = symlist->first;
			while (tree){
				if (BstSearch(tree->root_ptr, arg_data->token.value.string, &content)) break;
				tree = tree->next;
			}
			if (!tree) exit(SEMANTIC_UNDEF_ERROR);
			if (content->type != VARIABLE) exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
		}
		arg_data = arg_data->next;
		symtable_data = symtable_data->next;
	}
}

void ExamineFunctionDef (tAstNode *node, tSymtableList *symlist){
	tBstNodeContent *content = NULL;
	tSymtableListElem *tree = symlist->first;
	tArgDef *args = node->structure.func_def.args;
	while(tree){
 		if (BstSearch(tree->root_ptr, node->structure.func_def.token.value.string, &content)) exit(SEMANTIC_REDEF_ERROR);
		tree = tree->next;
	}
	content = (tBstNodeContent *) malloc(sizeof(tBstNodeContent));
	if (!content) exit (99);
	content->type = FUNCTION;
	content->value = (tFunctionVals *) malloc(sizeof(tFunctionVals));
	if (!content->value) exit (99);
	((tFunctionVals *)content->value)->loc_bst = node->structure.func_def.internal_symtable;
	((tFunctionVals *)content->value)->is_used = false;
	((tFunctionVals *)content->value)->ret_type = node->structure.func_def.ret_type_token.type;
	while(args){
		((tFunctionVals *)content->value)->paramCnt++;
		tBstNodeContent *arg = (tBstNodeContent *) malloc(sizeof(tBstNodeContent));
		if(!arg) exit(99);
		arg->type = VARIABLE;
		tVarVals *vals = (tVarVals *)arg->value;
		vals->is_constant = false;
		vals->is_used = false;
		vals->type = args->type_token.type;
		vals->value = NULL;
		BstInsert(&node->structure.func_def.internal_symtable, args->name_token.value.string, *arg);
		args = args->next;
	}
	((tFunctionVals *)content->value)->params = args;
	BstInsert(&symlist->first->root_ptr, node->structure.func_def.token.value.string, *content);
	SymtableListAdd(symlist, node->structure.func_def.internal_symtable);
	ExamineSemantics(node->structure.func_def.code, symlist);
}

void ExamineSemantics (tAstNode *node, tSymtableList *symlist){
	if(!node) return;
	switch (node->type){
		case STATEMENT:
			ExamineSemantics(node->structure.statement.function, symlist);
			ExamineSemantics(node->structure.statement.next_statement, symlist);
			break;
		case CODE:
			ExamineSemantics(node->structure.code.operation, symlist);
			ExamineSemantics(node->structure.code.next_code, symlist);
			break;
		case VAR:
			ExamineVar (node, symlist);
			break;
		case VAL:
			break;
		case WHILE:
			ExamineSemantics(node->structure.while_loop.expr, symlist);
			ExamineSemantics(node->structure.while_loop.nn_id, symlist);
			ExamineSemantics(node->structure.while_loop.code, symlist);
			break;
		case IF:
			ExamineSemantics(node->structure.if_block.expr, symlist);
			ExamineSemantics(node->structure.if_block.nn_id, symlist);
			ExamineSemantics(node->structure.if_block.if_code, symlist);
			ExamineSemantics(node->structure.if_block.else_code, symlist);
			break;
		case BIN_OP:
			ExamineSemantics(node->structure.bin_op.op1, symlist);
			ExamineSemantics(node->structure.bin_op.op2, symlist);
			break;
		case ASSIGN:
			ExamineSemantics(node->structure.assign.src, symlist);
			ExamineSemantics(node->structure.assign.dst, symlist);
			break;
		case CONST_DECL:
		case VAR_DECL:
			break;
		case FUNC_CALL:
			ExamineFunctionCall(node, symlist);
			break;
		case FUNC_DEF:
			ExamineFunctionDef(node, symlist);
			break;
	}
}
