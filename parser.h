/**
* @file parser.c
* @brief hlavičkový soubor k parseru (syntaktické analýze) překladače jayzka IFJ24
* @author Lucie Pojslová (xpojsll00)
*/

#include <stdio.h>
#include <stdlib.h>

#include "scanner.c"
#include "error.h"

int statement();

int next_statement();

int function();

int imported_ID();

int const_init();

int var_init();

int const_def();

int var_def();

int type_decl();

int null_type();

int non_null_type();

int type();

int const_decl();

int var_decl();

int argument_list_def();

int argument_def();

int next_argument_def();

int function_body();

int return_();

int return_expression();

int body();

int next_body_statement();

int body_statement();

int assign_value();

int if_block();

int if_();

int else_();

int non_null_ID();

int while_loop();

int function_call();

int argument_list();

int argument();

int next_argument();

int expression();

int operand();

int value();

int next_expression();

int operator_();
