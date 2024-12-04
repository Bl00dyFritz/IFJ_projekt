/**
 * Implementation of IFJ24 imperative language translator
 * @file generator.c
 * @brief Implementation for the code generator
 * @authors Alexander Žikla, xziklaa00
 *          Jakub Turek, xturekj00
*/

#include "generator.h"
#include "ast.h"

static int GlobalIfCounter = 1;
static int GlobalWhileCounter = 1;
static bool InWhile = false;
static bool Defined = false;

void GenInitial(void) {
    GenHead();
    GenBuiltInFuncs();
}

void GenHead(void) {
    printf(".IFJcode24\n");
    printf("JUMP $$main\n\n");
}

void GenMainHead(void) {
    printf("LABEL $$main\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR GF@tmp1\n");
    printf("DEFVAR GF@tmp2\n");
    printf("DEFVAR GF@tmp3\n");
    printf("DEFVAR GF@func_result\n");
}

void GenPushIntFloat(tAstNode *node) {
    if (node->type == VAL) {
        switch (node->structure.val.token.type) {
            case Token_Integer:
                printf("PUSHS int@%lld\n", (long long int)node->structure.val.token.value.integer);
                break;
            case Token_Float:
                printf("PUSHS float@%a\n", node->structure.val.token.value.decimal);
                break;
            default:
                break;
        }
    } else if (node->type == VAR) {
        printf("PUSHS LF@%s\n", node->structure.var.token.value.string);
    }
}

bool CheckNill(tAstNode *node) {
    if (node->structure.val.token.type == Token_null) {
        return true;
    } else return false;
}

void PrintDiv(tBinOp BO) {
    if ((BO.op1->type == VAL && BO.op1->structure.val.token.type == Token_Integer) || 
        (BO.op1->type == VAR && (BO.op1->structure.var.type == I32 || BO.op1->structure.var.type == NI32))) {
            printf("IDIVS\n");
        } else if ((BO.op2->type == VAL && BO.op2->structure.val.token.type == Token_Integer) || 
                   (BO.op2->type == VAR && (BO.op2->structure.var.type == I32 || BO.op2->structure.var.type == NI32))) {
                        printf("IDIVS\n");
                } else printf("DIVS\n");
}

void CheckSameType(tAstNode *node) {
    if (node->structure.bin_op.op1->type == VAR && node->structure.bin_op.op2->type == VAL) {
        if ((node->structure.bin_op.op1->structure.var.type == F64 || node->structure.bin_op.op1->structure.var.type == NF64) &&
            node->structure.bin_op.op2->structure.val.token.type == Token_Integer) {
                printf("POPS GF@tmp3\n");
                printf("CREATEFRAME\n");
                printf("CALL $$ifj_i2f\n");
                printf("PUSHS GF@func_result\n");
                printf("PUSHS GF@tmp3\n");
        } else if ((node->structure.bin_op.op1->structure.var.type == I32 || node->structure.bin_op.op1->structure.var.type == NI32) &&
                    node->structure.bin_op.op2->structure.val.token.type == Token_Float) {
                printf("POPS GF@tmp3\n");
                printf("CREATEFRAME\n");
                printf("CALL $$ifj_f2i\n");
                printf("PUSHS GF@func_result\n");
                printf("PUSHS GF@tmp3\n");
        }
    } else if (node->structure.bin_op.op2->type == VAR && node->structure.bin_op.op1->type == VAL) {
        if ((node->structure.bin_op.op2->structure.var.type == F64 || node->structure.bin_op.op2->structure.var.type == NF64) &&
             node->structure.bin_op.op1->structure.val.token.type == Token_Integer) {
                printf("CREATEFRAME\n");
                printf("CALL $$ifj_i2f\n");
                printf("PUSHS GF@func_result\n");
        } else if ((node->structure.bin_op.op2->structure.var.type == I32 || node->structure.bin_op.op2->structure.var.type == NI32) &&
                    node->structure.bin_op.op1->structure.val.token.type == Token_Float) {
                printf("CREATEFRAME\n");
                printf("CALL $$ifj_f2i\n");
                printf("PUSHS GF@func_result\n");
        }
    }
}

