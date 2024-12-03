/**
 * Implementation of semantic analysis of imperative language IFJ24
 * @file semantics.c
 * @brief Implementation of semantic analysis
 * @author Nikola Jordanov xjordan00
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "ast.h"
#include "symtable.h"
#include "error.h"


void InitSymtableList (tSymtableList *list){
	list->first = NULL;
}

void SymtableListAdd (tSymtableList *list, tBstNode *symtree){
	tSymtableListElem *tmp = (tSymtableListElem *) malloc(sizeof(tSymtableListElem));
	if (!tmp) exit (99);
	tmp->root_ptr = symtree;
	tmp->next = list->first;
	list->first = tmp;
}

void SymtableListRemove (tSymtableList *list){
	tSymtableListElem *tmp = list->first;
	list->first = list->first->next;
	BstDispose(&tmp->root_ptr);
	free(tmp);
}

void AssignVals (tVarVals **vals, tComData in_data){
	if (!(*vals)->value || !(*vals)->is_constant){
		switch((*vals)->type){
			case Token_i32:
				if (in_data.type != I32) exit(SEMANTIC_COMP_ERROR);
				AssignInt(vals, in_data.val);
				break;
			case Token_f64:
				if (in_data.type != F64) exit(SEMANTIC_COMP_ERROR);
				AssignDouble(vals, in_data.val);
				break;
			case Token_u8:
				if (in_data.type != U8) exit(SEMANTIC_COMP_ERROR);
				(*vals)->value = in_data.val;
				break;
			case Token_Ni32:
				if (in_data.type != NI32 && in_data.type != I32) exit(SEMANTIC_COMP_ERROR);
				AssignInt(vals, in_data.val);
				break;
			case Token_Nf64:
				if (in_data.type != NF64 && in_data.type != F64) exit(SEMANTIC_COMP_ERROR);
				AssignDouble(vals, in_data.val);
				break;
			case Token_Nu8:
				if (in_data.type != NU8 && in_data.type != U8) exit(SEMANTIC_COMP_ERROR);
				(*vals)->value = in_data.val;
				break;
			case Token_Empty:
				switch (in_data.type){
					case I32:
						(*vals)->type = Token_i32;
						goto i32;
					case NI32:
						(*vals)->type = Token_Ni32;
i32:
						AssignInt(vals, in_data.val);
						break;
					case F64:
						(*vals)->type = Token_f64;
						goto f64;
					case NF64:
						(*vals)->type = Token_Nf64;
f64:
						AssignDouble(vals, in_data.val);
						break;
					case U8:
						(*vals)->type = Token_u8;
						goto u8;
					case NU8:
						(*vals)->type = Token_Nu8;
u8:
						(*vals)->value = in_data.val;
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

void AssignType(tVarVals **vals,tType in_type){
	switch ((*vals)->type){
		case Token_u8:
			if (in_type!=U8) exit(SEMANTIC_COMP_ERROR);
			break;
		case Token_i32:
			if (in_type!=I32) exit(SEMANTIC_COMP_ERROR);
			break;
		case Token_f64:
			if (in_type!=F64) exit(SEMANTIC_COMP_ERROR);
			break;
		case Token_Nu8:
			if (in_type!=NU8 && in_type!=U8) exit(SEMANTIC_COMP_ERROR);
			break;
		case Token_Ni32:
			if (in_type!=NI32 && in_type!=I32) exit(SEMANTIC_COMP_ERROR);
			break;
		case Token_Nf64:
			if (in_type!=NF64 && in_type!=F64) exit(SEMANTIC_COMP_ERROR);
			break;
		case Token_Empty:
			switch (in_type){
				case U8:
					(*vals)->type = Token_u8;
					break;
				case NU8:
					(*vals)->type = Token_Nu8;
					break;
				case I32:
					(*vals)->type = Token_i32;
					break;
				case NI32:
					(*vals)->type = Token_Ni32;
					break;
				case F64:
					(*vals)->type = Token_f64;
					break;
				case NF64:
					(*vals)->type = Token_Nf64;
					break;
				default:exit(SEMANTIC_COMP_ERROR);
			}
			break;
		default: exit(INTERNAL_COMP_ERROR);
	}
}

void ExamineVar (tAstNode *node, tSymtableList *symlist, tComData *in_data, tComData *out_data){
	tBstNodeContent *content = NULL;
	tSymtableListElem *tree = symlist->first;
	while (tree){
		if (BstSearch(tree->root_ptr, node->structure.var.token.value.string, &content)) break;
		tree = tree->next;
	}
	if (!tree) exit(SEMANTIC_UNDEF_ERROR);

	void **vals = &content->value;
	tVarVals **v = (tVarVals**) vals;
	if (!in_data) goto no_data;
	if ((*in_data).val){
		AssignVals(v, *in_data);
		return;
	}
	else if ((*in_data).type != UNDEF){
		AssignType(v, (*in_data).type);
		return;
	}
no_data:
	(*v)->is_used = true;
	out_data->val = (*v)->value;
	if ((*v)->is_constant && (*v)->value) out_data->is_var = false;
	else out_data->is_var = true;
	switch ((*v)->type){
		case Token_u8:
			out_data->type = U8;
			break;
		case Token_i32:
			out_data->type = I32;
			break;
		case Token_f64:
			out_data->type = F64;
			break;
		case Token_Nu8:
			out_data->type = NU8;
			break;
		case Token_Ni32:
			out_data->type = NI32;
			break;
		case Token_Nf64:
			out_data->type = NF64;
			break;
		default:exit(INTERNAL_COMP_ERROR);
	}
}

void ExamineVal (tAstNode *node, tComData *out_data){
	out_data->is_var = false;
	switch (node->structure.val.token.type){
		case Token_Integer:
			out_data->type = I32;
			out_data->val = (int *) malloc(sizeof(int));
			int *i_ptr = (int *)(out_data->val);
			*i_ptr = node->structure.val.token.value.integer;
			break;
		case Token_Float:
			out_data->type = F64;
			out_data->val = (double *) malloc(sizeof(double));
			double *d_ptr = (double *)(out_data->val);
			*d_ptr = node->structure.val.token.value.decimal;
			break;
			break;
		case Token_string:
			exit(SEMANTIC_OTHER_ERROR);
			break;
		default:exit(INTERNAL_COMP_ERROR);
	}
}

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
		default:exit(INTERNAL_COMP_ERROR);
	}
	vals->type = type;
	vals->value = NULL; 
	BstInsAndReal(&symlist->first->root_ptr, node->structure.var_decl.token.value.string, content);
}

void ExamineBuiltInFunc(tAstNode *node, tType *out_type, tSymtableList *symlist){
	bBuiltinFuncs func_type = node->structure.func_call.name_token.value.BuiltInFunc;
	tArgs *args = node->structure.func_call.args;
	int arg_cnt = node->structure.func_call.arg_cnt;
	switch (func_type) {
		case BF_write:
			if (arg_cnt!=1)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			*out_type = VOID;
			break;
		case BF_readstr:
			if (arg_cnt!=0) 
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			*out_type = NU8;
			break;
		case BF_readi32:
			if (arg_cnt!=0) 
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			*out_type = NI32;
			break;
		case BF_readf64:
			if (arg_cnt!=0) 
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			*out_type = NF64;
			break;
		case BF_string:
			if (arg_cnt!=1) 
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (args->token.type!=Token_string){
				if (args->token.type!=Token_FuncID)
					exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				else{
					tBstNodeContent *content;
					tSymtableListElem *tree = symlist->first;
					while(tree){
						if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
						tree = tree->next;
					}
					if(!tree)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
					if(((tVarVals*)content->value)->type != Token_u8)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				}
			}
			*out_type = U8;
			break;
		case BF_concat:
			if (arg_cnt!=2) 
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (args->token.type!=Token_FuncID)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			tBstNodeContent *content;
			tSymtableListElem *tree = symlist->first;
			while(tree){
				if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
				tree = tree->next;
			}
			if(!tree)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (((tVarVals*)content->value)->type != Token_u8)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			args = args->next;//TODO
			if (args->token.type!=Token_FuncID)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			tree = symlist->first;
			while(tree){
				if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
				tree = tree->next;
			}
			if(!tree)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (((tVarVals*)content->value)->type != Token_u8)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			*out_type = U8;
			break;
		case BF_length:
			if (arg_cnt!=1) 
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (args->token.type!=Token_FuncID)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			tree = symlist->first;
			while(tree){
				if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
				tree = tree->next;
			}
			if(!tree)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (((tVarVals*)content->value)->type != Token_u8)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			*out_type = I32;
			break;
		case BF_i2f:
			if (arg_cnt!=1) 
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (args->token.type!=Token_Integer){
				if (args->token.type!=Token_FuncID)
					exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				else{
					tBstNodeContent *content;
					tSymtableListElem *tree = symlist->first;
					while(tree){
						if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
						tree = tree->next;
					}
					if(!tree)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
					if(((tVarVals*)content->value)->type != Token_i32)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				}	
			}
			*out_type = F64;
			break;
		case BF_f2i:
			if (arg_cnt!=1) 
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (args->token.type!=Token_Float){
				if (args->token.type != Token_FuncID)
					exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				else{
					tBstNodeContent *content;
					tSymtableListElem *tree = symlist->first;
					while(tree){
						if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
						tree = tree->next;
					}
					if(!tree)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
					if(((tVarVals*)content->value)->type != Token_f64)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				}
			}
			*out_type = I32;
			break;
		case BF_substring:
			if (arg_cnt!=3) 
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (args->token.type!=Token_FuncID)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			tree = symlist->first;
			while(tree){
				if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
				tree = tree->next;
			}
			if(!tree)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (((tVarVals*)content->value)->type != Token_u8)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);

			args = args->next;
			if (args->token.type!=Token_Integer){
				if (args->token.type!=Token_FuncID)
					exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				else{
					tBstNodeContent *content;
					tSymtableListElem *tree = symlist->first;
					while(tree){
						if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
						tree = tree->next;
					}
					if(!tree)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
					if(((tVarVals*)content->value)->type != Token_i32)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				}	
			}
			args = args->next;
			if (args->token.type!=Token_Integer){
				if (args->token.type!=Token_FuncID)
					exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				else{
					tBstNodeContent *content;
					tSymtableListElem *tree = symlist->first;
					while(tree){
						if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
						tree = tree->next;
					}
					if(!tree)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
					if(((tVarVals*)content->value)->type != Token_i32)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				}	
			}
			*out_type = NU8;
			break;
		case BF_ord:
			if (arg_cnt!=2) 
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (args->token.type!=Token_FuncID)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			tree = symlist->first;
			while(tree){
				if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
				tree = tree->next;
			}
			if(!tree)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (((tVarVals*)content->value)->type != Token_u8)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);

			args = args->next;
			if (args->token.type!=Token_Integer){
				if (args->token.type!=Token_FuncID)
					exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				else{
					tBstNodeContent *content;
					tSymtableListElem *tree = symlist->first;
					while(tree){
						if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
						tree = tree->next;
					}
					if(!tree)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
					if(((tVarVals*)content->value)->type != Token_i32)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				}	
			}
			*out_type = I32;
			break;
		case BF_chr:
			if (arg_cnt!=1) 
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (args->token.type!=Token_Integer){
				if (args->token.type!=Token_FuncID)
					exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				else{
					tBstNodeContent *content;
					tSymtableListElem *tree = symlist->first;
					while(tree){
						if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
						tree = tree->next;
					}
					if(!tree)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
					if(((tVarVals*)content->value)->type != Token_i32)
						exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
				}	
			}
			*out_type = U8;
			break;
		case BF_strcmp:
			if (arg_cnt!=2) 
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (args->token.type!=Token_FuncID)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			tree = symlist->first;
			while(tree){
				if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
				tree = tree->next;
			}
			if(!tree)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (((tVarVals*)content->value)->type != Token_u8)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			
			args = args->next;
			if (args->token.type!=Token_FuncID)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			tree = symlist->first;
			while(tree){
				if(BstSearch(symlist->first->root_ptr, args->token.value.string, &content))break;
				tree = tree->next;
			}
			if(!tree)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (((tVarVals*)content->value)->type != Token_u8)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			*out_type = I32;
			break;
	}
}

tType FuncType(tTokenType tok_type){
	switch (tok_type){
		case Token_u8:
			return U8;
		case Token_i32:
			return I32;
		case Token_f64:
			return F64;
		case Token_Nu8:
			return NU8;
		case Token_Ni32:
			return NI32;
		case Token_Nf64:
			return NF64;
		case Token_void:
			return VOID;
		default:exit(INTERNAL_COMP_ERROR);
	}
}

void ExamineFunctionCall (tAstNode *node, tSymtableList *symlist, tType *out_type){
	tBstNodeContent *content = NULL;
	tSymtableListElem *tree = symlist->first;
	tArgDef *symtable_data = NULL;
	tArgs *arg_data = NULL;

	if (node->structure.func_call.name_token.type == Token_BuildIn_Func){
		ExamineBuiltInFunc(node, out_type, symlist);
		return;
	}

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
		if (arg_data->token.type == Token_string)
			exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
		if (arg_data->token.type == Token_FuncID){
			tree = symlist->first;
			while (tree){
				if (BstSearch(tree->root_ptr, arg_data->token.value.string, &content)) break;
				tree = tree->next;
			}
			if (!tree) exit(SEMANTIC_UNDEF_ERROR);
			if (content->type != VARIABLE) exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (((tVarVals*)content->value)->type == Token_u8 && symtable_data->type_token.type != Token_u8)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (((tVarVals*)content->value)->type == Token_i32 && symtable_data->type_token.type != Token_i32)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (((tVarVals*)content->value)->type == Token_f64 && symtable_data->type_token.type != Token_f64)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (((tVarVals*)content->value)->type == Token_Nu8 && symtable_data->type_token.type != Token_Nu8)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (((tVarVals*)content->value)->type == Token_Ni32 && symtable_data->type_token.type != Token_Ni32)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
			if (((tVarVals*)content->value)->type == Token_Nf64 && symtable_data->type_token.type != Token_Nf64)
				exit(SEMANTIC_WRONG_NUM_OF_PAR_ERROR);
		}
		arg_data = arg_data->next;
		symtable_data = symtable_data->next;
	}
	*out_type = FuncType(symtable_data->type_token.type);
}

void ExamineAssign (tAstNode *node, tSymtableList *symlist){
	tComData data;
	ExamineSemantics(node->structure.assign.src, symlist, NULL, &data, NULL);
	ExamineVar(node->structure.assign.dst, symlist, &data, &data);
}

void CmpVars (tComData op1_data, tComData op2_data, tComData *out_data){
	switch (op1_data.type){
		case F64:
		case I32:
			if (op2_data.type != op1_data.type)
				exit(SEMANTIC_COMP_ERROR);
			out_data->is_var = true;
			out_data->type = op1_data.type;
			out_data->val = NULL;
			break;
		case NF64: case NI32: case BIN:
			exit(SEMANTIC_COMP_ERROR);
		default:exit(INTERNAL_COMP_ERROR);
	}
}

void CmpVarLit (tComData op1_data, tComData op2_data, tComData *out_data, bool is_div){
	switch(op1_data.type){
		case I32:
			if (op2_data.type == I32){
				out_data->type = I32;
				out_data->is_var = true;
				out_data->val = NULL;
			}
			else if(*(double*)op2_data.val-(int)*(double*)op2_data.val == 0.0) {
				out_data->type = I32;
				out_data->is_var = true;
				out_data->val = NULL;
			}
			else exit(SEMANTIC_COMP_ERROR);
			break;
		case F64:
			if (op2_data.type != I32 && op2_data.type != F64)
				exit(SEMANTIC_COMP_ERROR);
			if (is_div && op2_data.type == I32)
				exit(SEMANTIC_COMP_ERROR);
			out_data->type = F64;
			out_data->is_var = true;
				out_data->val = NULL;
			break;
		case NF64: case NI32: case BIN:
				exit(SEMANTIC_COMP_ERROR);
			break;
		default: exit(INTERNAL_COMP_ERROR);
	}
}

void ExamineBinOp(tAstNode *node, tSymtableList *symlist, tComData *out_data){
	tComData op1_data;
	tComData op2_data;
	ExamineSemantics(node->structure.bin_op.op1, symlist, NULL, &op1_data, NULL);
	ExamineSemantics(node->structure.bin_op.op1, symlist, NULL, &op2_data, NULL);
	if(op1_data.type == U8 || op1_data.type == NU8 || op2_data.type == U8 || op2_data.type == NU8)
		exit(SEMANTIC_COMP_ERROR);
	switch (node->structure.bin_op.token.type){
		case Token_Plus:
			if(op1_data.is_var && op2_data.is_var)
				CmpVars(op1_data, op2_data, out_data);
			else if(!op1_data.is_var && !op2_data.is_var){
				switch (op1_data.type){
					case F64:
						if (op2_data.type == F64){
							out_data->type = F64;
							out_data->is_var = false;
							out_data->val = (double*) malloc(sizeof(double));
							if (!out_data->val) exit(INTERNAL_COMP_ERROR);
							double *d_val = (double*) out_data->val;
							*d_val = *(double*)op1_data.val + *(double*)op2_data.val;
						}
						else if (op2_data.type == I32){
							int op1_val2int = (int)*(double*)op1_data.val;
							if (*(double*)op1_data.val-op1_val2int == 0.0){
								out_data->type = I32;
								out_data->is_var = false;
								out_data->val = (int*) malloc(sizeof(int));
								if (!out_data->val) exit(INTERNAL_COMP_ERROR);
								int *i_val = (int*) out_data->val;
								*i_val = op1_val2int + *(int*)op2_data.val;
							}
							else{
								out_data->type = F64;
								out_data->is_var = false;
								out_data->val = (double*) malloc(sizeof(double));
								if (!out_data->val) exit(INTERNAL_COMP_ERROR);
								double *d_val = (double*) out_data->val;
								*d_val = *(double*)op1_data.val + *(double*)op2_data.val;
							}
						}
						else exit(SEMANTIC_COMP_ERROR);
						break;
					case I32:
						if (op2_data.type == F64){
							int op2_val2int = (int)*(double*)op2_data.val;
							if (*(double*)op2_data.val - op2_val2int == 0.0){
								out_data->type = I32;
								out_data->is_var = false;
								out_data->val = (int*) malloc(sizeof(int));
								if (!out_data->val) exit(INTERNAL_COMP_ERROR);
								int *i_val = (int*) out_data->val;
								*i_val = op2_val2int + *(int*)op1_data.val;
							}
							else{
								out_data->type = F64;
								out_data->is_var = false;
								out_data->val = (double*) malloc(sizeof(double));
								double *f_val = (double*) out_data->val;
								*f_val = *(double*)op1_data.val + *(double*)op2_data.val;
							}
						}
						else{ 
							out_data->type = I32;
							out_data->is_var = false;
							out_data->val = (int *) malloc(sizeof(int));
							int *i_val = (int*) out_data->val;
							*i_val = *(int*)op1_data.val + *(int*)op2_data.val;
						}
						break;
					case NF64: case NI32: case BIN:
						exit(SEMANTIC_COMP_ERROR);
					default:exit(INTERNAL_COMP_ERROR);
				}
			}
			else{
				if(op1_data.is_var){
				CmpVarLit(op1_data, op2_data, out_data, false);
				}
				else CmpVarLit(op2_data, op1_data, out_data, false);
			}
			break;
		case Token_Minus:
			if(op1_data.is_var && op2_data.is_var)
				CmpVars(op1_data, op2_data, out_data);
			else if(!op1_data.is_var && !op2_data.is_var){
				switch (op1_data.type){
					case F64:
						if (op2_data.type == F64){
							out_data->type = F64;
							out_data->is_var = false;
							out_data->val = (double*) malloc(sizeof(double));
							if (!out_data->val) exit(INTERNAL_COMP_ERROR);
							double *d_val = (double*) out_data->val;
							*d_val = *(double*)op1_data.val - *(double*)op2_data.val;
						}
						else if (op2_data.type == I32){
							int op1_val2int = (int)*(double*)op1_data.val;
							if (*(double*)op1_data.val-op1_val2int == 0.0){
								out_data->type = I32;
								out_data->is_var = false;
								out_data->val = (int*) malloc(sizeof(int));
								if (!out_data->val) exit(INTERNAL_COMP_ERROR);
								int *i_val = (int*) out_data->val;
								*i_val = op1_val2int - *(int*)op2_data.val;
							}
							else{
								out_data->type = F64;
								out_data->is_var = false;
								out_data->val = (double*) malloc(sizeof(double));
								if (!out_data->val) exit(INTERNAL_COMP_ERROR);
								double *d_val = (double*) out_data->val;
								*d_val = *(double*)op1_data.val - *(double*)op2_data.val;
							}
						}
						else exit(SEMANTIC_COMP_ERROR);
						break;
					case I32:
						if (op2_data.type == F64){
							int op2_val2int = (int)*(double*)op2_data.val;
							if (*(double*)op2_data.val - op2_val2int == 0.0){
								out_data->type = I32;
								out_data->is_var = false;
								out_data->val = (int*) malloc(sizeof(int));
								if (!out_data->val) exit(INTERNAL_COMP_ERROR);
								int *i_val = (int*) out_data->val;
								*i_val = op2_val2int - *(int*)op1_data.val;
							}
							else{
								out_data->type = F64;
								out_data->is_var = false;
								out_data->val = (double*) malloc(sizeof(double));
								double *f_val = (double*) out_data->val;
								*f_val = *(double*)op1_data.val - *(double*)op2_data.val;
							}
						}
						else{ 
							out_data->type = I32;
							out_data->is_var = false;
							out_data->val = (int *) malloc(sizeof(int));
							int *i_val = (int*) out_data->val;
							*i_val = *(int*)op1_data.val - *(int*)op2_data.val;
						}
						break;
					case NF64: case NI32: case BIN:
						exit(SEMANTIC_COMP_ERROR);
					default:exit(INTERNAL_COMP_ERROR);
				}
			}
			else{
				if(op1_data.is_var){
				CmpVarLit(op1_data, op2_data, out_data, false);
				}
				else CmpVarLit(op2_data, op1_data, out_data, false);
			}
			break;
		case Token_Multiply:
			if(op1_data.is_var && op2_data.is_var)
				CmpVars(op1_data, op2_data, out_data);
			else if(!op1_data.is_var && !op2_data.is_var){
				switch (op1_data.type){
					case F64:
						if (op2_data.type == F64){
							out_data->type = F64;
							out_data->is_var = false;
							out_data->val = (double*) malloc(sizeof(double));
							if (!out_data->val) exit(INTERNAL_COMP_ERROR);
							double *d_val = (double*) out_data->val;
							*d_val = *(double*)op1_data.val * *(double*)op2_data.val;
						}
						else if (op2_data.type == I32){
							int op1_val2int = (int)*(double*)op1_data.val;
							if (*(double*)op1_data.val-op1_val2int == 0.0){
								out_data->type = I32;
								out_data->is_var = false;
								out_data->val = (int*) malloc(sizeof(int));
								if (!out_data->val) exit(INTERNAL_COMP_ERROR);
								int *i_val = (int*) out_data->val;
								*i_val = op1_val2int * *(int*)op2_data.val;
							}
							else{
								out_data->type = F64;
								out_data->is_var = false;
								out_data->val = (double*) malloc(sizeof(double));
								if (!out_data->val) exit(INTERNAL_COMP_ERROR);
								double *d_val = (double*) out_data->val;
								*d_val = *(double*)op1_data.val * *(double*)op2_data.val;
							}
						}
						else exit(SEMANTIC_COMP_ERROR);
						break;
					case I32:
						if (op2_data.type == F64){
							int op2_val2int = (int)*(double*)op2_data.val;
							if (*(double*)op2_data.val - op2_val2int == 0.0){
								out_data->type = I32;
								out_data->is_var = false;
								out_data->val = (int*) malloc(sizeof(int));
								if (!out_data->val) exit(INTERNAL_COMP_ERROR);
								int *i_val = (int*) out_data->val;
								*i_val = op2_val2int * *(int*)op1_data.val;
							}
							else{
								out_data->type = F64;
								out_data->is_var = false;
								out_data->val = (double*) malloc(sizeof(double));
								double *f_val = (double*) out_data->val;
								*f_val = *(double*)op1_data.val * *(double*)op2_data.val;
							}
						}
						else{ 
							out_data->type = I32;
							out_data->is_var = false;
							out_data->val = (int *) malloc(sizeof(int));
							out_data->val = (int *) malloc(sizeof(int));
							int *i_val = (int*) out_data->val;
							*i_val = *(int*)op1_data.val * *(int*)op2_data.val;
						}
						break;
					case NF64: case NI32: case BIN:
						exit(SEMANTIC_COMP_ERROR);
					default:exit(INTERNAL_COMP_ERROR);
				}
			}
			else{
				if(op1_data.is_var){
				CmpVarLit(op1_data, op2_data, out_data, false);
				}
				else CmpVarLit(op2_data, op1_data, out_data, false);
			}
			break;
		case Token_Divide:
			if(op1_data.is_var && op2_data.is_var)
				CmpVars(op1_data, op2_data, out_data);
			else if(!op1_data.is_var && !op2_data.is_var){
				switch (op1_data.type){
					case F64:
						if (op2_data.type == I32){
							int op1_val2int = (int)*(double*)op1_data.val;
							if (*(double*)op1_data.val-op1_val2int != 0.0)
								exit(SEMANTIC_COMP_ERROR);
							out_data->type = I32;
							out_data->is_var = false;
							out_data->val = (int*) malloc(sizeof(int));
							int *i_val = out_data->val;
							*i_val = (int)(op1_val2int / *(int*)op2_data.val);
						}
						else if (op2_data.type == F64){
							out_data->type = F64;
							out_data->is_var = false;
							out_data->val = (double*) malloc(sizeof(double));
							double *d_val = (double*) out_data->val;
							*d_val = *(double*)op1_data.val / *(double*)op2_data.val;
						}
						else exit(SEMANTIC_COMP_ERROR);
						break;
					case I32:
						if (op2_data.type == F64){
							int op2_val2int = (int)*(double*)op2_data.val;
							if (*(double*)op2_data.val-op2_val2int != 0.0)
								exit(SEMANTIC_COMP_ERROR);
							out_data->type = I32;
							out_data->is_var = false;
							out_data->val = (int*) malloc(sizeof(int));
							int *f_val = (int*) out_data->val;
							*f_val = (int)(*(int*)op1_data.val / op2_val2int);
						}
						else{ 
							out_data->type = I32;
							out_data->is_var = false;
							out_data->val = (int *) malloc(sizeof(int));
							int *i_val = (int*) out_data->val;
							*i_val = (int)(*(int*)op1_data.val / *(int*)op2_data.val);
						}
						break;
					case NF64: case NI32: case BIN:
						exit(SEMANTIC_COMP_ERROR);
					default:exit(INTERNAL_COMP_ERROR);
				}
			}
			else{
				if(op1_data.is_var){
				CmpVarLit(op1_data, op2_data, out_data, true);
				}
				else CmpVarLit(op2_data, op1_data, out_data, true);
			}
			break;
		case Token_Equal:
		case Token_Not_Equal:
		case Token_Lesser:
		case Token_Greater:
		case Token_Lesser_Equal:
		case Token_Greater_Equal:
			if(op1_data.is_var && op2_data.is_var){
				CmpVars(op1_data, op2_data, out_data);
				out_data->type = BIN;
			}
			else if(!op1_data.is_var && !op2_data.is_var){
				if (op1_data.type != I32 && op1_data.type != F64)
					exit(SEMANTIC_COMP_ERROR);
				if (op2_data.type != I32 && op2_data.type != F64)
					exit(SEMANTIC_COMP_ERROR);
				out_data->type = BIN;
				out_data->is_var = true;
				out_data->val = NULL;
			}
			else{
				if(op1_data.is_var){
				CmpVarLit(op1_data, op2_data, out_data, false);
				}
				else CmpVarLit(op2_data, op1_data, out_data, false);
				out_data->type = BIN;
			}
			break;
		default:exit(INTERNAL_COMP_ERROR);
	}
}

void NNDecl(tAstNode *node, tSymtableList *symlist){
	tBstNodeContent content;
	content.type = VARIABLE;
	content.value = (tVarVals *) malloc(sizeof(tVarVals));
	tVarVals *vals = (tVarVals *) content.value;
	vals->is_used = false;
	vals->is_constant = true;
	vals->type = Token_Empty;
	vals->value = node->structure.var.token.value.string;
	BstInsAndReal(&symlist->first->root_ptr, node->structure.var.token.value.string, content);
}

void ExamineWhile(tAstNode *node, tSymtableList *symlist, tTokenType *ret_type){
	SymtableListAdd(symlist, node->structure.while_loop.loc_symtree);
	tComData data;
	ExamineSemantics(node->structure.while_loop.expr, symlist, NULL, &data, ret_type);
	if (node->structure.while_loop.nn_id){
		if (data.type != NU8 && data.type != NI32 && data.type != NF64) exit(SEMANTIC_OTHER_ERROR);
		NNDecl(node->structure.while_loop.nn_id, symlist);
		ExamineVar(node->structure.while_loop.nn_id, symlist, &data, &data);
	}
	else{
		if (data.type != BIN) exit(SEMANTIC_COMP_ERROR);
	}
	ExamineSemantics(node->structure.while_loop.code, symlist, NULL, &data, ret_type);
	BstCheckUse(symlist->first->root_ptr);
	SymtableListRemove(symlist);
}

void ExamineIf(tAstNode *node, tSymtableList *symlist, tTokenType *ret_type){
	SymtableListAdd(symlist, node->structure.if_block.if_symtree);
	tComData data;
	ExamineSemantics(node->structure.if_block.expr, symlist, NULL, &data, ret_type);
	if (node->structure.if_block.nn_id){
		if (data.type != NU8 && data.type != NI32 && data.type != NF64) exit(SEMANTIC_OTHER_ERROR);
		NNDecl(node->structure.if_block.nn_id, symlist);
		ExamineVar(node->structure.if_block.nn_id, symlist, &data, &data);
	}
	else{
		if(data.type!=BIN) exit(SEMANTIC_COMP_ERROR);
	}
	ExamineSemantics(node->structure.if_block.if_code, symlist, NULL, &data, ret_type);
	ExamineSemantics(node->structure.if_block.else_code, symlist, NULL, &data, ret_type);
	BstCheckUse(symlist->first->root_ptr);
	SymtableListRemove(symlist);
}

void ExamineRet(tAstNode *node, tSymtableList *symlist, tTokenType *ret_type){
	tComData data;
	data.type = VOID;
	ExamineSemantics(node, symlist, NULL, &data, NULL);
	switch (data.type){
		case U8:
			if (*ret_type==Token_u8) exit(SEMANTIC_RETURN_ERROR);
			break;
		case I32:
			if (*ret_type==Token_i32) exit(SEMANTIC_RETURN_ERROR);
			break;
		case F64:
			if (*ret_type==Token_f64) exit(SEMANTIC_RETURN_ERROR);
			break;
		case NU8:
			if (*ret_type==Token_Nu8) exit(SEMANTIC_RETURN_ERROR);
			break;
		case NI32:
			if (*ret_type==Token_Ni32) exit(SEMANTIC_RETURN_ERROR);
			break;
		case NF64:
			if (*ret_type==Token_Nf64) exit(SEMANTIC_RETURN_ERROR);
			break;
		case VOID:
			if (*ret_type==Token_void) exit(SEMANTIC_RETURN_ERROR);
			break;
		default:exit(INTERNAL_COMP_ERROR);
	}
}

void BstCheckUse(tBstNode *tree){
	if (!tree) return;
	BstCheckUse(tree->left);
	BstCheckUse(tree->right);

	if(tree->content.type == FUNCTION){
		return;
	}
	tVarVals *vals = (tVarVals*)tree->content.value;
	if (!vals->is_used) exit(SEMANTIC_UNUSED_VAR_ERROR);
}

int CheckMain(tBstNode *tree){
	if (!tree) return 0;
	if (CheckMain(tree->left)) return 1;
	if (CheckMain(tree->right)) return 1;
	if (!strcmp(tree->key, "main")) return 1;
	return 0;
}

void ExamineSemantics (tAstNode *node, tSymtableList *symlist, tComData *in_data, tComData *out_data, tTokenType *ret_type){
	if(!node) return;
	switch (node->type){
		case STATEMENT:
			ExamineSemantics(node->structure.statement.function, symlist, in_data, out_data, ret_type);
			ExamineSemantics(node->structure.statement.next_statement, symlist, in_data, out_data, ret_type);
			break;
		case CODE:
			ExamineSemantics(node->structure.code.operation, symlist, in_data, out_data, ret_type);
			ExamineSemantics(node->structure.code.next_code, symlist, in_data, out_data, ret_type);
			break;
		case VAR:
			ExamineVar(node, symlist, in_data, out_data);
			break;
		case VAL:
			ExamineVal(node, out_data);
			break;
		case WHILE:
			ExamineWhile(node, symlist, ret_type);
			break;
		case IF:
			ExamineIf(node, symlist, ret_type);
			break;
		case BIN_OP:
			ExamineBinOp(node, symlist, out_data);
			break;
		case ASSIGN:
			ExamineAssign(node, symlist);
			break;
		case CONST_DECL:
		case VAR_DECL:
			ExamineDecl(node, symlist);
			break;
		case FUNC_CALL:
			ExamineFunctionCall(node, symlist, &out_data->type);
			break;
		case FUNC_DEF:
			SymtableListAdd(symlist, node->structure.func_def.internal_symtable);
			ExamineSemantics(node->structure.func_def.code, symlist, in_data, out_data, &node->structure.func_def.ret_type_token.type);
			BstCheckUse(symlist->first->root_ptr);
			SymtableListRemove(symlist);
			BstDispose(&node->structure.func_def.loc_symtree);
			node->structure.func_def.loc_symtree = NULL;
			break;
		case RET:
			ExamineRet(node, symlist, ret_type);
			break;
	}
}
