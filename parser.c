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

int statement(tToken *in_t){
	tToken token;
	if (in_t) token = *in_t;
	else GetToken(&token);
	function(&token);
	return 0;
}

int next_statement(void){
	tToken token;
	GetToken(&token);
	switch(token.type){
		case Token_pub: statement(&token);
						next_statement();
		case Token_EOF: break;
		default: exit(SYNTAX_ERROR);
	}
	return 0;
}

int function(tToken *in_t){
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
	argument_list_def(&token);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	type();
	GetToken(&token);
	if(token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
	function_body(&token);
	if(token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
	return 0;
}

int const_init(tToken *in_t){
	tToken token = *in_t;
	const_decl(&token);
	const_def(&token);
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
    return 0;
}

int var_init(tToken *in_t){
	tToken token = *in_t;
	var_decl(&token);
	var_def(&token);
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
    return 0;
}

int const_def(tToken *ret_t){
	tToken token;
	GetToken(&token);
	if (token.type==Token_Assign) expression(NULL, &token);
	*ret_t = token;
    return 0;
}

int var_def(tToken *ret_t){
	tToken token;
	GetToken(&token);
	if(token.type==Token_Assign) expression(NULL, &token);
	*ret_t = token;
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

int argument_list_def(tToken *ret_t){
	tToken token;
	GetToken(&token);
	if (token.type==Token_FuncID){
		argument_def(&token);
		next_argument_def(&token);
	}
	*ret_t = token;
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

int next_argument_def(tToken *ret_t){
	tToken token;
	GetToken(&token);
	if(token.type==Token_Comma) argument_list_def(&token);
	*ret_t = token;
    return 0;
}

int function_body(tToken *ret_t){
	body(ret_t);
    return 0;
}

int return_(tToken *in_t){
	tToken token = *in_t;
	if (token.type==Token_return){
		return_expression(&token);
		if(token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
	}
    return 0;
}

int return_expression(tToken *ret_t){
	tToken token;
	GetToken(&token);
	switch (token.type){
		case Token_FuncID:
		case Token_BuildIn_Func:
		case Token_Integer:
		case Token_Float:
		case Token_Lpar:expression(&token, &token);
		default:break;
	}
	*ret_t = token;
    return 0;
}

int body(tToken *ret_t){
	tToken token;
	body_statement(NULL, &token);
	next_body_statement(&token, ret_t);
    return 0;
}

int next_body_statement(tToken *in_t, tToken *ret_t){
	tToken token;
	if (in_t) token = *in_t;
	else GetToken(&token);
	switch (token.type){
		case Token_while:
		case Token_var:
		case Token_const:
		case Token_FuncID:
		case Token_BuildIn_Func:
		case Token_if: 
			body_statement(&token, &token);
			next_body_statement(&token, &token);
		default:break;
	}
	*ret_t = token;
    return 0;
}

int body_statement(tToken *in_t, tToken *ret_t){
	tToken token;
	if (in_t) token = *in_t;
	else GetToken(&token);
	switch(token.type){
		case Token_if: if_block(&token, &token); break;
		case Token_while: 
					   while_loop(&token); 
					   GetToken(&token);
					   break;
		case Token_var: 
					   var_init(&token);
					   GetToken(&token);
					   break;
		case Token_const: 
					   const_init(&token);
					   GetToken(&token);
					   break;
		case Token_FuncID: 
					   check_var_or_func(&token);
					   break;
		case Token_return: 
					   return_(&token);
					   GetToken(&token);
					   break;
		case Token_BuildIn_Func: 
						   function_call(&token);
						   GetToken(&token);
						   if(token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
						   GetToken(&token);
						   break;
		default: exit(SYNTAX_ERROR);
	}
	*ret_t = token;
    return 0;
}

int check_var_or_func(tToken *in_t){
	tToken token = *in_t;
	if (token.type!=Token_FuncID) exit(SYNTAX_ERROR);
	GetToken(&token);
	switch(token.type){
		case Token_Assign:
			expression(NULL, &token);
			break;
		case Token_Lpar:
			argument_list(&token);
			if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
			GetToken(&token);
			break;
		default: exit(SYNTAX_ERROR);
	}
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
	return 0;
}

int assign_value(void){
	tToken token;
	GetToken(&token);
	if (token.type!=Token_FuncID) exit(SYNTAX_ERROR);
	GetToken(&token);
	if (token.type!=Token_Assign) exit(SYNTAX_ERROR);
	expression(NULL, &token);
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
    return 0;
}

int if_block(tToken *in_t, tToken *ret_t){
	if_(in_t);
	else_(ret_t);
    return 0;
}

int if_(tToken *in_t){
	tToken token;
	token = *in_t;
	if (token.type!=Token_if) exit(SYNTAX_ERROR);
	GetToken(&token);
	if (token.type!=Token_Lpar) exit(SYNTAX_ERROR);
	expression(NULL, &token);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	non_null_ID(&token);
	if(token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
	body(&token);
	if(token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
    return 0;
}

int else_(tToken *ret_t){
	tToken token;
	GetToken(&token);
	if (token.type==Token_else){
		GetToken(&token);
		if (token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
		body(&token);
		if (token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
		GetToken(&token);
	}
	*ret_t = token;
    return 0;
}

int non_null_ID(tToken *ret_t){
	tToken token;
	GetToken(&token);
	if (token.type==Token_Pipe){
		GetToken(&token);
		if (token.type!=Token_FuncID) exit(SYNTAX_ERROR);
		GetToken(&token);
		if (token.type!=Token_Pipe) exit(SYNTAX_ERROR);
		GetToken (&token);
	}
	*ret_t = token;
    return 0;
}

int while_loop(tToken *in_t){
	tToken token;
	token = *in_t;
	if (token.type!=Token_while) exit(SYNTAX_ERROR);
	GetToken(&token);
	if (token.type!=Token_Lpar) exit(SYNTAX_ERROR);
	expression(NULL, &token);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
	non_null_ID(&token);
	if(token.type!=Token_Lbrack) exit(SYNTAX_ERROR);
	body(&token);
	if (token.type!=Token_Rbrack) exit(SYNTAX_ERROR);
    return 0;
}

int function_call(tToken *in_t){
	tToken token = *in_t;
	if (token.type!=Token_FuncID && token.type!=Token_BuildIn_Func) exit(SYNTAX_ERROR);
	GetToken(&token);
	if (token.type!=Token_Lpar) exit(SYNTAX_ERROR);
	argument_list(&token);
	if (token.type!=Token_Rpar) exit(SYNTAX_ERROR);
    return 0;
}

int argument_list(tToken *ret_t){
	tToken token;
	GetToken(&token);
	switch(token.type){
		case Token_FuncID:
			argument(&token);
			next_argument(&token);
			break;
		/*
		  Toto patri do bonusu, budemem resit pokud bude na to cas
		case Token_IFJ:
		case Token_Integer:
		case Token_Float:
		case Token_Lpar:
		    argument(&token);
		    next_argument();
		    break;
		 */
		 default:break;
	}
	*ret_t = token;
    return 0;
}

int argument(tToken *in_t){
	tToken token;
	token = *in_t;
	if (token.type!=Token_FuncID) exit(SYNTAX_ERROR);
	/*
	 Soucasti bonusu, zatim ignorovat
	switch(token.type){
		case Token_FuncID: check_var_or_func();break;
		case Token_IFJ: function_call(&token); break;
		case Token_Integer:
		case Token_Float:
		case Token_Lpar: expression(&token, NULL); break;
		default: exit(SYNTAX_ERROR);
	}
	*/
    return 0;
}

int next_argument(tToken *ret_t){
	tToken token;
	GetToken(&token);
	if (token.type==Token_Comma) argument_list(&token);
	*ret_t = token;
    return 0;
}

int expression(tToken *in_t, tToken *ret_t){
	tToken token;
	if (in_t) token = *in_t;
	else GetToken(&token);
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
	GetToken(&token);
	if(operator_(&token)){
		GetToken(&token);
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
    
    int prologue = PrologueScan();
    if(prologue != 0){
        return prologue;
    }
	statement(NULL);
	next_statement();

    return 0;
}
