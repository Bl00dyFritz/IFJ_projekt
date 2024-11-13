/**
 * Implementacia zásobniku pre generátor kódu
 * @file stack.h
 * @brief Hlavičkový súbor zásobníku pre generátor kódu IFJ24
 * @authors Jakub Turek, xturekj00
 */

#ifndef STACK_H
#define STACK_H

typedef enum {
    INT_TYPE,
    FLOAT_TYPE,
    CHAR_TYPE,
    STRING_TYPE
} DataType;

// Definícia štruktúry uzla zásobníka
typedef struct StackNode {
    DataType type;
    union {
        int intValue;
        float floatValue;
        char charValue;
        char *stringValue;
    } data;
    struct StackNode *next;
} sStackNode;

// Definícia štruktúry zásobníka
typedef struct Stack {
    sStackNode *top;
} sStack;

// Prototypy funkcií pre zásobník
void Stack_Init(sStack *s);
int Stack_IsEmpty(sStack *s);
int Stack_Push(sStack *s, DataType type, void *value);
int Stack_Pop(sStack *s, void *value);
void Stack_PopAll(sStack *s);

#endif // STACK_H