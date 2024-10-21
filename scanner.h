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
    State_Start,
    State_FuncID,
    State_Check_TypeID,
    State_TypeID,
    State_FuncID_1,
    State_Array,

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

    Token_Plus,
    Token_Minus,
    Token_Multiply,
    Token_Divide,

    Token_Integer,
    Token_Float,
    Token_String,

    Token_FuncID,
    Token_TypeID,
    
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

    Token_EOF,
    Token_BuildIn_Func,
    Token_IFJ
} tTokenType;

typedef enum {
    KW_const,
    KW_else,
    KW_if,
    KW_return,
    KW_var,
    KW_void,
    KW_while,
    KW_fn,
    KW_i32,
    KW_f64,
    KW_u8,
    KW_pub,
    KW_null
} kKeyword;

typedef union {
    char *string;
    int integer;
    double decimal;
    kKeyword keyword;
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

int PrologueScan();

void CheckKW(tToken *token, sStr *string);

int GetToken(tToken *token, int c);

int String_Init(sStr *str);

void String_Free(sStr *str);

int String_Add(sStr *str, char character);

#endif /** SCANNER_H **/