void GenStackOp(tAstNode *node) {
    if (node->structure.bin_op.op2->type == BIN_OP) {
        GenStackOp(node->structure.bin_op.op2);
    }
    if (node->structure.bin_op.op1->type == BIN_OP) {
        GenStackOp(node->structure.bin_op.op1);
    }
    GenPushIntFloat(node->structure.bin_op.op2);
    GenPushIntFloat(node->structure.bin_op.op1);
    CheckSameType(node);
    switch (node->structure.bin_op.token.type) {
        case Token_Lesser:
        case Token_Lesser_Equal:
        case Token_Greater:
        case Token_Greater_Equal:
            if (CheckNill(node->structure.bin_op.op1)) exit(53);
            if (CheckNill(node->structure.bin_op.op2)) exit(53);
            break;
        default:
            break;
    }
    
    switch (node->structure.bin_op.token.type) {
        case Token_Plus:
            printf("ADDS\n");
            break;
        case Token_Minus:
            printf("SUBS\n");
            break;
        case Token_Multiply:
            printf("MULS\n");
            break;
        case Token_Divide:
            //Checks if it's not going to divide with 0 
            if (node->structure.bin_op.op1->type == VAL) {
                if ((node->structure.bin_op.op1->structure.val.token.type == Token_Integer && node->structure.bin_op.op1->structure.val.token.value.integer == 0) ||
                    (node->structure.bin_op.op1->structure.val.token.type == Token_Float && node->structure.bin_op.op1->structure.val.token.value.decimal == 0)) {
                        exit(57);
                }
            } else if (node->structure.bin_op.op1->type == VAR) {
                if ((node->structure.bin_op.op1->structure.var.type == I32 || node->structure.bin_op.op1->structure.var.type == NI32) &&
                      node->structure.bin_op.op1->structure.var.val.i != NULL) {
                        if (*node->structure.bin_op.op1->structure.var.val.i == 0) {
                            exit(57);    
                        }
                      } else if ((node->structure.bin_op.op1->structure.var.type == F64 || node->structure.bin_op.op1->structure.var.type == F64) &&
                                  node->structure.bin_op.op1->structure.var.val.f != NULL) {
                                    if (*node->structure.bin_op.op1->structure.var.val.f == 0) {
                                        exit(57);    
                                    }
                }
            }
            PrintDiv(node->structure.bin_op);
            break;
        case Token_Equal:
            printf("EQS\n");
            break;
        case Token_Not_Equal:
            printf("EQS\n");
            printf("NOTS\n");
            break;
        case Token_Lesser_Equal:
            printf("LTS\n");
            if (node->structure.bin_op.op2->type == BIN_OP) {
                GenStackOp(node->structure.bin_op.op2);
            }
            if (node->structure.bin_op.op1->type == BIN_OP) {
                GenStackOp(node->structure.bin_op.op1);
            }
            GenPushIntFloat(node->structure.bin_op.op1);
            GenPushIntFloat(node->structure.bin_op.op2);
            printf("EQS\n");
            printf("ORS\n");
            break;
        case Token_Lesser:
            printf("LTS\n");
            break;
        case Token_Greater_Equal:
            printf("GTS\n");
            if (node->structure.bin_op.op2->type == BIN_OP) {
                GenStackOp(node->structure.bin_op.op2);
            }
            if (node->structure.bin_op.op1->type == BIN_OP) {
                GenStackOp(node->structure.bin_op.op1);
            }
            GenPushIntFloat(node->structure.bin_op.op1);
            GenPushIntFloat(node->structure.bin_op.op2);
            printf("EQS\n");
            printf("ORS\n");
            break;
        case Token_Greater:
            printf("GTS\n");
            break;
        default:
            break;
    }
}

