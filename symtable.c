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
 * @brief Pomocna funkce na uvolneni naalokovanou pamet v datech daneho uzlu
 * @param tree Odkaz na Uzel ze ktereho se bude uvolnovat pamet
 */
void FreeNodeContent(tBstNode **tree){
	if (!tree) exit(99);
	if ((*tree)->content.type==VARIABLE)
		free((*tree)->content.value->value);
	else{
		for (int i = 0; i<(*tree)->content.value.paramCnt; i++){
			free((*tree)->content.value->params[i]->name);
		}
		free((*tree)->content.value->params);
		free((*tree)->content.value);
	}
}

/**
 * @brief Inicializace stromu
 * @param tree Odkaz an ukazatel, ukazujici na koren (pod)stromu
 */
void BstInit(tBstNode **tree){
	if (!tree) exit(99);
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
	if (!tree) exit(99);
	//TODO 
}

/**
 * @brief Pomocna funkce ktera vymeni zadany uzel za nejpravjejsi uzel v zadanem (pod)stromu
 * @param target Cilovy uzel, ktery se ma vymenit
 * @param tree Odkaz na strom ve ktereho se hleda novy uzel
 */
void ReplaceByRightmost(tBstNode *target, tBstNode **tree){
	if (!tree || !target) exit(99);
	if (!(*tree)->right){
		target->key = (*tree)->key;
		FreeNodeContent(&target);
		target->content.value = (*tree)->content.value;
		target->content.type = (*tree)->content.type;
		if((*tree)->left){
			tBstNode *tmp = (*tree)->left;
			(*tree)->key = tmp->key;
			(*tree)->content.value = tmp->content.value;
			(*tree)->content.type = tmp->content.type;
			(*tree)->left = tmp->left;
			(*tree)->right = tmp->right;
			free(tmp);
		}
		else{
			free(*tree);
			(*tree) = NULL;
		}
	}
	else ReplaceByRightmost(target, &(*tree)->right);
}

/**
 * @brief Pomocna funkce ktera vymeni zadany usel za nejlevjesi ulzel v zadanem (pod)stromu
 * @param target Cilovy uzel, ktery se ma zmenit
 * @param tree Odkaz na strom ve ktereho se hleda novy uzel
 */
void ReplaceByLeftmost(tBstNode *target, tBstNode **tree){
	if (!tree || !target) exit(99);
	if (!(*tree)->left){
		target->key = (*tree)->key;
		FreeNodeContent(&target);
		target->content.value = (*tree)->content.value;
		target->content.type = (*tree)->content.type;
		if ((*tree)->right){
			tBstNode *tmp = (*tree)->right;
			(*tree)->key = tmp>key;
			(*tree)->content.value = tmp->content.value;
			(*tree)->content.type = tmp->content.type;
			(*tree)->left = tmp->left;
			(*tree)->right = tmp->right;
			free(tmp);
		}
		else{
			free(*tree);
			(*tree)=NULL;
		}
	}
	else ReplaceByLeftmost(target, &(*tree)->left);
}

/**
 * @brief Pomocne funkce ktera vraci velikost (pod)stromu
 * @param tree mereny strom
 * @return vyska stromu jako cele cislo int
 */
int CheckHeight(tBstNode *tree){
	int hl, hr;
	if (tree!=NULL){
		hl = CheckHeight(tree->left);
		hr = CheckHeight(tree->right);
		if (hl>hr) return hl+1;
		else return hr+1;
	}
	else return 0;
}

/**
 * @brief Odstraneni jednoho uzlu se stromu
 * @param tree Odkaz na strom ve ktereho se ma odstranit prvek
 * @param key Klic heldaneho uzlu
 */
void BstDelete(tBstNode **tree, char *key){
	if (!tree || !key) exit(99);
	//TODO
}

/**
 * @brief Vycisteni stromu, a vraceni do inicializovaneho stavu
 * @param tree Odkaz na cilovy strom
 */
void BstDispose(tBstNode **tree){
	if (!tree) exit(99);
	if ((*tree)==NULL) return;
	if ((*tree)->left) BstDispose(&(*tree)->left);
	if ((*tree)->left) BstDispose(&(*tree)->right);
	free((*tree)->key);
	FreeNodeContent(tree);
	free(*tree);
	(*tree)=NULL;
}
