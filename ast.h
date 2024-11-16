/**
 * @file ast.h
 * @brief Hlavickovy soubor pro datovu strukturu abstraktniho syntaktickeho stromu
 * @author Nikola Jordanov xjordan00
 */

#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "scanner.h"
#include "stack.h"

//declarace uzlu AST
typedef struct ast_node tAstNode;

//union na ukladani ciselnou hodnotu
typedef union value{
	int i;
	double f;
}tValue;

//enum na rozeznavani typu hodnot
typedef enum type{
	VOID,
	I32,
	F64,
	U8,
	NI32,
	NF64,
	NU8,
}tType;

//union na ukladani navratove hodnoty
typedef union ret_val{
	int i;
	double f;
	char *str;
	void *nil;
}tRetVal;

//struktura pro zadavani argumenty do definice funkci
typedef struct arg_def{
	tToken name_token;
	tToken type_token;
	struct arg_def *next;
}tArgDef;

//struktura uzlu definice funkci
typedef struct func_def{
	tToken token;
	tArgDef *args;
	tToken ret_type_token;
	tAstNode *code;
}tFuncDef;

//union na ukladani hodnot cisel nebo nazvy promen
typedef union id_or_val{
	char *name;
	int i;
	double f;
}tIdOrVal;

//enum pro vyber z tIdOrVal
typedef enum id_or_val_type{
	ID,
	INT,
	FLOAT
}tIdOrValType;

//struktura pro ukladani argumenty volanych funkci
typedef struct args{
	tToken token;
	struct args *next;
}tArgs;

//struktura uzlu pro volani funkce
typedef struct func_call{
	tToken name_token;
	tArgs *args;
}tFuncCall;

//struktura uzlu pro deklaraci promenne
typedef struct decl_var{
	tToken token;
	tType type;
}tVarDecl;

//struktura uzlu pro ukladani hodnoty do promenne
typedef struct assign{
	tAstNode *dst;
	tAstNode *src;
}tAssign;

//struktura uzlu pro binarni operace
typedef struct bin_op{
	tToken token;
	tAstNode *op1;
	tAstNode *op2;
	tValue res_val;
	tIdOrValType ret_type;
}tBinOp;

//struktura uzlu pro if block
typedef struct if_block{
	tAstNode *nn_id;
	tAstNode *expr;
	tAstNode *if_code;
	tAstNode *else_code;
}tIfBlock;

//struktura uzlu pro while loop
typedef struct while_loop{
	tAstNode *nn_id;
	tAstNode *expr;
	tAstNode *code;
}tWhileLoop;

//struktura uzlu cisla
typedef struct Num_val{
	tToken token;
}tNumVal;

//struktura uzlu promenne
typedef struct var{
	tToken token;
	bool is_const;
	tRetVal val;
	tType type;	
}tVar;

//structura uzlu statement
typedef struct statement{
	tAstNode *next_statement;
	tAstNode *function;
}tStatement;

//struktura uzlu kodu
typedef struct code{
	tAstNode *next_code;
	tAstNode *operation;
}tCode;

//union pro struktury uzlu
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
}tStructure;

//enum na pamatovani jaka je struktura uzlu
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
	FUNC_DEF
}tAstNodeType;

//definice uzlu stromu
struct ast_node{
	tStructure structure;
	tAstNodeType type;
};

void AddFuncCallNode(tAstNode **node_dest, tToken id_t, tTokenStack *arg_stack);
void AddAssignNode(tAstNode **node_dest);
void AddDeclNode(tAstNode **node_dest, tTokenStack *stack);
void AddWhileNode(tAstNode **node_dest);
void AddIfBlockNode(tAstNode **node_dest);
void AddFuncDefNode(tAstNode **node_dest, tTokenStack *stack, tTokenStack *arg_stack);
void AddStatmentNode(tAstNode **node_dest);
void AddCodeNode(tAstNode **node_dest);
void AddExpNode(tAstNode **node_dest);

#endif /** AST_H **/
