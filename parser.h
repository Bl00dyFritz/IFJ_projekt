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
#include "ast.h"
#include "stack.h"

int prolog(void);

int statement(tAstNode **synt_tree, tToken *in_t, tAstNode ***next_synt_tree);

int next_statement(tAstNode **synt_tree);

int function(tToken *in_t, tAstNode **synt_tree);

int const_init(tToken *in_t, tAstNode **synt_tree);

int var_init(tToken *in_t, tAstNode **synt_tree);

int const_def(tToken *id_tok, tToken *ret_t, tAstNode **synt_tree);

int var_def(tToken *id_tok, tToken *ret_t, tAstNode **synt_tree);

int type_decl(tTokenStack *stack);

int null_type(tToken *in_t);

int non_null_type(tToken *in_t);

int type(tTokenStack *stack);

int const_decl(tToken *in_t, tToken *id_tok, tAstNode **synt_tree);

int var_decl(tToken *in_t, tToken *id_tok, tAstNode **synt_tree);

int argument_list_def(tToken *ret_t, tTokenStack *arg_stack);

int argument_def(tToken *in_t, tTokenStack *arg_stack);

int next_argument_def(tToken *ret_t, tTokenStack *arg_stack);

int function_body(tToken *ret_t, tAstNode **synt_tree);

int return_(tToken *in_t, tAstNode **synt_tree);

int return_expression(tToken *ret_t, tAstNode **synt_tree);

int body(tToken *ret_t, tAstNode **synt_tree);

int next_body_statement(tToken *in_t, tToken *ret_t, tAstNode **synt_tree);

int body_statement(tToken *in_t, tToken *ret_t, tAstNode **synt_tree, tAstNode ***next_synt_tree);

int dispose(tToken *in_t, tAstNode **synt_tree);

int check_assign_or_func(tToken *in_t, tAstNode **synt_tree);

int if_block(tToken *in_t, tToken *ret_t, tAstNode **synt_tree);

int if_(tToken *in_t, tAstNode **synt_tree);

int else_(tToken *ret_t, tAstNode **synt_tree);

int non_null_ID(tToken *ret_t, tAstNode **synt_tree);

int while_loop(tToken *in_t, tAstNode **synt_tree);

int function_call(tToken *in_t, tAstNode **synt_tree);

int argument_list(tToken *ret_t, tTokenStack *stack);

int argument(tToken *in_t, tTokenStack *stack);

int next_argument(tToken *ret_t, tTokenStack *stack);

int expression(tToken *in_t, tToken *ret_t, tTokenStack *input_stack, tTokenStack *output_stack, tAstNode **synt_tree);

int check_var_or_func (tToken *in_t, tAstNode **synt_tree, tTokenStack *input_stack, tTokenStack *output_stack);

int operand(tToken *in_t, tTokenStack *input_stack, tTokenStack *output_stack, tAstNode **synt_tree);

int next_expression(tToken *ret_t, tTokenStack *input_stack, tTokenStack *output_stack, tAstNode **synt_tree);

int operator_(tToken *in_t);

#endif /** PARSER_H **/
