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
#include "GenStack.h"

void GenHead(void);

void GenMainHead(void);

void GenDefFunc(tFunctionVals *vals, char *FuncName);

void GenCallBuiltInFunc(tAstNode *node);

void GenCallFunc(tAstNode *node, tFunctionVals *FuncVals, char *FuncName);

void GenFuncEnd(tFunctionVals *vals);

void GenExpBegin(void);

void GenExpEnd(void);

void GenStackOp(tAstNode *node, tToken *token);

void GenIfStart(void);

void GenIfEnd(void);

void GenElseStart(void);

void GenElseEnd(void);

void GenWhileHead(void);

void GenWhile(void);

void GenWhileEnd(void);

void GenReturn(void);

void generate3AK(sStackGen *stack, tAstNode *tree);

void GenerateOutput(tAstNode *tree, sStackGen *stack, tFunctionVals *FuncVals, char *FuncName);

#endif /** GENERATOR_H **/