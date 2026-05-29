#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexico.h"
#include "token.h"
#include "ast.h"
#include "config.h"

static FILE *arqFonte = NULL;
static TabelaSimbolos *tabelaAtual = NULL;
static FILE *arqProducoes = NULL;
static Token tokenCorrente;
static int contadorProducao = 0;

static void avancar(void);
static void erroSintatico(TipoToken esperado);
static No *CasaToken(TipoToken esperado);
static No *consumirTokenAtual(void);
static int ehRelacao(TipoToken tipo);
static void imprimirProducao(const char *producao);

static No *programa(void);
static No *bloco(void);
static No *parte_declaracoes_de_variaveis(void);
static No *declaracao_de_variaveis(void);
static No *lista_de_identificadores(void);
static No *tipo(void);
static No *comando_composto(void);
static No *comando(void);
static No *atribuicao(void);
static No *comando_condicional(void);
static No *comando_repetitivo(void);
static No *expressao(void);
static No *expressao_simples(void);
static No *termo(void);
static No *fator(void);
static No *variavel(void);

static void montarNomeSaida(const char *entrada, const char *ext, char *saida) {
    const char *barra = strrchr(entrada, '/');
    const char *nomeArquivo = barra ? barra + 1 : entrada;

    char base[512];
    strncpy(base, nomeArquivo, sizeof(base) - 1);
    base[sizeof(base) - 1] = '\0';

    char *ponto = strrchr(base, '.');
    if (ponto) *ponto = '\0';

    snprintf(saida, 512, "saidas/%s.%s", base, ext);
}

static void avancar(void) {
    tokenCorrente = proximoToken(arqFonte, tabelaAtual);
}

static void erroSintatico(TipoToken esperado) {
    (void)esperado;
    if (tokenCorrente.tipo == TOKEN_EOF) {
        fprintf(stderr, "%d:fim de arquivo não esperado.\n", tokenCorrente.linha);
    } else {
        fprintf(stderr, "%d:token nao esperado [%s].\n", tokenCorrente.linha, tokenCorrente.lexema);
    }
    exit(1);
}

static void imprimirProducao(const char *producao) {
    contadorProducao++;
    printf("P%03d @L%-3d | %s\n", contadorProducao, tokenCorrente.linha, producao);
    if (arqProducoes) {
        fprintf(arqProducoes, "P%03d @L%-3d | %s\n", contadorProducao, tokenCorrente.linha, producao);
    }
}

static No *CasaToken(TipoToken esperado) {
    if (tokenCorrente.tipo == esperado) {
        No *folha = no_folha_token(&tokenCorrente);
        avancar();
        return folha;
    }
    erroSintatico(esperado);
    return NULL;
}

static No *consumirTokenAtual(void) {
    No *folha = no_folha_token(&tokenCorrente);
    avancar();
    return folha;
}

static int ehRelacao(TipoToken tipo) {
    return tipo == OP_EQ || tipo == OP_NE || tipo == OP_LT ||
           tipo == OP_LE || tipo == OP_GE || tipo == OP_GT;
}

static No *programa(void) {
    imprimirProducao("programa ::= program identificador ; bloco .");
    No *n = no_criar("programa");
    no_adicionar_filho(n, CasaToken(KW_PROGRAM));
    no_adicionar_filho(n, CasaToken(ID));
    no_adicionar_filho(n, CasaToken(SMB_SEM));
    no_adicionar_filho(n, bloco());
    no_adicionar_filho(n, CasaToken(SMB_DOT));
    return n;
}

static No *bloco(void) {
    imprimirProducao("bloco ::= parte_declaracoes_de_variaveis comando_composto");
    No *n = no_criar("bloco");
    No *parte = parte_declaracoes_de_variaveis();
    if (parte) {
        no_adicionar_filho(n, parte);
    }
    no_adicionar_filho(n, comando_composto());
    return n;
}

static No *parte_declaracoes_de_variaveis(void) {
    if (tokenCorrente.tipo != KW_VAR) {
        return NULL;
    }

    imprimirProducao("parte_declaracoes_de_variaveis ::= var declaracao_de_variaveis { ; declaracao_de_variaveis } ;");
    No *n = no_criar("parte_declaracoes_de_variaveis");
    no_adicionar_filho(n, CasaToken(KW_VAR));
    no_adicionar_filho(n, declaracao_de_variaveis());
    no_adicionar_filho(n, CasaToken(SMB_SEM));
    while (tokenCorrente.tipo == ID) {
        no_adicionar_filho(n, declaracao_de_variaveis());
        no_adicionar_filho(n, CasaToken(SMB_SEM));
    }
    return n;
}

