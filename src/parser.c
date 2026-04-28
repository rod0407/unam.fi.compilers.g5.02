#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef enum { 
    T_TYPE, T_ID, T_ASSIGN, T_NUMBER, 
    T_STRING_VAL, T_BOOL_VAL, 
    T_SEMI, T_EOF, T_ERROR 
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[256];
} Token;

// ESTRUCTURA
typedef struct ASTNode {
    char node_type[50];
    char var_type[50];
    char id_name[50];
    char expr_type[50]; 
    char expr_value[150];
    char semi[10];      
} ASTNode;

const char* input_code;
int pos = 0;
Token current_token;

int is_boundary(char c) {
    return !isalnum(c) && c != '_';
}


Token get_next_token() {
    Token t;
    while (isspace(input_code[pos])) pos++;
    
    if (input_code[pos] == '\0') { t.type = T_EOF; strcpy(t.lexeme, "EOF"); return t; }
    
    if (strncmp(&input_code[pos], "int", 3) == 0 && is_boundary(input_code[pos+3])) {
        t.type = T_TYPE; strcpy(t.lexeme, "int"); pos += 3; return t;
    }
    if (strncmp(&input_code[pos], "float", 5) == 0 && is_boundary(input_code[pos+5])) {
        t.type = T_TYPE; strcpy(t.lexeme, "float"); pos += 5; return t;
    }
    if (strncmp(&input_code[pos], "string", 6) == 0 && is_boundary(input_code[pos+6])) {
        t.type = T_TYPE; strcpy(t.lexeme, "string"); pos += 6; return t;
    }
    if (strncmp(&input_code[pos], "bool", 4) == 0 && is_boundary(input_code[pos+4])) {
        t.type = T_TYPE; strcpy(t.lexeme, "bool"); pos += 4; return t;
    }

    if (strncmp(&input_code[pos], "true", 4) == 0 && is_boundary(input_code[pos+4])) {
        t.type = T_BOOL_VAL; strcpy(t.lexeme, "true"); pos += 4; return t;
    }
    if (strncmp(&input_code[pos], "false", 5) == 0 && is_boundary(input_code[pos+5])) {
        t.type = T_BOOL_VAL; strcpy(t.lexeme, "false"); pos += 5; return t;
    }
    
    if (isalpha(input_code[pos])) {
        t.type = T_ID;
        int i = 0;
        while (isalnum(input_code[pos]) || input_code[pos] == '_') {
            t.lexeme[i++] = input_code[pos++];
        }
        t.lexeme[i] = '\0';
        return t;
    }
    
    if (isdigit(input_code[pos])) {
        t.type = T_NUMBER;
        int i = 0;
        while (isdigit(input_code[pos]) || input_code[pos] == '.') {
            t.lexeme[i++] = input_code[pos++];
        }
        t.lexeme[i] = '\0';
        return t;
    }

    if (input_code[pos] == '"') {
        t.type = T_STRING_VAL;
        int i = 0;
        pos++; 
        while (input_code[pos] != '"' && input_code[pos] != '\0' && i < 148) {
            t.lexeme[i++] = input_code[pos++];
        }
        if (input_code[pos] == '"') {
            pos++;
        }
        t.lexeme[i] = '\0';
        return t;
    }
    
    if (input_code[pos] == '=') { t.type = T_ASSIGN; strcpy(t.lexeme, "="); pos++; return t; }
    if (input_code[pos] == ';') { t.type = T_SEMI; strcpy(t.lexeme, ";"); pos++; return t; }
    
    // Error Léxico
    t.type = T_ERROR; 
    return t;
}

void eat(TokenType type) {
    if (current_token.type == type) {
        current_token = get_next_token();
    } else {
        printf("Parsing error...\n");
        exit(1);
    }
}

ASTNode* parse_declaration() {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    strcpy(node->node_type, "VarDecl");
    
    strcpy(node->var_type, current_token.lexeme);
    eat(T_TYPE);
    
    strcpy(node->id_name, current_token.lexeme);
    eat(T_ID);
    
    eat(T_ASSIGN);
    
    strcpy(node->expr_value, current_token.lexeme);
    
    if (current_token.type == T_NUMBER) {
        if (strchr(current_token.lexeme, '.') != NULL) {
            strcpy(node->expr_type, "float");
        } else {
            strcpy(node->expr_type, "int");
        }
        eat(T_NUMBER);
    } 
    else if (current_token.type == T_STRING_VAL) {
        strcpy(node->expr_type, "string");
        eat(T_STRING_VAL);
    } 
    else if (current_token.type == T_BOOL_VAL) {
        strcpy(node->expr_type, "bool");
        eat(T_BOOL_VAL);
    } 
    else {
        printf("Parsing error...\n");
        exit(1);
    }
    
    strcpy(node->semi, current_token.lexeme);
    eat(T_SEMI);
    
    return node;
}

// creacion del JSON
void print_json(ASTNode* node) {
    printf("{\n");
    printf("  \"node_type\": \"%s\",\n", node->node_type);
    printf("  \"var_type\": \"%s\",\n", node->var_type);
    printf("  \"id_name\": \"%s\",\n", node->id_name);
    printf("  \"expr_type\": \"%s\",\n", node->expr_type);
    printf("  \"expr_value\": \"%s\",\n", node->expr_value);
    printf("  \"semi\": \"%s\"\n", node->semi);
    printf("}\n");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Parsing error...\n");
        return 1;
    }
    
    input_code = argv[1];
    current_token = get_next_token();
    ASTNode* ast = parse_declaration();
    
    print_json(ast);
    free(ast);
    
    return 0;
}