/**
 * @file ast.c
 * @brief Abstract syntax tree implementation
 * @author Nikola Jordanov xjordan00
 */

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "stack.h"
#include "scanner.h"
#include "error.h"
#include "symtable.h"


void AddStatmentNode (tAstNode **node_dest){
	if(!node_dest) exit(INTERNAL_COMP_ERROR);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(INTERNAL_COMP_ERROR);
	(*node_dest)->type = STATEMENT;
	(*node_dest)->structure.statement.next_statement = NULL;
	(*node_dest)->structure.statement.function = NULL;
}

void AddCodeNode (tAstNode **node_dest){
	if(!node_dest) exit(INTERNAL_COMP_ERROR);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(INTERNAL_COMP_ERROR);
	(*node_dest)->type = CODE;
	(*node_dest)->structure.code.next_code = NULL;
	(*node_dest)->structure.code.operation = NULL;
}

void AddIfBlockNode (tAstNode **node_dest){
	if(!node_dest) exit(INTERNAL_COMP_ERROR);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if (!(*node_dest)) exit(INTERNAL_COMP_ERROR);
	(*node_dest)->type = IF;
	(*node_dest)->structure.if_block.else_code = NULL;
	(*node_dest)->structure.if_block.if_code = NULL;
	(*node_dest)->structure.if_block.expr = NULL;
	(*node_dest)->structure.if_block.nn_id = NULL;
	(*node_dest)->structure.if_block.if_symtree = NULL;
	(*node_dest)->structure.if_block.else_symtree = NULL;
}

void AddWhileNode (tAstNode **node_dest){
	if(!node_dest) exit(INTERNAL_COMP_ERROR);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(INTERNAL_COMP_ERROR);
	(*node_dest)->type = WHILE;
	(*node_dest)->structure.while_loop.code = NULL;
	(*node_dest)->structure.while_loop.expr = NULL;
	(*node_dest)->structure.while_loop.nn_id = NULL;
	(*node_dest)->structure.while_loop.loc_symtree = NULL;
}

void AddAssignNode (tAstNode **node_dest){
	if(!node_dest) exit(INTERNAL_COMP_ERROR);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(INTERNAL_COMP_ERROR);
	(*node_dest)->type = ASSIGN;
	(*node_dest)->structure.assign.dst = NULL;
	(*node_dest)->structure.assign.src = NULL;
}

void AddDeclNode (tAstNode **node_dest, tTokenStack *stack){
	if(!node_dest || !stack) exit(INTERNAL_COMP_ERROR);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(INTERNAL_COMP_ERROR);
	tToken token = TopTStack(stack);
	PopTStack(stack);
	switch (token.type){
		case Token_Dollar:
			(*node_dest)->structure.var_decl.type =UNDEF; break;
		case Token_void:
			(*node_dest)->structure.var_decl.type = VOID; break;
		case Token_i32:
			(*node_dest)->structure.var_decl.type = I32; break;
		case Token_f64:
			(*node_dest)->structure.var_decl.type = F64; break;
		case Token_u8:
			(*node_dest)->structure.var_decl.type = U8; break;
		case Token_Ni32:
			(*node_dest)->structure.var_decl.type = NI32; break;
		case Token_Nf64:
			(*node_dest)->structure.var_decl.type = NF64; break;
		case Token_Nu8:
			(*node_dest)->structure.var_decl.type = NU8; break;
		default: exit(INTERNAL_COMP_ERROR);
	}
	token = TopTStack(stack);
	(*node_dest)->structure.var_decl.token = token;
	PopTStack(stack);
	token = TopTStack(stack);
	PopTStack(stack);
	if (token.type == Token_const)
		(*node_dest)->type = CONST_DECL;
	else if(token.type == Token_var)
		(*node_dest)->type = VAR_DECL;
	else exit(INTERNAL_COMP_ERROR);
}

void AddFuncDefNode (tAstNode **node_dest, tTokenStack *stack, 
		tTokenStack *arg_stack){
	if(!node_dest || !stack) exit(INTERNAL_COMP_ERROR);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(INTERNAL_COMP_ERROR);
	tToken token = TopTStack(stack);
	PopTStack(stack);
	(*node_dest)->type = FUNC_DEF;
	(*node_dest)->structure.func_def.ret_type_token = token;
	token = TopTStack(stack);
	PopTStack(stack);
	(*node_dest)->structure.func_def.token = token;
	(*node_dest)->structure.func_def.internal_symtable = NULL;
	while(!StackIsEmpty(arg_stack)){
		tArgDef *tmp = (tArgDef *) malloc(sizeof(tArgDef));
		if(!tmp) exit(INTERNAL_COMP_ERROR);
		token = TopTStack(arg_stack);
		PopTStack(arg_stack);
		tmp->type_token = token;
		token = TopTStack(arg_stack);
		PopTStack(arg_stack);
		tmp->name_token = token;
		tmp->next = (*node_dest)->structure.func_def.args;
		(*node_dest)->structure.func_def.args = tmp;
	}
	(*node_dest)->structure.func_def.loc_symtree = NULL;
}

