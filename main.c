#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "token.h"
#include "tabela.h"
#include "lexico.h"
#include "erros.h"
#include "parser.h"
#include "config.h"

/*
 * Pega o nome base do arquivo (sem extensão e sem caminho)
 * e monta o caminho de saída dentro da pasta saidas/.
 * ex: ok1.pas -> saidas/ok1.lex
 */
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

    /* Cria a pasta saidas/ se não existir */
    mkdir("saidas", 0755);

    /* Abre o arquivo fonte passado como argumento */
    FILE *fonte = fopen(argv[1], "r");
    if (!fonte) {
        fprintf(stderr, "Erro ao abrir '%s'\n", argv[1]);
        return 1;
    }

    /* Monta os nomes dos arquivos de saída dentro de saidas/ */
    char arquivoLex[512], arquivoTs[512], arquivoErr[512];
    char arquivoSint[512], arquivoDot[512], arquivoPng[512], arquivoPdf[512];
    montarNome(argv[1], "lex",  arquivoLex);
    montarNome(argv[1], "ts",   arquivoTs);
    montarNome(argv[1], "err",  arquivoErr);
    montarNome(argv[1], "sint", arquivoSint);
    montarNome(argv[1], "dot",  arquivoDot);
    montarNome(argv[1], "png",  arquivoPng);
    montarNome(argv[1], "pdf",  arquivoPdf);

    /* Inicializa a tabela de símbolos com as palavras reservadas */
    TabelaSimbolos ts;
    ts_inicializar(&ts);

    /* Cria o arquivo .lex onde os tokens reconhecidos serão gravados */
    FILE *fLex = fopen(arquivoLex, "w");
    if (!fLex) { fclose(fonte); return 1; }

    printf("%s | %s | v%s\n", PROJETO_NOME, PROJETO_TURMA, PROJETO_VERSAO);
    printf("Arquivo de entrada: %s\n", argv[1]);
    printf("[Fase lexica] iniciando...\n");

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

    /* Salva os arquivos finais */
    ts_salvar(&ts, arquivoTs);
    salvarErros(arquivoErr);

    printf("\nArquivos gerados em saidas/:\n  %s\n  %s\n  %s\n", arquivoLex, arquivoTs, arquivoErr);
    if (obterTotalErros() == 0) {
        printf("  %s\n  %s\n  %s\n  %s\n", arquivoSint, arquivoDot, arquivoPng, arquivoPdf);
    }
    ts_imprimir(&ts);
    return 0;
}
