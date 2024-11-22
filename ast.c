/**
 * @file ast.c
 * @brief Implementace abstraktniho syntaktickeho stromu
 * @author Nikola Jordanov xjordan00
 */

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "stack.h"
#include "scanner.h"
#include "error.h"

/**
 * @brief Funkce na ukladani uzlu typu Statement do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 */
void AddStatmentNode (tAstNode **node_dest){
	if(!node_dest) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	(*node_dest)->type = STATEMENT;
	(*node_dest)->structure.statement.next_statement = NULL;
	(*node_dest)->structure.statement.function = NULL;
}

/**
 * @brief Funkce na ukladani uzlu typu Code do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 */
void AddCodeNode (tAstNode **node_dest){
	if(!node_dest) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	(*node_dest)->type = CODE;
	(*node_dest)->structure.code.next_code = NULL;
	(*node_dest)->structure.code.operation = NULL;
}

/**
 * @brief Funkce na ukladani uzlu typu IfBlock do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 */
void AddIfBlockNode (tAstNode **node_dest){
	if(!node_dest) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if (!(*node_dest)) exit(99);
	(*node_dest)->type = IF;
	(*node_dest)->structure.if_block.else_code = NULL;
	(*node_dest)->structure.if_block.if_code = NULL;
	(*node_dest)->structure.if_block.expr = NULL;
	(*node_dest)->structure.if_block.nn_id = NULL;
	(*node_dest)->structure.if_block.if_symtable = NULL;
	(*node_dest)->structure.if_block.else_symtable = NULL;
}

/**
 * @brief Funkce na ukladani uzlu typu WhileLoop do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 */
void AddWhileNode (tAstNode **node_dest){
	if(!node_dest) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	(*node_dest)->type = WHILE;
	(*node_dest)->structure.while_loop.code = NULL;
	(*node_dest)->structure.while_loop.expr = NULL;
	(*node_dest)->structure.while_loop.nn_id = NULL;
	(*node_dest)->structure.while_loop.while_symtable = NULL;
}

/**
 * @brief Funkce na ukladani uzlu typu Assign do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 */
void AddAssignNode (tAstNode **node_dest){
	if(!node_dest) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	(*node_dest)->type = ASSIGN;
	(*node_dest)->structure.assign.dst = NULL;
	(*node_dest)->structure.assign.src = NULL;
}

/**
 * @brief Funkce na ukladani uzlu typu VarDecl do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 * @param stack Zasobnik ve kterem se nachazy informace uzlu: Typ, nazev
 */
void AddDeclNode (tAstNode **node_dest, tTokenStack *stack){
	if(!node_dest || !stack) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	tToken token = TopTStack(stack);
	PopTStack(stack);
	switch (token.type){
		case Token_Dollar:
			(*node_dest)->structure.var_decl.type =UNDEF; break;
		case Token_void:
			(*node_dest)->structure.var_decl.type = VOID; break;
		case Token_i32:
			(*node_dest)->structure.var_decl.type = I32; break;
		case Token_f64:
			(*node_dest)->structure.var_decl.type = F64; break;
		case Token_u8:
			(*node_dest)->structure.var_decl.type = U8; break;
		case Token_Ni32:
			(*node_dest)->structure.var_decl.type = NI32; break;
		case Token_Nf64:
			(*node_dest)->structure.var_decl.type = NF64; break;
		case Token_Nu8:
			(*node_dest)->structure.var_decl.type = NU8; break;
		default: exit(99);
	}
	token = TopTStack(stack);
	(*node_dest)->structure.var_decl.token = token;
	PopTStack(stack);
	token = TopTStack(stack);
	PopTStack(stack);
	if (token.type == Token_const)
		(*node_dest)->type = CONST_DECL;
	else if(token.type == Token_var)
		(*node_dest)->type = VAR_DECL;
	else exit(99);
}

/**
 * @brief Funkce na ukladani uzlu typu FuncDef do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 * @param stack Zasobnik ve kterem se nahazi informace uzlu: typ, nazev
 * @param arg_stack Zasobnik ve kterem se nazhazi informace definovanych argumentu funkce: typ, nazev (ceka se ze je vzdy dostane ve dvojicich v tomto poradi n-krat, kde n je pocet parametru
 */
void AddFuncDefNode (tAstNode **node_dest, tTokenStack *stack, 
		tTokenStack *arg_stack){
	if(!node_dest || !stack) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	tToken token = TopTStack(stack);
	PopTStack(stack);
	(*node_dest)->type = FUNC_DEF;
	(*node_dest)->structure.func_def.ret_type_token = token;
	token = TopTStack(stack);
	PopTStack(stack);
	(*node_dest)->structure.func_def.token = token;
	(*node_dest)->structure.func_def.internal_symtable = NULL;
	while(!StackIsEmpty(arg_stack)){
		tArgDef *tmp = (tArgDef *) malloc(sizeof(tArgDef));
		if(!tmp) exit(99);
		token = TopTStack(arg_stack);
		PopTStack(arg_stack);
		tmp->type_token = token;
		token = TopTStack(arg_stack);
		PopTStack(arg_stack);
		tmp->name_token = token;
		tmp->next = (*node_dest)->structure.func_def.args;
		(*node_dest)->structure.func_def.args = tmp;
	}

}