void AddFuncCallNode (tAstNode **node_dest, tToken id_t,
		tTokenStack *arg_stack){
	if(!node_dest) exit(INTERNAL_COMP_ERROR);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(INTERNAL_COMP_ERROR);
	(*node_dest)->type = FUNC_CALL;
	(*node_dest)->structure.func_call.arg_cnt = 0;
	(*node_dest)->structure.func_call.name_token = id_t;
	while(arg_stack->top){
		tArgs *tmp = (tArgs *) malloc(sizeof(tArgs));
		if(!tmp) exit(INTERNAL_COMP_ERROR);
		tToken token = TopTStack(arg_stack);
		PopTStack(arg_stack);
		tmp->token = token;
		tmp->next = (*node_dest)->structure.func_call.args;
		(*node_dest)->structure.func_call.args = tmp;
		(*node_dest)->structure.func_call.arg_cnt++;
	}
}

void PrecedenceCheck (tToken *in_t, tTokenStack *input_stack,
		tTokenStack *output_stack){
	if(in_t){
		tToken token_in = *in_t;
		tToken token_top;
		if(!StackIsEmpty(input_stack)) token_top = TopTStack(input_stack);
		else {
			token_top.state = (sState)0;
			token_top.type = Token_Dollar;
			token_top.value.integer = '$';
		}
		int cmp_op = ' ';
		switch (token_in.type){
			case Token_Multiply:
			case Token_Divide:
				switch (token_top.type){
					case Token_Multiply:
					case Token_Divide:
					case Token_Integer:
					case Token_Float:
					case Token_null:
					case Token_FuncID:
					case Token_Rpar:
						cmp_op = '>';
						break;
					case Token_Plus:
					case Token_Minus:
					case Token_Equal:
					case Token_Not_Equal:
					case Token_Lesser:
					case Token_Greater:
					case Token_Lesser_Equal:
					case Token_Greater_Equal:
					case Token_Lpar:
					case Token_Dollar:
						cmp_op = '<';
						break;
					default: exit(INTERNAL_COMP_ERROR);
				}
				break;
			case Token_Plus:
			case Token_Minus:
				switch (token_top.type){
					case Token_Multiply:
					case Token_Divide:
					case Token_Plus:
					case Token_Minus:
					case Token_Integer:
					case Token_Float:
					case Token_null:
					case Token_FuncID:
					case Token_Rpar:
						cmp_op = '>';
						break;
					case Token_Equal:
					case Token_Not_Equal:
					case Token_Lesser:
					case Token_Greater:
					case Token_Lesser_Equal:
					case Token_Greater_Equal:
					case Token_Lpar:
					case Token_Dollar:
						cmp_op = '<';
						break;
					default: exit(INTERNAL_COMP_ERROR);
				}
				break;
			case Token_Equal:
			case Token_Not_Equal:
			case Token_Lesser:
			case Token_Greater:
			case Token_Lesser_Equal:
			case Token_Greater_Equal:
				switch (token_top.type){
					case Token_Multiply:
					case Token_Divide:
					case Token_Plus:
					case Token_Minus:
					case Token_Integer:
					case Token_Float:
					case Token_null:
					case Token_FuncID:
					case Token_Rpar:
						cmp_op = '>';
						break;
					case Token_Equal:
					case Token_Not_Equal:
					case Token_Lesser:
					case Token_Greater:
					case Token_Lesser_Equal:
					case Token_Greater_Equal:
						exit(SYNTAX_ERROR);
						break;
					case Token_Lpar:
					case Token_Dollar:
						cmp_op = '<';
						break;
					default: exit(INTERNAL_COMP_ERROR);
				}
				break;
			case Token_Integer:
			case Token_Float:
			case Token_null:
			case Token_FuncID:
			case Token_Lpar:
				switch (token_top.type){
					case Token_Multiply:
					case Token_Divide:
					case Token_Plus:
					case Token_Minus:
					case Token_Equal:
					case Token_Not_Equal:
					case Token_Lesser:
					case Token_Greater:
					case Token_Lesser_Equal:
					case Token_Greater_Equal:
					case Token_Lpar:
					case Token_Dollar:
						cmp_op = '<';
						break;
					case Token_Integer:
					case Token_Float:
					case Token_null:
					case Token_FuncID:
					case Token_Rpar:
						exit(SYNTAX_ERROR);
						break;
					default: exit(INTERNAL_COMP_ERROR);
				}
				break;
			case Token_Rpar:
				while (token_top.type!=Token_Lpar){
					if (StackIsEmpty(input_stack)) exit(SYNTAX_ERROR);
					PushTStack(output_stack, token_top);
					PopTStack(input_stack);
					token_top = TopTStack(input_stack);
				}
				PopTStack(input_stack);
				return;
				break;
			default: exit(INTERNAL_COMP_ERROR);
		}
		if (cmp_op=='<') PushTStack(input_stack, token_in);
		else if(cmp_op=='>'){
			PushTStack(output_stack, token_top);
			PopTStack(input_stack);
			PrecedenceCheck(in_t, input_stack, output_stack);
		}
		else exit(SYNTAX_ERROR);
	}
	else {
		while(!StackIsEmpty(input_stack)){
			tToken token = TopTStack(input_stack);
			PushTStack(output_stack, token);
			PopTStack(input_stack);
		}
	}
}

