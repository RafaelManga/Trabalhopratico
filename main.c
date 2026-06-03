#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "token.h"
#include "tabela.h"
#include "lexico.h"
#include "erros.h"
#include "parser.h"


static void montarNome(const char *entrada, const char *ext, char *saida) {
    /* Pega só o nome do arquivo, sem o caminho */
    const char *barra = strrchr(entrada, '/');
    const char *nomeArquivo = barra ? barra + 1 : entrada;

    char base[512];
    strncpy(base, nomeArquivo, sizeof(base) - 1);
    base[sizeof(base) - 1] = '\0';

    char *ponto = strrchr(base, '.');
    if (ponto) *ponto = '\0';

    snprintf(saida, 512, "saidas/%s.%s", base, ext);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.pas>\n", argv[0]);
        return 1;
    }

    mkdir("saidas", 0755);

    FILE *fonte = fopen(argv[1], "r");
    if (!fonte) {
        fprintf(stderr, "Erro ao abrir '%s'\n", argv[1]);
        return 1;
    }

    char arquivoLex[512], arquivoTs[512], arquivoErr[512];
    char arquivoSint[512], arquivoDot[512], arquivoPng[512], arquivoPdf[512];
    montarNome(argv[1], "lex",  arquivoLex);
    montarNome(argv[1], "ts",   arquivoTs);
    montarNome(argv[1], "err",  arquivoErr);
    montarNome(argv[1], "sint", arquivoSint);
    montarNome(argv[1], "dot",  arquivoDot);
    montarNome(argv[1], "png",  arquivoPng);
    montarNome(argv[1], "pdf",  arquivoPdf);

    TabelaSimbolos ts;
    ts_inicializar(&ts);

    // continua aqui rafael, lembrete para mim mesmo termianr isso
    FILE *fLex = fopen(arquivoLex, "w");
    if (!fLex) { fclose(fonte); return 1; }

    printf("Arquivo de entrada: %s\n", argv[1]);
    printf("[Fase lexica] iniciando...\n");

   
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

    if (obterTotalErros() == 0) {
        FILE *fonteSintatico = fopen(argv[1], "r");
        if (!fonteSintatico) {
            fprintf(stderr, "Erro ao reabrir '%s' para analise sintatica\n", argv[1]);
            return 1;
        }
        analisarSintatico(fonteSintatico, &ts, argv[1]);
        fclose(fonteSintatico);
    } else {
        printf("[Fase sintatica] ignorada: existem erros lexicos.\n");
    }

    ts_salvar(&ts, arquivoTs);
    salvarErros(arquivoErr);

    printf("\nArquivos gerados em saidas/:\n  %s\n  %s\n  %s\n", arquivoLex, arquivoTs, arquivoErr);
    if (obterTotalErros() == 0) {
        printf("  %s\n  %s\n  %s\n  %s\n", arquivoSint, arquivoDot, arquivoPng, arquivoPdf);
    }
    ts_imprimir(&ts);
    return 0;
}
