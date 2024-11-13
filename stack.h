/**
 * Implementacia zásobniku pre generátor kódu
 * @file stack.h
 * @brief Hlavičkový súbor zásobníku pre generátor kódu IFJ24
 * @authors Jakub Turek, xturekj00
 */

#ifndef STACK_H
#define STACK_H

// Definícia štruktúry uzla zásobníka
typedef struct StackNode {
    int value;
    struct StackNode *next;
} sStackNode;

// Definícia štruktúry zásobníka
typedef struct Stack {
    sStackNode *top;
} sStack;

// Prototypy funkcií pre zásobník
void Stack_Init(sStack *s);
int Stack_IsEmpty(sStack *s);
int Stack_Push(sStack *s, int value);
int Stack_Pop(sStack *s, int *value);
void Stack_PopAll(sStack *s);

#endif // STACK_H