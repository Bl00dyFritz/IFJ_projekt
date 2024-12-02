/**
 * @file ast.h
 * @brief Header file for the data structure of the abstract syntax tree
 * @author Nikola Jordanov xjordan00
 */

#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "scanner.h"
#include "stack.h"
#include "symtable.h"

/**
 * @brief AST node
 */
typedef struct ast_node tAstNode;

/**
 * @brief Union to store a numeric value
 */
typedef union value{
	int i;
	double f;
}tValue;

/**
 * @brief Enum for vlaue type recognizion
 */
typedef enum type{
	VOID,
	I32,
	F64,
	U8,
	NI32,
	NF64,
	NU8,
	BIN,
	UNDEF
}tType;

/**
 * @brief Union to store return value
 */
typedef union ret_val{
	int i;
	double f;
	char *str;
	void *nil;
}tRetVal;

/**
 * @brief Function definition node structure
 */
typedef struct func_def{
	tToken token;					//token containing the function name
	tArgDef *args;					//argument list
	tToken ret_type_token;			//token containing the function's return type
	tBstNode *internal_symtable;	//pointer to the symbol table of the given function
	tAstNode *code; 				//pointer to the code node of the given function
	tBstNode *loc_symtree; 			//pointer to the root of the local search tree
}tFuncDef;

/**
 * @brief Union for storing number values ​​or variable names
 */
typedef union id_or_val{
	char *name;
	int i;
	double f;
}tIdOrVal;

/**
 * @brief Enum to select from tIdOrVal
 */
typedef enum id_or_val_type{
	ID,
	INT,
	FLOAT
}tIdOrValType;

/**
 * @brief Structure for storing arguments of function calls
 */
typedef struct args{
	tToken token;		//argument name
	struct args *next; 	//pointer to next argument
}tArgs;

/**
 * @brief Node structure for function calls
 */
typedef struct func_call{
	tToken name_token;	//function name
	tArgs *args; 		//argument list
	int arg_cnt;
}tFuncCall;

/**
 * @brief Node structure for variable declaration
 */
typedef struct decl_var{
	tToken token;	//token containing variable name
	tType type;		//variable type
}tVarDecl;

/**
 * @brief Struktura uzlu pro ukladani hodnoty do promenne
 */
typedef struct assign{
	tAstNode *dst; //pointer to target variable node
	tAstNode *src; //pointer to the expression node of the assigned value
}tAssign;

/**
 * @brief Node structure for binary operations
 */
typedef struct bin_op{
	tToken token;			//token containing operator type
	tAstNode *op1;			//pointer to left oerand node
	tAstNode *op2;			//pointer to node of right operand
	tValue res_val; 		//found the value of the expression
	tIdOrValType ret_type; 	//result value type
}tBinOp;

/**
 * @brief Node structure for if block
 */
typedef struct if_block{
	tAstNode *nn_id; 			//node pointer to nonzero variable
	tAstNode *expr; 			//pointer to truth expression
	tBstNode *if_symtree; 		//pointer to local symbol tree for if block
	tBstNode *else_symtree; 	//pointer to a local symbol tree for the else block
	tAstNode *if_code; 			//pointer to the executed code in case of true expression
	tBstNode *if_symtable; 		//pointer to the if block symbol table
	tBstNode *else_symtable; 	//pointer to the else block's symbol table
	tAstNode *else_code; 		//pointer to executed code in the opposite case
}tIfBlock;

/**
 * @brief Node structure for while loop
 */
typedef struct while_loop{
	tAstNode *nn_id; 		//node pointer to nonzero variable
	tAstNode *expr; 		//pointer to truth expression
	tBstNode *loc_symtree;	//pointer to local symbol tree
	tAstNode *code; 		//pointer to cycle code
}tWhileLoop;

/**
 * @brief Value node structure
 */
typedef struct Num_val{
	tToken token;	//token containing the value of a numeric node or a string term
}tNumVal;

/**
 * @brief Node structure for variables
 */
typedef struct var{
	tToken token;	//token containing variable name
	bool is_const; 	//if true it's const, otherwise var
	tRetVal val; 	//value stored in variable
	tType type;	 	//value type
}tVar;

/**
 * @brief Node structure for statement
 */
typedef struct statement{
	tAstNode *next_statement;	//pointer to next statement node
	tAstNode *function; 		//pointer to function definition node
}tStatement;

