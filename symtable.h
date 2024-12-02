/**
 * Implementation of IFJ24 imperative language symbol table
 * @file symtable.h
 * @brief Header file for the symbol table
 * @author Nikola Jordanov, xjordan00
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <string.h>
#include <stdbool.h>
#include "scanner.h"

/**
 * @brief The node type of the binary search tree
 */
typedef struct bst_node tBstNode;

/**
 * @brief A structure for entering arguments into a function definition
 */
typedef struct arg_def{
	tToken name_token;		//token containing the name of the argument
	tToken type_token;		//token containing expected argument type
	struct arg_def *next; 	//pointer to next argument
}tArgDef;

/**
 * @brief Node data for f-ce
 */
typedef struct function_vals{
	tArgDef *params;		//parameter list
	int paramCnt;			//parameter array size
	tTokenType ret_type;	//return type
}tFunctionVals;

/**
 * @brief Node data for the variable
 */
typedef struct var_vals{
	void *value;		//reference to variable value
	tTokenType type;	//data type of the variable
	bool is_constant;	//stamping const from var
	bool is_used;		//check whether the change is used in scope
}tVarVals;

/**
 * @brief The type of the parameter in the node
 */
typedef enum{
	FUNCTION,
	VARIABLE
}tBstNodeContentType;

/**
 * @brief A structure for storing data in a node
 */
typedef struct node_content{
	void *value;				//pointer to the place in memory where the value is
	tBstNodeContentType type;	//type of the value needed to change the pointer from void to the appropriate type
}tBstNodeContent;

/**
 * @brief The structure of the node bvs
 */
struct bst_node{
	char *key;					//key == variable or function name
	tBstNodeContent content;	//node data
	struct bst_node *left;		//reference to lion's child
	struct bst_node *right;		//reference to right child
};

/**
 * @brief Tree initialization
 * @param tree Reference and pointer pointing to the root of the (sub)tree
 */
void BstInit(tBstNode **tree);

/**
 * @brief Added a new element to the tree
 * @param tree Reference to the (sub)tree in which it will be stored
 * @param key The key of the stored element
 * @param content The data of the stored element
 */
void BstInsert(tBstNode **tree, char *key, tBstNodeContent content);

/**
 * @brief A function that adds an element to the tree and then aligns it
 * @param tree Link to the tree being processed
 * @param key The key of the new element
 * @param content The data of the new element
 */
void BstInsAndReal(tBstNode **tree, char *key, tBstNodeContent content);

/**
 * @brief Search for an element in a tree
 * @param tree Reference to the tree in which to search
 * @param key Searched key (variable or function name)
 * @param content Link where the value of the found node should be returned in case of success
 * @return True if the node is found, false otherwise
 */
bool BstSearch(tBstNode *tree, char *key, tBstNodeContent **content);

/**
 * @brief Remove one node from the tree
 * @param tree Reference to the tree in which the element is to be removed
 * @param key The key of the held node
 */
void BstDelete(tBstNode **tree, char *key);

/**
 * @brief A function that removes an element from a tree and then evens the tree
 * @param tree Target tree
 * @param key The key of the element to be removed
 */
void BstDelAndReal(tBstNode **tree, char *key);

/**
 * @brief Clearing the tree, and returning to the initialized state
 * @param tree Reference to target tree
 */
void BstDispose(tBstNode **tree);

/**
 * @brief Helper function that replaces the specified node with the rightmost node in the specified (sub)tree
 * @param target The target node to replace
 * @param tree Link to the tree in which the new node is searched
 */
void ReplaceByRightmost(tBstNode *target, tBstNode **tree);

/**
 * @brief Helper function that replaces the specified node with the leftmost node in the specified (sub)tree
 * @param target Target node to change
 * @param tree Link to the tree in which the new node is searched
 */
void ReplaceByLeftmost(tBstNode *target, tBstNode **tree);

/**
 * @brief Helpful function that returns the size of the (sub)tree
 * @param tree Measured tree
 * @param first_break Pointer to the lowest node where the balance is broken, if not broken the value of the pointer does not change
 * @return tree height as integer int; if high-level balance is drawn, it returns -1
 */
int CheckHeight(tBstNode **tree, tBstNode ***first_break);

/**
 * @brief Helper function to free the allocated memory in the data of the given node
 * @param tree Reference to the Node from which the memory will be freed
 */
void FreeNodeContent(tBstNode **tree);

/**
 * @brief Helper function to rotate the tree by one node to the left in case of external imbalance
 * @param tree A reference to the tree to rotate
 */
void RotLeft(tBstNode **tree);

/**
 * @brief Helper function to rotate the tree by one node to the right in case of external imbalance
 * @param tree A reference to the tree to rotate
 */
void RotRight(tBstNode **tree);

/**
 * @brief Helper function to rotate the tree by one node to the left in case of internal imbalance
 * @param tree A reference to the tree to rotate
 */
void RotRLeft(tBstNode **tree);

/**
 * @brief Helper function to rotate the tree by one node to the right in case of internal imbalance
 * @param tree A reference to the tree to rotate
 */
void RotLRight(tBstNode **tree);

/**
 * @brief A function that checks for high-level balance violations and corrects them
 * @param tree (Sub)tree that is checked
 */
void Realign(tBstNode **tree);

#endif /** SYMTABLE_H **/