void GenDefineVarsForWhile(tAstNode *node, int IfCouter, int WhileCounter) {
    int ic = IfCouter;
    int wc = WhileCounter;
    while (node) {
        if (node->structure.code.operation->type == VAR_DECL || node->structure.code.operation->type == CONST_DECL) {
            printf("DEFVAR LF@%s\n", node->structure.code.operation->structure.var_decl.token.value.string);
        } else if (node->structure.code.operation->type == WHILE) {
            printf("DEFVAR LF@WhileTmp%d\n", wc++);
            if (node->structure.code.operation->structure.while_loop.nn_id != NULL) {
                printf("DEFVAR LF@%s\n", node->structure.code.operation->structure.while_loop.nn_id->structure.var.token.value.string);
            }
            GenDefineVarsForWhile(node->structure.code.operation->structure.while_loop.code, ic, wc);
        } else if (node->structure.code.operation->type == IF) {
            printf("DEFVAR LF@$IfTmp%d\n", ic++);
            if (node->structure.code.operation->structure.if_block.nn_id != NULL) {
                    printf("DEFVAR LF@%s\n", node->structure.code.operation->structure.if_block.nn_id->structure.var.token.value.string);
            }
            GenDefineVarsForWhile(node->structure.code.operation->structure.if_block.if_code, ic, wc);
            GenDefineVarsForWhile(node->structure.code.operation->structure.if_block.else_code, ic, wc);
        }
        node = node->structure.code.next_code;
    }
    Defined = true;
}

void GenWhile(int LocalWhileCounter) {
    printf("POPS LF@WhileTmp%d\n", LocalWhileCounter);
    printf("JUMPIFEQ $EndWhile%d$ LF@WhileTmp%d bool@false\n", LocalWhileCounter,LocalWhileCounter);
}

void GenWhileNN(int LocalWhileCounter) {
    printf("POPS LF@WhileTmp%d\n", LocalWhileCounter);
    printf("JUMPIFEQ $EndWhile%d$ LF@WhileTmp%d nil@nil\n", LocalWhileCounter, LocalWhileCounter);
}

void GenWhileEnd(int LocalWhileCounter) {
    printf("JUMP $while%d$\n", LocalWhileCounter);
    printf("LABEL $EndWhile%d$\n", LocalWhileCounter);
}

