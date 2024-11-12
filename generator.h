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

void GenHead(void);

void GenStackPush(void);

void GenStackPop(void);

void GenMath(void);

void GenCheckVar(void);

void GenDefVar(void);

void GenAssignVar(void);

void GenDefFunc(void);

void GenCallFunc(tBstNode *node);

void GenExpBegin(void);

void GenExpEnd(void);

void GenExp(tBstNode *node, tToken *token);

void GenIfStart(void);

void GenIfEnd(void);

void GenElseStart(void);

void GenElseEnd(void);

void GenWhileStart(void);

void GenWhileEnd(void);

void GenReturn(void);

#endif /** GENERATOR_H **/