static No *declaracao_de_variaveis(void) {
    imprimirProducao("declaracao_de_variaveis ::= lista_de_identificadores : tipo");
    No *n = no_criar("declaracao_de_variaveis");
    no_adicionar_filho(n, lista_de_identificadores());
    no_adicionar_filho(n, CasaToken(SMB_COL));
    no_adicionar_filho(n, tipo());
    return n;
}

static No *lista_de_identificadores(void) {
    imprimirProducao("lista_de_identificadores ::= identificador { , identificador }");
    No *n = no_criar("lista_de_identificadores");
    no_adicionar_filho(n, CasaToken(ID));
    while (tokenCorrente.tipo == SMB_COM) {
        no_adicionar_filho(n, CasaToken(SMB_COM));
        no_adicionar_filho(n, CasaToken(ID));
    }
    return n;
}

static No *tipo(void) {
    No *n = no_criar("tipo");
    if (tokenCorrente.tipo == KW_INTEGER) {
        imprimirProducao("tipo ::= integer");
        no_adicionar_filho(n, CasaToken(KW_INTEGER));
    } else if (tokenCorrente.tipo == KW_REAL) {
        imprimirProducao("tipo ::= real");
        no_adicionar_filho(n, CasaToken(KW_REAL));
    } else {
        erroSintatico(KW_INTEGER);
    }
    return n;
}

static No *comando_composto(void) {
    imprimirProducao("comando_composto ::= begin comando { ; comando } end");
    No *n = no_criar("comando_composto");
    no_adicionar_filho(n, CasaToken(KW_BEGIN));
    no_adicionar_filho(n, comando());
    while (tokenCorrente.tipo == SMB_SEM) {
        no_adicionar_filho(n, CasaToken(SMB_SEM));
        if (tokenCorrente.tipo == KW_END) {
            break;
        }
        no_adicionar_filho(n, comando());
    }
    no_adicionar_filho(n, CasaToken(KW_END));
    return n;
}

static No *comando(void) {
    No *n = no_criar("comando");
    if (tokenCorrente.tipo == ID) {
        imprimirProducao("comando ::= atribuicao");
        no_adicionar_filho(n, atribuicao());
    } else if (tokenCorrente.tipo == KW_BEGIN) {
        imprimirProducao("comando ::= comando_composto");
        no_adicionar_filho(n, comando_composto());
    } else if (tokenCorrente.tipo == KW_IF) {
        imprimirProducao("comando ::= comando_condicional");
        no_adicionar_filho(n, comando_condicional());
    } else if (tokenCorrente.tipo == KW_WHILE) {
        imprimirProducao("comando ::= comando_repetitivo");
        no_adicionar_filho(n, comando_repetitivo());
    } else {
        erroSintatico(ID);
    }
    return n;
}

static No *atribuicao(void) {
    imprimirProducao("atribuicao ::= variavel := expressao");
    No *n = no_criar("atribuicao");
    no_adicionar_filho(n, variavel());
    no_adicionar_filho(n, CasaToken(OP_ASS));
    no_adicionar_filho(n, expressao());
    return n;
}

static No *comando_condicional(void) {
    imprimirProducao("comando_condicional ::= if expressao then comando [ else comando ]");
    No *n = no_criar("comando_condicional");
    no_adicionar_filho(n, CasaToken(KW_IF));
    no_adicionar_filho(n, expressao());
    no_adicionar_filho(n, CasaToken(KW_THEN));
    no_adicionar_filho(n, comando());
    if (tokenCorrente.tipo == KW_ELSE) {
        no_adicionar_filho(n, CasaToken(KW_ELSE));
        no_adicionar_filho(n, comando());
    }
    return n;
}

static No *comando_repetitivo(void) {
    imprimirProducao("comando_repetitivo ::= while expressao do comando");
    No *n = no_criar("comando_repetitivo");
    no_adicionar_filho(n, CasaToken(KW_WHILE));
    no_adicionar_filho(n, expressao());
    no_adicionar_filho(n, CasaToken(KW_DO));
    no_adicionar_filho(n, comando());
    return n;
}

