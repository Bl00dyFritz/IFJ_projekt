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
} StackNode;

// Definícia štruktúry zásobníka
typedef struct Stack {
    StackNode *top;
} Stack;

// Prototypy funkcií pre zásobník
void Stack_Init(Stack *s);
int Stack_IsEmpty(Stack *s);
int Stack_Push(Stack *s, int value);
int Stack_Pop(Stack *s, int *value);
void Stack_PopAll(Stack *s);

#endif // STACK_H