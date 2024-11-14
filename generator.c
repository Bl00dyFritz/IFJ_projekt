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
    printf("POPS GF@%%func_result\n");
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

void GenWhileHead(void) {
    pritnf("LABEL $while$\n");
}

void GenWhile(void) {
    printf("POPS GF@func_result");
    printf("JUMPIFEQ $EndWhile$ GF@func_result bool@false\n");
}

void GenWhileEnd(void) {
    printf("JUMP $while$\n");
    printf("LABEL $EndWhile$\n");
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
            printf("DEFVAR LF@term%d\n", i);
            printf("POPS LF@term%d\n", i); 
        }
        for (int i = 0; i < vals->paramCnt; i++) {
            printf("WRITE LF@term%d\n", i);
        }
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.concat") == 0) {
        printf("PUSHFRAME\n");
        printf("DEFVAR LF@l1\n");
        printf("DEFVAR LF@l2\n");
        printf("POPS LF@l2\n");
        printf("POPS LF@l1\n");
        printf("DEFVAR LF@result\n");
	    printf("CONCAT LF@result LF@l1 LF@l2\n");
	    printf("PUSHS LF@result\n");
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
        printf("DEFVAR LF@StrTerm\n");
        printf("POPS LF@StrTerm\n");
        printf("PUSHS LF@StrTerm\n");
        printf("POPS GF@func_result\n");
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
        printf("DEFVAR LF@string\n");
        printf("DEFVAR LF@StartIndex\n");
        printf("DEFVAR LF@EndIndex\n");
        printf("PUSHS LF@string\n");
        printf("PUSHS LF@StartIndex\n");
        printf("PUSHS LF@EndIndex\n");
        printf("DEFVAR LF@StringLength\n");
        printf("STRLEN LF@StringLength LF@string\n");
        printf("DEFVAR LF@BOOLresultSUBSTRING\n");
        printf("DEFVAR LF@SubStringResult");
        printf("DEFVAR LF@TempString");

        printf("LT LF@BOOLresultSUBSTRING LF@StartIndex ind@0\n");
        printf("JUMPIFEQ $ReturnNULL$ LF@BOOLresultSUBSTRING bool@true\n");
        printf("LT LF@BOOLresultSUBSTRING LF@EndIndex ind@0\n");
        printf("JUMPIFEQ $ReturnNULL$ LF@BOOLresultSUBSTRING bool@true\n");
        printf("GT LF@BOOLresultSUBSTRING LF@StartIndex LF@EndIndex\n");
        printf("JUMPIFEQ $ReturnNULL$ LF@BOOLresultSUBSTRING bool@true\n");
        printf("GT LF@BOOLresultSUBSTRING LF@EndIndex LF@StringLength\n");
        printf("JUMPIFEQ $ReturnNULL$ LF@BOOLresultSUBSTRING bool@true\n");
        printf("SUB LF@StringLength LF@StringLength int@1\n");
        printf("GT LF@BOOLresultSUBSTRING LF@StartIndex LF@StringLength\n");
        printf("JUMPIFEQ $ReturnNULL$ LF@BOOLresultSUBSTRING bool@true\n");

        printf("LABEL $SubStringLOOP$\n");
        printf("JUMPIFEQ $ENDofSUBloop$ LF@StartIndex LF@index2\n");
        printf("GETCHAR LF@TempString LF@string LF@StartIndex\n");
        printf("CONCAT LF@SubStringResult LF@SubStringResult LF@TempString\n");
        printf("ADD LF@StartIndex LF@StartIndex int@1\n");
        printf("JUMP $SubStringLOOP$");

        printf("LABEL $ENDofSUBloop$");
        printf("PUSHS SubStringResult\n");
        printf("JUMP $ENDofSUBSTRINGfunc$");

        printf("LABEL $ReturnNULL$\n");
        printf("PUSHS nil@nil\n");

        printf("LABEL $ENDofSUBSTRINGfunc$\n");
        printf("POPS GF@func_resul\n");
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.ord") == 0) {
        printf("PUSHFRAME\n");
        printf("DEFVAR LF@s\n");
        printf("POPS LF@s\n");
        printf("DEFVAR LF@ORDindex\n");
        printf("POPS LF@ORDindex\n");
        printf("DEFVAR LF@sLength\n");
        printf("STRLEN LF@sLength LF@s");
        printf("DEFVAR LF@$BOOLresult\n");

        printf("JUMPIFEQ $returnORD0$ LF@sLength int@0\n");
        printf("GT LF@$BOOLresult LF@ORDindex LF@sLength\n");
        printf("JUMPIFEQ $returnORD0$ LF@$BOOLresult bool@true\n");
        printf("LT LF@$BOOLresult LF@ORDindex int@1\n");
        printf("JUMPIFEQ $returnORD0$ LF@$BOOLresult bool@true\n");
        printf("STRI2INT LF@s LF@s LF@ORDindex\n");
        printf("PUSHS LF@s\n");
        printf("JUMP $ENDofORDfunction$\n");

        printf("LABEL $returnORD0$\n");
        printf("PUSHS int@0\n");

        printf("LABEL $ENDofORDfunction$\n");
        printf("POPS GF@func_resul\n");
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.chr") == 0) {
        printf("PUSHFRAME\n");
        printf("DEFVAR LF@InVal\n");
        printf("POPS LF@InVal\n");
        printf("DEFVAR LF@CHRresult\n");
        printf("INT2CHAR LF@CHRresult LF@InVal\n");
        printf("PUSHS LF@CHRresult\n");
        printf("POPS GF@func_resul\n");
        printf("POPFRAME\n");
    } else if (strcmp(node->key, "ifj.strcmp") == 0) {
        printf("PUSHFRAME\n");
        pritnf("DEFVAR LF@counter");
        printf("DEFVAR LF@s1\n");
        printf("DEFVAR LF@s2\n");
        printf("POPS LF@s1\n");
        printf("POPS LF@s2\n");
        printf("DEFVAR LF@lenS1\n");
        printf("DEFVAR LF@lenS2\n");
        printf("STRLEN LF@lenS1 LF@s1\n");
        printf("STRLEN LF@lenS2 LF@s2\n");
        printf("DEFVAR LF@c1\n");
        printf("DEFVAR LF@c2\n");
        printf("DEFVAR LF@GTresult\n");

        printf("LABEL $strcmpLOOP$\n");
        printf("JUMPIFEQ $strcmpEND$ LF@counter LF@lenS1\n");
        printf("JUMPIFEQ $strcmpEND$ LF@counter LF@lenS2\n");
        printf("GETCHAR LF@c1 LF@s1 LF@counter\n");
        printf("GETCHAR LF@c2 LF@s2 LF@counter\n");
        printf("JUMPIFEQ $NextChar$ LF@c1 LF@c2\n");
        printf("GT LF@GTresult LF@c1 LF@c2\n");
        printf("JUMPIFEQ $ReturnSTRCMP1$ LF@GTresult bool@true\n");
        printf("JUMP $ReturnSTRCMPNeg1$\n");

        printf("LABEL $NextChar$\n");
        printf("ADD LF@counter LF@counter int@1\n");
        printf("JUMP $strcmpLOOP$\n");

        printf("LABEL $strcmpEND$\n");
        printf("JUMPIFEQ $returnSTRCMP0$ LF@lenS1 LF@lenS2\n");
        printf("GT LF@GTresult LF@lenS1 LF@lenS2\n");
        printf("JUMPIFEQ $ReturnSTRCMP1$ LF@GTresult bool@true\n");
        printf("JUMP $ReturnSTRCMPNeg1$\n");

        printf("LABEL ReturnSTRCMPNeg1\n");
        printf("PUSHS int@-1\n");
        printf("JUMP $ENDofSTRCMPfunc$\n");

        printf("LABEL $returnSTRCMP0$\n");
        printf("PUSHS int@0\n");
        printf("JUMP $ENDofSTRCMPfunc$\n");

        printf("LABEL $ReturnSTRCMP1$\n");
        printf("PUSHS int@1\n");
        
        printf("LABEL $ENDofSTRCMPfunc$\n");
        printf("POPS GF@func_result");
        printf("POPFRAME\n");
    } else {
        printf("CALL %s\n", node->key);
        printf("LABEL %s\n", node->key);
        printf("PUSHFRAME\n");
        for (int i = vals->paramCnt; i > 0; i--) {
            printf("DEFVAR LF@l%d", i);
            printf("POPS LF@l%d", i);
        }
    }
}

