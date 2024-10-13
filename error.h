/**
 * @file error.h
 * @author Alexander Žikla
 * @brief Error codes
 */

#ifndef ERROR_H
#define ERROR_H

#define LEXICAL_ERROR 1                      //Chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému)
#define SYNTAX_ERROR 2                       //Chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu, chybějící hlavička)
#define SEMANTIC_UNDEF_ERROR 3               //Sémantická chyba v programu – nedefinovaná funkce či proměnná
#define SEMANTIC_WRONG_NUM_OF_PAR_ERROR 4    //Sémantická chyba v programu – špatný počet/typ parametrů u volání funkce
#define SEMANTIC_REDEF_ERROR 5               //Sémantická chyba v programu – redefinice proměnné nebo funkce
#define SEMANTIC_RETURN_ERROR 6              //Chybějící/přebývající výraz v příkazu návratu z funkce
#define SEMANTIC_COMP_ERROR 7                //Sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech
#define SEMANTIC_INFER_ERROR 8               //Sémantická chyba odvození typu – typ proměnné není uveden a nelze odvodit od použitého výrazu
#define SEMANTIC_UNUSED_VAR_ERROR 9          //Sémantická chyba nevyužité proměnné v jejím rozsahu platnosti
#define SEMANTIC_OTHER_ERROR 10              //Ostatní sémantické chyby
#define INTERNAL_COMP_ERROR 99               //Interní chyba překladače tj. neovlivněná vstupním programem

#endif /** ERROR_H **/