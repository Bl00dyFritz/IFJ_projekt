/**
* @file parser.c
* @brief hlavičkový soubor k parseru (syntaktické analýze) překladače jayzka IFJ24
* @author Lucie Pojslová (xpojsll00)
*/

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "error.h"

int statement(tToken *in_t);

int next_statement(void);

int function(tToken *in_t);

int const_init(tToken *in_t);

int var_init(tToken *in_t);

int const_def(tToken *ret_t);

int var_def(tToken *ret_t);

int type_decl(void);

int null_type(tToken *in_t);

int non_null_type(tToken *in_t);

int type(void);

int const_decl(tToken *in_t);

int var_decl(tToken *in_t);

int argument_list_def(tToken *ret_t);

int argument_def(tToken *in_t);

int next_argument_def(tToken *ret_t);

int function_body(tToken *ret_t);

int return_(tToken *in_t);

int return_expression(tToken *ret_t);

int body(tToken *ret_t);

int next_body_statement(tToken *in_t, tToken *ret_t);

int body_statement(tToken *in_t, tToken *ret_t);

int check_var_or_func(tToken *in_t);

int assign_value(void);

int if_block(tToken *in_t, tToken *ret_t);

int if_(tToken *in_t);

int else_(tToken *ret_t);

int non_null_ID(tToken *ret_t);

int while_loop(tToken *in_t);

int function_call(tToken *in_t);

int argument_list(tToken *ret_t);

int argument(tToken *in_t);

int next_argument(tToken *ret_t);

int expression(tToken *in_t, tToken *ret_t);

int operand(tToken *in_t);

int next_expression(tToken *ret_t);

int operator_(tToken *in_t);

#endif /** PARSER_H **/
