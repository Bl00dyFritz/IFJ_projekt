/**
* @file parser.c
* @brief hlavičkový soubor k parseru (syntaktické analýze) překladače jayzka IFJ24
* @author Lucie Pojslová (xpojsll00)
*/

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "semantics.h"
#include "generator.h"
#include "scanner.h"
#include "error.h"
#include "ast.h"
#include "stack.h"
#include "symtable.h"


/**
 * @brief Handles the prolog of the source code, ensuring correct syntax.
 * @return Returns an error code or 0 if successful.
 */
int prolog(void);

/**
 * @brief Parses a statement in the source code and builds an AST node for it.
 * @param synt_tree Pointer to the current AST node.
 * @param in_t Pointer to the current token.
 * @param next_synt_tree Pointer to the next AST node pointer.
 * @param func_tree Pointer to the function tree for scope checking.
 * @return Returns an error code or 0 if successful.
 */
int statement(tAstNode **synt_tree, tToken *in_t, tAstNode ***next_synt_tree, tBstNode **func_tree);

/**
 * @brief Parses the next statement in the sequence.
 * @param synt_tree Pointer to the current AST node.
 * @param func_tree Pointer to the function tree for scope checking.
 * @return Returns an error code or 0 if successful.
 */
int next_statement(tAstNode **synt_tree, tBstNode **func_tree);

/**
 * @brief Parses a function definition.
 * @param in_t Pointer to the current token.
 * @param synt_tree Pointer to the AST node representing the function.
 * @param func_tree Pointer to the function tree for scope checking.
 * @return Returns an error code or 0 if successful.
 */
int function(tToken *in_t, tAstNode **synt_tree, tBstNode **func_tree);

/**
 * @brief Parses a constant initialization.
 * @param in_t Pointer to the current token.
 * @param synt_tree Pointer to the AST node representing the constant.
 * @return Returns an error code or 0 if successful.
 */
int const_init(tToken *in_t, tAstNode **synt_tree, tAstNode ***out_tree);

/**
 * @brief Parses a variable initialization.
 * @param in_t Pointer to the current token.
 * @param synt_tree Pointer to the AST node representing the variable.
 * @return Returns an error code or 0 if successful.
 */
int var_init(tToken *in_t, tAstNode **synt_tree, tAstNode ***out_tree);

/**
 * @brief Parses a constant definition.
 * @param id_tok Token representing the identifier of the constant.
 * @param ret_t Token for returning information.
 * @param synt_tree Pointer to the AST node representing the constant.
 * @return Returns an error code or 0 if successful.
 */
int const_def(tToken *in_t ,tToken *id_tok, tToken *ret_t, tAstNode **synt_tree);

/**
 * @brief Parses a variable definition.
 * @param id_tok Token representing the identifier of the variable.
 * @param ret_t Token for returning information.
 * @param synt_tree Pointer to the AST node representing the variable.
 * @return Returns an error code or 0 if successful.
 */
int var_def(tToken *in_t ,tToken *id_tok, tToken *ret_t, tAstNode **synt_tree);

/**
 * @brief Parses a type declaration from the token stack.
 * @param stack Pointer to the token stack.
 * @return Returns an error code or 0 if successful.
 */
int type_decl(tTokenStack *stack, tToken *out_t);

/**
 * @brief Parses a null type declaration.
 * @param in_t Pointer to the current token.
 * @return Returns an error code or 0 if successful.
 */
int null_type(tToken *in_t);

/**
 * @brief Parses a non-null type declaration.
 * @param in_t Pointer to the current token.
 * @return Returns an error code or 0 if successful.
 */
int non_null_type(tToken *in_t);

/**
 * @brief Parses a general type declaration.
 * @param stack Pointer to the token stack.
 * @return Returns an error code or 0 if successful.
 */
int type(tTokenStack *stack);

/**
 * @brief Parses a constant declaration.
 * @param in_t Pointer to the current token.
 * @param id_tok Token representing the identifier.
 * @param synt_tree Pointer to the AST node representing the constant.
 * @return Returns an error code or 0 if successful.
 */
int const_decl(tToken *in_t, tToken *out_t, tToken *id_tok, tAstNode **synt_tree);

