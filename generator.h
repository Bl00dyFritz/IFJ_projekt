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

void GenInitial(void);

void GenHead(void);

void GenMainHead(void);

void GenDefFunc(tAstNode *node);

void GenBuiltInFuncs(void);

void GenCallFunc(tAstNode *node);

void GenFuncEnd(tAstNode *node);

void GenAssign(tAstNode *node);

void GenPushIntFloat(tAstNode *node);

bool CheckNill(tAstNode *node);

void GenStackOp(tAstNode *node);

void GenIfStart(void);

void GenIfStartNN(void);

void GenIfEnd(void);

void GenElseStart(void);

void GenElseEnd(void);

void GenWhileHead(void);

void GenWhile(void);

void GenWhileNN(void);

void GenWhileEnd(void);

void Generate(sStackGen *stack);

void generate3AK(sStackGen *stack, tAstNode *tree);

void GenerateOutput(tAstNode *node);

#endif /** GENERATOR_H **/