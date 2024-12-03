/**
 * Implementation of IFJ24 imperative language translator
 * @file generator.h
 * @brief Header file for the code generator
 * @authors Alexander Žikla, xziklaa00
 *          Jakub Turek, xturekj00
*/

#ifndef GENERATOR_H
#define GENERATOR_H

#include "parser.h"
#include "symtable.h"
#include "scanner.h"

/**
 * @brief Generates the initial code setup.
 */
void GenInitial(void);

/**
 * @brief Generates the code for the .
 */
void GenHead(void);

/**
 * @brief Generates the code for the main function's header.
 */
void GenMainHead(void);

/**
 * @brief Generates the code for a user-defined function.
 * @param node The AST node representing the function definition.
 */
void GenDefFunc(tAstNode *node);

/**
 * @brief Generates code for built-in functions.
 */
void GenBuiltInFuncs(void);

/**
 * @brief Generates the code to call a function.
 * @param node The AST node representing the function call.
 */
void GenCallFunc(tAstNode *node);

/**
 * @brief Generates the code to end a function definition.
 *
 * @param node The AST node representing the function definition.
 */
void GenFuncEnd(tAstNode *node);

/**
 * @brief Generates the code for an assignment operation.
 *
 * @param node The AST node representing the assignment.
 */
void GenAssign(tAstNode *node);

/**
 * @brief Generates code to push an integer or float value onto the stack.
 *
 * @param node The AST node representing the integer or float value.
 */
void GenPushIntFloat(tAstNode *node);

/**
 * @brief Checks if a value is nil.
 *
 * @param node The AST node representing the value.
 * @return True if the value is nil, false otherwise.
 */
bool CheckNill(tAstNode *node);

/**
 * @brief Generates code for stack operations.
 *
 * @param node The AST node representing the stack operation.
 */
void GenStackOp(tAstNode *node);

/**
 * @brief Generates code for the start of an if statement.
 * @param LocalIfCounter Counter to distinguish from different IF's
 */
void GenIfStart(int LocalIfCounter);

/**
 * @brief Generates code for the start of an if statement with a non-nil check.
 * @param LocalIfCounter Counter to distinguish from different IF's
 */
void GenIfStartNN(int LocalIfCounter);

/**
 * @brief Generates code for the end of an if statement.
 * @param LocalIfCounter Counter to distinguish from different IF's
 */
void GenIfEnd(int LocalIfCounter);

/**
 * @brief Generates code for the end of an if statement with a non-nil check.
 * @param LocalIfCounter Counter to distinguish from different IF's
 */
void GenIfEndNN(int LocalIfCounter);

/**
 * @brief Generates code for the start of an else statement.
 * @param LocalIfCounter Counter to distinguish from different IF's
 */
void GenElseStart(int LocalIfCounter);

/**
 * @brief Generates code for the end of an else statement.
 * @param LocalIfCounter Counter to distinguish from different IF's
 */
void GenElseEnd(int LocalIfCounter);

/**
 * @brief Generates code for the start of a while loop.
 * @param LocalIfCounter Counter to distinguish from different WHILE's
 */
void GenWhile(int LocalWhileCounter);

/**
 * @brief Generates code for the start of a while loop with a non-nil check.
 * @param LocalIfCounter Counter to distinguish from different WHILE's
 */
void GenWhileNN(int LocalWhileCounter);

/**
 * @brief Generates code for the end of a while loop.
 * @param LocalIfCounter Counter to distinguish from different WHILE's
 */
void GenWhileEnd(int LocalWhileCounter);

/**
 * @brief Generates code for output statements.
 *
 * @param node The AST node representing the output statement.
 */
void GenerateOutput(tAstNode *node);

#endif /** GENERATOR_H **/