/**
 * @brief Funkce na ukladani uzlu typu FuncCall do ast
 * @param node_dest Odkaz na pozice v stromu kde se uzel ma pridat
 * @param id_t Token obsahujici nazev funkce
 * @param arg_stack Zasobnik obsahujuci tokeny nazvu nebo hodnoty argumentu
 */
void AddFuncCallNode (tAstNode **node_dest, tToken id_t,
		tTokenStack *arg_stack){
	if(!node_dest) exit(99);
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	(*node_dest)->type = FUNC_CALL;
	(*node_dest)->structure.func_call.arg_cnt = 0;
	(*node_dest)->structure.func_call.name_token = id_t;
	while(arg_stack->top){
		tArgs *tmp = (tArgs *) malloc(sizeof(tArgs));
		if(!tmp) exit(99);
		tToken token = TopTStack(arg_stack);
		PopTStack(arg_stack);
		tmp->token = token;
		tmp->next = (*node_dest)->structure.func_call.args;
		(*node_dest)->structure.func_call.args = tmp;
		(*node_dest)->structure.func_call.arg_cnt++;
	}
}

/**
 * @brief Funkce, ktera provadi precedencni analyzu a dle ni sklada zasobnik vystupnich tokenu, ktery se pak pouzije na skladani vyrazovy podstrom v ast
 * @param in_t Vstupni token, jeho hodnota se precedencne porovnava s hodnotu vrcholu vstupoveho zasobniku
 * @param input_stack Vstupovy zasobnik, dle vysledku precedencniho provovnani se do bud do toho uklada hodnotu in_t nebo jeho vrchol se posouva do vrcholu vystupoveho zasobniku
 * @param output_stack Vystupovy zasobnik do ktereho se podle precedence ukladaji operace/operandy
 */
void PrecedenceCheck (tToken *in_t, tTokenStack *input_stack,
		tTokenStack *output_stack){
	if(in_t){
		tToken token_in = *in_t;
		tToken token_top;
		if(!StackIsEmpty(input_stack)) token_top = TopTStack(input_stack);
		else {
			token_top.state = 0;
			token_top.type = Token_Dollar;
			token_top.value.integer = '$';
		}
		int cmp_op = ' ';
		switch (token_in.type){
			case Token_Multiply:
			case Token_Divide:
				switch (token_top.type){
					case Token_Multiply:
					case Token_Divide:
					case Token_Integer:
					case Token_Float:
					case Token_FuncID:
					case Token_Rpar:
						cmp_op = '>';
						break;
					case Token_Plus:
					case Token_Minus:
					case Token_Equal:
					case Token_Not_Equal:
					case Token_Lesser:
					case Token_Greater:
					case Token_Lesser_Equal:
					case Token_Greater_Equal:
					case Token_Lpar:
					case Token_Dollar:
						cmp_op = '<';
						break;
					default: exit(99);
				}
				break;
			case Token_Plus:
			case Token_Minus:
				switch (token_top.type){
					case Token_Multiply:
					case Token_Divide:
					case Token_Plus:
					case Token_Minus:
					case Token_Integer:
					case Token_Float:
					case Token_FuncID:
					case Token_Rpar:
						cmp_op = '>';
						break;
					case Token_Equal:
					case Token_Not_Equal:
					case Token_Lesser:
					case Token_Greater:
					case Token_Lesser_Equal:
					case Token_Greater_Equal:
					case Token_Lpar:
					case Token_Dollar:
						cmp_op = '<';
						break;
					default: exit(99);
				}
				break;
			case Token_Equal:
			case Token_Not_Equal:
			case Token_Lesser:
			case Token_Greater:
			case Token_Lesser_Equal:
			case Token_Greater_Equal:
				switch (token_top.type){
					case Token_Multiply:
					case Token_Divide:
					case Token_Plus:
					case Token_Minus:
					case Token_Integer:
					case Token_Float:
					case Token_FuncID:
					case Token_Rpar:
						cmp_op = '>';
						break;
					case Token_Equal:
					case Token_Not_Equal:
					case Token_Lesser:
					case Token_Greater:
					case Token_Lesser_Equal:
					case Token_Greater_Equal:
						exit(SYNTAX_ERROR);
						break;
					case Token_Lpar:
					case Token_Dollar:
						cmp_op = '<';
						break;
					default: exit(99);
				}
				break;
			case Token_Integer:
			case Token_Float:
			case Token_FuncID:
			case Token_Lpar:
				switch (token_top.type){
					case Token_Multiply:
					case Token_Divide:
					case Token_Plus:
					case Token_Minus:
					case Token_Equal:
					case Token_Not_Equal:
					case Token_Lesser:
					case Token_Greater:
					case Token_Lesser_Equal:
					case Token_Greater_Equal:
					case Token_Lpar:
					case Token_Dollar:
						cmp_op = '<';
						break;
					case Token_Integer:
					case Token_Float:
					case Token_FuncID:
					case Token_Rpar:
						exit(SYNTAX_ERROR);
						break;
					default: exit(99);
				}
				break;
			case Token_Rpar:
				while (token_top.type!=Token_Lpar){
					if (StackIsEmpty(input_stack)) exit(SYNTAX_ERROR);
					PushTStack(output_stack, token_top);
					PopTStack(input_stack);
					token_top = TopTStack(input_stack);
				}
				PopTStack(input_stack);
				return;
				break;
			default: exit(99);
		}
		if (cmp_op=='<') PushTStack(input_stack, token_in);
		else if(cmp_op=='>'){
			PushTStack(output_stack, token_top);
			PopTStack(input_stack);
			PrecedenceCheck(in_t, input_stack, output_stack);
		}
		else exit(SYNTAX_ERROR);
	}
	else {
		while(!StackIsEmpty(input_stack)){
			tToken token = TopTStack(input_stack);
			PushTStack(output_stack, token);
			PopTStack(input_stack);
		}
	}
}

