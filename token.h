#ifndef TOKEN_H
#define TOKEN_H

/* Tamanho máximo de um lexema (nome de variável, número, etc.) */
#define MAX_LEXEMA 256

/*
 * Todos os tipos de token que a linguagem MicroPascal possui.
 * O analisador léxico vai classificar cada pedaço do código
 * em um desses tipos.
 */
typedef enum {
    /* Palavras reservadas da linguagem */
    KW_PROGRAM, KW_VAR, KW_INTEGER, KW_REAL,
    KW_BEGIN, KW_END, KW_IF, KW_THEN,
    KW_ELSE, KW_WHILE, KW_DO,

    /* Nome de variável ou função criado pelo programador */
    ID,

    /* Números: inteiro (ex: 10) ou real com decimal obrigatória (ex: 3.14) */
    NUM_INT, NUM_REAL,

    /* Operadores de comparação */
    OP_EQ,   /* =  igual */
    OP_NE,   /* <> diferente */
    OP_LT,   /* <  menor que */
    OP_LE,   /* <= menor ou igual */
    OP_GT,   /* >  maior que */
    OP_GE,   /* >= maior ou igual */

    /* Operadores aritméticos */
    OP_AD,   /* +  soma */
    OP_MIN,  /* -  subtração */
    OP_MUL,  /* *  multiplicação */
    OP_DIV,  /* /  divisão */
    OP_ASS,  /* := atribuição */

    /* Símbolos de pontuação */
    SMB_SEM, /* ;  ponto e vírgula */
    SMB_COM, /* ,  vírgula */
    SMB_OPA, /* (  abre parêntese */
    SMB_CPA, /* )  fecha parêntese */
    SMB_COL, /* :  dois pontos */
    SMB_DOT, /* .  ponto final */
    SMB_OBC, /* {  abre comentário */
    SMB_CBC, /* }  fecha comentário */

    /* Casos especiais */
    TOKEN_ERRO,
    TOKEN_EOF
} TipoToken;

/*
 * Representa um token encontrado no código fonte.
 * Guarda o tipo, o texto original e onde ele apareceu.
 */
typedef struct {
    TipoToken tipo;
    char lexema[MAX_LEXEMA]; /* texto exato encontrado no código */
    int linha;               /* em qual linha estava */
    int coluna;              /* em qual coluna estava */
} Token;

/* Converte um tipo de token para texto legível (usado na saída .lex) */
const char* nomeToken(TipoToken tipo);

#endif
