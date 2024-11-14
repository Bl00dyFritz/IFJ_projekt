/**
 * Implementace překladače imperativního jazyka IFJ24
 * @file generator.h
 * @brief Hlavičkový súbor pre generátor kódu
 * @authors Alexander Žikla, xziklaa00
 *          Jakub Turek, xturekj00
*/

#ifndef GENERATOR_H
#define GENERATOR_H

#include "parser.h"
#include "symtable.h"
#include "scanner.h"
#include "stack.c"

void GenHead(void);

void GenDefFunc(tFunctionVals *vals, char *FuncName);

void GenCallFunc(tBstNode *node, tFunctionVals *vals);

void GenFuncEnd(tFunctionVals *vals);

void GenExpBegin(void);

void GenExpEnd(void);

void GenStackOp(tBstNode *node, tToken *token);

void GenIfStart(void);

void GenIfEnd(void);

void GenElseStart(void);

void GenElseEnd(void);

void GenWhileHead(void);

void GenWhile(void);

void GenWhileEnd(void);

void GenReturn(void);

void generate3AK(tBstNode *strom, sStack *stack);

void GenerateOutput(tBstNode *strom, sStack *stack, tToken token);

#endif /** GENERATOR_H **/