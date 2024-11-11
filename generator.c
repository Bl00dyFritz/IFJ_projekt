/**
 * Implementace překladače imperativního jazyka IFJ24
 * @file generator.c
 * @brief Implementácia generátoru kódu
 * @authors Alexander Žikla, xziklaa00
 *          Jakub Turek, xturekj00
*/

#include "generator.h"

void GenExp(tBstNode *node, tToken Token) {
    switch (Token.type) {
        case Token_Plus:

            break;
        case Token_Minus:

            break;
        case Token_Multiply:

            break;
        case Token_Divide:

            break;
        case Token_Integer:

            break;
        case Token_Float:

            break;
        case Token_concat:

            break;
        case Token_Equal:

            break;
        case Token_Not_Equal:

            break;
        case Token_Lesser_Equal:

            break;
        case Token_Lesser:

            break;
        case Token_Greater_Equal:

            break;
        case Token_Greater:

            break;
        case Token_String:

            break;
        default:
            break;
    }
}

void GenCallFunc(tBstNode *node) {
    if (strcmp(node->key, "ifj.write") == 0) {
        
    } else if (strcmp(node->key, "ifj.readstr") == 0) {
    
    } else if (strcmp(node->key, "ifj.readi32") == 0) {
        
    } else if (strcmp(node->key, "ifj.readf64") == 0) {
        
    } else if (strcmp(node->key, "ifj.string") == 0) {
       
    } else if (strcmp(node->key, "ifj.concat") == 0) {
        
    } else if (strcmp(node->key, "ifj.length") == 0) {
        
    } else if (strcmp(node->key, "ifj.i2f") == 0) {
        
    } else if (strcmp(node->key, "ifj.f2i") == 0) {
        
    } else if (strcmp(node->key, "ifj.substring") == 0) {
        
    } else if (strcmp(node->key, "ifj.ord") == 0) {
        
    } else if (strcmp(node->key, "ifj.chr") == 0) {
        
    } else if (strcmp(node->key, "ifj.strcmp") == 0) {
        
    } else {

    }
}