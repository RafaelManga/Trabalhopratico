#ifndef TOKEN_H
#define TOKEN_H

#define MAX_LEXEMA 256


typedef enum {
    KW_PROGRAM, KW_VAR, KW_INTEGER, KW_REAL,
    KW_BEGIN, KW_END, KW_IF, KW_THEN,
    KW_ELSE, KW_WHILE, KW_DO,

    ID,

    NUM_INT, NUM_REAL,

    OP_EQ,   /* =  igual */
    OP_NE,   /* <> diferente */
    OP_LT,   /* <  menor que */
    OP_LE,   /* <= menor ou igual */
    OP_GT,   /* >  maior que */
    OP_GE,   /* >= maior ou igual */


    OP_AD,   /* +  soma */
    OP_MIN,  /* -  subtração */
    OP_MUL,  /* *  multiplicação */
    OP_DIV,  /* /  divisão */
    OP_ASS,  /* := atribuição */

    
    SMB_SEM, /* ;  ponto e vírgula */
    SMB_COM, /* ,  vírgula */
    SMB_OPA, /* (  abre parêntese */
    SMB_CPA, /* )  fecha parêntese */
    SMB_COL, /* :  dois pontos */
    SMB_DOT, /* .  ponto final */
    SMB_OBC, /* {  abre comentário */
    SMB_CBC, /* }  fecha comentário */

   
    TOKEN_ERRO,
    TOKEN_EOF
} TipoToken;


typedef struct {
    TipoToken tipo;
    char lexema[MAX_LEXEMA]; 
    int linha;              
    int coluna;             
} Token;


const char* nomeToken(TipoToken tipo);

#endif
// ficou daora, orgulho demais