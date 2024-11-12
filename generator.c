/**
 * Implementace překladače imperativního jazyka IFJ24
 * @file generator.c
 * @brief Implementácia generátoru kódu
 * @authors Alexander Žikla, xziklaa00
 *          Jakub Turek, xturekj00
*/

#include "generator.h"

void GenHead(void) {
    printf(".IFJcode24\n");
    printf("DEFVAR GF@%%input\n");
    printf("MOVE GF@%%input string@main\n");
    printf("DEFVAR GF@tmp1\n");
    printf("DEFVAR GF@tmp2\n");
    printf("DEFVAR GF@tmp3\n");
    printf("DEFVAR GF@func_result\n");
    printf("JUMP $$main\n");
}

void GenExpBegin() {
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
}

void GenExpEnd() {
    printf("POPS GF@%%exprresult\n");
    printf("POPFRAME\n");
}

void GenStackOp(tBstNode *node, tToken *token) {
    switch (token->type) {
        case Token_Plus:
            printf("ADDS\n");
            break;
        case Token_Minus:
            printf("SUBS\n");
            break;
        case Token_Multiply:
            printf("MULS\n");
            break;
        case Token_Divide:                          //Vyriesit ako s IDIV
            printf("DIVS\n");
            break;
        case Token_Integer:
            printf("PUSHS int@%d\n", token->value.integer);
            break;
        case Token_Float:
            printf("PUSHS float@%d\n", token->value.decimal);
            break;
        case Token_Equal:
            printf("EQS\n");
            break;
        case Token_Not_Equal:
            printf("EQS\n");
            printf("NOT\n");
            break;
        case Token_Lesser_Equal:

            break;
        case Token_Lesser:
            printf("LTS\n");
            break;
        case Token_Greater_Equal:

            break;
        case Token_Greater:
            printf("GTS\n");
            break;
        case Token_String:

            break;
        default:
            break;
    }
}

void GenCallFunc(tBstNode *node, tFunctionVals *vals) {
    printf("CREATEFRAME\n");
    for (int i = vals->paramCnt; i > 0; i--) {
        printf("DEFVAR TF@t%d", i);
        printf("PUSHS TF@t%d", i);
    }
    if (strcmp(node->key, "ifj.write") == 0) {
        printf("PUSHFRAME\n");
        for (int i = vals->paramCnt; i > 0; i--) {
            printf("DEFVAR LF@%%term%d\n", i);
            printf("POPS LF@%%term%d\n", i); 
        }
        for (int i = 0; i < vals->paramCnt; i++) {
            printf("WRITE LF@%%term%d\n", i);
        }
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.concat") == 0) {
        printf("PUSHFRAME\n");
        printf("DEFVAR LF@%%l1\n");
        printf("DEFVAR LF@%%l2\n");
        printf("POPS LF@l2\n");
        printf("POPS LF@l1\n");
        printf("DEFVAR LF@%%result\n");
	    printf("CONCAT LF@%%result LF@%%l1 LF@%%l2\n");
	    printf("PUSHS LF@%%result\n");
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.readstr") == 0) {
        printf("PUSHFRAME\n");
        printf("DEFVAR LF@retval\n");
        printf("READ LF@retval string\n");
        printf("PUSHS LF@retval");
        printf("POPS GF@func_result\n");
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.readi32") == 0) {
        printf("PUSHFRAME\n");
        printf("DEFVAR LF@retval\n");
        printf("READ LF@retval int\n");
        printf("PUSHS LF@retval\n");
        printf("POPS GF@func_result\n");
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.readf64") == 0) {
        printf("PUSHFRAME\n");
        printf("DEFVAR LF@retval\n");
        printf("READ LF@retval float\n");
        printf("PUSHS LF@retval\n");
        printf("POPS GF@func_result\n");
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.string") == 0) {
       printf("PUSHFRAME\n");
        printf("DEFVAR LF@retval\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("POPFRAME\n");
    }else if (strcmp(node->key, "ifj.length") == 0) {
        printf("PUSHFRAME\n");
        printf("DEFVAR LF@retval\n");
        printf("DEFVAR LF@str\n");
        printf("POPS LF@str\n");
        printf("STRLEN LF@retval LF@str\n");
        printf("PUSH LF@retval");
        printf("POPS GF@func_result");
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.i2f") == 0) {
        printf("PUSHFRAME\n");
        printf("DEFVAR LF@retval\n");
        printf("POPS LF@retval\n");
        printf("INT2FLOAT LF@retval\n");
        printf("PUSHS LF@retval\n");
        printf("POPS GF@func_resul\n");
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.f2i") == 0) {
        printf("PUSHFRAME\n");
        printf("DEFVAR LF@retval\n");
        printf("POPS LF@retval\n");
        printf("FLOAT2INT LF@retval\n");
        printf("PUSHS LF@retval\n");
        printf("POPS GF@func_resul\n");
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.substring") == 0) {
        printf("PUSHFRAME\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.ord") == 0) {
        printf("PUSHFRAME\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.chr") == 0) {
        printf("PUSHFRAME\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.strcmp") == 0) {
        printf("PUSHFRAME\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("POPFRAME\n");
    } else {
        printf("CALL %s\n", node->key);
        printf("LABEL %s\n", node->key);
        printf("PUSHFRAME\n");
        for (int i = vals->paramCnt; i > 0; i--) {
            printf("DEFVAR TF@t%d", i);
            printf("POPS TF@t%d", i);
        }
    }
}

void GenFuncEnd(tFunctionVals *vals) {
    if (vals->ret_type != Token_null) {
        printf("POP GF@func_result\n");
    }
    printf("POPFRAME\n");
    printf("RETURN\n");
}