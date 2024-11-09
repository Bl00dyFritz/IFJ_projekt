/**
 * Implementace tabulky symbolu imperativniho jazyka IFJ24
 * @file symtable.h
 * @brief Hlavickovy soubor pro tabulka symbolu
 * @author Nikola Jordanov, xjordan00
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <string.h>
#include <stdbool.h>
#include "scanner.h"

struct bst_node;

//vstupni parametr f-ce
typedef struct param_type{
	char *name;			//ID parametru
	tTokenType type;	//typ parametru
}tParam;

//data uzlu pro f-ce
typedef struct function_vals{
	tParam *params;		//pole parametru
	int paramCnt;		//velikost pole parametru
	tTokenType ret_type;//navratovy typ
	struct bst_node *loc_bst;	//odkaz na lokalni vyhledavaci strom
	bool defined;		//ulozeni ci f-ce byla definovana
}tFunctionVals;

//data uzlu pro promenou
typedef struct var_vals{
	void *value;		//odkaz na hodnota promenne
	tTokenType type;	//datovy typ promenne
	bool is_constant;	//odlisovani const od var
}tVarVals;

//typ parametru v uzlu
typedef enum{
	FUNCTION = 0,
	VARIABLE
}tBstNodeContentType;

//structura na ukladani data do uzlu
typedef struct node_content{
	void *value;				//ukazatel na misto v pameti kde je hodnota
	tBstNodeContentType type;	//typ hodnoty, potrebne pro zmena ukazatele z void na prislusny typ
}tBstNodeContent;

//struktura uzlu bvs
typedef struct bst_node{
	char *key;					//klic == nazev promenne ci funkce
	tBstNodeContent content;	//data uzlu
	struct bst_node *left;		//odkaz na levy potomek
	struct bst_node *right;		//odkaz na pravy potomek
}tBstNode;

void BstInit(tBstNode **tree);

void BstInsert(tBstNode **tree, char *key, tBstNodeContent content);
void BstInsAndReal(tBstNode **tree, char *key, tBstNodeContent content);

bool BstSearch(tBstNode *tree, char *key, tBstNodeContent **content);

void BstDelete(tBstNode **tree, char *key);
void BstDelAndReal(tBstNode **tree, char *key);
void BstDispose(tBstNode **tree);

void ReplaceByRightmost(tBstNode *target, tBstNode **tree);
void ReplaceByLeftmost(tBstNode *target, tBstNode **tree);
int CheckHeight(tBstNode *tree, tBstNode **first_break);
void FreeNodeContent(tBstNode **tree);

void RotLeft(tBstNode **tree);
void RotRight(tBstNode **tree);
void RotRLeft(tBstNode **tree);
void RotLRight(tBstNode **tree);
void Realign(tBstNode **tree);

#endif
