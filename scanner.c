/**
 * Implementace překladače imperativního jazyka IFJ24
 * @file scanner.c
 * @brief Scanner - Implementácia lexikálneho analyzátora
 * @author Alexander Žikla, xziklaa00
 */
#include "scanner.h"

FILE *file;

void SetSourceFile(FILE *f) {
    file = f;
}

int GetToken(tToken *token) {
    int c;
	sStr str;
    if (String_Init(&str)) return INTERNAL_COMP_ERROR;
    token->state = State_Start;
    token->type = Token_Empty;
    char HexVal[3] = {0};
    int WhiteSpaceCount = 0;
    bool Completed = false;
    while (!Completed) {
        c = getc(file);
        switch (token->state) {
            case State_Start:
                switch (c) {
                    case '/':
                            token->state = State_Slash;
                        break;
                    case '(':
                            token->state = State_Lpar;
                        break;
                    case ')':
                            token->state = State_Rpar;
                        break;
                    case '{':
                            token->state = State_Lbrack;
                        break;
                    case '}':
                            token->state = State_Rbrack;
                        break;
                    case '+':
                            token->state = State_Plus;
                        break;
                    case '-':
                            token->state = State_Minus;
                        break;
                    case '*':
                            token->state = State_Multiply;
                        break;
                    case ':':
                            token->state = State_Colon;
                        break;
                    case ';':
                            token->state = State_Semicolon;
                        break;
                    case ',':
                            token->state = State_Comma;
                        break;
                    case '.':
                            token->state = State_Dot;
                        break;
                    case '=':
                            token->state = State_Assign_or_Equal;
                        break;
                    case '<':
                            token->state = State_Lesser_Equal;
                        break;
                    case '>':
                            token->state = State_Greater_Equal;
                        break;
                    case '!':
                            token->state = State_Ex_Mark;
                        break;
                    case '"':
                            token->state = State_String;
                        break;
                    case '[':
                            token->state = State_Array;
                            String_Add(&str, c);
                        break;
                    case '?':
                            token->state = State_Check_TypeID;
                        break;
                    case EOF:
                            token->state = State_EOF;
                        break;
                    default:
                        if (c == 'i') {
                            token->state = State_IFJ_1;
                            String_Add(&str, c);
                        } else if (isalpha(c) || c == '_') {
                            token->state = State_FuncID;
                            token->type = Token_FuncID;
                            String_Add(&str, c);
                        } else if (isdigit(c)) {
                            token->state = State_Integer;
                            token->type = Token_Integer;
                            String_Add(&str, c);
                        } else if (isspace(c) || c == '\n' || c == '\t') {
                            token->state = State_Start;
                        } else {
                            String_Free(&str);
                            return LEXICAL_ERROR;
                        }
                        break;
                }
                break;
            case State_IFJ_1:
                if (c == 'f') {
                    String_Add(&str, c);
                    token->state = State_IFJ_2;
                } else if (isalnum(c)) {
                    ungetc(c, file);
                    token->state = State_FuncID;
                }
                break;
            case State_IFJ_2:
                if (c == 'j') {
                    String_Add(&str, c);
                    WhiteSpaceCount = 0;
                    token->state = State_IFJ_3;
                } else {
                    ungetc(c, file);
                    token->state = State_FuncID;
                }
                break;
            case State_IFJ_3:
                if (isspace(c)) {
                    WhiteSpaceCount++;
                    token->state = State_IFJ_3;
                } else if (c == '.') {
                    String_Add(&str, c);
                    WhiteSpaceCount = 0;
                    token->state = State_IFJ_4; 
                } else if (WhiteSpaceCount == 0) {              //Mozu byt funkcie alebo premenne menom ifjnnnnn  
                    ungetc(c, file);
                    token->state = State_FuncID;
                } else {
                    token->type = Token_IFJ;
                    Completed = true;
                    ungetc(c, file);
                }
                break;
            case State_IFJ_4:
                if (isspace(c)) {
                    token->state = State_IFJ_4;
                } else {
                    ungetc(c, file);
                    token->state = State_BuildIn_Func;
                }
                break;
            case State_BuildIn_Func:
                if (isalnum(c)) {
                    String_Add(&str, c);
                } else {
                    token->type = Token_BuildIn_Func;
                    ungetc(c, file);
                    Completed = true;
                }
                break;
            case State_FuncID:
                if (isalpha(c) || c == '_') {
                    String_Add(&str, c);
                } else if (isdigit(c)) {
                    token->state = State_TypeID;
                    ungetc(c, file);
                } else {
                    ungetc(c, file);
                    token->type = Token_FuncID;
                    CheckKW(token, &str);
                    Completed = true;
                }
                break;
            case State_Array:
                if (c == ']') {
                    String_Add(&str, c);
                    token->state = State_Array_1;
                } else if (isspace(c)) {
                    token->state = State_Array;
                } else { 
                    String_Free(&str);
                    return LEXICAL_ERROR;
                }
                break;
            case State_Array_1:
                if (c == 'u') {
                    String_Add(&str, c);
                    token->state = State_Array_2;
                } else if (isspace(c)) {
                    token->state = State_Array_1;
                } else {
                    String_Free(&str);
                    return LEXICAL_ERROR;    
                }
                break;
            case State_Array_2:
                if (c == '8') {
                    String_Add(&str, c);
                    token->type = Token_u8;
                    token->value.keyword = KW_u8;
                    Completed = true;
                } else {
                    String_Free(&str);
                    return LEXICAL_ERROR;
                }
                break;
            case State_Check_TypeID:
                if (c == 'f' || c == 'i') {
                    String_Add(&str, c);
                    token->state = State_TypeID;
                    token->type = Token_FuncID;
                } else if (c == '[') {
                    String_Add(&str, c);
                    token->state = State_Array;
                } else { 
                    String_Free(&str);
                    return LEXICAL_ERROR;
                }
                break;
            case State_TypeID:
                if (isdigit(c)) {
                    String_Add(&str, c);
                } else if (isalpha(c) || c == '_') {
                    token->type= Token_FuncID;
                    token->state = State_FuncID_1;
                    ungetc(c, file);
                } else {
                    ungetc(c, file);
                    CheckKW(token, &str);
                    Completed = true;
                }
                break;
            case State_FuncID_1:
                if (isalnum(c) || c == '_') { 
                    String_Add(&str, c);
                } else {
                    ungetc(c, file);
                    Completed = true;
                }
                break;
            case State_Integer:
                if (isdigit(c)) {
                    String_Add(&str, c);
                } else if (c == '.') {
                    String_Add(&str, c);
                    token->state = State_Check_Float;
                } else if (c == 'e' || c == 'E') {
                    String_Add(&str, c);
                    token->state = State_Check_Exp;
                } else {
                    ungetc(c, file);
                    char *tmp = NULL;
                    unsigned long val = strtoul(str.string, &tmp, 10);
                    if (*tmp != '\0') {
                        String_Free(&str);
                        fprintf(stderr, "Error: Failed to convert string to integer\n");
                        return INTERNAL_COMP_ERROR;
                    }
                    token->type = Token_Integer;
                    token->value.integer = val;
                    Completed = true;
                }
                break;
            case State_Check_Float:
                if (isdigit(c)) {
                    token->state = State_Float;
                    ungetc(c, file);
                } else {
                    String_Free(&str);
                    return LEXICAL_ERROR;
                }
                break;
            case State_Float:
                if (isdigit(c)) {
                    String_Add(&str, c);
                } else if (c == 'e' || c == 'E') {
                    token->state = State_Check_Exp;
                    String_Add(&str, c);
                } else {
                    ungetc(c, file);
                    char *tmp = NULL;
                    double val = strtod(str.string, &tmp);
                    if (*tmp != '\0') {
                        String_Free(&str);
                        fprintf(stderr, "Error: Failed to convert string to double\n");
                        return INTERNAL_COMP_ERROR;
                    }
                    token->type = Token_Float;
                    token->value.decimal = val;
                    Completed = true;
                }
                break;
            case State_Check_Exp:
                if (c == '+' || c == '-') {
                    token->state = State_UnSigned_Signed_Exp;
                    String_Add(&str, c);
                } else if (isdigit(c)) {
                    token->state = State_Exp;
                    ungetc(c, file);
                } else {
                    String_Free(&str);
                    fprintf(stderr, "Error: Exponent (E/e) must be followed by a digit or plus/minus sign\n");
                    return LEXICAL_ERROR;
                }
                break;
            case State_Exp:
                if (isdigit(c)) {
                    String_Add(&str, c);
                } else {
                    ungetc(c, file);
                    char *tmp = NULL;
                    double val = strtod(str.string, &tmp);
                    if (*tmp != '\0') {
                        String_Free(&str);
                        fprintf(stderr, "Error: Failed to convert string to float\n");
                        return INTERNAL_COMP_ERROR;
                    }
                    token->type = Token_Float;
                    token->value.decimal = val;
                    Completed = true;
                }
                break;
            case State_UnSigned_Signed_Exp:
                if (isdigit(c)) {
                    token->state = State_Exp;
                    ungetc(c, file);
                } else {
                    String_Free(&str);
                    fprintf(stderr, "Error: Exponent with a sign (E+/-) must be followed by a digit\n");
                    return LEXICAL_ERROR;
                }
                break;
            case State_String:
                if (c < 32) {
                    String_Free(&str);
                    fprintf(stderr, "Error: Unsupported characters\n");
                    return LEXICAL_ERROR;
                } else if (c == '"') {
                    token->state = State_StringEnd;
                } else if (c == '\\') {
                    token->state = State_Esc_Seq;
                } else {
                    String_Add(&str, c);
                }
                break;
            case State_Esc_Seq:
                switch (c) {
                    case 'n':
                        String_Add(&str, '\n');                                           
                        token->state = State_String;
                        break;
                    case 'r':
                        String_Add(&str, '\r');
                        token->state = State_String;
                        break;
                    case 't':
                        String_Add(&str, '\t');
                        token->state = State_String;
                        break;
                    case '\\':
                        String_Add(&str, '\\');
                        token->state = State_String;
                        break;
                    case '"':
                        String_Add(&str, '\"');
                        token->state = State_String;
                        break;
                    case 'x':
                        token->state = State_HexVal;
                        break;
                    default:
                        String_Free(&str);
                        fprintf(stderr, "Error: Invalid escape sequence\n");
                        return LEXICAL_ERROR;
                }
                break;
            case State_HexVal:
                if (isdigit(c)) {
                    HexVal[0] = c;
                    token->state = State_Hex_1;
                } else {
                    String_Free(&str);
                    fprintf(stderr, "Error: Invalid Hexadecimal number\n");
                    return LEXICAL_ERROR;
                }
                break;
            case State_Hex_1:
                if (isdigit(c)) {
                    HexVal[1] = c;
                    HexVal[2] = '\0';
                    char *tmp = NULL;
                    long val = strtol(HexVal, &tmp, 16);
                    if (*tmp != '\0') {
                        String_Free(&str);
                        fprintf(stderr, "Error: Failed to convert string to Hexadecimal number\n");
                        return INTERNAL_COMP_ERROR;
                    }
                    String_Add(&str, val); 
                    token->state = State_String;
                } else {
                    String_Free(&str);
                    fprintf(stderr, "Error: Invalid Hexadecimal number\n");
                    return LEXICAL_ERROR;
                }
                break;
            case State_StringEnd:
                token->type = Token_String;
                Completed = true;
                ungetc(c, file);
                break;
            case State_Slash:
                if (c == '/') {
                    token->state = State_Comment;
                } else {
                    token->type = Token_Divide;
                    Completed = true;
                    ungetc(c, file);
                }
                break;
            case State_Comment:
                if (c == '\n') {
                    token->state = State_Start;
                } else if (c == EOF) {
                    token->type = Token_EOF;
                    Completed = true;
                }
                break;
            case State_Lpar:
                token->type = Token_Lpar;
                Completed = true;
                ungetc(c, file);
                break;
            case State_Rpar:
                token->type = Token_Rpar;
                Completed = true;
                ungetc(c, file);
                break;
            case State_Lbrack:
                token->type = Token_Lbrack;
                Completed = true;
                ungetc(c, file);
                break;
            case State_Rbrack:
                token->type = Token_Rbrack;
                Completed = true;
                ungetc(c, file);
                break;
            case State_Plus:
                token->type = Token_Plus;
                Completed = true;
                ungetc(c, file);
                break;
            case State_Minus:
                token->type = Token_Minus;
                Completed = true;
                ungetc(c, file);
                break;
            case State_Multiply:
                token->type = Token_Multiply;
                Completed = true;
                ungetc(c, file);
                break;
            case State_Colon:
                token->type = Token_Colon;
                Completed = true;
                ungetc(c, file);
                break;
            case State_Semicolon:
                token->type = Token_Semicolon;
                Completed = true;
                ungetc(c, file);
                break;
            case State_Dot:
                token->type = Token_Dot;
                Completed = true;
                ungetc(c, file);
                break;
            case State_Comma:
                token->type = Token_Comma;
                Completed = true;
                ungetc(c, file);
                break;
            case State_Assign_or_Equal:
                if (c != '=') {
                    token->type = Token_Assign;
                    Completed = true;
                    ungetc(c, file);
                } else if (c == '=') {
                    token->type = Token_Equal;
                    Completed = true;
                    ungetc(c, file);
                }
                break;
            case State_Ex_Mark:
                if (c == '=') {
                    token->state = State_Not_Equal;
                } else {
                    String_Free(&str);
                    return LEXICAL_ERROR;
                }
                break;
            case State_Not_Equal:
                token->type = Token_Not_Equal;
                Completed = true;
                ungetc(c, file);
                break;
            case State_Lesser_Equal:
                if (c == '=') {
                    token->type = Token_Lesser_Equal;
                } else {
                    token->type = Token_Lesser;
                    ungetc(c, file);
                }
                Completed = true;
                break;
            case State_Greater_Equal:
                if (c == '=') {
                    token->type = Token_Greater_Equal;
                } else {
                    token->type = Token_Greater;
                    ungetc(c, file);
                }
                Completed = true;
                break;
            case State_EOF:
                token->type = Token_EOF;
                Completed = true;

                break;
        }
    }

    if (token->type == Token_String || token->type == Token_FuncID || token->type == Token_BuildIn_Func) {
        token->value.string = str.string;
    } else {
        String_Free(&str);
    } 

    return 0;
}

