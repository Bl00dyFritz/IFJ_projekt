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

/**
 * @brief Typ uzel binarnih vyhledavaciho stromu
 */
typedef struct bst_node tBstNode;

/**
 * @brief Struktura pro zadavani argumenty do definice funkci
 */
typedef struct arg_def{
	tToken name_token; //token obsahujici nazvu argumentu
	tToken type_token; //token obsahujici ocekavany typ argumentu
	struct arg_def *next; //ukazatel na dalsi argument
}tArgDef;

/**
 * @brief Data uzlu pro f-ce
 */
typedef struct function_vals{
	tArgDef *params;		//seznam parametru
	int paramCnt;		//velikost pole parametru
	tTokenType ret_type;//navratovy typ
	struct bst_node *loc_bst;	//odkaz na lokalni vyhledavaci strom
	bool is_used;
}tFunctionVals;

/**
 * @brief Data uzlu pro promenou
 */
typedef struct var_vals{
	void *value;		//odkaz na hodnota promenne
	tTokenType type;	//datovy typ promenne
	bool is_used;
	bool is_constant;	//odlisovani const od var
}tVarVals;

/**
 * @brief Typ parametru v uzlu
 */
typedef enum{
	FUNCTION,
	VARIABLE
}tBstNodeContentType;

/**
 * @brief Struktura na ukladani data do uzlu
 */
typedef struct node_content{
	void *value;				//ukazatel na misto v pameti kde je hodnota
	tBstNodeContentType type;	//typ hodnoty, potrebne pro zmena ukazatele z void na prislusny typ
}tBstNodeContent;

/**
 * @brief Struktura uzlu bvs
 */
struct bst_node{
	char *key;					//klic == nazev promenne ci funkce
	tBstNodeContent content;	//data uzlu
	struct bst_node *left;		//odkaz na levy potomek
	struct bst_node *right;		//odkaz na pravy potomek
};

/**
 * @brief Inicializace stromu
 * @param tree Odkaz an ukazatel, ukazujici na koren (pod)stromu
 */
void BstInit(tBstNode **tree);

/**
 * @brief Pridani novy prvek do stromu
 * @param tree Odkaz na (pod)strom do kteryho se bude ukladat
 * @param key Klic ukladaneho prvku
 * @param content Data ukladaneho prvku
 */
void BstInsert(tBstNode **tree, char *key, tBstNodeContent content);

/**
 * @brief Funkce ktera prida prvek do stromu a pak ho vyrovna
 * @param tree Odkaz na vypracovavany strom
 * @param key Klic noveho prvku
 * @param content Data noveho prvku
 */
void BstInsAndReal(tBstNode **tree, char *key, tBstNodeContent content);

/**
 * @brief Vyhledavani prvku v stromu
 * @param tree Odkaz na strom ve kterem se vyhledava
 * @param key Vyhledavany klic (nazev promenne ci funkce)
 * @param content Odkaz kde se ma vratit hodnota nalezeneho uzlu v pripade uspechu
 * @return True pokud je uzel nalezen, jinak false
 */
bool BstSearch(tBstNode *tree, char *key, tBstNodeContent **content);

/**
 * @brief Odstraneni jednoho uzlu se stromu
 * @param tree Odkaz na strom ve ktereho se ma odstranit prvek
 * @param key Klic heldaneho uzlu
 */
void BstDelete(tBstNode **tree, char *key);

/**
 * @brief Funkce ktera odstrani prvek ze stromu a pak vyrovna strom
 * @param tree Cilovy strom
 * @param key Klic prvku ktery se ma odstranit
 */
void BstDelAndReal(tBstNode **tree, char *key);

/**
 * @brief Vycisteni stromu, a vraceni do inicializovaneho stavu
 * @param tree Odkaz na cilovy strom
 */
void BstDispose(tBstNode **tree);

/**
 * @brief Pomocna funkce ktera vymeni zadany uzel za nejpravjejsi uzel v zadanem (pod)stromu
 * @param target Cilovy uzel, ktery se ma vymenit
 * @param tree Odkaz na strom ve ktereho se hleda novy uzel
 */
void ReplaceByRightmost(tBstNode *target, tBstNode **tree);

/**
 * @brief Pomocna funkce ktera vymeni zadany usel za nejlevjesi ulzel v zadanem (pod)stromu
 * @param target Cilovy uzel, ktery se ma zmenit
 * @param tree Odkaz na strom ve ktereho se hleda novy uzel
 */
void ReplaceByLeftmost(tBstNode *target, tBstNode **tree);

/**
 * @brief Pomocne funkce ktera vraci velikost (pod)stromu
 * @param tree Mereny strom
 * @param first_break Ukazatel na nejnizi uzel kde vyvazenost je narusena, pokud neni narusena hodnota ukazatele se nemeni
 * @return vyska stromu jako cele cislo int; pokud je narisena vyskova vyvazenost vraci -1
 */
int CheckHeight(tBstNode *tree, tBstNode **first_break);

/**
 * @brief Pomocna funkce na uvolneni naalokovanou pamet v datech daneho uzlu
 * @param tree Odkaz na Uzel ze ktereho se bude uvolnovat pamet
 */
void FreeNodeContent(tBstNode **tree);

/**
 * @brief Pomocna funkce na rotace stromu o jeden uzel doleva pri vnejsi nevyvazenost
 * @param tree Odkaz na strom k rotaci
 */
void RotLeft(tBstNode **tree);

/**
 * @brief Pomocna funkce na rotace stromu o jeden uzel doprava pri vnejsi nevyvazenost
 * @param tree Odkaz na strom k rotaci
 */
void RotRight(tBstNode **tree);

/**
 * @brief Pomocna funkce na rotace stromu o jeden uzel doleva pri vnitrni nevyvazenost
 * @param tree Odkaz na strom k rotaci
 */
void RotRLeft(tBstNode **tree);

/**
 * @brief Pomocna funkce na rotace stromu o jeden uzel doprava pri vnitrni nevyvazenost
 * @param tree Odkaz na strom k rotaci
 */
void RotLRight(tBstNode **tree);

/**
 * @brief Funkce ktera kontroluje poruseni vyskove vyvazenosti a opravuje ji
 * @param tree (Pod)strom ktery je kontrolovan
 */
void Realign(tBstNode **tree);

#endif /** SYMTABLE_H **/
