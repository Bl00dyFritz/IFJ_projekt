/**
 * Implementation of IFJ24 imperative language symbol table
 * @file symtable.c
 * @brief Symbol table implementation
 * @author Nikola Jordanov, xjordan00
 */

#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void FreeNodeContent(tBstNode **tree){
	if (!tree) exit(INTERNAL_COMP_ERROR);
	if ((*tree)->content.type==VARIABLE)
		free(((tVarVals*)(*tree)->content.value)->value);
	else{
		for (int i = 0; i<((tFunctionVals*)(*tree)->content.value)->paramCnt; i++){
			free(&((tFunctionVals*)(*tree)->content.value)->params[i]);
		}
		free(((tFunctionVals*)(*tree)->content.value)->params);
		free((*tree)->content.value);
	}
}

void BstInit(tBstNode **tree){
	if (!tree) exit(INTERNAL_COMP_ERROR);
	(*tree) = NULL;
}

void RotLeft(tBstNode **tree){
	if (!tree) exit(INTERNAL_COMP_ERROR);
	if(!(*tree)) return;
	tBstNode *tmp = (*tree)->right;
	(*tree)->right = tmp->left;
	tmp->left = (*tree);
	(*tree) = tmp;
}

void RotRight(tBstNode **tree){
	if (!tree) exit(INTERNAL_COMP_ERROR);
	if (!(*tree)) return;
	tBstNode *tmp = (*tree)->left;
	(*tree)->left = tmp->right;
	tmp->right = (*tree);
	(*tree) = tmp;
}

void RotRLeft(tBstNode **tree){
	if (!tree) exit (INTERNAL_COMP_ERROR);
	if (!(*tree)) return;
	tBstNode *tmpA = (*tree);
	tBstNode *tmpB = (*tree)->right;
	(*tree) = tmpB->left;
	tmpA->right = (*tree)->left;
	tmpB->left = (*tree)->right;
	(*tree)->left = tmpA;
	(*tree)->right = tmpB;
}

void RotLRight(tBstNode **tree){
	if (!tree) exit(INTERNAL_COMP_ERROR);
	if (!(*tree)) return;
	tBstNode *tmpA = (*tree);
	tBstNode *tmpB = (*tree)->left;
	(*tree) = tmpB->right;
	tmpB->right = (*tree)->left;
	tmpA->left = (*tree)->right;
	(*tree)->left = tmpB;
	(*tree)->right = tmpA;
}

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

void BstInsert(tBstNode **tree, char *key, tBstNodeContent content){
	if (!tree) exit(INTERNAL_COMP_ERROR);
	if (!(*tree)){
		(*tree) = (tBstNode *) malloc(sizeof(tBstNode));
		if (!(*tree)) return;
		(*tree)->key = (char *) malloc(sizeof(key));
		if (!(*tree)->key) return;
		strcpy((*tree)->key, key);
		(*tree)->content = content;
		(*tree)->left = NULL;
		(*tree)->right = NULL;
	}
	else{
		if (strcmp(key, (*tree)->key)<0)
			BstInsert(&(*tree)->left, key, content);
		else if (strcmp(key, (*tree)->key)>0)
			BstInsert(&(*tree)->right, key, content);
		else{
			FreeNodeContent(tree);
			(*tree)->content = content;
		}
	}
}

void ReplaceByRightmost(tBstNode *target, tBstNode **tree){
	if (!tree || !target) exit(INTERNAL_COMP_ERROR);
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

void ReplaceByLeftmost(tBstNode *target, tBstNode **tree){
	if (!tree || !target) exit(INTERNAL_COMP_ERROR);
	if (!(*tree)->left){
		target->key = (*tree)->key;
		FreeNodeContent(&target);
		target->content.value = (*tree)->content.value;
		target->content.type = (*tree)->content.type;
		if ((*tree)->right){
			tBstNode *tmp = (*tree)->right;
			(*tree)->key = tmp->key;
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
} //not used atm

int CheckHeight(tBstNode **tree, tBstNode ***first_break){
	int hl, hr;
	if (*tree!=NULL){
		hl = CheckHeight(&(*tree)->left, first_break);
		if (hl==-1) return -1;
		hr = CheckHeight(&(*tree)->right, first_break);
		if (hr==-1) return -1;
		if (hl>hr){
			if(hl-hr>1){
				*first_break = tree;
				return -1;
			}
			return hl+1;
		}
		else{
			if (hr-hl>1){
				*first_break = tree;
				return -1;
			}
			return hr+1;
		}
	}
	else return 0;
}

void Realign (tBstNode **tree){
	if(!tree) exit(INTERNAL_COMP_ERROR);
	tBstNode **br = NULL;
	if (CheckHeight(tree, &br)!=-1) return;
	int lh = CheckHeight(&(*br)->left, &br);
	int rh = CheckHeight(&(*br)->right, &br);
	int cmp1;
	int cmp2;
	if (lh>rh){
		cmp1='l';
		lh = CheckHeight(&(*br)->left->left, &br);
		rh = CheckHeight(&(*br)->left->right, &br);
	}
	else{
		cmp1='r';
		lh = CheckHeight(&(*br)->right->left, &br);
		rh = CheckHeight(&(*br)->right->right, &br);
	}
	if(lh>rh) cmp2='l';
	else cmp2='r';
	switch(cmp1){
		case 'l':
			if (cmp2=='l') RotRight(br);
			else if(cmp2=='r') RotLRight(br);
			break;
		case 'r':
			if (cmp2=='l') RotRLeft(br);
			else if(cmp2=='r') RotLeft(br);
			break;
	}
}

void BstDelete(tBstNode **tree, char *key){
	if (!tree || !key) exit(INTERNAL_COMP_ERROR);
	if (!(*tree)) return;
	if (strcmp(key, (*tree)->key)<0)
		BstDelete(&(*tree)->left, key);
	else if(strcmp(key, (*tree)->key)>0)
		BstDelete(&(*tree)->right, key);
	else{
		if (!(*tree)->left && !(*tree)->right){
			FreeNodeContent(tree);
			free((*tree)->key);
			free(*tree);
			(*tree) = NULL;
		}
		else if ((*tree)->left && (*tree)->right){
			ReplaceByRightmost((*tree), &(*tree)->left);
		}
		else{
			tBstNode *tmp = NULL;
			if (!(*tree)->left) tmp = (*tree)->right;
			else if(!(*tree)->right) tmp = (*tree)->left;
			free((*tree)->key);
			(*tree)->key = tmp->key;
			FreeNodeContent(tree);
			(*tree)->content = tmp->content;
			(*tree)->left = tmp->left;
			(*tree)->right = tmp->right;
			free(tmp);
		}
	}
}

void BstInsAndReal(tBstNode **tree, char *key, tBstNodeContent content){
	if (!tree || !key) exit(INTERNAL_COMP_ERROR);
	BstInsert(tree, key, content);
	Realign(tree);
}

void BstDelAndReal(tBstNode **tree, char *key){
	if (!tree || !key) exit(INTERNAL_COMP_ERROR);
	BstDelete(tree, key);
	Realign(tree);
}

void BstDispose(tBstNode **tree){
	if (!tree) exit(INTERNAL_COMP_ERROR);
	if ((*tree)==NULL) return;
	if ((*tree)->left) BstDispose(&(*tree)->left);
	if ((*tree)->right) BstDispose(&(*tree)->right);
	free((*tree)->key);
	FreeNodeContent(tree);
	free(*tree);
	(*tree)=NULL;
}
