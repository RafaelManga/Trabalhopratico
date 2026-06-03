#ifndef AST_H
#define AST_H

#include "token.h"

typedef struct No No;

No *no_criar(const char *rotulo);
No *no_folha_token(const Token *tok);
void no_adicionar_filho(No *pai, No *filho);

void ast_imprimir_texto(const No *raiz, int profundidade);
void ast_exportar_dot(const No *raiz, const char *arquivoDot);
int ast_gerar_imagem(const char *arquivoDot, const char *arquivoPng, const char *arquivoPdf);
void no_liberar(No *no);

#endif
