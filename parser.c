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
	return;
}

int next_statement(){ //next statement in a program
	tToken token;
	GetToken(&token);
	switch(token->type){
		case Token_pub: statement(&token);
						next_statement();
						break;
		case Token_EOF: exit(0);
		default: exit(100);//TODO dat spravny error code
	}
	return;
}

int function(tToken *in_t){ //declaration of a function
    tToken token;
	tTokenType cmp_type = Token_pub;
	if (in_t) token = *in_t;
	else GetToken(&token);
	int err_found = 1;
	while (token->type==cmp_type){
		if(cmp_type==Token_Lpar) {
			err_found = 0;
			break;
		}
		GetToken(&token);
		switch cmp_type{
			case Token_pub: cmp_type=Token_fn; break;
			case Token_fn: cmp_type=Token_FuncID; break;
			case Token_FuncID: cmp_type=Token_Lpar; break;
			default: exit(100)//TODO dat spravny err code
		}
		argument_list_def();
	}
	return;
}

int imported_ID(){ //imported function identifiers
    return;
}

int const_init(){
    return;
}

int var_init(){ 
    return;
}

int const_def(){ 
    return;
}

int var_def(){
    return;
}

int type_decl(){
    return;
}

int null_type(){
    return;
}

int non_null_type(){
    return;
}

int type(){
    return;
}

int const_decl(){
    return;
}

int var_decl(){
    return;
}

int argument_list_def(){
    return;
}

int argument_def(){
    return;
}

int next_argument_def(){
    return;
}

int function_body(){
    return;
}

int return_(){
    return;
}

int return_expression(){
    return;
}

int body(){
    return;
}

int next_body_statement(){
    return;
}

int body_statement(){
    return;
}

int assign_value(){
    return;
}

int if_block(){
    return;
}

int if_(){
    return;
}
int else_(){
    return;
}

int non_null_ID(){
    return;
}

int while_loop(){
    return;
}

int function_call(){
    return;
}

int argument_list(){
    return;
}

int argument(){
    return;
}

int next_argument(){
    return;
}

int expression(){
    return;
}

int operand(){
    return;
}

int value(){
    return;
}

int next_expression(){
    return;
}

int operator_(){
    return;
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
