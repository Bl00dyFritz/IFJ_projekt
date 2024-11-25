/**
* @file parser.c
* @brief implementace parseru (syntaktické analýzy) překladače jayzka IFJ24
* @author Lucie Pojslová (xpojsll00)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "error.h"
#include "scanner.h"
#include "ast.h"
#include "symtable.h"
#include "stack.h"

int prolog(void){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type!=Token_const) exit(SYNTAX_ERROR);
	lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type!=Token_IFJ) exit(SYNTAX_ERROR);
	lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type!=Token_Assign) exit(SYNTAX_ERROR);
	lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type!=Token_AtImport) exit(SYNTAX_ERROR);
	lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type!=Token_Lpar) exit(SYNTAX_ERROR);
	lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type!=Token_string) exit(SYNTAX_ERROR);
	if (strcmp(token.value.string, "ifj24.zig")) exit(SYNTAX_ERROR);
	lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
	return 0;
}//OK

int statement(tAstNode **synt_tree, tToken *in_t,tAstNode ***next_synt_tree){
	tToken token;
	int lex_ret=0;
	if (in_t) token = *in_t;
	else {
		lex_ret=GetToken(&token);
		if(lex_ret) exit(lex_ret);
	}
	AddStatmentNode(synt_tree);
	function(&token, &(*synt_tree)->structure.statement.function);
	*next_synt_tree = &(*synt_tree)->structure.statement.next_statement;
	return 0;
}//OK

int next_statement(tAstNode **synt_tree){
	tToken token;
	tAstNode **current_p = synt_tree;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	switch(token.type){
		case Token_pub: statement(synt_tree, &token, &current_p);
						next_statement(current_p);
		case Token_EOF: break;
		default: exit(SYNTAX_ERROR);
	}
	return 0;
}//OK

int function(tToken *in_t, tAstNode **synt_tree){
    tToken token;
	int lex_ret;
	tTokenStack stack;
	tTokenStack arg_stack;
	InitTStack(&stack);
	InitTStack(&arg_stack);
	tTokenType cmp_type = Token_pub;
	if (in_t) token = *in_t;
	else {
		lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
	}
	int err_found = 1;
	while (token.type==cmp_type){
		if(cmp_type==Token_Lpar) {
			err_found = 0;
			break;
		}
		lex_ret = GetToken(&token);
		if(lex_ret) exit(lex_ret);
		switch (cmp_type){
			case Token_pub: cmp_type=Token_fn; break;
			case Token_fn: cmp_type=Token_FuncID; break;
			case Token_FuncID: 
						   cmp_type=Token_Lpar;
						   PushTStack(&stack, token);
						   break;
			default: exit(99);
		}
	}
	if (err_found) exit(SYNTAX_ERROR);
	argument_list_def(&token, &arg_stack);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	type(&stack);
	AddFuncDefNode(synt_tree, &stack, &arg_stack);
	lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if(token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
	function_body(&token, &(*synt_tree)->structure.func_def.code);
	if(token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
	return 0;
}//OK

int const_init(tToken *in_t, tAstNode **synt_tree){
	tToken token = *in_t;
	const_decl(&token, &token, synt_tree);
	AddCodeNode(&(*synt_tree)->structure.code.next_code);
	tAstNode **next_cnode = &(*synt_tree)->structure.code.next_code;
	const_def(&token, &token, next_cnode);
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
	*synt_tree = *next_cnode;
    return 0;
}//OK

int var_init(tToken *in_t, tAstNode **synt_tree){
	tToken token = *in_t;
	var_decl(&token, &token, synt_tree);
	AddCodeNode(&(*synt_tree)->structure.code.next_code);
	tAstNode **next_cnode = &(*synt_tree)->structure.code.next_code;
	var_def(&token, &token, next_cnode);
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
	*synt_tree = *next_cnode;
    return 0;
}//OK

int const_def(tToken *id_tok, tToken *ret_t, tAstNode **synt_tree){
	tToken token;
	AddAssignNode(&(*synt_tree)->structure.code.operation);
	tAstNode **assign_node = &(*synt_tree)->structure.code.operation;
	tTokenStack stack;
	InitTStack(&stack);
	PushTStack(&stack, *id_tok);
	AddExpNodes(&(*assign_node)->structure.assign.dst, &stack);
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type==Token_Assign) expression(NULL, &token, &(*assign_node)->structure.assign.src);
	*ret_t = token;
    return 0;
}//OK

int var_def(tToken *id_tok, tToken *ret_t, tAstNode **synt_tree){
	tToken token;
	AddAssignNode(&(*synt_tree)->structure.code.operation);
	tAstNode **assign_node = &(*synt_tree)->structure.code.operation;
	tTokenStack stack;
	InitTStack(&stack);
	PushTStack(&stack, *id_tok);
	AddExpNodes(&(*assign_node)->structure.assign.dst, &stack);
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if(token.type==Token_Assign) expression(NULL, &token, &(*assign_node)->structure.assign.src);
	*ret_t = token;
    return 0;
}//OK

int type_decl(tTokenStack *stack){
	tToken token;
	int lex_ret = GetToken(&token);
	if(lex_ret) exit(lex_ret);
	if (token.type==Token_Colon) type(stack);
	else{
		tToken no_type_tok;
		no_type_tok.type = Token_void;
		PushTStack(stack, no_type_tok);
	}
    return 0;
}//OK

int null_type(tToken *in_t){
	tToken token;
	if (in_t) token=*in_t;
	else {
		int lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
	}
	if (token.type!=Token_Ni32 && token.type!=Token_Nf64 && token.type!=Token_Nu8) return 1;
    return 0;
}//OK

int non_null_type(tToken *in_t){
	tToken token;
	if (in_t) token=*in_t;
	else {
		int lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
	}
	if (token.type!=Token_i32 && token.type!=Token_f64 && token.type!=Token_u8 && token.type!=Token_void) return 1;
    return 0;
}//OK

int type(tTokenStack *stack){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if(non_null_type(&token) && null_type(&token)) exit(SYNTAX_ERROR);
	PushTStack(stack, token);
    return 0;
}//OK

int const_decl(tToken *in_t, tToken *id_tok, tAstNode **synt_tree){
	tToken token = *in_t;
	tTokenStack stack;
	InitTStack(&stack);
	if (token.type!=Token_const) exit(SYNTAX_ERROR);
	PushTStack(&stack, token);
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if(token.type!=Token_FuncID) exit(SYNTAX_ERROR);
	*id_tok = token;
	PushTStack(&stack, token);
	type_decl(&stack);
	AddDeclNode(&(*synt_tree)->structure.code.operation, &stack);
    return 0;
}//OK

int var_decl(tToken *in_t, tToken *id_tok, tAstNode **synt_tree){
	tToken token = *in_t;
	tTokenStack stack;
	InitTStack (&stack);
	if(token.type!=Token_var) exit(SYNTAX_ERROR);
	PushTStack(&stack, token);
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if(token.type!=Token_FuncID) exit(SYNTAX_ERROR);
	*id_tok = token;
	PushTStack(&stack, token);
	type_decl(&stack);
	AddDeclNode(&(*synt_tree)->structure.code.operation, &stack);
    return 0;
}//OK

int argument_list_def(tToken *ret_t, tTokenStack *arg_stack){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type==Token_FuncID){
		argument_def(&token, arg_stack);
		next_argument_def(&token, arg_stack);
	}
	*ret_t = token;
    return 0;
}//OK

int argument_def(tToken *in_t, tTokenStack *arg_stack){
	tToken token;
	int lex_ret;
	if(in_t) token = *in_t;
	else {
		lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
	}
	if(token.type!=Token_FuncID) exit(SYNTAX_ERROR);
	PushTStack(arg_stack, token);
	lex_ret = GetToken(&token);
	if(lex_ret) exit(lex_ret);
	if(token.type!=Token_Colon) exit(SYNTAX_ERROR);
	type(arg_stack);
    return 0;
}//OK

int next_argument_def(tToken *ret_t, tTokenStack *arg_stack){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if(token.type==Token_Comma) argument_list_def(&token, arg_stack);
	*ret_t = token;
    return 0;
}//OK

int function_body(tToken *ret_t, tAstNode **synt_tree){
	body(ret_t, synt_tree);
    return 0;
}//OK

int return_(tToken *in_t, tAstNode **synt_tree){
	tToken token = *in_t;
	if (token.type==Token_return){
		AddRetNode(synt_tree);
		tAstNode **exp_tree = &(*synt_tree)->structure.ret.ret_expr;
		return_expression(&token, exp_tree);
		if(token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
	}
    return 0;
}//OK

int return_expression(tToken *ret_t, tAstNode **synt_tree){
	tToken token;
	int lex_ret = GetToken(&token);
	if(lex_ret) exit(lex_ret);
	switch (token.type){
		case Token_FuncID:
		case Token_BuildIn_Func:
		case Token_Integer:
		case Token_Float:
		case Token_Lpar:expression(&token, &token, synt_tree);
		default:break;
	}
	*ret_t = token;
    return 0;
}//OK

int body(tToken *ret_t, tAstNode **synt_tree){
	tToken token;
	tAstNode **current_p = synt_tree;
	body_statement(NULL, &token, synt_tree, &current_p);
	next_body_statement(&token, ret_t, current_p);
    return 0;
}//OK

int next_body_statement(tToken *in_t, tToken *ret_t, tAstNode **synt_tree){
	tToken token;
	tAstNode **current_p = synt_tree;
	if (in_t) token = *in_t;
	else {
		int lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
	}
	switch (token.type){
		case Token_while:
		case Token_var:
		case Token_const:
		case Token_FuncID:
		case Token_BuildIn_Func:
		case Token_if:
		case Token_Underscore:
			body_statement(&token, &token, synt_tree, &current_p);
			next_body_statement(&token, &token, current_p);
		default:break;
	}
	*ret_t = token;
    return 0;
}//OK

int body_statement(tToken *in_t, tToken *ret_t, tAstNode **synt_tree, tAstNode ***next_synt_tree){
	tToken token;
	int lex_ret = 0;
	if (in_t) token = *in_t;
	else {
		lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
	}
	AddCodeNode(synt_tree);
	tAstNode **code_tree = &(*synt_tree)->structure.code.operation;
	switch(token.type){
		case Token_if: if_block(&token, &token, code_tree); break;
		case Token_while: 
					   while_loop(&token, code_tree); 
					   lex_ret = GetToken(&token);
					   break;
		case Token_var: 
					   var_init(&token, synt_tree);
					   lex_ret = GetToken(&token);
					   break;
		case Token_const: 
					   const_init(&token, synt_tree);
					   lex_ret = GetToken(&token);
					   break;
		case Token_FuncID: 
					   check_var_or_func(&token, code_tree);
					   break;
		case Token_return: 
					   return_(&token, code_tree);
					   lex_ret = GetToken(&token);
					   break;
		case Token_Underscore:
					   dispose(&token, code_tree);
					   break;
		case Token_BuildIn_Func: 
						   function_call(&token, code_tree);
						   lex_ret = GetToken(&token);
						   if (lex_ret) exit(lex_ret);
						   if(token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
						   lex_ret = GetToken(&token);
						   break;
		default: exit(SYNTAX_ERROR);
	}
	if(lex_ret) exit(lex_ret);
	*next_synt_tree = &(*synt_tree)->structure.code.next_code;
	*ret_t = token;
    return 0;
}//OK
 
int dispose(tToken *in_t, tAstNode **synt_tree){
	tToken token = *in_t;
	if (token.type!=Token_Underscore) exit(SYNTAX_ERROR);
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if(token.type!=Token_Assign) exit(SYNTAX_ERROR);
	expression(NULL, &token, synt_tree);
	if(token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
	return 0;
}//OK

int check_var_or_func(tToken *in_t, tAstNode **synt_tree){
	tToken token = *in_t;
	if (token.type!=Token_FuncID) exit(SYNTAX_ERROR);
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	switch(token.type){
		case Token_Assign:
			expression(NULL, &token);
			break;
		case Token_Lpar:
			argument_list(&token);
			if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
			lex_ret = GetToken(&token);
			if (lex_ret) exit(lex_ret);
			break;
		default: exit(SYNTAX_ERROR);
	}
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
	return 0;
}

int if_block(tToken *in_t, tToken *ret_t, tAstNode **synt_tree){
	if_(in_t, synt_tree);
	else_(ret_t, synt_tree);
    return 0;
}//OK

int if_(tToken *in_t, tAstNode **synt_tree){
	tToken token;
	token = *in_t;
	if (token.type!=Token_if) exit(SYNTAX_ERROR);
	AddIfBlockNode(synt_tree);
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type!=Token_Lpar) exit(SYNTAX_ERROR);
	expression(NULL, &token, &(*synt_tree)->structure.if_block.expr);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	non_null_ID(&token, &(*synt_tree)->structure.if_block.nn_id);
	if(token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
	body(&token, &(*synt_tree)->structure.if_block.if_code);
	if(token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
    return 0;
}//OK

int else_(tToken *ret_t,tAstNode **synt_tree){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type==Token_else){
		lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
		if (token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
		body(&token, &(*synt_tree)->structure.if_block.else_code);
		if (token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
		lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
	}
	*ret_t = token;
    return 0;
}//OK

int non_null_ID(tToken *ret_t, tAstNode **synt_tree){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type==Token_Pipe){
		lex_ret = GetToken(&token);
		if(lex_ret) exit(lex_ret);
		if (token.type!=Token_FuncID) exit(SYNTAX_ERROR);
		tTokenStack *stack;
		InitTStack(stack);
		PushTStack(stack, token);
		AddExpNodes(synt_tree, stack);
		lex_ret = GetToken(&token);
		if(lex_ret) exit(lex_ret);
		if (token.type!=Token_Pipe) exit(SYNTAX_ERROR);
		lex_ret = GetToken (&token);
		if(lex_ret) exit(lex_ret);
	}
	*ret_t = token;
    return 0;
}//OK

int while_loop(tToken *in_t, tAstNode **synt_tree){
	tToken token;
	token = *in_t;
	if (token.type!=Token_while) exit(SYNTAX_ERROR);
	AddWhileNode(synt_tree);
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type!=Token_Lpar) exit(SYNTAX_ERROR);
	expression(NULL, &token, &(*synt_tree)->structure.while_loop.expr);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	non_null_ID(&token, &(*synt_tree)->structure.while_loop.nn_id);
	if(token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
	body(&token, &(*synt_tree)->structure.while_loop.code);
	if (token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
    return 0;
}//OK

int function_call(tToken *in_t, tAstNode **synt_tree){
	tToken token = *in_t;
	if (token.type!=Token_FuncID && token.type!=Token_BuildIn_Func) exit(SYNTAX_ERROR);
	int lex_ret = GetToken(&token);
	if(lex_ret) exit(lex_ret);
	if (token.type!=Token_Lpar) exit(SYNTAX_ERROR);
	argument_list(&token);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
    return 0;
}

int argument_list(tToken *ret_t){
	tToken token;
	int lex_ret = GetToken(&token);
	if(lex_ret) exit(lex_ret);
	switch(token.type){
		case Token_FuncID:
			argument(&token);
			next_argument(&token);
			break;
		 default:break;
	}
	*ret_t = token;
    return 0;
}

int argument(tToken *in_t){
	tToken token;
	token = *in_t;
	if (token.type!=Token_FuncID) exit(SYNTAX_ERROR);
    return 0;
}

int next_argument(tToken *ret_t){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type==Token_Comma) argument_list(&token);
	*ret_t = token;
    return 0;
}

int expression(tToken *in_t, tToken *ret_t){
	tToken token;
	if (in_t) token = *in_t;
	else {
		int lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
	}
	operand(&token);
	next_expression(&token);
	*ret_t = token;
    return 0;
}

int operand(tToken *in_t){
	tToken token;
	token = *in_t;
	switch(token.type){
		case Token_FuncID: check_var_or_func(&token); break;
		case Token_BuildIn_Func: function_call(&token); break;
		case Token_Integer:
		case Token_string:
		case Token_Float: break;
		case Token_Lpar: expression(NULL, &token);
						 if(token.type!=Token_Rpar) exit(SYNTAX_ERROR);
						 break;
		default: exit(SYNTAX_ERROR);
	}
    return 0;
}

int next_expression(tToken *ret_t){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if(operator_(&token)){
		lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
		operand(&token);
		next_expression(&token);
	}
	*ret_t = token;
    return 0;
}

int operator_(tToken *in_t){
	switch(in_t->type){
		case Token_Multiply:
		case Token_Divide:
		case Token_Plus:
		case Token_Minus:
		case Token_Equal:
		case Token_Not_Equal:
		case Token_Lesser:
		case Token_Greater:
		case Token_Lesser_Equal:
		case Token_Greater_Equal:return 1;
		default:break;
	}
    return 0;
}

int program(void){ //the whole program
   	tAstNode *synt_tree = NULL;
	tAstNode **current_p = &synt_tree;
	tBstNode *func_tree;
	BstInit(&func_tree);
	prolog(); 
	statement(&synt_tree, NULL, &current_p);
	next_statement(current_p);

    return 0;
}//OK
