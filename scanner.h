/**
 * Implementace překladače imperativního jazyka IFJ24
 * @file scanner.h
 * @brief Hlavičkový súbor pre lexikálny analyzátor
 * @author Alexander Žikla, xziklaa00
 * 
 * TODO: V FSM prepísať Intiger na Integer
 *       Pridať v FSM State_Array z Start -> Typeid a z FuncID -> TypeId
 *       Pridať v FSM State_Array z Typeid -> State_FuncID_1
 *       Pridat v FSM State_Check_Float z Integer -> Float 
 *       Pridat v FSM State_Check_Exp z Float -> Exp, ten pojde do Exp a Signed/Unsigned
 *       Potom Signed/Unsigned pojde do Exp (Signed/Unsigned -> Exp)
 *       Odstrániť State_Quote z FSM
 *       Pridať v FSM states State_IFJ_n
 * 
 *       Okomentovať I guess
 */


#ifndef SCANNER_H
#define SCANNER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "error.h"

#define MAX_LENGTH 128

typedef enum {
    State_AtImport,
    State_Start,
    State_FuncID,
    State_Check_TypeID,
    State_TypeID,
    State_FuncID_1,
    State_Array,
    State_Array_1,
    State_Array_2,

    State_IFJ_1,
    State_IFJ_2,
    State_IFJ_3,
    State_IFJ_4,
    State_BuildIn_Func,
    
    State_Integer,
    State_Float,
    State_Check_Float,
    State_Exp,
    State_Check_Exp,
    State_UnSigned_Signed_Exp,

    State_String,
    State_Esc_Seq,
    State_HexVal,
    State_Hex_1,
    State_StringEnd,

    State_Pipe,
    State_Slash,
    State_Comment,
    State_Lpar,
    State_Rpar,
    State_Lbrack,
    State_Rbrack,
    State_Plus,
    State_Minus,
    State_Multiply,
    State_Colon,
    State_Semicolon,
    State_Dot,
    State_Comma,
    State_Assign_or_Equal,
    State_Ex_Mark,
    State_Not_Equal,
    State_Lesser_Equal,
    State_Greater_Equal,

    State_EOF,
} sState;

typedef enum {
    Token_Empty,
    Token_AtImport,

    Token_Plus,
    Token_Minus,
    Token_Multiply,
    Token_Divide,

    Token_Integer,
    Token_Float,
    Token_String,

    Token_FuncID,
    
    Token_const,
    Token_else,
    Token_fn,
    Token_if,
    Token_null,
    Token_pub,
    Token_return,
    Token_var,
    Token_void,
    Token_while,
    Token_i32,
    Token_f64,
    Token_u8,
    Token_Ni32,
    Token_Nf64,
    Token_Nu8,

    Token_Assign,
    Token_Equal,
    Token_Not_Equal,
    Token_Lesser_Equal,
    Token_Lesser,
    Token_Greater_Equal,
    Token_Greater,

    Token_Quote,
    Token_Esc_Seq,

    Token_Lpar,
    Token_Rpar,
    Token_Lbrack,
    Token_Rbrack,
    Token_Colon,
    Token_Semicolon,
    Token_Dot,
    Token_Comma,
    Token_Pipe,

    Token_write,
    Token_readstr,
    Token_readi32,
    Token_readf64,
    Token_string,
    Token_concat,
    Token_length,
    Token_i2f,
    Token_f2i,
    Token_substring,
    Token_ord,
    Token_chr,
    Token_strcmp,

    Token_EOF,
    Token_BuildIn_Func,
    Token_IFJ,

	Token_Dollar //pomocny typ pro precedencni analyzu
} tTokenType;

typedef enum {
    BF_write,
    BF_readstr,
    BF_readi32,
    BF_readf64,
    BF_string,
    BF_concat,
    BF_length,
    BF_i2f,
    BF_f2i,
    BF_substring,
    BF_ord,
    BF_chr,
    BF_strcmp
} bBuiltinFuncs;

typedef union {
    char *string;
    int integer;
    double decimal;
    bBuiltinFuncs BuiltInFunc;
} tTokenVal;

typedef struct {
    tTokenVal value;
    tTokenType type;
    sState state;
} tToken;

typedef struct {
    char *string;
    int length;
    int max_length;
} sStr;

void SetSourceFile(FILE *f);

void CheckKW(tToken *token, sStr *string);

int CheckToken(tToken *token, sStr *string);

int CheckBuildInFunc(tToken *token, sStr *string);

int GetToken(tToken *token);

int String_Init(sStr *str);

void String_Free(sStr *str);

int String_Add(sStr *str, char character);

#endif /** SCANNER_H **/
