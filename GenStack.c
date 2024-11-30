/**
 * Implementacia zásobniku pre generátor kódu
 * @file stack.c
 * @brief Implementácia zásobníku pre generátor kódu IFJ24
 * @authors Jakub Turek, xturekj00
 *          Alexander Žikla, xziklaa00
 */

#include "GenStack.h"

void StackGen_Init(sStackGen *s) {
    s->top = NULL;
}

int StackGen_IsEmpty(sStackGen *s) {
    return s->top == NULL;
}

int StackGen_Push(sStackGen *s, tAstNode *value) {
    sStackGenNode *new_node = (sStackGenNode *)malloc(sizeof(sStackGenNode));
    if (!new_node) {
        fprintf(stderr, "Error: Allocation failed\n");
        exit(99);
    }
    new_node->node = (tAstNode *)malloc(sizeof(tAstNode));
    if (!new_node->node) {
        fprintf(stderr, "Error: Allocation failed\n");
        free(new_node);
        exit(99);
    }
    
    new_node->node->type = value->type;
    new_node->node->structure = value->structure;

    new_node->next = s->top;
    s->top = new_node;
    return 0;
}


int StackGen_Pop(sStackGen *s, tAstNode *value) {
    if (StackGen_IsEmpty(s)) {
        fprintf(stderr, "Error: Stack underflow\n");
        exit(99);
    }
    sStackGenNode *temp = s->top;
    value->type = temp->node->type;
    value->structure = temp->node->structure;
    s->top = s->top->next;
    free(temp->node);
    free(temp);
    return 0;
}

void StackGen_PopAll(sStackGen *s) {
    while (!StackGen_IsEmpty(s)) {
        sStackGenNode *temp = s->top;
        s->top = s->top->next;
        free(temp->node);
        free(temp);
    }
}