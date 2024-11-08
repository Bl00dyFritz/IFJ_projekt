/**
 * Implementace tabulky symbolu imperativniho jazyka IFJ24
 * @file symtable.c
 * @brief Implementace tabulky symbolu
 * @author Nikola Jordanov, xjordan00
 */

#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Inicializace stromu
 * @param tree Odkaz an ukazatel, ukazujici na koren (pod)stromu
 */
void BstInit(tBstNode **tree){
	(*tree) = NULL;
}

/**
 * @brief Vyhledavani prvku v stromu
 * @param tree Odkaz na strom ve kterem se vyhledava
 * @param key Vyhledavany klic (nazev promenne ci funkce)
 * @param content Odkaz kde se ma vratit hodnota nalezeneho uzlu v pripade uspechu
 * @return True pokud je uzel nalezen, jinak false
 */
bool BstSearch(tBstNode *tree, char *key, tBstNodeContent **content){
	if (!tree) return false;	//pri prazdnem stromu vraci false
	else{
		if (!strcmp(tree->key, key)){	//v pripad nalezeni uzlu, vrati True a ukazatel na data uzlu
			(*content) = &tree->content;
			return true;
		}
		else{
			if (strcmp(tree->key, key)>0){
				return BstSearch(tree->left, key, content);
			}
			else
				return BstSearch(tree->right, key, content);
		}
	}
}

/**
 * @brief Pridani novy prvek do stromu
 * @param tree Odkaz na (pod)strom do kteryho se bude ukladat
 * @param key Klic ukladaneho prvku
 * @param content Data ukladaneho prvku
 */
void BstInsert(tBstNode **tree, char *key, tBstNodeContent content){
	//TODO 
}

/**
 * @brief Pomocna funkce ktera vymeni zadany uzel za nejpravjejsi uzel v zadanem stromu
 * @param target Cilovy uzel, ktery se ma vymenit
 * @param tree Odkaz na strom ve ktereho se hleda novy uzel
 */
void ReplaceByRightmost(tBstNode *target, tBstNode **tree){
	//TODO
}

/**
 * @brief Odstraneni jednoho uzlu se stromu
 * @param tree Odkaz na strom ve ktereho se ma odstranit prvek
 * @param key Klic heldaneho uzlu
 */
void BstDelete(tBstNode **tree, char *key){
	//TODO
}

/**
 * @brief Vycisteni stromu, a vraceni do inicializovaneho stavu
 * @param tree Odkaz na cilovy strom
 */
void BstDispose(tBstNode **tree){
	//TODO
}
