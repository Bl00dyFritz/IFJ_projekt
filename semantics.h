/**
 * Implementation of semantic analysis of imperative language IFJ24
 * @file semantics.h
 * @brief Header file for semantic analysis
 * @author Nikola Jordanov xjordan00
 */

#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symtable.h"
#include "error.h"

/**
 * @brief the structure of the lookup table list element
 */
typedef struct symtable_list_elem{
	tBstNode *root_ptr;
	struct symtable_list_elem *next;
}tSymtableListElem;

/**
 * @brief lookup table list structure
 */
typedef struct symtable_list{
	tSymtableListElem *first;
}tSymtableList;

/**
 * @brief auxiliary structure for passing data between functions
 */
typedef struct com_link{
	void *val;		//Pointer to values, intended for division
	tType type; 	//The type of the given value
	bool is_var; 	//true if it is a variable or expression, false if it is a literal or constant with a known value
}tComData;

/**
 * @brief Function initializing a list of lookup tables
 * @param list Pointer to the list to be initialized
 */
void InitSymtableList (tSymtableList *list);

/**
 * @brief A function that adds another element to the beginning of a list
 * @param list Pointer to the list to which the element should be added
 * @param symtree pointer to the root node of the lookup table to be added
 */
void SymtableListAdd (tSymtableList *list, tBstNode *symtree);

/**
 * @brief A function that removes the first element from a list
 * @param list Pointer to the list from which to remove the element
 */
void SymtableListRemove (tSymtableList *list);

/**
 * @brief Assigns values to a variable based on the provided data.
 * @param vals Pointer to the variable values structure.
 * @param in_data Input data to be assigned.
 */
void AssignVals(tVarVals **vals, tComData in_data);

/**
 * @brief Assigns an integer value to the variable values structure.
 * @param vals Pointer to the variable values structure.
 * @param in_val Pointer to the integer value to be assigned.
 */
void AssignInt(tVarVals **vals, void *in_val);

/**
 * @brief Assigns a double value to the variable values structure.
 * @param vals Pointer to the variable values structure.
 * @param in_val Pointer to the double value to be assigned.
 */
void AssignDouble(tVarVals **vals, void *in_val);

/**
 * @brief Analyzes a built-in function and assigns the output type accordingly.
 * @param node Pointer to the AST node representing the built-in function.
 * @param out_type Pointer to store the output type.
 */
void ExamineBuiltInFunc(tAstNode *node, tType *out_type, tSymtableList *symlist);

/**
 * @brief Assigns a type to the variable values structure.
 * @param vals Pointer to the variable values structure.
 * @param in_type Type to be assigned.
 */
void AssignType(tVarVals **vals, tType in_type);

/**
 * @brief Compares two variables and stores the result in the output data.
 * @param op1_data Data of the first operand.
 * @param op2_data Data of the second operand.
 * @param out_data Pointer to store the result of the comparison.
 */
void CmpVars(tComData op1_data, tComData op2_data, tComData *out_data);

/**
 * @brief Compares a variable with a literal and stores the result in the output data.
 * @param op1_data Data of the variable operand.
 * @param op2_data Data of the literal operand.
 * @param out_data Pointer to store the result of the comparison.
 * @param is_div Boolean indicating if the operation involves division.
 */
void CmpVarLit(tComData op1_data, tComData op2_data, tComData *out_data, bool is_div);

/**
 * @brief Checks the usage of variables in a given symbol table tree.
 * @param tree Pointer to the symbol table tree.
 */
void BstCheckUse(tBstNode *tree);

/**
 * @brief Analyzes a function definition, adding it to the symbol table and verifying semantics.
 * @param node Pointer to the AST node representing the function definition.
 * @param symlist Pointer to the symbol table list.
 */
void ExamineFunctionDef(tAstNode *node, tSymtableList *symlist);

/**
 * @brief Analyzes a function call, checking arguments and return type.
 * @param node Pointer to the AST node representing the function call.
 * @param symlist Pointer to the symbol table list.
 * @param out_type Pointer to store the output type of the function call.
 */
void ExamineFunctionCall(tAstNode *node, tSymtableList *symlist, tType *out_type);

/**
 * @brief Examines a variable, checking its declaration and usage.
 * @param node Pointer to the AST node representing the variable.
 * @param symlist Pointer to the symbol table list.
 * @param in_data Pointer to the input data for examination.
 * @param out_data Pointer to store the output data.
 */
void ExamineVar(tAstNode *node, tSymtableList *symlist, tComData *in_data, tComData *out_data);

/**
 * @brief Analyzes a value node and determines its type and value.
 * @param node Pointer to the AST node representing the value.
 * @param out_data Pointer to store the output data.
 */
void ExamineVal(tAstNode *node, tComData *out_data);

/**
 * @brief Examines a variable or constant declaration, updating the symbol table accordingly.
 * @param node Pointer to the AST node representing the declaration.
 * @param symlist Pointer to the symbol table list.
 */
void ExamineDecl(tAstNode *node, tSymtableList *symlist);

/**
 * @brief Analyzes a while loop, verifying the condition and the body.
 * @param node Pointer to the AST node representing the while loop.
 * @param symlist Pointer to the symbol table list.
 * @param ret_type Pointer to store the return type.
 */
void ExamineWhile(tAstNode *node, tSymtableList *symlist, tTokenType *ret_type);

/**
 * @brief Analyzes an if statement, checking the condition and the correctness of both branches.
 * @param node Pointer to the AST node representing the if statement.
 * @param symlist Pointer to the symbol table list.
 * @param ret_type Pointer to store the return type.
 */
void ExamineIf(tAstNode *node, tSymtableList *symlist, tTokenType *ret_type);

/**
 * @brief Examines a binary operation, such as addition or multiplication, and verifies operand types.
 * @param node Pointer to the AST node representing the binary operation.
 * @param symlist Pointer to the symbol table list.
 * @param out_data Pointer to store the output data.
 */
void ExamineBinOp(tAstNode *node, tSymtableList *symlist, tComData *out_data);

/**
 * @brief Analyzes an assignment operation, ensuring the target variable is declared and the types are compatible.
 * @param node Pointer to the AST node representing the assignment.
 * @param symlist Pointer to the symbol table list.
 */
void ExamineAssign(tAstNode *node, tSymtableList *symlist);

/**
 * @brief Analyzes a return statement, verifying the returned value matches the function's return type.
 * @param node Pointer to the AST node representing the return statement.
 * @param symlist Pointer to the symbol table list.
 * @param ret_type Pointer to store the return type.
 */
void ExamineRet(tAstNode *node, tSymtableList *symlist, tTokenType *ret_type);

/**
 * @brief Checks for the presence of the main function
 * @param tree the root of the function symtable binary searchtree
 * @return returns 1 if the main function has been found otherwise 0
 */
int CheckMain(tBstNode *tree);

/**
 * @brief Conducts a semantic analysis on a given AST node, ensuring all nodes are used correctly and adhere to the language rules.
 * @param node Pointer to the AST node to be analyzed.
 * @param symlist Pointer to the symbol table list.
 * @param in_data Pointer to the input data for examination.
 * @param out_data Pointer to store the output data.
 * @param ret_type Pointer to store the return type.
 */
void ExamineSemantics(tAstNode *node, tSymtableList *symlist, tComData *in_data, tComData *out_data, tTokenType *ret_type);

#endif /** SEMANTICS_H **/
