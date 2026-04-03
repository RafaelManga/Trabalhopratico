#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "tabela.h"
#include "lexico.h"
#include "erros.h"

/*
 * Pega o nome do arquivo de entrada (ex: teste.pas)
 * e gera o nome do arquivo de saída trocando a extensão.
 * ex: teste.pas -> teste.lex / teste.ts / teste.err
 */
static void montarNome(const char *entrada, const char *ext, char *saida) {
    char base[512];
    strncpy(base, entrada, sizeof(base) - 1);

    char *ponto = strrchr(base, '.');
    if (ponto) *ponto = '\0';

    snprintf(saida, 512, "%s.%s", base, ext);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.pas>\n", argv[0]);
        return 1;
    }

    /* Abre o arquivo fonte passado como argumento */
    FILE *fonte = fopen(argv[1], "r");
    if (!fonte) {
        fprintf(stderr, "Erro ao abrir '%s'\n", argv[1]);
        return 1;
    }

    /* Monta os nomes dos três arquivos de saída */
    char arquivoLex[512], arquivoTs[512], arquivoErr[512];
    montarNome(argv[1], "lex", arquivoLex);
    montarNome(argv[1], "ts",  arquivoTs);
    montarNome(argv[1], "err", arquivoErr);

    /* Inicializa a tabela de símbolos com as palavras reservadas */
    TabelaSimbolos ts;
    ts_inicializar(&ts);

    /* Cria o arquivo .lex onde os tokens reconhecidos serão gravados */
    FILE *fLex = fopen(arquivoLex, "w");
    if (!fLex) { fclose(fonte); return 1; }

    printf("Analisando: %s\n", argv[1]);

    /*
     * Loop principal: fica pedindo o próximo token até chegar no fim do arquivo.
     * Cada token válido é gravado no .lex no formato:
     * <NOME_TOKEN, lexema> linha coluna
     */
    Token tok;
    do {
        tok = proximoToken(fonte, &ts);

        if (tok.tipo == TOKEN_ERRO) continue; /* erro já registrado no lexico.c */

        if (tok.tipo == TOKEN_EOF) break;

        fprintf(fLex, "<%s, %s> %d %d\n",
            nomeToken(tok.tipo), tok.lexema, tok.linha, tok.coluna);

    } while (tok.tipo != TOKEN_EOF);

    fclose(fonte);
    fclose(fLex);

    /* Salva os arquivos finais */
    ts_salvar(&ts, arquivoTs);
    salvarErros(arquivoErr);

    printf("Saida:\n  %s\n  %s\n  %s\n", arquivoLex, arquivoTs, arquivoErr);
    ts_imprimir(&ts);
    return 0;
}
