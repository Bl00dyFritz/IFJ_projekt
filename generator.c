/**
 * Implementace překladače imperativního jazyka IFJ24
 * @file generator.c
 * @brief Implementácia generátoru kódu
 * @authors Alexander Žikla, xziklaa00
 *          Jakub Turek, xturekj00
*/

#include "generator.h"
#include "ast.h"

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
                if (node->structure.bin_op.token.type == Token_Divide) {
                    printf("INT2FLOATS\n");
                }
                break;
            case Token_Float:
                printf("PUSHS float@%a\n", node->structure.val.token.value.decimal);
                if (node->structure.bin_op.token.type == Token_Divide) {
                    printf("FLOAT2INTS\n");
                }
                break;
            default:
                break;
        }
    } else if (node->type == VAR) {
        printf("PUSHS LF@%s\n", node->structure.var.token.value.string);
        if (node->structure.bin_op.token.type == Token_Divide) {
            switch (node->structure.var.type) {
                case I32: case NI32:
                    printf("INT2FLOATS\n");
                    break;
	            case F64: case NF64:
                    printf("FLOAT2INTS\n");
                    break;
            }
        }
    }
}

bool CheckNill(tAstNode *node) {
    if (node->structure.val.token.type == Token_null) {
        return true;
    } else return false;
}

