/**
 * Implementacia zásobniku pre generátor kódu
 * @file stack.c
 * @brief Implementácia zásobníku pre generátor kódu IFJ24
 * @authors Jakub Turek, xturekj00
 */

#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

void Stack_Init(sStack *s) {
    s->top = NULL;
}

int Stack_IsEmpty(sStack *s) {
    return s->top == NULL;
}

int Stack_Push(sStack *s, int value) {
    sStackNode *new_node = (sStackNode *)malloc(sizeof(sStackNode));
    if (new_node == NULL) {
        fprintf(stderr, "Error: Allocation failed\n");
        return 1;
    }
    new_node->value = value;
    new_node->next = s->top;
    s->top = new_node;
    return 0;
}

int Stack_Pop(sStack *s, int *value) {
    if (Stack_IsEmpty(s)) {
        fprintf(stderr, "Error: Stack underflow\n");
        return 1;
    }
    sStackNode *temp = s->top;
    *value = temp->value;
    s->top = s->top->next;
    free(temp);
    return 0;
}

void Stack_PopAll(sStack *s) {
    while (!Stack_IsEmpty(s)) {
        int temp;
        Stack_Pop(s, &temp);
    }
}


