#include <stdio.h>
#include <ctype.h>
#include "erros.h"

void analisarArquivo(const char *caminhoOriginal) {
    FILE *input = fopen(caminhoOriginal, "r");
    if (!input) {
        perror("Erro ao abrir arquivo de entrada");
        return;
    }

    int c;
    int linha = 1;
    int coluna = 0;
    int emComentario = 0;
    int linInicioComentario, colInicioComentario;

    while ((c = fgetc(input)) != EOF) {
        coluna++;

        //  Gerenciamento de Linhas e Colunas
        
        if (c == '\n') {
            linha++;
            coluna = 0;
            continue;
        }

        //  Lógica simplificada de Comentário

        if (!emComentario && c == '{') {
            emComentario = 1;
            linInicioComentario = linha;
            colInicioComentario = coluna;
            continue;
        }
        
        if (emComentario) {
            if (c == '}') {
                emComentario = 0;
            }
            continue; // Ignora tudo dentro do comentário
        }

        //  Detecção de Caracteres Inválidos (Ex: @, $, %)

        if (c == '@' || c == '$' || c == '%' || c == '`') {
            char detalhe[50];
            sprintf(detalhe, "Simbolo '%c' nao reconhecido", c);
            registrarErro("CARACTER_INVALIDO", linha, coluna, detalhe);
            continue;
        }

        //  Se não for espaço, dígito ou letra (e não for operador válido), é erro
        
        if (!isspace(c) && !isalnum(c) && strchr("+-*/=();,<>", c) == NULL) {
            char detalhe[50];
            sprintf(detalhe, "Caractere '%c' fora do padrao", c);
            registrarErro("ERRO_DESCONHECIDO", linha, coluna, detalhe);
        }
    }

    //  Verificação de Comentário não fechado

    if (emComentario) {
        registrarErro("COMENTARIO_NAO_FECHADO", linInicioComentario, colInicioComentario, 
                      "Fim de arquivo atingido sem fechar o comentario '}'");
    }

    fclose(input);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.pas>\n", argv[0]);
        return 1;
    }

    // Lógica para nomes de arquivo
    char nomeBase[256];
    strcpy(nomeBase, argv[1]);
    char *p = strrchr(nomeBase, '.');
    if (p) *p = '\0';

    char arquivoErr[265];
    sprintf(arquivoErr, "%s.err", nomeBase);

    // Inicia a análise
    analisarArquivo(argv[1]);

    // Salva o relatório final
    salvarErros(arquivoErr);

    if (obterTotalErros() > 0) {
        printf("\nAnalise concluida com %d erros. Verifique o arquivo %s\n", 
                obterTotalErros(), arquivoErr);
    } else {
        printf("\nAnalise concluida com sucesso! Nenhum erro encontrado.\n");
    }

    return 0;
}