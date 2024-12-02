/**
 * @file error.h
 * @author Alexander Žikla, xziklaa00
 * @brief Error codes
 */

#ifndef ERROR_H
#define ERROR_H

#define LEXICAL_ERROR 1                      //Error in the program within the lexical analysis (incorrect structure of the current lexeme)
#define SYNTAX_ERROR 2                       //Error in the program within the syntactic analysis (incorrect program syntax, missing header)
#define SEMANTIC_UNDEF_ERROR 3               //Semantic error in the program - undefined function or variable
#define SEMANTIC_WRONG_NUM_OF_PAR_ERROR 4    //Semantic error in the program – wrong number/type of parameters when calling the function
#define SEMANTIC_REDEF_ERROR 5               //Semantic error in the program – redefinition of variable or function
#define SEMANTIC_RETURN_ERROR 6              //Missing/remaining expression in the return command from the function
#define SEMANTIC_COMP_ERROR 7                //Semantic type compatibility error in arithmetic, string and relational expressions
#define SEMANTIC_INFER_ERROR 8               //Semantic type derivation error – the type of the variable is not specified and cannot be derived from the used expression
#define SEMANTIC_UNUSED_VAR_ERROR 9          //Semantic error of an unused variable in its scope of validity
#define SEMANTIC_OTHER_ERROR 10              //Other semantic errors
#define INTERNAL_COMP_ERROR 99               //Internal compiler error, ie unaffected by the input program

#endif /** ERROR_H **/