void CheckKW(tToken *token, sStr *str) {
    if (strcmp(str->string, "const") == 0) {
        token->type = Token_const;
        token->value.keyword = KW_const;
    } else if (strcmp(str->string, "else") == 0) {
        token->type = Token_else;
        token->value.keyword = KW_else;
    } else if (strcmp(str->string, "fn") == 0) {
        token->type = Token_fn;
        token->value.keyword = KW_fn;
    } else if (strcmp(str->string, "if") == 0) {
        token->type = Token_if;
        token->value.keyword = KW_if;
    } else if (strcmp(str->string, "i32") == 0) {
        token->type = Token_i32;
        token->value.keyword = KW_i32;
    } else if (strcmp(str->string, "f64") == 0) {
        token->type = Token_f64;
        token->value.keyword = KW_f64;
    } else if (strcmp(str->string, "null") == 0) {
        token->type = Token_null;
        token->value.keyword = KW_null;
    } else if (strcmp(str->string, "pub") == 0) {
        token->type = Token_pub;
        token->value.keyword = KW_pub;
    } else if (strcmp(str->string, "return") == 0) {
        token->type = Token_return;
        token->value.keyword = KW_return;
    } else if (strcmp(str->string, "var") == 0) {
        token->type = Token_var;
        token->value.keyword = KW_var;
    } else if (strcmp(str->string, "void") == 0) {
        token->type = Token_void;
        token->value.keyword = KW_void;
    } else if (strcmp(str->string, "while") == 0) {
        token->type = Token_while;
        token->value.keyword = KW_while;
    }
}

