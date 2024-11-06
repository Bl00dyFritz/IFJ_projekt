/**
* @file parser.c
* @brief implementace parseru (syntaktické analýzy) překladače jayzka IFJ24
* @author Lucie Pojslová (xpojsll00)
*/

#include "parser.h"

int statement(){ //statement in a program
    return;
}

int next_statement(){ //next statement in a program
    return;
}

int function(){ //declaration of a function
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