/**
 * @brief Node structure for code
 */
typedef struct code{
	tAstNode *next_code; //pointer to next code
	tAstNode *operation; //pointer to processed code expression
}tCode;

typedef struct ret{
	tAstNode *ret_expr;
	tType type;
	tValue val;
}tRet;

/**
 * @brief Union for node structures
 */
typedef union ast_node_structure{
	tStatement statement;
	tCode code;
	tVar var;
	tNumVal val;
	tWhileLoop while_loop;
	tIfBlock if_block;
	tBinOp bin_op;
	tAssign assign;
	tVarDecl var_decl;
	tFuncCall func_call;
	tFuncDef func_def;
	tRet ret;
}tStructure;

/**
 * @brief Enum to remember what the structure of the node is
 */
typedef enum ast_node_type{
	STATEMENT,
	CODE,
	VAR,
	VAL,
	WHILE,
	IF,
	BIN_OP,
	ASSIGN,
	CONST_DECL,
	VAR_DECL,
	FUNC_CALL,
	FUNC_DEF,
	RET
}tAstNodeType;

//tree node definition
struct ast_node{
	tStructure structure; 	//node data structure
	tAstNodeType type; 		//data structure type
};

/**
 * @brief Function for storing a node of type FuncCall in ast
 * @param node_dest Pointer to the position in the tree where the node should be added
 * @param id_t Token containing the name of the function
 * @param arg_stack A stack containing name or value tokens of the argument
 */
void AddFuncCallNode(tAstNode **node_dest, tToken id_t, tTokenStack *arg_stack);

/**
 * @brief Function for storing a node of type Assign in ast
 * @param node_dest Pointer to the position in the tree where the node should be added
 */
void AddAssignNode(tAstNode **node_dest);

/**
 * @brief Function to store node of type VarDecl in ast
 * @param node_dest Pointer to the position in the tree where the node should be added
 * @param stack Stack containing node information: Type, name
 */
void AddDeclNode(tAstNode **node_dest, tTokenStack *stack);

/**
 * @brief Function to save a WhileLoop type node to ast
 * @param node_dest Pointer to the position in the tree where the node should be added
 */
void AddWhileNode(tAstNode **node_dest);

/**
 * @brief Function to save a node of type IfBlock to ast
 * @param node_dest Pointer to the position in the tree where the node should be added
 */
void AddIfBlockNode(tAstNode **node_dest);

/**
 * @brief Function for storing a node of type FuncDef in ast
 * @param node_dest Pointer to the position in the tree where the node should be added
 * @param stack Stack in which node information is placed: type, name
 * @param arg_stack The stack in which the information of the defined argument of the function is placed: type, name (it is expected that it will always be received in pairs in this order n times, where n is the number of parameters)
 */
void AddFuncDefNode(tAstNode **node_dest, tTokenStack *stack, tTokenStack *arg_stack);

/**
 * @brief Function for storing a node of type Statement in ast
 * @param node_dest Pointer to the position in the tree where the node should be added
 */
void AddStatmentNode(tAstNode **node_dest);

/**
 * @brief Function for saving a node of type Code to ast
 * @param node_dest Pointer to the position in the tree where the node should be added
 */
void AddCodeNode(tAstNode **node_dest);

/**
 * @brief Function to save node type Ret to ast
 * @param node_dest Pointer to the position in the tree where the node should be added
 */
void AddRetNode(tAstNode **node_dest);

/**
 * @brief A function that composes a subtree from the start to ast
 * @param node_dest The position where the expression subtree should be stored
 * @param stack Precedentially arranged stack
 */
void AddExpNodes(tAstNode **node_dest, tTokenStack *stack);

/**
 * @brief A function that performs a precedent analysis and builds a stack of output tokens according to it, which is then used to build an expression subtree in ast
 * @param in_t Input token, its value is precedentially compared with the value of the top of the input stack
 * @param input_stack The input stack, depending on the result of the precedent processing, either the in_t value is stored in it or its top is moved to the top of the output stack
 * @param output_stack Output stack into which operations/operands are stored according to precedent
 */
void PrecedenceCheck(tToken *in_t, tTokenStack *input_stack, tTokenStack *output_stack);

/**
 * @brief A function that deletes an array and frees the memory it occupies
 * @param tree Pointer to the beginning of the tree
 */
void AstDispose(tAstNode **tree);

#endif /** AST_H **/