void AddRetNode(tAstNode **node_dest){
	if (!node_dest) exit(INTERNAL_COMP_ERROR);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(INTERNAL_COMP_ERROR);
	(*node_dest)->type = RET;
	(*node_dest)->structure.ret.ret_expr = NULL;
	(*node_dest)->structure.ret.type = VOID;
}

void AddExpNodes (tAstNode **node_dest, tTokenStack *stack){
	if(!node_dest) exit(INTERNAL_COMP_ERROR);
	if(!stack) exit(SYNTAX_ERROR);
	if (StackIsEmpty(stack)) return;
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(INTERNAL_COMP_ERROR);
	tToken token = TopTStack(stack);
	PopTStack(stack);
	switch (token.type){
		case Token_Multiply:
		case Token_Divide:
		case Token_Plus:
		case Token_Minus:
		case Token_Equal:
		case Token_Not_Equal:
		case Token_Lesser:
		case Token_Greater:
		case Token_Lesser_Equal:
		case Token_Greater_Equal:
			(*node_dest)->type = BIN_OP;
			(*node_dest)->structure.bin_op.token = token;
			(*node_dest)->structure.bin_op.op1 = NULL;
			(*node_dest)->structure.bin_op.op2 = NULL;
			break;
		case Token_Integer:
		case Token_Float:
		case Token_null:
			(*node_dest)->type = VAL;
			(*node_dest)->structure.val.token = token;
			return;
		case Token_FuncID:
			(*node_dest)->type = VAR;
			(*node_dest)->structure.var.token = token;
			return;
		default: exit(INTERNAL_COMP_ERROR);
	}
	AddExpNodes(&(*node_dest)->structure.bin_op.op1, stack);
	AddExpNodes(&(*node_dest)->structure.bin_op.op2, stack);
}

void AstDispose (tAstNode **tree){
	if(!tree) exit(INTERNAL_COMP_ERROR);
	if(!(*tree)) return;
	switch ((*tree)->type){
		case STATEMENT:
			AstDispose(&(*tree)->structure.statement.next_statement);
			AstDispose(&(*tree)->structure.statement.function);
			break;
		case CODE:
			AstDispose(&(*tree)->structure.code.next_code);
			AstDispose(&(*tree)->structure.code.operation);
		case VAR:
		case VAL:
			break;
		case WHILE:
			AstDispose(&(*tree)->structure.while_loop.code);
			AstDispose(&(*tree)->structure.while_loop.expr);
			AstDispose(&(*tree)->structure.while_loop.nn_id);
			break;
		case IF:
			AstDispose(&(*tree)->structure.if_block.else_code);
			AstDispose(&(*tree)->structure.if_block.if_code);
			AstDispose(&(*tree)->structure.if_block.expr);
			AstDispose(&(*tree)->structure.if_block.nn_id);
			break;
		case BIN_OP:
			AstDispose(&(*tree)->structure.bin_op.op1);
			AstDispose(&(*tree)->structure.bin_op.op2);
			break;
		case ASSIGN:
			AstDispose(&(*tree)->structure.assign.dst);
			AstDispose(&(*tree)->structure.assign.src);
		case CONST_DECL:
		case VAR_DECL:
			break;
		case FUNC_CALL:
			while((*tree)->structure.func_call.args){
				tArgs *tmp = (*tree)->structure.func_call.args;
				(*tree)->structure.func_call.args = tmp;
				free(tmp);
			}
			break;
		case FUNC_DEF:
			while((*tree)->structure.func_def.args){
				tArgDef *tmp = (*tree)->structure.func_def.args;
				(*tree)->structure.func_def.args = tmp;
				free(tmp);
			}
			AstDispose(&(*tree)->structure.func_def.code);
			break;
		case RET:
			AstDispose(&(*tree)->structure.ret.ret_expr);
	}
	free(*tree);
}
