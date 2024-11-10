/**
* @file parser.c
* @brief hlavičkový soubor k parseru (syntaktické analýze) překladače jayzka IFJ24
* @author Lucie Pojslová (xpojsll00)
*/

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "error.h"

int statement(tToken *in_t);

int next_statement(void);

int function(tToken *in_t);

int function_id(void);

int const_init(void);

int var_init(void);

int const_def(void);

int var_def(void);

int type_decl(void);

int null_type(tToken *in_t);

int non_null_type(tToken *in_t);

int type(void);

int const_decl(void);

int var_decl(void);

int argument_list_def(void);

int argument_def(void);

int next_argument_def(void);

int function_body(void);

int return_(void);

int return_expression(void);

int body(void);

int next_body_statement(void);

int body_statement(void);

int assign_value(void);

int if_block(void);

int if_(void);

int else_(void);

int non_null_ID(void);

int while_loop(void);

int function_call(void);

int argument_list(void);

int argument(void);

int next_argument(void);

int expression(void);

int operand(void);

int value(void);

int next_expression(void);

int operator_(void);
