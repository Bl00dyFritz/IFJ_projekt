/**
* @file parser.c
* @brief implementation of the parser (syntax analysis) of the IFJ24 language translator
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
#include "semantics.h"
#include "generator.h"

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
}

int statement(tAstNode **synt_tree, tToken *in_t,tAstNode ***next_synt_tree, tBstNode **func_tree){
	tToken token;
	int lex_ret=0;
	if (in_t) token = *in_t;
	else {
		lex_ret=GetToken(&token);
		if(lex_ret) exit(lex_ret);
	}
	AddStatmentNode(synt_tree);
	function(&token, &(*synt_tree)->structure.statement.function, func_tree);
	*next_synt_tree = &(*synt_tree)->structure.statement.next_statement;
	return 0;
}

int next_statement(tAstNode **synt_tree, tBstNode **func_tree){
	tToken token;
	tAstNode **current_p = synt_tree;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	switch(token.type){
		case Token_pub: statement(synt_tree, &token, &current_p, func_tree);
						next_statement(current_p, func_tree);
		case Token_EOF: break;
		default: exit(SYNTAX_ERROR);
	}
	return 0;
}

int function(tToken *in_t, tAstNode **synt_tree, tBstNode **func_tree){
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
			default: exit(INTERNAL_COMP_ERROR);
		}
	}
	if (err_found) exit(SYNTAX_ERROR);
	argument_list_def(&token, &arg_stack);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	type(&stack);
	tBstNodeContent *tmp = NULL;
	AddFuncDefNode(synt_tree, &stack, &arg_stack);
	if (BstSearch(*func_tree, (*synt_tree)->structure.func_def.token.value.string, &tmp)) exit(SEMANTIC_REDEF_ERROR);
	tBstNodeContent content;
	content.type = FUNCTION;
	content.value = (tFunctionVals *) malloc(sizeof(tFunctionVals));
	if (!content.value) exit(INTERNAL_COMP_ERROR);
	tFunctionVals *func_vals = (tFunctionVals *) content.value;
	func_vals->ret_type = (*synt_tree)->structure.func_def.ret_type_token.type;
	tArgDef *args = (*synt_tree)->structure.func_def.args;
	func_vals->params = args;
	while (args){
		func_vals->paramCnt++;
		tBstNodeContent *arg = (tBstNodeContent *) malloc(sizeof(tBstNodeContent));
		if(!arg) exit(INTERNAL_COMP_ERROR);
		arg->type = VARIABLE;
		arg->value = (tVarVals*) malloc(sizeof(tVarVals));
		tVarVals *vals = (tVarVals *) arg->value;
		vals->is_constant = true;
		vals->is_used = false;
		vals->value = NULL;
		vals->type = args->type_token.type;
		BstInsAndReal(&(*synt_tree)->structure.func_def.loc_symtree, args->name_token.value.string, *arg);
		args = args->next;
	}
	BstInsAndReal(func_tree, (*synt_tree)->structure.func_def.token.value.string, content);
	lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if(token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
	function_body(&token, &(*synt_tree)->structure.func_def.code);
	if(token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
	return 0;
}

int const_init(tToken *in_t, tAstNode **synt_tree, tAstNode ***out_tree){
	tToken token = *in_t;
	tToken decl_out;
	const_decl(&token, &decl_out, &token, synt_tree);
	AddCodeNode(&(*synt_tree)->structure.code.next_code);
	tAstNode **next_cnode = &(*synt_tree)->structure.code.next_code;
	const_def(&decl_out ,&token, &token, next_cnode);
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
	*out_tree = next_cnode;
    return 0;
}

int var_init(tToken *in_t, tAstNode **synt_tree, tAstNode ***out_tree){
	tToken token = *in_t;
	tToken decl_out;
	var_decl(&token, &decl_out, &token, synt_tree);
	AddCodeNode(&(*synt_tree)->structure.code.next_code);
	tAstNode **next_cnode = &(*synt_tree)->structure.code.next_code;
	var_def(&decl_out ,&token, &token, next_cnode);
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
	*out_tree = next_cnode;
    return 0;
}

int const_def(tToken *in_t ,tToken *id_tok, tToken *ret_t, tAstNode **synt_tree){
	tToken token;
	AddAssignNode(&(*synt_tree)->structure.code.operation);
	tAstNode **assign_node = &(*synt_tree)->structure.code.operation;
	tTokenStack stack;
	InitTStack(&stack);
	PushTStack(&stack, *id_tok);
	AddExpNodes(&(*assign_node)->structure.assign.dst, &stack);
	token = *in_t;
	if (token.type==Token_Assign){
		tTokenStack input_stack;
		tTokenStack output_stack;
		InitTStack(&input_stack);
		InitTStack(&output_stack);
		if (expression(NULL, &token, &input_stack, &output_stack, &(*assign_node)->structure.assign.src)) goto is_func_call;
		PrecedenceCheck(NULL, &input_stack, &output_stack);
		AddExpNodes(&(*assign_node)->structure.assign.src, &output_stack);
	}
is_func_call:
	*ret_t = token;
    return 0;
}

int var_def(tToken *in_t ,tToken *id_tok, tToken *ret_t, tAstNode **synt_tree){
	tToken token;
	AddAssignNode(&(*synt_tree)->structure.code.operation);
	tAstNode **assign_node = &(*synt_tree)->structure.code.operation;
	tTokenStack stack;
	InitTStack(&stack);
	PushTStack(&stack, *id_tok);
	AddExpNodes(&(*assign_node)->structure.assign.dst, &stack);
	token = *in_t;
	if(token.type==Token_Assign){ 
		tTokenStack input_stack;
		tTokenStack output_stack;
		InitTStack(&input_stack);
		InitTStack(&output_stack);
		if (expression(NULL, &token, &input_stack, &output_stack, &(*assign_node)->structure.assign.src)) goto is_func_call;
		PrecedenceCheck(NULL, &input_stack, &output_stack);
		AddExpNodes(&(*assign_node)->structure.assign.src, &output_stack);
	}
is_func_call:
	*ret_t = token;
    return 0;
}

int type_decl(tTokenStack *stack, tToken *out_t){
	tToken token;
	int lex_ret = GetToken(&token);
	if(lex_ret) exit(lex_ret);
	if (token.type==Token_Colon){
		type(stack);
		int lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
	}
	else{
		tToken no_type_tok;
		no_type_tok.type = Token_Dollar;
		PushTStack(stack, no_type_tok);
	}
	*out_t = token;
    return 0;
}

int null_type(tToken *in_t){
	tToken token;
	if (in_t) token=*in_t;
	else {
		int lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
	}
	if (token.type!=Token_Ni32 && token.type!=Token_Nf64 && token.type!=Token_Nu8) return 1;
    return 0;
}

int non_null_type(tToken *in_t){
	tToken token;
	if (in_t) token=*in_t;
	else {
		int lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
	}
	if (token.type!=Token_i32 && token.type!=Token_f64 && token.type!=Token_u8 && token.type!=Token_void) return 1;
    return 0;
}

int type(tTokenStack *stack){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if(non_null_type(&token) && null_type(&token)) exit(SYNTAX_ERROR);
	PushTStack(stack, token);
    return 0;
}

int const_decl(tToken *in_t, tToken *out_t, tToken *id_tok, tAstNode **synt_tree){
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
	type_decl(&stack, &token);
	AddDeclNode(&(*synt_tree)->structure.code.operation, &stack);
	*out_t = token;
    return 0;
}

int var_decl(tToken *in_t, tToken *out_t, tToken *id_tok, tAstNode **synt_tree){
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
	type_decl(&stack, &token);
	AddDeclNode(&(*synt_tree)->structure.code.operation, &stack);
	*out_t = token;
    return 0;
}

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
}

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
}

int next_argument_def(tToken *ret_t, tTokenStack *arg_stack){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if(token.type==Token_Comma) argument_list_def(&token, arg_stack);
	*ret_t = token;
    return 0;
}

int function_body(tToken *ret_t, tAstNode **synt_tree){
	body(NULL, ret_t, synt_tree);
    return 0;
}

int return_(tToken *in_t, tAstNode **synt_tree){
	tToken token = *in_t;
	if (token.type==Token_return){
		AddRetNode(synt_tree);
		tAstNode **exp_tree = &(*synt_tree)->structure.ret.ret_expr;
		return_expression(&token, exp_tree);
		if(token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
	}
    return 0;
}

int return_expression(tToken *ret_t, tAstNode **synt_tree){
	tToken token;
	tTokenStack input_stack;
	tTokenStack output_stack;
	InitTStack(&input_stack);
	InitTStack(&output_stack);
	int lex_ret = GetToken(&token);
	if(lex_ret) exit(lex_ret);
	switch (token.type){
		case Token_FuncID:
		case Token_BuildIn_Func:
		case Token_Integer:
		case Token_Float:
		case Token_Lpar:
			if (expression(&token, &token, &input_stack, &output_stack, synt_tree)) goto is_func_call;
			PrecedenceCheck(NULL, &input_stack, &output_stack);
			AddExpNodes(synt_tree, &output_stack);
is_func_call:
		default:break;
	}
	*ret_t = token;
    return 0;
}

int body(tToken *in_t, tToken *ret_t, tAstNode **synt_tree){
	tToken token;
	tAstNode **current_p = synt_tree;
	if (in_t){
		token = *in_t;
		body_statement(&token, &token, synt_tree, &current_p);
	}
	else
		body_statement(NULL, &token, synt_tree, &current_p);
	next_body_statement(&token, ret_t, current_p);
    return 0;
}

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
		case Token_return:
		case Token_Underscore:
			body_statement(&token, &token, synt_tree, &current_p);
			next_body_statement(&token, &token, current_p);
		default:break;
	}
	*ret_t = token;
    return 0;
}

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
					   var_init(&token, synt_tree, &synt_tree);
					   lex_ret = GetToken(&token);
					   break;
		case Token_const: 
					   const_init(&token, synt_tree, &synt_tree);
					   lex_ret = GetToken(&token);
					   break;
		case Token_FuncID: 
					   check_assign_or_func(&token, code_tree);
					   lex_ret = GetToken(&token);
					   break;
		case Token_return: 
					   return_(&token, code_tree);
					   lex_ret = GetToken(&token);
					   break;
		case Token_Underscore:
					   dispose(&token, code_tree);
					   lex_ret = GetToken(&token);
					   if (lex_ret) exit(lex_ret);
					   break;
		case Token_BuildIn_Func: 
						   function_call(&token, code_tree);
						   lex_ret = GetToken(&token);
						   if (lex_ret) exit(lex_ret);
						   if(token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
						   lex_ret = GetToken(&token);
						   break;
		default:exit(SYNTAX_ERROR);
	}
	if(lex_ret) exit(lex_ret);
	*next_synt_tree = &(*synt_tree)->structure.code.next_code;
	*ret_t = token;
    return 0;
}
 
int dispose(tToken *in_t, tAstNode **synt_tree){
	tToken token = *in_t;
	if (token.type!=Token_Underscore) exit(SYNTAX_ERROR);
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if(token.type!=Token_Assign) exit(SYNTAX_ERROR);
	tTokenStack input_stack;
	tTokenStack output_stack;
	InitTStack(&input_stack);
	InitTStack(&output_stack);
	if (expression(NULL, &token, &input_stack, &output_stack, synt_tree)) goto is_func_call;
	PrecedenceCheck(NULL, &input_stack, &output_stack);
	AddExpNodes(synt_tree, &output_stack);
is_func_call:
	if(token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
	return 0;
}

int check_assign_or_func(tToken *in_t, tAstNode **synt_tree){
	tToken token = *in_t;
	tToken function_id = *in_t;
	if (token.type!=Token_FuncID) exit(SYNTAX_ERROR);
	tTokenStack stack;
	InitTStack(&stack);
	PushTStack(&stack, token);
	tTokenStack input_stack;
	tTokenStack output_stack;
	InitTStack(&input_stack);
	InitTStack(&output_stack);
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	switch(token.type){
		case Token_Assign:
			AddAssignNode(synt_tree);
			AddExpNodes(&(*synt_tree)->structure.assign.dst, &stack);
			tAstNode **exp_tree = &(*synt_tree)->structure.assign.src;
			if (expression(NULL, &token, &input_stack, &output_stack, exp_tree)) goto is_func_call;
			PrecedenceCheck(NULL, &input_stack, &output_stack);
			AddExpNodes(exp_tree, &output_stack);
is_func_call:
			break;
		case Token_Lpar:
			PopTStack(&stack);
			argument_list(&token, &stack);
			if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
			AddFuncCallNode(synt_tree, function_id, &stack);
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
}

int if_(tToken *in_t, tAstNode **synt_tree){
	tToken token;
	token = *in_t;
	if (token.type!=Token_if) exit(SYNTAX_ERROR);
	AddIfBlockNode(synt_tree);
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type!=Token_Lpar) exit(SYNTAX_ERROR);
	tTokenStack input_stack;
	tTokenStack output_stack;
	InitTStack(&input_stack);
	InitTStack(&output_stack);
	if (expression(NULL, &token, &input_stack, &output_stack, &(*synt_tree)->structure.if_block.expr)) goto is_func_call;
	PrecedenceCheck(NULL, &input_stack, &output_stack);
	AddExpNodes(&(*synt_tree)->structure.if_block.expr, &output_stack);
is_func_call:
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	non_null_ID(&token, &(*synt_tree)->structure.if_block.nn_id);
	if(token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
	body(NULL ,&token, &(*synt_tree)->structure.if_block.if_code);
	if(token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
    return 0;
}

int else_(tToken *ret_t,tAstNode **synt_tree){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type==Token_else){
		lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
		if (token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
		lex_ret = GetToken(&token);
		if (token.type == Token_Rbrack) goto nocode;
		body(&token, &token, &(*synt_tree)->structure.if_block.else_code);
nocode:
		if (token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
		lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
	}
	*ret_t = token;
    return 0;
}

int non_null_ID(tToken *ret_t, tAstNode **synt_tree){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type==Token_Pipe){
		lex_ret = GetToken(&token);
		if(lex_ret) exit(lex_ret);
		if (token.type!=Token_FuncID) exit(SYNTAX_ERROR);
		tTokenStack stack;
		InitTStack(&stack);
		PushTStack(&stack, token);
		AddExpNodes(synt_tree, &stack);
		lex_ret = GetToken(&token);
		if(lex_ret) exit(lex_ret);
		if (token.type!=Token_Pipe) exit(SYNTAX_ERROR);
		lex_ret = GetToken (&token);
		if(lex_ret) exit(lex_ret);
	}
	*ret_t = token;
    return 0;
}

int while_loop(tToken *in_t, tAstNode **synt_tree){
	tToken token;
	token = *in_t;
	if (token.type!=Token_while) exit(SYNTAX_ERROR);
	AddWhileNode(synt_tree);
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type!=Token_Lpar) exit(SYNTAX_ERROR);
	tTokenStack input_stack;
	tTokenStack output_stack;
	InitTStack(&input_stack);
	InitTStack(&output_stack);
	if (expression(NULL, &token, &input_stack, &output_stack, &(*synt_tree)->structure.while_loop.expr)) goto is_func_call;
	PrecedenceCheck(NULL, &input_stack, &output_stack);
	AddExpNodes(&(*synt_tree)->structure.while_loop.expr, &output_stack);
is_func_call:
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	non_null_ID(&token, &(*synt_tree)->structure.while_loop.nn_id);
	if(token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
	body(NULL, &token, &(*synt_tree)->structure.while_loop.code);
	if (token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
    return 0;
}

int function_call(tToken *in_t, tAstNode **synt_tree){
	tToken token = *in_t;
	tToken function_id = token;
	tTokenStack stack;
	InitTStack(&stack);
	if (token.type!=Token_FuncID && token.type!=Token_BuildIn_Func) exit(SYNTAX_ERROR);
	int lex_ret = GetToken(&token);
	if(lex_ret) exit(lex_ret);
	if (token.type!=Token_Lpar) exit(SYNTAX_ERROR);
	argument_list(&token, &stack);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	AddFuncCallNode(synt_tree, function_id, &stack);
    return 0;
}

int argument_list(tToken *ret_t, tTokenStack *stack){
	tToken token;
	int lex_ret = GetToken(&token);
	if(lex_ret) exit(lex_ret);
	switch(token.type){
		case Token_FuncID:
		case Token_Integer:
		case Token_Float:
		case Token_string:
			argument(&token, stack);
			next_argument(&token, stack);
		default:
			break;
	}
	*ret_t = token;
    return 0;
}

int argument(tToken *in_t, tTokenStack *stack){
	tToken token;
	token = *in_t;
	PushTStack(stack, token);
    return 0;
}

int next_argument(tToken *ret_t, tTokenStack *stack){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type==Token_Comma) argument_list(&token, stack);
	*ret_t = token;
    return 0;
}

int expression(tToken *in_t, tToken *ret_t, tTokenStack *input_stack, tTokenStack *output_stack, tAstNode **synt_tree){
	tToken token;
	int func_in_exp = 0;
	if (in_t) token = *in_t;
	else {
		int lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
	}
	func_in_exp = operand(&token, input_stack, output_stack, synt_tree);
	next_expression(&token,&token, input_stack, output_stack, synt_tree);
	*ret_t = token;
    return func_in_exp;
}

int check_var_or_func(tToken *in_t, tAstNode **synt_tree, tTokenStack *input_stack, tTokenStack *output_stack){
	tToken token;
	int lex_ret = GetToken(&token);
	if (lex_ret) exit(lex_ret);
	if (token.type==Token_Lpar){
		tTokenStack arg_stack;
		InitTStack(&arg_stack);
		argument_list(&token, &arg_stack);
		AddFuncCallNode(synt_tree, *in_t, &arg_stack);
		if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
		lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
		*in_t = token;
		return 1;
	}
	PrecedenceCheck(in_t, input_stack, output_stack);
	*in_t = token;
	return 0;
}

int operand(tToken *in_t, tTokenStack *input_stack, tTokenStack *output_stack, tAstNode **synt_tree){
	tToken token;
	token = *in_t;
	int lex_ret;
	switch(token.type){
		case Token_FuncID: 
			if (check_var_or_func(&token, synt_tree, input_stack, output_stack)){
				*in_t = token;
				return 1; 
			}
			break;
		case Token_BuildIn_Func: 
			function_call(&token, synt_tree); 
			lex_ret = GetToken(&token);
			if(lex_ret) exit(lex_ret);
			*in_t = token;
			return 1;
		case Token_null:
		case Token_Integer:
		case Token_Float:
			PrecedenceCheck(&token, input_stack, output_stack);
			lex_ret = GetToken(&token);
			if(lex_ret) exit(lex_ret);
			break;
		case Token_Lpar: 
			PrecedenceCheck(&token, input_stack, output_stack);			 
			if (expression(NULL, &token, input_stack, output_stack, synt_tree)) exit(SYNTAX_ERROR);
			if(token.type!=Token_Rpar) exit(SYNTAX_ERROR);
			PrecedenceCheck(&token, input_stack, output_stack);
			lex_ret = GetToken(&token);
			if(lex_ret) exit(lex_ret);
			break;
		default: exit(SYNTAX_ERROR);
	}
	*in_t = token;
    return 0;
}

int next_expression(tToken *in_t, tToken *ret_t, tTokenStack *input_stack, tTokenStack *output_stack, tAstNode **synt_tree){
	tToken token = *in_t;
	int lex_ret;
	if(operator_(&token)){
		PrecedenceCheck(&token, input_stack, output_stack);
		lex_ret = GetToken(&token);
		if (lex_ret) exit(lex_ret);
		if (operand(&token, input_stack, output_stack, synt_tree)) exit(SYNTAX_ERROR);
		next_expression(&token,&token, input_stack, output_stack, synt_tree);
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
	SetSourceFile(stdin);
	BstInit(&func_tree);
	prolog(); 
	statement(&synt_tree, NULL, &current_p, &func_tree);
	next_statement(current_p, &func_tree);
	tSymtableList symlist;
	InitSymtableList(&symlist);
	tComData data;
	SymtableListAdd(&symlist, func_tree);
	if(!CheckMain(func_tree)) exit(SEMANTIC_UNDEF_ERROR);
	ExamineSemantics(synt_tree, &symlist, NULL, &data, NULL);
	GenInitial();
	GenerateOutput(synt_tree);
    return 0;
}