/**
 * @brief Parses a variable declaration.
 * @param in_t Pointer to the current token.
 * @param id_tok Token representing the identifier.
 * @param synt_tree Pointer to the AST node representing the variable.
 * @return Returns an error code or 0 if successful.
 */
int var_decl(tToken *in_t, tToken *out_t, tToken *id_tok, tAstNode **synt_tree);

/**
 * @brief Parses the definition of an argument list.
 * @param ret_t Token for returning information.
 * @param arg_stack Pointer to the stack of arguments.
 * @return Returns an error code or 0 if successful.
 */
int argument_list_def(tToken *ret_t, tTokenStack *arg_stack);

/**
 * @brief Parses a single argument definition.
 * @param in_t Pointer to the current token.
 * @param arg_stack Pointer to the stack of arguments.
 * @return Returns an error code or 0 if successful.
 */
int argument_def(tToken *in_t, tTokenStack *arg_stack);

/**
 * @brief Parses the next argument definition in the list.
 * @param ret_t Token for returning information.
 * @param arg_stack Pointer to the stack of arguments.
 * @return Returns an error code or 0 if successful.
 */
int next_argument_def(tToken *ret_t, tTokenStack *arg_stack);

/**
 * @brief Parses the body of a function.
 * @param ret_t Token for returning information.
 * @param synt_tree Pointer to the AST node representing the function body.
 * @return Returns an error code or 0 if successful.
 */
int function_body(tToken *ret_t, tAstNode **synt_tree);

/**
 * @brief Parses a return statement.
 * @param in_t Pointer to the current token.
 * @param synt_tree Pointer to the AST node representing the return statement.
 * @return Returns an error code or 0 if successful.
 */
int return_(tToken *in_t, tAstNode **synt_tree);

/**
 * @brief Parses an expression in a return statement.
 * @param ret_t Token for returning information.
 * @param synt_tree Pointer to the AST node representing the return expression.
 * @return Returns an error code or 0 if successful.
 */
int return_expression(tToken *ret_t, tAstNode **synt_tree);

/**
 * @brief Parses a general function or block body.
 * @param ret_t Token for returning information.
 * @param synt_tree Pointer to the AST node representing the body.
 * @return Returns an error code or 0 if successful.
 */
int body(tToken *ret_t, tAstNode **synt_tree);

/**
 * @brief Parses the next statement in a function or block body.
 * @param in_t Pointer to the current token.
 * @param ret_t Token for returning information.
 * @param synt_tree Pointer to the AST node representing the body.
 * @return Returns an error code or 0 if successful.
 */
int next_body_statement(tToken *in_t, tToken *ret_t, tAstNode **synt_tree);

/**
 * @brief Parses a single statement in a function or block body.
 * @param in_t Pointer to the current token.
 * @param ret_t Token for returning information.
 * @param synt_tree Pointer to the AST node representing the statement.
 * @param next_synt_tree Pointer to the next AST node pointer.
 * @return Returns an error code or 0 if successful.
 */
int body_statement(tToken *in_t, tToken *ret_t, tAstNode **synt_tree, tAstNode ***next_synt_tree);

/**
 * @brief Disposes of a specific node in the AST.
 * @param in_t Pointer to the current token.
 * @param synt_tree Pointer to the AST node to be disposed.
 * @return Returns an error code or 0 if successful.
 */
int dispose(tToken *in_t, tAstNode **synt_tree);

/**
 * @brief Checks whether the given token represents an assignment or function call.
 * @param in_t Pointer to the current token.
 * @param synt_tree Pointer to the AST node for the assignment or function call.
 * @return Returns an error code or 0 if successful.
 */
int check_assign_or_func(tToken *in_t, tAstNode **synt_tree);

/**
 * @brief Parses an if-block.
 * @param in_t Pointer to the current token.
 * @param ret_t Token for returning information.
 * @param synt_tree Pointer to the AST node representing the if-block.
 * @return Returns an error code or 0 if successful.
 */
int if_block(tToken *in_t, tToken *ret_t, tAstNode **synt_tree);

/**
 * @brief Parses an if statement.
 * @param in_t Pointer to the current token.
 * @param synt_tree Pointer to the AST node representing the if statement.
 * @return Returns an error code or 0 if successful.
 */
