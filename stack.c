/**
 * Implementacia zásobniku pre generátor kódu
 * @file stack.c
 * @brief Implementácia zásobníku pre generátor kódu IFJ24
 * @authors Jakub Turek, xturekj00
 */

#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void Stack_Init(sStack *s) {
    s->top = NULL;
}

int Stack_IsEmpty(sStack *s) {
    return s->top == NULL;
}

int Stack_Push(sStack *s, DataType type, void *value) {
    sStackNode *new_node = (sStackNode *)malloc(sizeof(sStackNode));
    if (new_node == NULL) {
        fprintf(stderr, "Error: Allocation failed\n");
        return 1;
    }
    switch (type) {
        case INT_TYPE:
            new_node->data.intValue = *((int *)value);
            break;
        case FLOAT_TYPE:
            new_node->data.floatValue = *((float *)value);
            break;
        case CHAR_TYPE:
            new_node->data.charValue = *((char *)value);
            break;
        case STRING_TYPE:
            new_node->data.stringValue = strdup((char *)value);
            if (new_node->data.stringValue == NULL) {
                fprintf(stderr, "Error: String duplication failed\n");
                free(new_node);
                return 1;
            }
            break;
        default:
            fprintf(stderr, "Error: Unknown data type\n");
            free(new_node);
            return 1;
    }
    new_node->type = type;
    new_node->next = s->top;
    s->top = new_node;
    return 0;
}

int Stack_Pop(sStack *s, void *value) {
    if (Stack_IsEmpty(s)) {
        fprintf(stderr, "Error: Stack underflow\n");
        return 1;
    }
    sStackNode *temp = s->top;
    switch (temp->type) {
        case INT_TYPE:
            *((int *)value) = temp->data.intValue;
            break;
        case FLOAT_TYPE:
            *((float *)value) = temp->data.floatValue;
            break;
        case CHAR_TYPE:
            *((char *)value) = temp->data.charValue;
            break;
        case STRING_TYPE:
            *((char **)value) = strdup(temp->data.stringValue);
            if (*((char **)value) == NULL) {
                fprintf(stderr, "Error: String duplication failed\n");
                return 1;
            }
            break;
        default:
            fprintf(stderr, "Error: Unknown data type\n");
            return 1;
    }
    s->top = s->top->next;
    if (temp->type == STRING_TYPE) {
        free(temp->data.stringValue);
    }
    free(temp);
    return 0;
}

void Stack_PopAll(sStack *s) {
    while (!Stack_IsEmpty(s)) {
        int temp;
        Stack_Pop(s, &temp);
    }
}