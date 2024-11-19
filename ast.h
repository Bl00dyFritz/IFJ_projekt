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

/**
 * @brief Uzel AST
 */
typedef struct ast_node tAstNode;

/**
 * @brief Union na ukladani ciselnou hodnotu
 */
typedef union value{
	int i;
	double f;
}tValue;

/**
 * @brief Enum na rozeznavani typu hodnot
 */
typedef enum type{
	VOID,
	I32,
	F64,
	U8,
	NI32,
	NF64,
	NU8,
}tType;

/**
 * @brief Union na ukladani navratove hodnoty
 */
typedef union ret_val{
	int i;
	double f;
	char *str;
	void *nil;
}tRetVal;

/**
 * @brief Struktura pro zadavani argumenty do definice funkci
 */
typedef struct arg_def{
	tToken name_token; //token obsahujici nazvu argumentu
	tToken type_token; //token obsahujici ocekavany typ argumentu
	struct arg_def *next; //ukazatel na dalsi argument
}tArgDef;

/**
 * @brief Struktura uzlu definice funkci
 */
typedef struct func_def{
	tToken token; //token obsahujici nazev funkci
	tArgDef *args; //seznam argumentu
	tToken ret_type_token; //token obsahujici navratovy typ funkci
	tAstNode *code; //ukazatel na uzel kodu dane funkce
}tFuncDef;

/**
 * @brief Union na ukladani hodnot cisel nebo nazvy promen
 */
typedef union id_or_val{
	char *name;
	int i;
	double f;
}tIdOrVal;

/**
 * @brief Enum pro vyber z tIdOrVal
 */
typedef enum id_or_val_type{
	ID,
	INT,
	FLOAT
}tIdOrValType;

/**
 * @brief Struktura pro ukladani argumenty volanych funkci
 */
typedef struct args{
	tToken token; //nazev argumentu
	struct args *next; //ukazatel na dalsi argument
}tArgs;

/**
 * @brief Struktura uzlu pro volani funkce
 */
typedef struct func_call{
	tToken name_token; //nazev funkce
	tArgs *args; //seznam argumentu
}tFuncCall;

/**
 * @brief Struktura uzlu pro deklaraci promenne
 */
typedef struct decl_var{
	tToken token; //token obsahujici nazev promenne
	tType type; //token obsahujici typ promenne
}tVarDecl;

/**
 * @brief Struktura uzlu pro ukladani hodnoty do promenne
 */
typedef struct assign{
	tAstNode *dst; //ukazatel na uzel cilove promenne
	tAstNode *src; //ukazatel na uzel vyrazu prirazovane hodnoty
}tAssign;

/**
 * @brief Struktura uzlu pro binarni operace
 */
typedef struct bin_op{
	tToken token; //token obsahujici typ operatora
	tAstNode *op1; //ukazatel na uzel leveho oeranda
	tAstNode *op2; //ukazatel na uzel praveho operanda
	tValue res_val; //vysledan hodnota vyrazu
	tIdOrValType ret_type; //typ vysledne hodnoty
}tBinOp;

/**
 * @brief Struktura uzlu pro if block
 */
typedef struct if_block{
	tAstNode *nn_id; //ukazatel na uzel nenulove promenne
	tAstNode *expr; //ukazatel na pravdivostni vyraz
	tAstNode *if_code; //ukazatel na vykonavany kod v pripade splneni pravdivostniho vyrazu
	tAstNode *else_code; //ukazatel na vykonavany kod v opacnem pripadu
}tIfBlock;

/**
 * @brief Struktura uzlu pro while loop
 */
typedef struct while_loop{
	tAstNode *nn_id; //ukazatel na uzel nenulove promenne
	tAstNode *expr; //ukazatel na pravdivostni vyraz
	tAstNode *code; //ukazatel na kod cyklu
}tWhileLoop;

/**
 * @brief Struktura uzlu cisla
 */
typedef struct Num_val{
	tToken token; //token obsahujici hodnotu ciselneho uzlu
}tNumVal;

/**
 * @brief Struktura uzlu promenne
 */
