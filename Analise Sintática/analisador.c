#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    PROGRAM, VAR, INTEGER, REAL, BEGIN, END, IF, THEN, ELSE, WHILE, DO, ASSIGN, IDENTIFIER, NUMBER,
    PLUS, MINUS, MULT, DIV, EQUAL, NOTEQUAL, LESS, LESSEQUAL, GREATER, GREATEREQUAL, SEMICOLON,
    COLON, COMMA, DOT, OPENPAR, CLOSEPAR, INVALID, EOF_TOKEN
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[100];
    int line;
} Token;

FILE *source_code;
int current_line = 1;


Token getNextToken();
void CasaToken(TokenType expected);
void Programa();
void Bloco();
void DeclaracaoVariaveis();
void ComandoComposto();
void Comando();
void Atribuicao();
void ComandoCondicional();
void ComandoRepetitivo();
void Expressao();
void ExpressaoSimples();
void Termo();
void Fator();

void erro(const char *mensagem, Token token) {
    fprintf(stderr, "%d:%s '%s'.\n", token.line, mensagem, token.lexeme);
    exit(EXIT_FAILURE);
}

Token getNextToken() {
    Token token = { .type = INVALID, .line = current_line };
    int c;

    while ((c = fgetc(source_code)) != EOF) {
        if (c == '\n') {
            current_line++;
        } else if (isspace(c)) {
            continue;
        } else if (c == '{') { 
            while ((c = fgetc(source_code)) != EOF && c != '}') {
                if (c == '\n') current_line++;
            }
        } else {
            break;
        }
    }

    if (c == EOF) {
        token.type = EOF_TOKEN;
        strcpy(token.lexeme, "EOF");
        return token;
    }

    if (isalpha(c)) { 
        int i = 0;
        token.lexeme[i++] = c;
        while (isalnum(c = fgetc(source_code)) && i < 99) {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        ungetc(c, source_code);

        if (strcmp(token.lexeme, "program") == 0) token.type = PROGRAM;
        else if (strcmp(token.lexeme, "var") == 0) token.type = VAR;
        else if (strcmp(token.lexeme, "integer") == 0) token.type = INTEGER;
        else if (strcmp(token.lexeme, "real") == 0) token.type = REAL;
        else if (strcmp(token.lexeme, "begin") == 0) token.type = BEGIN;
        else if (strcmp(token.lexeme, "end") == 0) token.type = END;
        else if (strcmp(token.lexeme, "if") == 0) token.type = IF;
        else if (strcmp(token.lexeme, "then") == 0) token.type = THEN;
        else if (strcmp(token.lexeme, "else") == 0) token.type = ELSE;
        else if (strcmp(token.lexeme, "while") == 0) token.type = WHILE;
        else if (strcmp(token.lexeme, "do") == 0) token.type = DO;
        else token.type = IDENTIFIER;

        printf("Token: %s, Type: %d\n", token.lexeme, token.type);
        return token;
    }
    if (isdigit(c)) {
        int i = 0;
        token.lexeme[i++] = c;
        while (isdigit(c = fgetc(source_code)) && i < 99) {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        ungetc(c, source_code);
        token.type = NUMBER;
        return token;
    }

    token.lexeme[0] = c;
    token.lexeme[1] = '\0';
    switch (c) {
        case ':':
            if ((c = fgetc(source_code)) == '=') {
                strcpy(token.lexeme, ":=");
                token.type = ASSIGN;
            } else {
                ungetc(c, source_code);
                token.type = COLON;
            }
            break;
        case '+': token.type = PLUS; break;
        case '-': token.type = MINUS; break;
        case '*': token.type = MULT; break;
        case '/': token.type = DIV; break;
        case '=': token.type = EQUAL; break;
        case '<':
            if ((c = fgetc(source_code)) == '>') {
                strcpy(token.lexeme, "<>");
                token.type = NOTEQUAL;
            } else if (c == '=') {
                strcpy(token.lexeme, "<=");
                token.type = LESSEQUAL;
            } else {
                ungetc(c, source_code);
                token.type = LESS;
            }
            break;
        case '>':
            if ((c = fgetc(source_code)) == '=') {
                strcpy(token.lexeme, ">=");
                token.type = GREATEREQUAL;
            } else {
                ungetc(c, source_code);
                token.type = GREATER;
            }
            break;
        case ';': token.type = SEMICOLON; break;
        case ',': token.type = COMMA; break;
        case '.': token.type = DOT; break;
        case '(': token.type = OPENPAR; break;
        case ')': token.type = CLOSEPAR; break;
        default: token.type = INVALID; break;
    }

    return token;
}
void CasaToken(TokenType expected) {
    Token token = getNextToken();
    if (token.type != expected) {
        erro("token nao esperado", token);
    }
}

void Programa() {
    CasaToken(PROGRAM);
    CasaToken(IDENTIFIER);
    CasaToken(SEMICOLON);
    Bloco();
    CasaToken(DOT);
}

void Bloco() {
    DeclaracaoVariaveis();
    ComandoComposto();
}

void DeclaracaoVariaveis() {
}

void ComandoComposto() {
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo MicroPascal>\n", argv[0]);
        return 1;
    }

    source_code = fopen(argv[1], "r");
    if (!source_code) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    Programa();
    fclose(source_code);

    printf("Análise concluída com sucesso.\n");
    return 0;
}

