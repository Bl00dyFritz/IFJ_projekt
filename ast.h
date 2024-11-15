/**
 * @file ast.h
 * @brief Hlavickovy soubor pro datovu strukturu abstraktniho syntaktickeho stromu
 * @author Nikola Jordanov xjordan00
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "scanner.h"

//declarace uzlu AST
typedef struct ast_node tAstNode;

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
	char *name;
	tType type;
	struct arg_def *next;
}tArgDef;

//struktura uzlu definice funkci
typedef struct func_def{
	char *name;
	tArgDef *args;
	tType ret_type;
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
	tIdOrVal data;
	tIdOrValType data_type;
	struct args *next;
}tArgs;

//struktura uzlu pro volani funkce
typedef struct func_call{
	char *name;
	tRetVal ret_val;
	tType ret_type;
	tArgs *args;
}tFuncCall;

//struktura uzlu pro declaraci konstanty
typedef struct decl_const{
	char *name;
	tType type;
}tConstDecl;

//struktura uzlu pro deklaraci promenne
typedef struct decl_var{
	char *name;
	tType type;
}tVarDecl;

//struktura uzlu pro ukladani hodnoty do promenne
typedef struct assign{
	char *dst;
	tAstNode *src;
}tAssign;

//enum na odlisovani binarnich operatoru
typedef enum op_type{
	MUL,
	DIV,
	ADD,
	SUB,
	EQ,
	NEQ,
	LESS,
	MORE,
	LESS_EQ,
	MEQ_EQ
}tOpType;

//struktura uzlu pro binarni operace
typedef struct bin_op{
	tAstNode *op1;
	tAstNode *op2;
	tOpType operation;
}tBinOp;

//struktura uzlu pro if block
typedef struct if_block{
	tAstNode *expr;
	tAstNode *if_code;
	tAstNode *else_code;
}tIfBlock;

//struktura uzlu pro while loop
typedef struct while_loop{
	tAstNode *expr;
	tAstNode *code;
}tWhileLoop;

//union na ukladani ciselnou hodnotu
typedef union value{
	int i;
	double f;
}tValue;

//struktura uzlu cisla
typedef struct Num_val{
	tValue val;
	tIdOrValType type;
}tNumVal;

//struktura uzlu promenne
typedef struct var{
	char *name;
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
	tValue val;
	tWhileLoop while_loop;
	tIfBlock if_block;
	tBinOp bin_op;
	tAssign assign;
	tConstDecl const_decl;
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