typedef struct var{
	tToken token; //token obsahujici jmeno promenne
	bool is_const; //pokud true jde o const, jinak var
	tRetVal val; //hodnota ulozena v promenne
	tType type;	 //typ hodnoty
}tVar;

/**
 * @brief Struktura uzlu statement
 */
typedef struct statement{
	tAstNode *next_statement; //ukazatel na dalsi statement uzel
	tAstNode *function; //ukazatel na uzel definice funkci
}tStatement;

/**
 * @brief Struktura uzlu kodu
 */
typedef struct code{
	tAstNode *next_code; //ukazatel na dalsi kod
	tAstNode *operation; //ukazatel na zpracovavany vyraz kodu
}tCode;

/**
 * @brief Union pro struktury uzlu
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
}tStructure;

/**
 * @brief Enum na pamatovani jaka je struktura uzlu
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
	FUNC_DEF
}tAstNodeType;

//definice uzlu stromu
struct ast_node{
	tStructure structure; //struktura data uzlu
	tAstNodeType type; //typ struktury dat
};

/**
 * @brief Funkce na ukladani uzlu typu FuncCall do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 * @param id_t Token obsahujici nazev funkce
 * @param arg_stack Zasobnik obsahujuci tokeny nazvu nebo hodnoty argumentu
 */
void AddFuncCallNode(tAstNode **node_dest, tToken id_t, tTokenStack *arg_stack);

/**
 * @brief Funkce na ukladani uzlu typu Assign do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 */
void AddAssignNode(tAstNode **node_dest);

/**
 * @brief Funkce na ukladani uzlu typu VarDecl do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 * @param stack Zasobnik ve kterem se nachazy informace uzlu: Typ, nazev
 */
void AddDeclNode(tAstNode **node_dest, tTokenStack *stack);

/**
 * @brief Funkce na ukladani uzlu typu WhileLoop do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 */
void AddWhileNode(tAstNode **node_dest);

/**
 * @brief Funkce na ukladani uzlu typu IfBlock do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 */
void AddIfBlockNode(tAstNode **node_dest);

/**
 * @brief Funkce na ukladani uzlu typu FuncDef do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 * @param stack Zasobnik ve kterem se nahazi informace uzlu: typ, nazev
 * @param arg_stack Zasobnik ve kterem se nazhazi informace definovanych argumentu funkce: typ, nazev (ceka se ze je vzdy dostane ve dvojicich v tomto poradi n-krat, kde n je pocet parametru
 */
void AddFuncDefNode(tAstNode **node_dest, tTokenStack *stack, tTokenStack *arg_stack);

/**
 * @brief Funkce na ukladani uzlu typu Statement do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 */
void AddStatmentNode(tAstNode **node_dest);

/**
 * @brief Funkce na ukladani uzlu typu Code do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 */
void AddCodeNode(tAstNode **node_dest);

/**
 * @brief Funkce ktera sklada podstrom z vyrazu do ast
 * @param node_dest Pozice kde se ma ulozit vyrazovy podstrom
 * @param stack Precedencne usporadany zasobnik
 */
void AddExpNodes(tAstNode **node_dest, tTokenStack *stack);

/**
 * @brief Funkce, ktera provadi precedencni analyzu a dle ni sklada zasobnik vystupnich tokenu, ktery se pak pouzije na skladani vyrazovy podstrom v ast
 * @param in_t Vstupni token, jeho hodnota se precedencne porovnava s hodnotu vrcholu vstupoveho zasobniku
 * @param input_stack Vstupovy zasobnik, dle vysledku precedencniho provovnani se do bud do toho uklada hodnotu in_t nebo jeho vrchol se posouva do vrcholu vystupoveho zasobniku
 * @param output_stack Vystupovy zasobnik do ktereho se podle precedence ukladaji operace/operandy
 */
void PrecedenceCheck(tToken *in_t, tTokenStack *input_stack, tTokenStack *output_stack);

/**
 * @brief Funkce, ktera vymaze ast a uvolni pamet kterou zabira
 * @param tree Ukazatel na zacatek stromu
 */
void AstDispose(tAstNode **tree);

#endif /** AST_H **/
