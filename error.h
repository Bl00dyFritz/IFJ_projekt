/**
 * @file error.h
 * @author Alexander Žikla
 * @brief Error codes
 */

#ifndef ERROR_H
#define ERROR_H

typedef enum {
    NO_ERROR,                               //Žádna chyba
    LEXICAL_ERROR,                          //Chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému)
    SYNTAX_ERROR,                           //Chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu, chybějící hlavička, atp.)
    SEMANTIC_UNDEF_ERROR,                   //Sémantická chyba v programu – nedefinovaná funkce či proměnná
    SEMANTIC_WRONG_NUM_OF_PAR_ERROR,        //Sémantická chyba v programu – špatný počet/typ parametrů u volání funkce
    SEMANTIC_REDEF_ERROR,                   //Sémantická chyba v programu – redefinice proměnné nebo funkce
    SEMANTIC_RETURN_ERROR,                  //Chybějící/přebývající výraz v příkazu návratu z funkce
    SEMANTIC_COMP_ERROR,                    //Sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech
    SEMANTIC_INFER_ERROR,                   //Sémantická chyba odvození typu – typ proměnné není uveden a nelze odvodit od použitého výrazu
    SEMANTIC_UNUSED_VAR_ERROR,              //Sémantická chyba nevyužité proměnné v jejím rozsahu platnosti
    SEMANTIC_OTHER_ERROR,                   //Ostatní sémantické chyby
    INTERNAL_COMP_ERROR = 99                //Interní chyba překladače tj. neovlivněná vstupním programem
} ERROR_CODES;

#endif /** ERROR_H **/