/**
 * @brief Funkce ktera sklada podstrom z vyrazu do ast
 * @param node_dest Pozice kde se ma ulozit vyrazovy podstrom
 * @param stack Precedencne usporadany zasobnik
 */
void AddExpNodes (tAstNode **node_dest, tTokenStack *stack){
	if(!node_dest) exit(99);
	if(!stack) exit(SYNTAX_ERROR);
	if (StackIsEmpty(stack)) return;
	(*node_dest) = (tAstNode *) malloc(sizeof(tAstNode));
	if(!(*node_dest)) exit(99);
	tToken token = TopTStack(stack);
	switch (token.type){
		case Token_Multiply:
		case Token_Divide:
		case Token_Plus:
		case Token_Minus:
		case Token_Equal:
		case Token_Not_Equal:
		case Token_Lesser:
		case Token_Greater:
		case Token_Lesser_Equal:
		case Token_Greater_Equal:
			(*node_dest)->type = BIN_OP;
			(*node_dest)->structure.bin_op.token = token;
			(*node_dest)->structure.bin_op.op1 = NULL;
			(*node_dest)->structure.bin_op.op2 = NULL;
			break;
		case Token_Integer:
		case Token_Float:
			(*node_dest)->type = VAL;
			(*node_dest)->structure.val.token = token;
			return;
		case Token_FuncID:
			(*node_dest)->type = VAR;
			(*node_dest)->structure.var.token = token;
			return;
		default: exit(99);
	}
	AddExpNodes(&(*node_dest)->structure.bin_op.op1, stack);
	AddExpNodes(&(*node_dest)->structure.bin_op.op2, stack);
}

/**
 * @brief Funkce, ktera vymaze ast a uvolni pamet kterou zabira
 * @param tree Ukazatel na zacatek stromu
 */
void AstDispose (tAstNode **tree){
	if(!tree) exit(99);
	if(!(*tree)) return;
	switch ((*tree)->type){
		case STATEMENT:
			AstDispose(&(*tree)->structure.statement.next_statement);
			AstDispose(&(*tree)->structure.statement.function);
			break;
		case CODE:
			AstDispose(&(*tree)->structure.code.next_code);
			AstDispose(&(*tree)->structure.code.operation);
		case VAR:
		case VAL:
			break;
		case WHILE:
			AstDispose(&(*tree)->structure.while_loop.code);
			AstDispose(&(*tree)->structure.while_loop.expr);
			AstDispose(&(*tree)->structure.while_loop.nn_id);
			break;
		case IF:
			AstDispose(&(*tree)->structure.if_block.else_code);
			AstDispose(&(*tree)->structure.if_block.if_code);
			AstDispose(&(*tree)->structure.if_block.expr);
			AstDispose(&(*tree)->structure.if_block.nn_id);
			break;
		case BIN_OP:
			AstDispose(&(*tree)->structure.bin_op.op1);
			AstDispose(&(*tree)->structure.bin_op.op2);
			break;
		case ASSIGN:
			AstDispose(&(*tree)->structure.assign.dst);
			AstDispose(&(*tree)->structure.assign.src);
		case CONST_DECL:
		case VAR_DECL:
			break;
		case FUNC_CALL:
			while((*tree)->structure.func_call.args){
				tArgs *tmp = (*tree)->structure.func_call.args;
				(*tree)->structure.func_call.args = tmp;
				free(tmp);
			}
			break;
		case FUNC_DEF:
			while((*tree)->structure.func_def.args){
				tArgDef *tmp = (*tree)->structure.func_def.args;
				(*tree)->structure.func_def.args = tmp;
				free(tmp);
			}
			AstDispose(&(*tree)->structure.func_def.code);
			break;
	}
	free(*tree);
}