void GenStackOp(tAstNode *node) {
    GenPushIntFloat(node->structure.bin_op.op1);
    GenPushIntFloat(node->structure.bin_op.op2);
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
            if (node->structure.bin_op.op2->structure.val.token.value.integer == 0) {
                exit(57);
            }
            if (node->structure.bin_op.op1->structure.val.token.value.integer < 0 || node->structure.bin_op.op2->structure.val.token.value.integer < 0) {
                printf("IDIVS\n");
            } else {
                printf("DIVS\n");
            }
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

void GenWhileHead(void) {
    printf("LABEL $while$\n");
}

void GenWhile(void) {
    printf("POPS GF@tmp2\n");
    printf("JUMPIFEQ $EndWhile$ GF@tmp2 bool@false\n");
}

void GenWhileNN(void) {
    printf("POPS GF@tmp2\n");
    printf("JUMPIFEQ $EndWhile$ GF@tmp2 nil@nil\n");
}

void GenWhileEnd(void) {
    printf("JUMP $while$\n");
    printf("LABEL $EndWhile$\n");
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
    printf("DEFVAR LF@StrTerm\n");
    printf("POPS LF@StrTerm\n");
    printf("PUSHS LF@StrTerm\n");
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
    printf("POPS GF@func_resul\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
    //ifj.f2i
    printf("LABEL $$ifj_f2i\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@retval\n");
    printf("POPS LF@retval\n");
    printf("FLOAT2INT LF@retval LF@retval\n");
    printf("PUSHS LF@retval\n");
    printf("POPS GF@func_resul\n");
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
    printf("POPS GF@func_resul\n");
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
    printf("POPS GF@func_resul\n");
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

    printf("LABEL ReturnSTRCMPNeg1\n");
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

void GenCallFunc(tAstNode *node) {
    tArgs *TmpCF = node->structure.func_call.args;
    tArgs *Prev = NULL;
    tArgs *Next = NULL;
    while (TmpCF) {
        Next = TmpCF->next;
        TmpCF->next = Prev;
        Prev = TmpCF;
        TmpCF = Next;
    }
    TmpCF = Prev;
    while (TmpCF) {
        printf("PUSHS LF@%s\n", TmpCF->token.value.string);
        TmpCF = TmpCF->next;
    }
    if (node->structure.func_call.name_token.type = Token_BuildIn_Func) {
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
                                                node->structure.assign.src->structure.var.val.f);
                break;
            case U8: case NU8:
                printf("MOVE LF@%s string@%s\n", node->structure.assign.dst->structure.var.token.value.string,
                                                node->structure.assign.src->structure.var.val.str);
                break;
            case VOID: case UNDEF:
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

void GenIfStart(void) {
    printf("POPS GF@tmp1\n");
    printf("JUMPIFNEQ $$else_label GF@tmp1 bool@true\n");
}

void GenIfStartNN(void) {
    printf("POPS GF@tmp1\n");
    printf("JUMPIFEQ $$else_label GF@tmp1 nil@nil\n");
}

void GenIfEnd(void) {
    printf("JUMPIFNEQ $$endif_label GF@tmp1 bool@false\n");
}

void GenElseStart(void) {
    printf("LABEL $$else_label\n");
}

void GenElseEnd(void) {
    printf("LABEL $$endif_label\n");
}

void generate3AK(sStackGen *stack, tAstNode *tree) {
    if (tree == NULL) return;
    StackGen_Push(stack, tree);
}

void Generate(sStackGen *stack) {
    GenInitial();
    tAstNode val;
    while (!StackGen_IsEmpty(stack)) {
        StackGen_Pop(stack, &val);
        GenerateOutput(&val);
    }
}

void GenerateOutput(tAstNode *node) {
    tAstNode *tmpS;
    tAstNode *tmpC;
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
                    printf("MOVE LF@%s float@%a\n", node->structure.var.token.value.string, node->structure.var.val.f);
                    break;
	            case U8: case NU8:
                    printf("MOVE LF@%s string@%s\n", node->structure.var.token.value.string, node->structure.var.val.str);
                    break;
                default:
                    exit(56);
                    break;
            }
            break;
	    case WHILE:
            GenWhileHead();
            if (node->structure.while_loop.nn_id != NULL) {
                printf("PUSHS LF@%s\n", node->structure.while_loop.nn_id->structure.var.token.value.string);
                GenWhileNN();
            } else {
                GenerateOutput(node->structure.while_loop.expr);
                GenWhile();
            }
            GenerateOutput(node->structure.while_loop.code);
            if (node->structure.while_loop.nn_id != NULL) {
                printf("PUSHS LF@%s\n", node->structure.while_loop.nn_id->structure.var.token.value.string);
            } else if (node->structure.while_loop.expr->structure.bin_op.op1->type == VAR) {
                printf("POPS LF@%s\n", node->structure.while_loop.expr->structure.bin_op.op1->structure.var.token.value.string);
            } else if (node->structure.while_loop.expr->structure.bin_op.op2->type == VAR) {
                printf("POPS LF@%s\n", node->structure.while_loop.expr->structure.bin_op.op2->structure.var.token.value.string);
            }
            GenWhileEnd();
            break;
	    case IF:
            if (node->structure.if_block.nn_id != NULL) {
                printf("PUSHS LF@%s\n", node->structure.if_block.nn_id->structure.var.token.value.string);
                GenIfStartNN();
            } else {
                GenerateOutput(node->structure.if_block.expr);
                GenIfStart();
            }
            GenerateOutput(node->structure.if_block.if_code);
            GenIfEnd();
            if (node->structure.if_block.else_code) {
                GenElseStart();
                GenerateOutput(node->structure.if_block.else_code);
            }
            GenElseEnd();
            break;
	    case BIN_OP:
            GenStackOp(node);
            break;
	    case ASSIGN:
            GenAssign(node);
            break;
	     case CONST_DECL: case VAR_DECL:
            printf("DEFVAR LF@%s\n", node->structure.var_decl.token.value.string);
            break;
	    case FUNC_CALL:
            printf("CREATEFRAME\n");
            GenCallFunc(node);
            break;
	    case FUNC_DEF:
            if (!strcmp(node->structure.func_def.token.value.string, "main")) {
                GenMainHead();
                GenerateOutput(node->structure.func_def.code);
            } else {
                GenDefFunc(node);
                GenerateOutput(node->structure.func_def.code);
                GenFuncEnd(node);
            }
            break;
        default:
            break;
    }
}