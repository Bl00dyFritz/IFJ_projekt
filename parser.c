/**
* @file parser.c
* @brief implementace parseru (syntaktické analýzy) překladače jayzka IFJ24
* @author Lucie Pojslová (xpojsll00)
*/

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "error.h"
#include "scanner.h"

int statement(tToken *in_t){ //statement in a program
	function(in_t);
	return 0;
}

int next_statement(void){ //next statement in a program
	tToken token;
	GetToken(&token);
	switch(token.type){
		case Token_pub: statement(&token);
						next_statement();
						break;
		case Token_EOF: exit(0);
		default: exit(SYNTAX_ERROR);
	}
	return 0;
}

int function(tToken *in_t){ //declaration of a function
    tToken token;
	tTokenType cmp_type = Token_pub;
	if (in_t) token = *in_t;
	else GetToken(&token);
	int err_found = 1;
	while (token.type==cmp_type){
		if(cmp_type==Token_Lpar) {
			err_found = 0;
			break;
		}
		GetToken(&token);
		switch (cmp_type){
			case Token_pub: cmp_type=Token_fn; break;
			case Token_fn: cmp_type=Token_FuncID; break;
			case Token_FuncID: cmp_type=Token_Lpar; break;
			default: exit(99);
		}
	}
	if (err_found) exit(SYNTAX_ERROR);
	argument_list_def();
	err_found = 1;
	GetToken(&token);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	type();
	GetToken(&token);
	if(token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
	function_body();
	GetToken(&token);
	if(token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
	return 0;
}

int function_id(void){
	tToken token;
	GetToken(&token);
	switch (token.type){
		case Token_BuildIn_Func: return 0;
		case Token_FuncID: return 0;
		default: exit(SYNTAX_ERROR);
	}
	return 0;
}

int const_init(tToken *in_t){
	tToken token = *in_t;
	const_decl(in_t);
	const_def();
	GetToken(&token);
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
    return 0;
}

int var_init(tToken *in_t){
	tToken token = *in_t;
	var_decl(in_t);
	var_def();
	GetToken(&token);
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
    return 0;
}

int const_def(void){
	tToken token;
	GetToken(&token);
	if (token.type==Token_Assign) expression(NULL);
    return 0;
}

int var_def(void){
	tToken token;
	GetToken(&token);
	if(token.type==Token_Assign) expression(NULL);
    return 0;
}

int type_decl(void){
	tToken token;
	GetToken(&token);
	if (token.type==Token_Colon) type();
    return 0;
}

int null_type(tToken *in_t){
	tToken token;
	if (in_t) token=*in_t;
	else GetToken(&token);
	if (token.type!=Token_Ni32 && token.type!=Token_Nf64 && token.type!=Token_Nu8) return 1;
    return 0;
}

int non_null_type(tToken *in_t){
	tToken token;
	if (in_t) token=*in_t;
	else GetToken(&token);
	if (token.type!=Token_i32 && token.type!=Token_f64 && token.type!=Token_u8 && token.type!=Token_void) return 1;
    return 0;
}

int type(void){
	tToken token;
	GetToken(&token);
	if(non_null_type(&token) && null_type(&token)) exit(SYNTAX_ERROR);
    return 0;
}

int const_decl(tToken *in_t){
	tToken token = *in_t;
	if (token.type!=Token_const) exit(SYNTAX_ERROR);
	GetToken(&token);
	if(token.type!=Token_FuncID) exit(SYNTAX_ERROR);
	type_decl();
    return 0;
}

int var_decl(tToken *in_t){
	tToken token = *in_t;
	if(token.type!=Token_var) exit(SYNTAX_ERROR);
	GetToken(&token);
	if(token.type!=Token_FuncID) exit(SYNTAX_ERROR);
	type_decl();
    return 0;
}

int argument_list_def(void){
	tToken token;
	GetToken(&token);
	if (token.type==Token_FuncID){
		argument_def(&token);
		next_argument_def();
	}
    return 0;
}

int argument_def(tToken *in_t){
	tToken token;
	if(in_t) token = *in_t;
	else GetToken(&token);
	if(token.type!=Token_FuncID) exit(SYNTAX_ERROR);
	GetToken(&token);
	if(token.type!=Token_Colon) exit(SYNTAX_ERROR);
	type();
    return 0;
}

int next_argument_def(void){
	tToken token;
	GetToken(&token);
	if(token.type==Token_Comma) argument_list_def();
    return 0;
}

int function_body(void){
	body();
	return_();
    return 0;
}

int return_(void){
	tToken token;
	GetToken(&token);
	if (token.type==Token_return){
		return_expression();
		GetToken(&token);
		if(token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
	}
    return 0;
}

int return_expression(void){
	tToken token;
	GetToken(&token);
	switch (token.type){
		case Token_FuncID:
		case Token_BuildIn_Func:
		case Token_Integer:
		case Token_Float:
		case Token_Lpar:expression(&token);
		default:break;
	}
    return 0;
}

int body(void){
	body_statement(NULL);
	next_body_statement();
    return 0;
}

int next_body_statement(void){
	tToken token;
	GetToken(&token);
	switch (token.type){
		case Token_while:
		case Token_var:
		case Token_const:
		case Token_FuncID:
		case Token_BuildIn_Func:
		case Token_if: 
			body_statement(&token);
			next_body_statement();
		default:break;
	}
    return 0;
}

int body_statement(tToken *in_t){
	tToken token;
	if (in_t) token = *in_t;
	else GetToken(&token);
	switch(token.type){
		case Token_if: if_block(&token); break;
		case Token_while: while_loop(&token); break;
		case Token_var: var_init(&token); break;
		case Token_const: const_init(&token); break;
		case Token_FuncID: check_var_or_func(); break;
		case Token_BuildIn_Func: 
						   function_call(&token);
						   GetToken(&token);
						   if(token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
						   break;
		default: exit(SYNTAX_ERROR);
	}
    return 0;
}

int check_var_or_func(void){
	//TODO navrhnout spravne
	return 0;
}

int assign_value(void){
	//TODO navazuje na check_var_or_func
    return 0;
}

int if_block(tToken *in_t){
	if_(in_t);
	else_();
    return 0;
}

int if_(tToken *in_t){
	tToken token;
	token = *in_t;
	if (token.type!=Token_if) exit(SYNTAX_ERROR);
	GetToken(&token);
	if (token.type!=Token_Lpar) exit(SYNTAX_ERROR);
	expression(NULL);
	GetToken(&token);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	non_null_ID();
	GetToken(&token);
	if(token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
	body();
	GetToken(&token);
	if(token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
    return 0;
}
int else_(void){
	tToken token;
	GetToken(&token);
	if (token.type!=Token_else) exit(SYNTAX_ERROR);
	GetToken(&token);
	if (token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
	body();
	GetToken(&token);
	if (token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
    return 0;
}

int non_null_ID(void){
	tToken token;
	GetToken(&token);
	if (token.type==Token_Pipe){
		GetToken(&token);
		if (token.type!=Token_FuncID) exit(SYNTAX_ERROR);
		GetToken(&token);
		if (token.type!=Token_Pipe) exit(SYNTAX_ERROR);
	}
    return 0;
}

int while_loop(tToken *in_t){
	tToken token;
	token = *in_t;
	if (token.type!=Token_while) exit(SYNTAX_ERROR);
	GetToken(&token);
	if (token.type!=Token_Lpar) exit(SYNTAX_ERROR);
	expression(NULL);
	GetToken(&token);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	non_null_ID();
	GetToken(&token);
	if(token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
	body();
	GetToken(&token);
	if (token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
    return 0;
}

int function_call(tToken *in_t){
	//TODO nacazuje na check_var_or_func
    return 0;
}

int argument_list(void){
	tToken token;
	GetToken(&token);
	switch(token.type){
		case Token_FuncID:
		case Token_IFJ:
		case Token_Integer:
		case Token_Float:
		case Token_Lpar:
			argument(&token);
			next_argument();
			break;
		default:break;
	}
    return 0;
}

int argument(tToken *in_t){
	tToken token;
	token = *in_t;
	switch(token.type){
		case Token_FuncID: check_var_or_func();break;
		case Token_IFJ: function_call(&token); break;
		case Token_Integer:
		case Token_Float:
		case Token_Lpar: expression(&token); break;
		default: exit(SYNTAX_ERROR);
	}
    return 0;
}

int next_argument(void){
	tToken token;
	GetToken(&token);
	if (token.type==Token_Comma) argument_list();
    return 0;
}

int expression(tToken *in_t){
	tToken token;
	if (in_t) token = *in_t;
	else GetToken(&token);
	operand(&token);
	next_expression();
    return 0;
}

int operand(tToken *in_t){
	tToken token;
	token = *in_t;
	switch(token.type){
		case Token_FuncID: check_var_or_func(); break;
		case Token_BuildIn_Func: function_call(&token); break;
		case Token_Integer:
		case Token_Float: break;
		case Token_Lpar: expression(NULL);
						 GetToken(&token);
						 if(token.type!=Token_Rpar) exit(SYNTAX_ERROR);
						 break;
		default: exit(SYNTAX_ERROR);
	}
    return 0;
}

int next_expression(void){
	tToken token;
	GetToken(&token);
	if(operator_(&token)){
		GetToken(&token);
		operand(&token);
		next_expression();
	}
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
    
    tToken token;
    
    int prologue = PrologueScan();
    if(prologue != 0){
        return prologue;
    }
    int token_function = GetToken(&token);
    if(token_function != 0){
        return token_function;
    }


    return 0;
}