int PrologueScan(void) {
    sStr str;
    int c;
    if (String_Init(&str)) return INTERNAL_COMP_ERROR;

    c = getc(file);
    while (c != ';') {
        String_Add(&str, c);
        c = getc(file);
    }
    String_Add(&str, c);
    
    if (strcmp(str.string, "const ifj = @import(\"ifj24.zig\");")) {
        String_Free(&str);
        fprintf(stderr, "Error: The program has to start with a Prologue\n");
        return LEXICAL_ERROR;
    }

    String_Free(&str);
    return 0;
}

int String_Init(sStr *str) {
    str->string = (char*)malloc(MAX_LENGTH);
    if (str->string == NULL) {
        fprintf(stderr, "Error: Allocation failed\n");
        return 1;
    } else {
        str->length = 0;
        str->max_length = MAX_LENGTH;
        str->string[0] = '\0';
        return 0;
    }
}

void String_Free(sStr *str) {
    free(str->string);
}

int String_Add(sStr *str, char c) {
    if ((str->length + 1) >= str->max_length ) {
        char *temp = realloc(str->string, str->max_length + MAX_LENGTH);
        if (temp == NULL) {
            fprintf(stderr, "Error: Allocation failed\n");
            return 1;
        } else {
            str->string = temp;
            str->max_length += MAX_LENGTH;
        }
    }
    str->string[str->length] = c;
    str->length++;
    str->string[str->length] = '\0';
    return 0;
}