void GenDefFunc(tFunctionVals *vals, char *FuncName) {
    printf("LABEL $%s\n", FuncName);
    printf("PUSHFRAME\n");
}

void GenFuncEnd(tFunctionVals *vals) {
    if (vals->ret_type != Token_null) {
        printf("POP GF@func_result\n");
    }
    printf("POPFRAME\n");
    printf("RETURN\n");
}

/**
 * 
 * tBstNode *strom -- vymeni sa za ukazatel na strom (Niko vytvori)
 * dostaneme ukazatel na strom, zasobnik
 * potom to dat do zasobniku
 */
void generate3AK(tBstNode *strom, sStack *stack) {
    tBstNode *tmp = strom;
    while (tmp) {
        Stack_Push(stack, tmp->content.type, tmp->content.value);
        tmp = tmp->right;
    }
}

/**
 * 
 * tBstNode *strom -- vymenit za 3AK strukturu (idk co to bude, Niko tvori)
 * dostaneme 3AK ()
 * vypopuje zasobnik
 * Assembler kod vytlaci
 */
void GenerateOutput(tBstNode *strom, sStack *stack, tToken token) {
    void *val;
    DataType Dtype;
    GenHead();
    while (stack) {
        Stack_Pop(stack, val);
        switch (token.type) {
            
            default:
                break;
        }
    }

    if (!Stack_IsEmpty(stack)) {
        Stack_PopAll(stack);
    }
}
