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

int const_init(tToken *in_t);

int var_init(tToken *in_t);

int const_def(void);

int var_def(void);

int type_decl(void);

int null_type(tToken *in_t);

int non_null_type(tToken *in_t);

int type(void);

int const_decl(tToken *in_t);

int var_decl(tToken *in_t);

int argument_list_def(void);

int argument_def(tToken *in_t);

int next_argument_def(void);

int function_body(void);

int return_(void);

int return_expression(void);

int body(void);

int next_body_statement(void);

int body_statement(tToken *in_t);

int check_var_or_func(void);

int assign_value(void);

int if_block(tToken *in_t);

int if_(tToken *in_t);

int else_(void);

int non_null_ID(void);

int while_loop(tToken *in_t);

int function_call(tToken *in_t);

int argument_list(void);

int argument(tToken *in_t);

int next_argument(void);

int expression(tToken *in_t);

int operand(tToken *in_t);

int next_expression(void);

int operator_(tToken *in_t);