static No *expressao(void) {
    imprimirProducao("expressao ::= expressao_simples [ relacao expressao_simples ]");
    No *n = no_criar("expressao");
    no_adicionar_filho(n, expressao_simples());
    if (ehRelacao(tokenCorrente.tipo)) {
        No *rel = no_criar("relacao");
        no_adicionar_filho(rel, consumirTokenAtual());
        no_adicionar_filho(n, rel);
        no_adicionar_filho(n, expressao_simples());
    }
    return n;
}

static No *expressao_simples(void) {
    imprimirProducao("expressao_simples ::= [ + | - ] termo { ( + | - ) termo }");
    No *n = no_criar("expressao_simples");
    if (tokenCorrente.tipo == OP_AD || tokenCorrente.tipo == OP_MIN) {
        no_adicionar_filho(n, consumirTokenAtual());
    }
    no_adicionar_filho(n, termo());
    while (tokenCorrente.tipo == OP_AD || tokenCorrente.tipo == OP_MIN) {
        no_adicionar_filho(n, consumirTokenAtual());
        no_adicionar_filho(n, termo());
    }
    return n;
}

static No *termo(void) {
    imprimirProducao("termo ::= fator { ( * | / ) fator }");
    No *n = no_criar("termo");
    no_adicionar_filho(n, fator());
    while (tokenCorrente.tipo == OP_MUL || tokenCorrente.tipo == OP_DIV) {
        no_adicionar_filho(n, consumirTokenAtual());
        no_adicionar_filho(n, fator());
    }
    return n;
}

static No *fator(void) {
    No *n = no_criar("fator");
    if (tokenCorrente.tipo == ID) {
        imprimirProducao("fator ::= variavel");
        no_adicionar_filho(n, variavel());
    } else if (tokenCorrente.tipo == NUM_INT || tokenCorrente.tipo == NUM_REAL) {
        imprimirProducao("fator ::= numero");
        no_adicionar_filho(n, consumirTokenAtual());
    } else if (tokenCorrente.tipo == SMB_OPA) {
        imprimirProducao("fator ::= ( expressao )");
        no_adicionar_filho(n, CasaToken(SMB_OPA));
        no_adicionar_filho(n, expressao());
        no_adicionar_filho(n, CasaToken(SMB_CPA));
    } else {
        erroSintatico(ID);
    }
    return n;
}

static No *variavel(void) {
    imprimirProducao("variavel ::= identificador");
    No *n = no_criar("variavel");
    no_adicionar_filho(n, CasaToken(ID));
    return n;
}

void analisarSintatico(FILE *fonte, TabelaSimbolos *ts, const char *arquivoEntrada) {
    char arquivoSint[512];
    char arquivoDot[512];
    char arquivoPng[512];
    char arquivoPdf[512];

    montarNomeSaida(arquivoEntrada, "sint", arquivoSint);
    montarNomeSaida(arquivoEntrada, "dot", arquivoDot);
    montarNomeSaida(arquivoEntrada, "png", arquivoPng);
    montarNomeSaida(arquivoEntrada, "pdf", arquivoPdf);

    arqFonte = fonte;
    tabelaAtual = ts;
    arqProducoes = fopen(arquivoSint, "w");
    contadorProducao = 0;
    lexico_resetar_linha(); /* garante que a linha começa em 1 na fase sintatica */
    avancar();

    printf("\n[Fase sintatica] %s | %s\n", PROJETO_NOME, PROJETO_TURMA);
    if (arqProducoes) {
        fprintf(arqProducoes, "%s | %s | %s\n", PROJETO_NOME, PROJETO_TURMA, PROJETO_VERSAO);
        fprintf(arqProducoes, "Arquivo: %s\n", arquivoEntrada);
        fprintf(arqProducoes, "Formato: P### @L### | producao\n\n");
    }

    No *raiz = programa();

    if (tokenCorrente.tipo != TOKEN_EOF) {
        erroSintatico(TOKEN_EOF);
    }

    printf("[OK] Sintaxe valida (%d producoes aplicadas).\n", contadorProducao);
    ast_imprimir_texto(raiz, 0);
    ast_exportar_dot(raiz, arquivoDot);
    ast_gerar_imagem(arquivoDot, arquivoPng, arquivoPdf);

    if (arqProducoes) {
        fprintf(arqProducoes, "\nTotal de producoes: %d\n", contadorProducao);
        fclose(arqProducoes);
        printf("Log de producoes: %s\n", arquivoSint);
    }

    no_liberar(raiz);
}
