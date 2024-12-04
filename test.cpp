#include <gtest/gtest.h>
#include <vector>
#include <stdexcept>
#include "generator.c"
#include "scanner.c"
#include "ast.h"

//g++ test_generate_output.cpp -lgtest -lgtest_main -pthread -o test_generate_output

// Google Test cases
TEST(GenerateOutputTest, PopAndRecursiveBehavior) {

    tAstNode *tree = (tAstNode *)malloc(sizeof(tAstNode)*64);
    tree->type = STATEMENT;
    tree->structure.statement.function = (tAstNode *)malloc(sizeof(tAstNode)*32);
    tree->structure.statement.function->type = FUNC_DEF;
    char *name = (char*)malloc(sizeof(char)*8);
    strcpy(name, "main");
    tree->structure.statement.function->structure.func_def.token.value.string = name;
    tree->structure.statement.function->structure.func_def.code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->type = CODE;
    tree->structure.statement.function->structure.func_def.code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.operation->type = VAR_DECL;
    tree->structure.statement.function->structure.func_def.code->structure.code.operation->structure.var_decl.type = F64;
    char *name1 = (char*)malloc(sizeof(char)*16);
    strcpy(name1, "FloatVar");
    tree->structure.statement.function->structure.func_def.code->structure.code.operation->structure.var_decl.token.value.string = name1;
    
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->type = CODE;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);

    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.operation->type = VAR;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.operation->structure.var.val.f = 3.14;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.operation->structure.var.type = F64;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.operation->structure.var.token.value.string = name1;

    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->type = CODE;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->type = IF;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.expr = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.expr->type = BIN_OP;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.expr->structure.bin_op.op1 = (tAstNode *)malloc(sizeof(tAstNode)*16);

    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.expr->structure.bin_op.op1->type = VAL;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.expr->structure.bin_op.op1->structure.val.token.type = Token_Integer;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.expr->structure.bin_op.op1->structure.val.token.value.integer = 5;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.expr->structure.bin_op.op2 = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.expr->structure.bin_op.op2->type = VAL;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.expr->structure.bin_op.op2->structure.val.token.type = Token_Integer;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.expr->structure.bin_op.op2->structure.val.token.value.integer = 10;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.expr->structure.bin_op.token.type = Token_Lesser_Equal;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.if_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.if_code->type = CODE;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.if_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.if_code->structure.code.operation->type = BIN_OP;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.if_code->structure.code.operation->structure.bin_op.op1 = tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.operation;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.if_code->structure.code.operation->structure.bin_op.op1->type = VAR;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.if_code->structure.code.operation->structure.bin_op.op2 = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.if_code->structure.code.operation->structure.bin_op.op2->type = VAL;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.if_code->structure.code.operation->structure.bin_op.op2->structure.val.token.value.decimal = 1.5;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.if_code->structure.code.operation->structure.bin_op.op2->structure.val.token.type = Token_Float;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.if_code->structure.code.operation->structure.bin_op.token.type = Token_Plus;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.else_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.else_code->type = ASSIGN;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.else_code->structure.assign.src = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.else_code->structure.assign.src->type = VAR;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.else_code->structure.assign.src->structure.var.type = VOID;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.if_block.else_code->structure.assign.dst = tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.operation;
    
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->type = CODE;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.func_call.name_token.value.BuiltInFunc = BF_write;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.func_call.name_token.type = Token_BuildIn_Func;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.func_call.args = (tArgs *)malloc(sizeof(tArgs)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.func_call.args->next = NULL;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.func_call.args->token.value.string = name1;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.func_call.args->token.type = Token_FuncID;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->type = FUNC_CALL;

    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->type = CODE;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);

    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->type = VAR_DECL;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var_decl.type = I32;
    char *name2 = (char*)malloc(sizeof(char)*4);
    strcpy(name2, "i");
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var_decl.token.value.string = name2;
    
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->type = CODE;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->type = VAR;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var.token.value.string = name2;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var.val.i = 3;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var.type = I32;
    tAstNode *IvarTMP = tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation;
    
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->type = CODE;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);

    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->type = WHILE;

    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.while_loop.expr = (tAstNode *)malloc(sizeof(tAstNode)*16);

    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.while_loop.expr->type = BIN_OP;

    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.while_loop.expr->structure.bin_op.op1 = IvarTMP;
    
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.while_loop.expr->structure.bin_op.op2 = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.while_loop.expr->structure.bin_op.op2->type = VAL;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.while_loop.expr->structure.bin_op.op2->structure.val.token.value.integer = 0;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.while_loop.expr->structure.bin_op.op2->structure.val.token.type = Token_Integer;
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.while_loop.expr->structure.bin_op.token.type = Token_Not_Equal;

    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.while_loop.code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.while_loop.code->type = CODE;
    tAstNode *TMPwhileCode = tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.while_loop.code;
    TMPwhileCode->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    TMPwhileCode->structure.code.operation->type = BIN_OP;
    TMPwhileCode->structure.code.operation->structure.bin_op.op1 = IvarTMP;
    TMPwhileCode->structure.code.operation->structure.bin_op.op2 = (tAstNode *)malloc(sizeof(tAstNode)*16);
    TMPwhileCode->structure.code.operation->structure.bin_op.op2->type = VAL;
    TMPwhileCode->structure.code.operation->structure.bin_op.op2->structure.val.token.value.integer = 1;
    TMPwhileCode->structure.code.operation->structure.bin_op.op2->structure.val.token.type = Token_Integer;
    TMPwhileCode->structure.code.operation->structure.bin_op.token.type = Token_Minus;

    TMPwhileCode->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    TMPwhileCode->structure.code.next_code->type = CODE;
    TMPwhileCode->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    TMPwhileCode->structure.code.next_code->structure.code.operation->type = FUNC_CALL;
    TMPwhileCode->structure.code.next_code->structure.code.operation->structure.func_call.name_token.type = Token_BuildIn_Func;
    TMPwhileCode->structure.code.next_code->structure.code.operation->structure.func_call.name_token.value.BuiltInFunc = BF_write;
    TMPwhileCode->structure.code.next_code->structure.code.operation->structure.func_call.args = (tArgs *)malloc(sizeof(tArgs)*16);
    TMPwhileCode->structure.code.next_code->structure.code.operation->structure.func_call.args->token.value.string = name2;
    TMPwhileCode->structure.code.next_code->structure.code.operation->structure.func_call.args->token.type = Token_FuncID;
    TMPwhileCode->structure.code.next_code->structure.code.operation->structure.func_call.args->next = NULL;

    tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);

    tAstNode *idk = tree->structure.statement.function->structure.func_def.code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code;
    idk->type = CODE;
    idk->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    idk->structure.code.operation->type = VAR_DECL;
    char *name3 = (char*)malloc(sizeof(char)*16);
    strcpy(name3, "MeineStringe");
    idk->structure.code.operation->structure.var_decl.token.value.string = name3;
    idk->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    idk->structure.code.next_code->type = CODE;
    idk->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    idk->structure.code.next_code->structure.code.operation->type = VAR_DECL;
    char *name4 = (char*)malloc(sizeof(char)*16);
    strcpy(name4, "Starte");
    idk->structure.code.next_code->structure.code.operation->structure.var_decl.token.value.string = name4;
    idk->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    idk->structure.code.next_code->structure.code.next_code->type = CODE;
    idk->structure.code.next_code->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    idk->structure.code.next_code->structure.code.next_code->structure.code.operation->type = VAR_DECL;
    char *name5 = (char*)malloc(sizeof(char)*16);
    strcpy(name5, "Ende");
    idk->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var_decl.token.value.string = name5;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->type = CODE;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->type = VAR;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var.val.i = 3;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var.type = I32;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var.token.value.string = name4;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->type = CODE;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->type = VAR;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var.val.i = 14;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var.type = I32;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var.token.value.string = name5;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->type = CODE;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->type = VAR;
    char *name6 = (char*)malloc(sizeof(char)*32);
    strcpy(name6, "NeviemHadamToSkoci");
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var.val.str = name6;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var.type = U8;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.var.token.value.string = name3;
    idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);

    tAstNode *tmpassign = idk->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code;
    tmpassign->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tmpassign->structure.code.operation->type = ASSIGN;
    tmpassign->structure.code.operation->structure.assign.dst = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tmpassign->structure.code.operation->structure.assign.dst->type = VAR;
    tmpassign->structure.code.operation->structure.assign.dst->structure.var.token.value.string = name3;
    
    tmpassign->structure.code.operation->structure.assign.src = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tmpassign->structure.code.operation->structure.assign.src->type = FUNC_CALL;
    tmpassign->structure.code.operation->structure.assign.src->structure.func_call.name_token.type = Token_BuildIn_Func;
    tmpassign->structure.code.operation->structure.assign.src->structure.func_call.name_token.value.BuiltInFunc = BF_substring;
    tmpassign->structure.code.operation->structure.assign.src->structure.func_call.arg_cnt = 3;
    tmpassign->structure.code.operation->structure.assign.src->structure.func_call.args = (tArgs *)malloc(sizeof(tArgs)*16);
    tmpassign->structure.code.operation->structure.assign.src->structure.func_call.args->token.value.string = name3;
    tmpassign->structure.code.operation->structure.assign.src->structure.func_call.args->token.type = Token_FuncID;
    tmpassign->structure.code.operation->structure.assign.src->structure.func_call.args->next = (tArgs *)malloc(sizeof(tArgs)*16);
    tmpassign->structure.code.operation->structure.assign.src->structure.func_call.args->next->token.value.string = name4;
    tmpassign->structure.code.operation->structure.assign.src->structure.func_call.args->next->token.type = Token_FuncID;
    tmpassign->structure.code.operation->structure.assign.src->structure.func_call.args->next->next = (tArgs *)malloc(sizeof(tArgs)*16);
    tmpassign->structure.code.operation->structure.assign.src->structure.func_call.args->next->next->token.value.string = name5;
    tmpassign->structure.code.operation->structure.assign.src->structure.func_call.args->next->next->token.type = Token_FuncID;

    tmpassign->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tmpassign->structure.code.next_code->type = CODE;
    tmpassign->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tmpassign->structure.code.next_code->structure.code.operation->type = FUNC_CALL;
    tmpassign->structure.code.next_code->structure.code.operation->structure.func_call.name_token.type =Token_BuildIn_Func;
    tmpassign->structure.code.next_code->structure.code.operation->structure.func_call.name_token.value.BuiltInFunc = BF_write;
    tmpassign->structure.code.next_code->structure.code.operation->structure.func_call.args = (tArgs *)malloc(sizeof(tArgs)*16);
    tmpassign->structure.code.next_code->structure.code.operation->structure.func_call.args->token.value.string = name3;
    tmpassign->structure.code.next_code->structure.code.operation->structure.func_call.args->next = NULL;
    tmpassign->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tmpassign->structure.code.next_code->structure.code.next_code->type = CODE;
    tmpassign->structure.code.next_code->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);

    char *name7 = (char*)malloc(sizeof(char)*16);
    strcpy(name7, "kaamo");
    char *name8 = (char*)malloc(sizeof(char)*16);
    strcpy(name8, "AsiNeee");

    tAstNode *tree2 = tmpassign->structure.code.next_code->structure.code.next_code->structure.code.operation;
    tree2->type = IF;
    tree2->structure.if_block.expr = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree2->structure.if_block.expr->type = BIN_OP;
    tree2->structure.if_block.expr->structure.bin_op.op1 = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree2->structure.if_block.expr->structure.bin_op.op2 = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree2->structure.if_block.expr->structure.bin_op.op1->type = VAL;
    tree2->structure.if_block.expr->structure.bin_op.op1->structure.val.token.type = Token_Integer;
    tree2->structure.if_block.expr->structure.bin_op.op1->structure.val.token.value.integer = 5;
    tree2->structure.if_block.expr->structure.bin_op.op2->type = VAL;
    tree2->structure.if_block.expr->structure.bin_op.op2->structure.val.token.type = Token_Integer;
    tree2->structure.if_block.expr->structure.bin_op.op2->structure.val.token.value.integer = 7;
    tree2->structure.if_block.expr->structure.bin_op.token.type = Token_Equal;
    tree2->structure.if_block.if_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree2->structure.if_block.if_code->type = VAR_DECL;
    tree2->structure.if_block.if_code->structure.var_decl.token.value.string = name7;
    tree2->structure.if_block.else_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree2->structure.if_block.else_code->type = VAR_DECL;
    tree2->structure.if_block.else_code->structure.var_decl.token.value.string = name8;

    tmpassign->structure.code.next_code->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    char *name9 = (char*)malloc(sizeof(char)*16);
    strcpy(name9, "StringIFJ24");
    char *name10 = (char*)malloc(sizeof(char)*64);
    strcpy(name10, "\nRetaze#cd\tlhy \"sad\n");

    tAstNode *tree3 = tmpassign->structure.code.next_code->structure.code.next_code->structure.code.next_code;
    tree3->type = CODE;
    tree3->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree3->structure.code.operation->type = VAR_DECL;
    tree3->structure.code.operation->structure.var_decl.type = U8;
    tree3->structure.code.operation->structure.var_decl.token.value.string = name9;
    tree3->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree3->structure.code.next_code->type = CODE;
    tree3->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree3->structure.code.next_code->structure.code.operation->type = ASSIGN;
    tree3->structure.code.next_code->structure.code.operation->structure.assign.dst = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree3->structure.code.next_code->structure.code.operation->structure.assign.src = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree3->structure.code.next_code->structure.code.operation->structure.assign.dst->type = VAR;
    tree3->structure.code.next_code->structure.code.operation->structure.assign.dst->structure.var.token.value.string = name9;
    tree3->structure.code.next_code->structure.code.operation->structure.assign.src->type = FUNC_CALL;
    tree3->structure.code.next_code->structure.code.operation->structure.assign.src->structure.func_call.arg_cnt = 1;
    tree3->structure.code.next_code->structure.code.operation->structure.assign.src->structure.func_call.name_token.value.BuiltInFunc = BF_string;
    tree3->structure.code.next_code->structure.code.operation->structure.assign.src->structure.func_call.name_token.type = Token_BuildIn_Func;
    tree3->structure.code.next_code->structure.code.operation->structure.assign.src->structure.func_call.args = (tArgs *)malloc(sizeof(tArgs)*16);
    tree3->structure.code.next_code->structure.code.operation->structure.assign.src->structure.func_call.args->next = NULL;
    tree3->structure.code.next_code->structure.code.operation->structure.assign.src->structure.func_call.args->token.type = Token_string;
    tree3->structure.code.next_code->structure.code.operation->structure.assign.src->structure.func_call.args->token.value.string = name10;
    tree3->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree3->structure.code.next_code->structure.code.next_code->type = CODE;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree3->structure.code.next_code->structure.code.next_code->structure.code.operation->type = FUNC_CALL;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.func_call.arg_cnt = 1;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.func_call.name_token.type = Token_BuildIn_Func;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.func_call.name_token.value.BuiltInFunc = BF_write;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.func_call.args = (tArgs *)malloc(sizeof(tArgs)*16);
    tree3->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.func_call.args->token.value.string = name9;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.func_call.args->token.type = Token_FuncID;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code->type = CODE;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->type = ASSIGN;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.assign.dst = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.assign.src = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.assign.dst->type = VAR;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.assign.dst->structure.var.token.value.string = name9;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.assign.src->type = FUNC_CALL;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.assign.src->structure.func_call.name_token.value.BuiltInFunc = BF_readstr;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.assign.src->structure.func_call.name_token.type = Token_BuildIn_Func;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.operation->structure.assign.src->structure.func_call.args = NULL;
    tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tAstNode *tree4 = tree3->structure.code.next_code->structure.code.next_code->structure.code.next_code->structure.code.next_code;
    tree4->type = CODE;
    tree4->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree4->structure.code.operation->type = FUNC_CALL;
    tree4->structure.code.operation->structure.func_call.name_token.type = Token_BuildIn_Func;
    tree4->structure.code.operation->structure.func_call.name_token.value.BuiltInFunc = BF_write;
    tree4->structure.code.operation->structure.func_call.args = (tArgs *)malloc(sizeof(tArgs)*16);
    tree4->structure.code.operation->structure.func_call.args->token.value.string = name9;
    tree4->structure.code.operation->structure.func_call.args->token.type = Token_FuncID;
    tree4->structure.code.next_code = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree4->structure.code.next_code->type = CODE;
    tree4->structure.code.next_code->structure.code.operation = (tAstNode *)malloc(sizeof(tAstNode)*16);
    tree4->structure.code.next_code->structure.code.operation->type = FUNC_CALL;
    tree4->structure.code.next_code->structure.code.operation->structure.func_call.name_token.value.BuiltInFunc = BF_write;
    tree4->structure.code.next_code->structure.code.operation->structure.func_call.name_token.type = Token_BuildIn_Func;
    tree4->structure.code.next_code->structure.code.operation->structure.func_call.args = (tArgs *)malloc(sizeof(tArgs)*16);
    tree4->structure.code.next_code->structure.code.operation->structure.func_call.args->token.type = Token_string;
    char *WriteString1 = (char*)malloc(sizeof(char)*64);
    strcpy(WriteString1, "Uvidime co \nto\032 spravi\n");
    tree4->structure.code.next_code->structure.code.operation->structure.func_call.args->token.value.string = WriteString1;


    GenInitial();
    GenerateOutput(tree);

 
}

//g++ test.cpp -lgtest -lgtest_main -pthread -o test_generate_output

// Main function for running all tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
