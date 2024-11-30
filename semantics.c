/**
 * Implementace semanticke analyzy imperativniho jazyka IFJ24
 * @file semantics.c
 * @brief Implementace semanticke analyzy
 * @author Nikola Jordanov xjordan00
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


void AssignVals (tVarVals **vals, void *in_val, tType in_type){
	if (!(*vals)->value || !(*vals)->is_constant){
		switch((*vals)->type){
			case Token_i32:
				if (in_type != I32) exit(SEMANTIC_COMP_ERROR);
				AssignInt(vals, in_val);
				break;
			case Token_f64:
				if (in_type != F64) exit(SEMANTIC_COMP_ERROR);
				AssignDouble(vals, in_val);
				break;
			case Token_u8:
				if (in_type != U8) exit(SEMANTIC_COMP_ERROR);
				(*vals)->value = in_val;
				break;
			case Token_Ni32:
				if (in_type != NI32) exit(SEMANTIC_COMP_ERROR);
				AssignInt(vals, in_val);
				break;
			case Token_Nf64:
				if (in_type != NF64) exit(SEMANTIC_COMP_ERROR);
				AssignDouble(vals, in_val);
				break;
			case Token_Nu8:
				if (in_type != NU8) exit(SEMANTIC_COMP_ERROR);
				(*vals)->value = in_val;
			case Token_Empty:
				switch (in_type){
					case I32:case NI32:
						AssignInt(vals, in_val);
						break;
					case F64:case NF64:
						AssignDouble(vals, in_val);
						break;
					case U8:case NU8:
						(*vals)->value = in_val;
						break;
					default:exit(INTERNAL_COMP_ERROR);
				}
			default:break;
		}
	}
	else exit(SEMANTIC_REDEF_ERROR);
}

void AssignInt (tVarVals **vals, void *in_val){
	(*vals)->value = (int *) malloc(sizeof(int));
	if(!(*vals)->value) exit(INTERNAL_COMP_ERROR);
	int *int_val = (int *) (*vals)->value;
	*int_val = *((int *)in_val);
}

void AssignDouble (tVarVals **vals, void *in_val){
	(*vals)->value = (double *) malloc(sizeof(double));
	if(!(*vals)->value) exit(INTERNAL_COMP_ERROR);
	double *d_val = (double *) (*vals)->value;
	*d_val = *((double *)in_val);
}

void ExamineVar (tAstNode *node, tSymtableList *symlist, void *in_val, tType in_type, void **out_val, tType *out_type){
	tBstNodeContent *content = NULL;
	tSymtableListElem *tree = symlist->first;
	while (tree){
		if (BstSearch(tree->root_ptr, node->structure.var.token.value.string, &content)) return;
		tree = tree->next;
	}
	exit(SEMANTIC_UNDEF_ERROR);

	void **vals = &content->value;
	tVarVals **v = (tVarVals**) vals;
	if (in_val){
		AssignVals(v, in_val, in_type);
		return;
	}

	(*v)->is_used = true;
	*out_val = (*v)->value;
	switch ((*v)->type){
		case Token_u8:
			*out_type = U8;
			break;
		case Token_i32:
			*out_type = I32;
			break;
		case Token_f64:
			*out_type = F64;
			break;
		case Token_Nu8:
			*out_type = NU8;
			break;
		case Token_Ni32:
			*out_type = NI32;
			break;
		case Token_Nf64:
			*out_type = NF64;
			break;
		default:exit(INTERNAL_COMP_ERROR);
	}
}//OK

void ExamineVal (tAstNode *node, void **out_val, tType *out_type){
	switch (node->structure.val.token.type){
		case Token_Integer:
			*out_type = I32;
			*out_val = (int *) malloc(sizeof(int));
			int *i_ptr = (int *)(*out_val);
			*i_ptr = node->structure.val.token.value.integer;
			break;
		case Token_Float:
			*out_type = F64;
			*out_val = (double *) malloc(sizeof(double));
			double *d_ptr = (double *)(*out_val);
			*d_ptr = node->structure.val.token.value.decimal;
			break;
			break;
		case Token_string:
			exit(SEMANTIC_OTHER_ERROR);
			break;
		default:exit(INTERNAL_COMP_ERROR);
	}
}//OK

void ExamineDecl (tAstNode *node, tSymtableList *symlist){
	tSymtableListElem *tree = symlist->first;
	tBstNodeContent *tmp = NULL;
	while(tree){
		if(BstSearch(tree->root_ptr, node->structure.var_decl.token.value.string, &tmp)) exit(SEMANTIC_REDEF_ERROR);
		tree = tree->next;
	}
	tBstNodeContent content;
	content.type = VARIABLE;
	content.value = (tVarVals *) malloc(sizeof(tVarVals));
	tVarVals *vals = (tVarVals *) content.value;
	vals->is_used = false;
	if (node->type == CONST_DECL) vals->is_constant = true;
	else if(node->type == VAR_DECL) vals->is_constant = false;
	else exit(99);
	tTokenType type;
	switch (node->structure.var_decl.type){
		case VOID: type = Token_void;
				   break;
		case I32:  type = Token_i32;
				   break;
		case F64:  type = Token_f64;
				   break;
		case U8:   type = Token_u8;
				   break;
		case NI32: type = Token_Ni32;
				   break;
		case NF64: type = Token_Nf64;
				   break;
		case NU8:  type = Token_Nu8;
				   break;
		case UNDEF:type = Token_Empty;
				   break;
	}
	vals->type = type;
	vals->value = NULL; 
	BstInsert(&symlist->first->root_ptr, node->structure.var_decl.token.value.string, content);
}//OK

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
			//ExamineVar (node, symlist);
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
			ExamineDecl(node, symlist);
			break;
		case FUNC_CALL:
			ExamineFunctionCall(node, symlist);
			break;
		case FUNC_DEF:
			SymtableListAdd(symlist, node->structure.func_def.internal_symtable);
			ExamineSemantics(node->structure.func_def.code, symlist);
			SymtableListRemove(symlist);
			BstDispose(&node->structure.func_def.loc_symtree);
			node->structure.func_def.loc_symtree = NULL;
			break;
		case RET:
			break;
	}
}
