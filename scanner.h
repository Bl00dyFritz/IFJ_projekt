/**
 * Implementation of the IFJ24 imperative language compiler
 * @file scanner.h
 * @brief Header file for the lexical analyzer
 * @author Alexander Žikla, xziklaa00
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

/**
 * @brief Enum for states of the FSM
 */
typedef enum {
    State_Start,
    State_AtImport,
    State_Underscore,
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

/**
 * @brief Enum for types of Token
 */
typedef enum {
    Token_Empty,
    Token_AtImport,
    Token_Underscore,

    Token_Plus,
    Token_Minus,
    Token_Multiply,
    Token_Divide,

    Token_Integer,
    Token_Float,
    Token_string,

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

    Token_EOF,
    Token_BuildIn_Func,
    Token_IFJ,

	Token_Dollar         //Auxiliary type for precedent analysis
} tTokenType;

/**
 * @brief Enum for Built-in functions
 */
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

/**
 * @brief Union for token values
 */
typedef union {
    char *string;
    int integer;
    double decimal;
    bBuiltinFuncs BuiltInFunc;
} tTokenVal;

/**
 * @brief Structure for token
 */
typedef struct {
    tTokenVal value;
    tTokenType type;
    sState state;
} tToken;

/**
 * @brief Structure for dynamic string 
 */
typedef struct {
    char *string;
    int length;
    int max_length;
} sStr;

/**
 * @brief Set file with source code 
 * @param f Pointer to the source file 
 */
void SetSourceFile(FILE *f);

/**
 * @brief Checks if the string matches with one of the keywords and sets token->type accordingly
 * @param token Pointer to the token structure 
 * @param string Pointer to the string structure containing the input text
 */
void CheckKW(tToken *token, sStr *string);

/**
 * @brief Checks if the string matches with one of the var types and sets token->type accordingly
 * @param token Pointer to the token structure 
 * @param string Pointer to the string structure containing the input text
 * @return 1 - if string matches with some type
 */
int CheckVarType(tToken *token, sStr *string);

/**
 * @brief Checks if the string matches with one of the Built-in functions and set token->value.BuiltInFunc accordingly
 * @param token Pointer to the token structure 
 * @param string Pointer to the string structure containing the input text
 * @return 1 - if string matches with some function
 */
int CheckBuildInFunc(tToken *token, sStr *string);

/**
 * @brief Reads the next token from the input   
 * @param token Pointer to the token structure, where the needed values will be stored
 * @return 0 - if reading token was done without error
 */
int GetToken(tToken *token);

/**
 * @brief Inicialization of the dynamic string structure
 * @param str Pointer to the string structure
 * @return 1 - Allocation failed
 */
int String_Init(sStr *str);

/**
 * @brief Frees the memory allocated for the dynamic string
 * @param str Pointer to the string structure
 */
void String_Free(sStr *str);

/**
 * @brief Adds a character to the end of the string
 * @param str Pointer to the string structure
 * @param character Char to add to the string 
 * @return 1 - Allocation failed
 */
int String_Add(sStr *str, char character);

#endif /** SCANNER_H **/
