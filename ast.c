/**
 * @file ast.c
 * @brief Implementace abstraktniho syntaktickeho stromu
 * @author Nikola Jordanov xjordan00
 */

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "stack.h"
#include "scanner.h"
#include "error.h"

void AddStatmentNode (tAstNode **node_dest){
	if(!node_dest) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	(*node_dest)->type = STATEMENT;
	(*node_dest)->structure.statement.next_statement = NULL;
	(*node_dest)->structure.statement.function = NULL;
}

void AddCodeNode (tAstNode **node_dest){
	if(!node_dest) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	(*node_dest)->type = CODE;
	(*node_dest)->structure.code.next_code = NULL;
	(*node_dest)->structure.code.operation = NULL;
}

void AddIfBlockNode (tAstNode **node_dest){
	if(!node_dest) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if (!(*node_dest)) exit(99);
	(*node_dest)->type = IF;
	(*node_dest)->structure.if_block.else_code = NULL;
	(*node_dest)->structure.if_block.if_code = NULL;
	(*node_dest)->structure.if_block.expr = NULL;
	(*node_dest)->structure.if_block.nn_id = NULL;
}

void AddWhileNode (tAstNode **node_dest){
	if(!node_dest) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	(*node_dest)->type = WHILE;
	(*node_dest)->structure.while_loop.code = NULL;
	(*node_dest)->structure.while_loop.expr = NULL;
	(*node_dest)->structure.while_loop.nn_id = NULL;
}

void AddAssignNode (tAstNode **node_dest){
	if(!node_dest) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	(*node_dest)->type = ASSIGN;
	(*node_dest)->structure.assign.dst = NULL;
	(*node_dest)->structure.assign.src = NULL;
}

void AddDeclNode (tAstNode **node_dest, tTokenStack *stack){
	if(!node_dest || !stack) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	tToken token = TopTStack(stack);
	PopTStack(stack);
	switch (token.type){
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
		default: exit(99);
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
	else exit(99);
}


void AddFuncDefNode (tAstNode **node_dest, tTokenStack *stack, 
		tTokenStack *arg_stack){
	if(!node_dest || !stack) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	tToken token = TopTStack(stack);
	PopTStack(stack);
	(*node_dest)->type = FUNC_DEF;
	(*node_dest)->structure.func_def.ret_type_token = token;
	token = TopTStack(stack);
	PopTStack(stack);
	(*node_dest)->structure.func_def.token = token;
	while(arg_stack){
		tArgDef *tmp = (tArgDef *) malloc(sizeof(tArgDef));
		if(!tmp) exit(99);
		token = TopTStack(arg_stack);
		PopTStack(arg_stack);
		tmp->type_token = token;
		token = TopTStack(arg_stack);
		PopTStack(arg_stack);
		tmp->name_token = token;
		tmp->next = (*node_dest)->structure.func_def.args;
		(*node_dest)->structure.func_def.args = tmp;
	}

}

void AddFuncCallNode (tAstNode **node_dest, tToken id_t,
		tTokenStack *arg_stack){
	if(!node_dest) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	(*node_dest)->type = FUNC_CALL;
	(*node_dest)->structure.func_call.name_token = id_t;
	while(arg_stack){
		tArgs *tmp = (tArgs *) malloc(sizeof(tArgs));
		if(!tmp) exit(99);
		tToken token = TopTStack(arg_stack);
		PopTStack(arg_stack);
		tmp->token = token;
		tmp->next = (*node_dest)->structure.func_call.args;
		(*node_dest)->structure.func_call.args = tmp;
	}
}