void GenBuiltInFuncs(void) {
    //ifj.write
    printf("LABEL $$ifj_write\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@WriteTerm\n");
    printf("POPS LF@WriteTerm\n");
    printf("WRITE LF@WriteTerm\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
    //ifj.readstr
    printf("LABEL $$ifj_readstr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");
    printf("READ LF@retval string\n");
    printf("PUSHS LF@retval\n");
    printf("POPS GF@func_result\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
    //ifj.readi32
    printf("LABEL $$ifj_readi32\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");
    printf("READ LF@retval int\n");
    printf("PUSHS LF@retval\n");
    printf("POPS GF@func_result\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
    //ifj.readf64
    printf("LABEL $$ifj_readf64\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");
    printf("READ LF@retval float\n");
    printf("PUSHS LF@retval\n");
    printf("POPS GF@func_result\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
    //ifj.string
    printf("LABEL $$ifj_string\n");
    printf("PUSHFRAME\n");
    printf("POPS GF@func_result\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
    //ifj.concat
    printf("LABEL $$ifj_concat\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@l1\n");
    printf("DEFVAR LF@l2\n");
    printf("POPS LF@l1\n");
    printf("POPS LF@l2\n");
    printf("DEFVAR LF@result\n");
	printf("CONCAT LF@result LF@l1 LF@l2\n");
	printf("PUSHS LF@result\n");
    printf("POPS GF@func_result\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
    //ifj.length
    printf("LABEL $$ifj_length\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");
    printf("DEFVAR LF@str\n");
    printf("POPS LF@str\n");
    printf("STRLEN LF@retval LF@str\n");
    printf("PUSHS LF@retval\n");
    printf("POPS GF@func_result\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
    //ifj.i2f
    printf("LABEL $$ifj_i2f\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");
    printf("POPS LF@retval\n");
    printf("INT2FLOAT LF@retval LF@retval\n");
    printf("PUSHS LF@retval\n");
    printf("POPS GF@func_result\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
    //ifj.f2i
    printf("LABEL $$ifj_f2i\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");
    printf("POPS LF@retval\n");
    printf("FLOAT2INT LF@retval LF@retval\n");
    printf("PUSHS LF@retval\n");
    printf("POPS GF@func_result\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
    //ifj.substring
    printf("LABEL $$ifj_substring\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@string\n");
    printf("DEFVAR LF@StartIndex\n");
    printf("DEFVAR LF@EndIndex\n");
    printf("POPS LF@string\n");
    printf("POPS LF@StartIndex\n");
    printf("POPS LF@EndIndex\n");
    printf("DEFVAR LF@StringLength\n");
    printf("STRLEN LF@StringLength LF@string\n");
    printf("DEFVAR LF@BOOLresultSUBSTRING\n");
    printf("DEFVAR LF@SubStringResult\n");
    printf("DEFVAR LF@TempString\n");
    printf("MOVE LF@SubStringResult string@\n");

    printf("LT LF@BOOLresultSUBSTRING LF@StartIndex int@0\n");
    printf("JUMPIFEQ $ReturnNULL$ LF@BOOLresultSUBSTRING bool@true\n");
    printf("LT LF@BOOLresultSUBSTRING LF@EndIndex int@0\n");
    printf("JUMPIFEQ $ReturnNULL$ LF@BOOLresultSUBSTRING bool@true\n");
    printf("GT LF@BOOLresultSUBSTRING LF@StartIndex LF@EndIndex\n");
    printf("JUMPIFEQ $ReturnNULL$ LF@BOOLresultSUBSTRING bool@true\n");
    printf("GT LF@BOOLresultSUBSTRING LF@EndIndex LF@StringLength\n");
    printf("JUMPIFEQ $ReturnNULL$ LF@BOOLresultSUBSTRING bool@true\n");
    printf("SUB LF@StringLength LF@StringLength int@1\n");
    printf("GT LF@BOOLresultSUBSTRING LF@StartIndex LF@StringLength\n");
    printf("JUMPIFEQ $ReturnNULL$ LF@BOOLresultSUBSTRING bool@true\n");

    printf("LABEL $SubStringLOOP$\n");
    printf("JUMPIFEQ $ENDofSUBloop$ LF@StartIndex LF@EndIndex\n");
    printf("GETCHAR LF@TempString LF@string LF@StartIndex\n");
    printf("CONCAT LF@SubStringResult LF@SubStringResult LF@TempString\n");
    printf("ADD LF@StartIndex LF@StartIndex int@1\n");
    printf("JUMP $SubStringLOOP$\n");

    printf("LABEL $ENDofSUBloop$\n");
    printf("PUSHS LF@SubStringResult\n");
    printf("JUMP $ENDofSUBSTRINGfunc$\n");

    printf("LABEL $ReturnNULL$\n");
    printf("PUSHS nil@nil\n");

    printf("LABEL $ENDofSUBSTRINGfunc$\n");
    printf("POPS GF@func_result\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
    //ifj.ord
    printf("LABEL $$ifj_ord\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@s\n");
    printf("POPS LF@s\n");
    printf("DEFVAR LF@ORDindex\n");
    printf("POPS LF@ORDindex\n");
    printf("DEFVAR LF@sLength\n");
    printf("STRLEN LF@sLength LF@s\n");
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
    printf("POPS GF@func_result\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
    //ifj.chr
    printf("LABEL $$ifj_chr\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@InVal\n");
    printf("POPS LF@InVal\n");
    printf("DEFVAR LF@CHRresult\n");
    printf("INT2CHAR LF@CHRresult LF@InVal\n");
    printf("PUSHS LF@CHRresult\n");
    printf("POPS GF@func_result\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
    //ifj.strcmp
    printf("LABEL $$ifj_strcmp\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@counter\n");
    printf("DEFVAR LF@s1\n");
    printf("DEFVAR LF@s2\n");
    printf("POPS LF@s1\n");
    printf("POPS LF@s2\n");
    printf("MOVE LF@counter int@0\n");
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

    printf("LABEL $ReturnSTRCMPNeg1$\n");
    printf("PUSHS int@-1\n");
    printf("JUMP $ENDofSTRCMPfunc$\n");

    printf("LABEL $returnSTRCMP0$\n");
    printf("PUSHS int@0\n");
    printf("JUMP $ENDofSTRCMPfunc$\n");

    printf("LABEL $ReturnSTRCMP1$\n");
    printf("PUSHS int@1\n");

    printf("LABEL $ENDofSTRCMPfunc$\n");
    printf("POPS GF@func_result\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
}

void StringToIFJ24string(tAstNode *node) {
    int i = 0;
    int j = 0;
    char *TmpStrIn =  node->structure.func_call.args->token.value.string;
    char *TmpStrOut = (char *)malloc(strlen(TmpStrIn) * 4);
    if (TmpStrOut == NULL) {
        fprintf(stderr, "Error: Allocation failed\n");
        exit(INTERNAL_COMP_ERROR);
    }
    while (TmpStrIn[i]) {
        if (TmpStrIn[i] <= 32 || TmpStrIn[i] == '#' || TmpStrIn[i] == '"' || TmpStrIn[i] == '\\') {
            j += sprintf(&TmpStrOut[j], "\\%03d", (unsigned char)TmpStrIn[i]);
        } else {
            TmpStrOut[j++] = TmpStrIn[i];
        }
        i++;
    }
    TmpStrOut[j] = '\0';
    free(TmpStrIn);
    node->structure.func_call.args->token.value.string = TmpStrOut;
}

void GenCallFunc(tAstNode *node) {
    tArgs *TmpCF = node->structure.func_call.args;
    tArgs *Prev = NULL;
    tArgs *Next = NULL;
    //Changing order of Args
    while (TmpCF) {
        Next = TmpCF->next;
        TmpCF->next = Prev;
        Prev = TmpCF;
        TmpCF = Next;
    }
    TmpCF = Prev;
    if (node->structure.func_call.name_token.value.BuiltInFunc == BF_string || 
       (node->structure.func_call.name_token.value.BuiltInFunc == BF_write && TmpCF->token.type == Token_string)) {
        StringToIFJ24string(node);
    }
    while (TmpCF) {
        switch (TmpCF->token.type) {
            case Token_FuncID:
                printf("PUSHS LF@%s\n", TmpCF->token.value.string);
                break;
            case Token_Integer:
                printf("PUSHS int@%lld\n", (long long int)TmpCF->token.value.integer);
                break; 
            case Token_Float:
                printf("PUSHS float@%a\n", TmpCF->token.value.decimal);
                break;
            case Token_string:
                printf("PUSHS string@%s\n", TmpCF->token.value.string);
                break;
            default: break;
        }
        TmpCF = TmpCF->next;
    }
    if (node->structure.func_call.name_token.type == Token_BuildIn_Func) {
        switch (node->structure.func_call.name_token.value.BuiltInFunc) {
            case BF_write: printf("CALL $$ifj_write\n"); break;
            case BF_readstr: printf("CALL $$ifj_readstr\n"); break;
            case BF_readi32: printf("CALL $$ifj_readi32\n"); break;
            case BF_readf64: printf("CALL $$ifj_readf64\n"); break;
            case BF_string: printf("CALL $$ifj_string\n"); break;
            case BF_concat: printf("CALL $$ifj_concat\n"); break;
            case BF_length: printf("CALL $$ifj_length\n"); break;
            case BF_i2f: printf("CALL $$ifj_i2f\n"); break;
            case BF_f2i: printf("CALL $$ifj_f2i\n"); break;
            case BF_substring: printf("CALL $$ifj_substring\n"); break;
            case BF_ord: printf("CALL $$ifj_ord\n"); break;
            case BF_chr: printf("CALL $$ifj_chr\n"); break;
            case BF_strcmp: printf("CALL $$ifj_strcmp\n"); break;
            default: break;
        }
    } else {
        printf("CALL $$%s\n", node->structure.func_call.name_token.value.string);
    }
}

void GenDefFunc(tAstNode *node) {
    printf("LABEL $$%s\n", node->structure.func_call.name_token.value.string);
    printf("PUSHFRAME\n");
    tArgDef *tmp = node->structure.func_def.args;
    while (tmp) {
        printf("DEFVAR LF@%s\n", tmp->name_token.value.string);
        printf("POPS LF@%s\n", tmp->name_token.value.string);
        tmp = tmp->next;
    }
}

void GenFuncEnd(tAstNode *node) {
    if (node->structure.func_def.ret_type_token.type != Token_null) {
        printf("POPS GF@func_result\n");
    }
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void GenAssign(tAstNode *node) {
    if (node->structure.assign.src->type == VAR) {
        switch (node->structure.assign.src->structure.var.type) {
            case I32: case NI32:
                printf("MOVE LF@%s int@%lld\n", node->structure.assign.dst->structure.var.token.value.string, 
                                                (long long int)node->structure.assign.src->structure.var.val.i);
                break;
            case F64: case NF64:
                printf("MOVE LF@%s float@%a\n", node->structure.assign.dst->structure.var.token.value.string,
                                                *node->structure.assign.src->structure.var.val.f);
                break;
            case VOID: case UNDEF:
                printf("MOVE LF@%s nil@nil\n", node->structure.assign.dst->structure.var.token.value.string);
                break;    
            default:
                break;
        }
    } else if (node->structure.assign.src->type == VAL) {
        switch (node->structure.assign.src->structure.val.token.type) {
            case Token_Integer:
                printf("MOVE LF@%s int@%lld\n", node->structure.assign.dst->structure.var.token.value.string, 
                                                (long long int)node->structure.assign.src->structure.val.token.value.integer);
                break;
            case Token_Float:
                printf("MOVE LF@%s float@%a\n", node->structure.assign.dst->structure.var.token.value.string,
                                                node->structure.assign.src->structure.val.token.value.decimal);
                break;
            case Token_null:
                printf("MOVE LF@%s nil@nil\n", node->structure.assign.dst->structure.var.token.value.string);
                break;
            default:
                break;
        }
    } else if (node->structure.assign.src->type == FUNC_CALL) {
        GenerateOutput(node->structure.assign.src);
        printf("MOVE LF@%s GF@func_result\n", node->structure.assign.dst->structure.var.token.value.string);
    } else if (node->structure.assign.src->type == BIN_OP) {
        GenerateOutput(node->structure.assign.src);
        printf("POPS LF@%s\n", node->structure.assign.dst->structure.var.token.value.string);
    }
}

void GenIfStart(int LocalIfCounter) {
    printf("POPS LF@$IfTmp%d\n", LocalIfCounter);
    printf("JUMPIFNEQ $else_label%d$ LF@$IfTmp%d bool@true\n", LocalIfCounter, LocalIfCounter);
}

void GenIfStart2(int LocalIfCounter) {
    printf("POPS LF@$IfTmp%d\n", LocalIfCounter);
    printf("JUMPIFNEQ $endif_label%d$ LF@$IfTmp%d bool@true\n", LocalIfCounter, LocalIfCounter);
}

void GenIfStartNN(int LocalIfCounter) {
    printf("POPS LF@$IfTmp%d\n", LocalIfCounter);
    printf("JUMPIFEQ $else_label%d$ LF@$IfTmp%d nil@nil\n", LocalIfCounter, LocalIfCounter);
}

void GenIfStartNN2(int LocalIfCounter) {
    printf("POPS LF@$IfTmp%d\n", LocalIfCounter);
    printf("JUMPIFEQ $endif_label%d$ LF@$IfTmp%d nil@nil\n", LocalIfCounter, LocalIfCounter);
}

void GenIfEnd(int LocalIfCounter) {
    printf("JUMPIFNEQ $endif_label%d$ LF@$IfTmp%d bool@false\n", LocalIfCounter, LocalIfCounter);
}

void GenIfEndNN(int LocalIfCounter) {
    printf("JUMPIFNEQ $endif_label%d$ LF@$IfTmp%d nil@nil\n", LocalIfCounter, LocalIfCounter);
}

void GenElseStart(int LocalIfCounter) {
    printf("LABEL $else_label%d$\n", LocalIfCounter);
}

void GenElseEnd(int LocalIfCounter) {
    printf("LABEL $endif_label%d$\n", LocalIfCounter);
}

void GenerateOutput(tAstNode *node) {
    tAstNode *tmpS;
    tAstNode *tmpC;
    tAstNode *tmpW;
    int LocalIfCounter;
    int LocalWhileCounter;
    switch (node->type) {
        case STATEMENT:
            tmpS = node;
            while (tmpS) {
                GenerateOutput(tmpS->structure.statement.function);
                tmpS = tmpS->structure.statement.next_statement;
            }
            break;
	    case CODE:
            tmpC = node;
            while (tmpC) {
                GenerateOutput(tmpC->structure.code.operation);
                tmpC = tmpC->structure.code.next_code;
            }
            break;
	    case VAR:
            switch (node->structure.var.type) {
                case VOID: case UNDEF:
                    printf("MOVE LF@%s nil@nil\n", node->structure.var.token.value.string);
                    break;
	            case I32: case NI32: 
                    printf("MOVE LF@%s int@%lld\n", node->structure.var.token.value.string, (long long int)node->structure.var.val.i);
                    break;
	            case F64: case NF64:
                    printf("MOVE LF@%s float@%a\n", node->structure.var.token.value.string, *node->structure.var.val.f);
                    break;
                case U8: case NU8:
                    printf("MOVE LF@%s string@%s\n", node->structure.var.token.value.string, node->structure.var.val.str);
                    break;
                default:
                    exit(56);
                    break;
            }
            break;
        case VAL:
            switch (node->structure.val.token.type){
                case Token_Integer:
                    printf("PUSHS int@%lld\n", (long long int)node->structure.val.token.value.integer);
                    break;
                case Token_Float:
                    printf("PUSHS float@%a\n", node->structure.val.token.value.decimal);
                    break;
                default:
                    break;
            }
            break;  
	    case WHILE:
            LocalWhileCounter = GlobalWhileCounter++;
            tmpW = node->structure.while_loop.code;
            if (!Defined) {
                GenDefineVarsForWhile(tmpW, GlobalIfCounter, GlobalWhileCounter);
            }
            if (!InWhile) {
                printf("DEFVAR LF@WhileTmp%d\n", LocalWhileCounter);
                if (node->structure.while_loop.nn_id != NULL) {
                    printf("DEFVAR LF@%s\n", node->structure.while_loop.nn_id->structure.var.token.value.string);
                }
                InWhile = true;
            }
            if (node->structure.while_loop.nn_id != NULL) {
                printf("LABEL $while%d$\n", LocalWhileCounter);
                printf("MOVE LF@%s LF@%s\n", node->structure.while_loop.nn_id->structure.var.token.value.string, node->structure.while_loop.expr->structure.var.token.value.string);
                printf("PUSHS LF@%s\n", node->structure.while_loop.nn_id->structure.var.token.value.string);
                GenWhileNN(LocalWhileCounter);
            } else {
                printf("LABEL $while%d$\n", LocalWhileCounter);
                GenerateOutput(node->structure.while_loop.expr);
                GenWhile(LocalWhileCounter);
            }
            GenerateOutput(node->structure.while_loop.code);
            if (node->structure.while_loop.nn_id != NULL) {
                printf("PUSHS LF@%s\n", node->structure.while_loop.nn_id->structure.var.token.value.string);
            } else if (node->structure.while_loop.expr->structure.bin_op.op1->type == VAR) {
                printf("PUSHS LF@%s\n", node->structure.while_loop.expr->structure.bin_op.op1->structure.var.token.value.string);
            } else if (node->structure.while_loop.expr->structure.bin_op.op2->type == VAR) {
                printf("PUSHS LF@%s\n", node->structure.while_loop.expr->structure.bin_op.op2->structure.var.token.value.string);
            }
            GenWhileEnd(LocalWhileCounter);
            GlobalWhileCounter++;
            InWhile = false;
            Defined = false;
            break;
	    case IF:
            LocalIfCounter = GlobalIfCounter++;
            if (!InWhile) {
                printf("DEFVAR LF@$IfTmp%d\n", LocalIfCounter);
                if (node->structure.if_block.nn_id != NULL) {
                    printf("DEFVAR LF@%s\n", node->structure.if_block.nn_id->structure.var.token.value.string);
                }
            }
            if (node->structure.if_block.nn_id != NULL) {
                printf("MOVE LF@%s LF@%s\n", node->structure.if_block.nn_id->structure.var.token.value.string, node->structure.if_block.expr->structure.var.token.value.string);
                printf("PUSHS LF@%s\n", node->structure.if_block.nn_id->structure.var.token.value.string);
                if (node->structure.if_block.else_code) {
                    GenIfStartNN(LocalIfCounter);
                } else {
                    GenIfStartNN2(LocalIfCounter);
                }
                GenerateOutput(node->structure.if_block.if_code);
                GenIfEndNN(LocalIfCounter);
            } else {
                GenerateOutput(node->structure.if_block.expr);
                if (node->structure.if_block.else_code) {
                    GenIfStart(LocalIfCounter);
                } else {
                    GenIfStart2(LocalIfCounter);
                }
                GenerateOutput(node->structure.if_block.if_code);
                GenIfEnd(LocalIfCounter);
            }
            if (node->structure.if_block.else_code) {
                GenElseStart(LocalIfCounter);
                GenerateOutput(node->structure.if_block.else_code);
            }
            GenElseEnd(LocalIfCounter);
            break;
	    case BIN_OP:
            GenStackOp(node);
            break;
	    case ASSIGN:
            GenAssign(node);
            break;
	     case CONST_DECL: case VAR_DECL:
            if (!InWhile) {
                printf("DEFVAR LF@%s\n", node->structure.var_decl.token.value.string);
            }
            break;
	    case FUNC_CALL:
            printf("CREATEFRAME\n");
            GenCallFunc(node);
            break;
	    case FUNC_DEF:
            printf("\n");
            if (!strcmp(node->structure.func_def.token.value.string, "main")) {
                GenMainHead();
                GenerateOutput(node->structure.func_def.code);
                printf("POPFRAME\n");
                printf("EXIT int@0\n");
            } else {
                GenDefFunc(node);
                GenerateOutput(node->structure.func_def.code);
                GenFuncEnd(node);
            }
            break;
        case RET:
            if (node->structure.ret.ret_expr != NULL) {
                if (node->structure.ret.ret_expr->type == VAR) {
                    if (node->structure.ret.ret_expr->structure.var.type == VOID || node->structure.ret.ret_expr->structure.var.type == NUL) {
                        printf("POPFRAME\n");
                        printf("RETURN\n");
                    }
                    printf("PUSHS LF@%s\n", node->structure.ret.ret_expr->structure.var.token.value.string);
                    printf("POPFRAME\n");
                    printf("RETURN\n");
                } else if (node->structure.ret.ret_expr->type == VAL) {
                    switch (node->structure.ret.ret_expr->structure.val.token.type) {
                        case Token_Integer:
                            printf("PUSHS int@%lld\n", (long long int)node->structure.ret.ret_expr->structure.val.token.value.integer);
                            printf("POPFRAME\n");
                            printf("RETURN\n");
                            break;
                        case Token_Float:
                            printf("PUSHS float@%a\n", node->structure.ret.ret_expr->structure.val.token.value.decimal);
                            printf("POPFRAME\n");
                            printf("RETURN\n");
                            break;
                        default:
                            break;
                    }
                } else {
                    GenerateOutput(node->structure.ret.ret_expr);
                }
            } else {
                switch (node->structure.ret.type) {
                    case VOID:
                        printf("POPFRAME\n");
                        printf("RETURN\n");
                        break;
                    case I32: case NI32:
                        printf("PUSHS int@%lld\n", (long long int)node->structure.ret.val.i);
                        printf("POPFRAME\n");
                        printf("RETURN\n");
                        break;
	                case F64: case NF64:
                        printf("PUSHS int@%a\n", node->structure.ret.val.f);
                        printf("POPFRAME\n");
                        printf("RETURN\n");
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }
}