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

int const_init(void){
	tToken token;
	const_decl();
	const_def();
	GetToken(&token);
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
    return 0;
}

int var_init(void){
	tToken token;
	var_decl();
	var_def();
	GetToken(&token);
	if (token.type!=Token_Semicolon) exit(SYNTAX_ERROR);
    return 0;
}

int const_def(void){
	tToken token;
	GetToken(&token);
	if (token.type==Token_Assign) expression();
    return 0;
}

int var_def(void){
	tToken token;
	GetToken(&token);
	if(token.type==Token_Assign) expression();
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
	if (token.type!=Token_Ni32 || token.type!=Token_Nf64 || token.type!=Token_Nu8) return 1;
    return 0;
}

int non_null_type(tToken *in_t){
	tToken token;
	if (in_t) token=*in_t;
	else GetToken(&token);
	if (token.type!=Token_i32 || token.type!=Token_f64 || token.type!=Token_u8 || token.type!=Token_void) return 1;
    return 0;
}

int type(void){
	tToken token;
	GetToken(&token);
	if(non_null_type(&token) && null_type(&token)) exit(SYNTAX_ERROR);
    return 0;
}

int const_decl(void){
	tToken token;
	GetToken(&token);
	if (token.type!=Token_const) exit(SYNTAX_ERROR);
	GetToken(&token);
	if(token.type!=Token_FuncID) exit(SYNTAX_ERROR);
	type_decl();
    return 0;
}

int var_decl(){
    return 0;
}

int argument_list_def(){
    return 0;
}

int argument_def(){
    return 0;
}

int next_argument_def(){
    return 0;
}

int function_body(){
    return 0;
}

int return_(){
    return 0;
}

int return_expression(){
    return 0;
}

int body(){
    return 0;
}

int next_body_statement(){
    return 0;
}

int body_statement(){
    return 0;
}

int assign_value(){
    return 0;
}

int if_block(){
    return 0;
}

int if_(){
    return 0;
}
int else_(){
    return 0;
}

int non_null_ID(){
    return 0;
}

int while_loop(){
    return 0;
}

int function_call(){
    return 0;
}

int argument_list(){
    return 0;
}

int argument(){
    return 0;
}

int next_argument(){
    return 0;
}

int expression(){
    return 0;
}

int operand(){
    return 0;
}

int value(){
    return 0;
}

int next_expression(){
    return 0;
}

int operator_(){
    return 0;
}

int program(){ //the whole program
    
    tToken *token;
    int c;
    
    int prologue = PrologueScan();
    if(prologue != 0){
        return prologue;
    }
    int token_function = GetToken(token, c);
    if(token_function != 0){
        return token_function;
    }


    return 0;
}
