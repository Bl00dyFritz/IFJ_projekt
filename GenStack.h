/**
 * Implementacia zásobniku pre generátor kódu
 * @file stack.h
 * @brief Hlavičkový súbor zásobníku pre generátor kódu IFJ24
 * @authors Jakub Turek, xturekj00
 *          Alexander Žikla, xziklaa00
 */

#ifndef STACK_H
#define STACK_H

#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// Definícia štruktúry uzla zásobníka
typedef struct StackGenNode {
    tAstNode *node;
    struct StackGenNode *next;
} sStackGenNode;

// Definícia štruktúry zásobníka
typedef struct Stack {
    sStackGenNode *top;
} sStackGen;

// Prototypy funkcií pre zásobník
void StackGen_Init(sStackGen *s);
int StackGen_IsEmpty(sStackGen *s);
int StackGen_Push(sStackGen *s, tAstNode *value);
int StackGen_Pop(sStackGen *s, tAstNode *value);
void StackGen_PopAll(sStackGen *s);

#endif // STACK_H