int if_(tToken *in_t, tAstNode **synt_tree);

/**
 * @brief Parses an else statement.
 * @param ret_t Token for returning information.
 * @param synt_tree Pointer to the AST node representing the else statement.
 * @return Returns an error code or 0 if successful.
 */
int else_(tToken *ret_t, tAstNode **synt_tree);

/**
 * @brief Parses an identifier that cannot be null.
 * @param ret_t Token for returning information.
 * @param synt_tree Pointer to the AST node representing the identifier.
 * @return Returns an error code or 0 if successful.
 */
int non_null_ID(tToken *ret_t, tAstNode **synt_tree);

/**
 * @brief Parses a while loop.
 * @param in_t Pointer to the current token.
 * @param synt_tree Pointer to the AST node representing the while loop.
 * @return Returns an error code or 0 if successful.
 */
int while_loop(tToken *in_t, tAstNode **synt_tree);

/**
 * @brief Parses a function call.
 * @param in_t Pointer to the current token.
 * @param synt_tree Pointer to the AST node representing the function call.
 * @return Returns an error code or 0 if successful.
 */
int function_call(tToken *in_t, tAstNode **synt_tree);

/**
 * @brief Parses an argument list.
 * @param ret_t Token for returning information.
 * @param stack Pointer to the token stack.
 * @return Returns an error code or 0 if successful.
 */
int argument_list(tToken *ret_t, tTokenStack *stack);

/**
 * @brief Parses a single argument.
 * @param in_t Pointer to the current token.
 * @param stack Pointer to the token stack.
 * @return Returns an error code or 0 if successful.
 */
int argument(tToken *in_t, tTokenStack *stack);

/**
 * @brief Parses the next argument in the list.
 * @param ret_t Token for returning information.
 * @param stack Pointer to the token stack.
 * @return Returns an error code or 0 if successful.
 */
int next_argument(tToken *ret_t, tTokenStack *stack);

/**
 * @brief Parses an expression in the source code.
 * @param in_t Pointer to the current token.
 * @param ret_t Token for returning information.
 * @param input_stack Pointer to the input token stack.
 * @param output_stack Pointer to the output token stack.
 * @param synt_tree Pointer to the AST node representing the expression.
 * @return Returns an error code or 0 if successful.
 */
int expression(tToken *in_t, tToken *ret_t, tTokenStack *input_stack, tTokenStack *output_stack, tAstNode **synt_tree);

/**
 * @brief Checks if the current token represents a variable or function.
 * @param in_t Pointer to the current token.
 * @param synt_tree Pointer to the AST node representing the variable or function.
 * @param input_stack Pointer to the input token stack.
 * @param output_stack Pointer to the output token stack.
 * @return Returns an error code or 0 if successful.
 */
int check_var_or_func(tToken *in_t, tAstNode **synt_tree, tTokenStack *input_stack, tTokenStack *output_stack);

/**
 * @brief Parses an operand in an expression.
 * @param in_t Pointer to the current token.
 * @param input_stack Pointer to the input token stack.
 * @param output_stack Pointer to the output token stack.
 * @param synt_tree Pointer to the AST node representing the operand.
 * @return Returns an error code or 0 if successful.
 */
int operand(tToken *in_t, tTokenStack *input_stack, tTokenStack *output_stack, tAstNode **synt_tree);

/**
 * @brief Parses the next part of an expression.
 * @param ret_t Token for returning information.
 * @param input_stack Pointer to the input token stack.
 * @param output_stack Pointer to the output token stack.
 * @param synt_tree Pointer to the AST node representing the expression.
 * @return Returns an error code or 0 if successful.
 */
int next_expression(tToken *in_t, tToken *ret_t, tTokenStack *input_stack, tTokenStack *output_stack, tAstNode **synt_tree);

/**
 * @brief Parses an operator.
 * @param in_t Pointer to the current token.
 * @return Returns an error code or 0 if successful.
 */
int operator_(tToken *in_t);

/**
 * @brief Entry point for parsing the entire program.
 * @return Returns an error code or 0 if successful.
 */
int program(void);

#endif /** PARSER_H **/
