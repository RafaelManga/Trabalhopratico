#ifndef TOKEN_H
#define TOKEN_H

#define MAX_LEXEMA 256

typedef enum {
    KW_PROGRAM, KW_VAR, KW_INTEGER, KW_REAL,
    KW_BEGIN, KW_END, KW_IF, KW_THEN,
    KW_ELSE, KW_WHILE, KW_DO,
    ID, NUM_INT, NUM_REAL,
    OP_EQ, OP_NE, OP_LT, OP_LE, OP_GT, OP_GE,
    OP_AD, OP_MIN, OP_MUL, OP_DIV, OP_ASS,
    SMB_SEM, SMB_COM, SMB_OPA, SMB_CPA,
    SMB_COL, SMB_DOT, SMB_OBC, SMB_CBC,
    TOKEN_ERRO, TOKEN_EOF
} TipoToken;

typedef struct {
    TipoToken tipo;
    char lexema[MAX_LEXEMA];
    int linha;
    int coluna;
} Token;

const char* nomeToken(TipoToken tipo);

#endif