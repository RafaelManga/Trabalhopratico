#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"






// talvez tirar isso com o ana sinaatico, mas por enquanto deixa aqui

#define CAP_INICIAL 4

struct No {
    char rotulo[128];
    char lexema[MAX_LEXEMA];
    int eh_folha;
    No **filhos;
    int num_filhos;
    int cap_filhos;
    int id_dot;
};
static int g_proximo_id = 0;

No *no_criar(const char *rotulo) {
    No *n = calloc(1, sizeof(No));
    if (!n) return NULL;
    strncpy(n->rotulo, rotulo, sizeof(n->rotulo) - 1);
    n->eh_folha = 0;
    n->cap_filhos = CAP_INICIAL;
    n->filhos = malloc((size_t)n->cap_filhos * sizeof(No *));
    return n;
}
No *no_folha_token(const Token *tok) {
    No *n = calloc(1, sizeof(No));
    if (!n) return NULL;
    snprintf(n->rotulo, sizeof(n->rotulo), "%s", nomeToken(tok->tipo));
    strncpy(n->lexema, tok->lexema, sizeof(n->lexema) - 1);
    n->eh_folha = 1;
    return n;
}

void no_adicionar_filho(No *pai, No *filho) {
    if (!pai || !filho) return;
    if (pai->num_filhos >= pai->cap_filhos) {
        pai->cap_filhos *= 2;
        pai->filhos = realloc(pai->filhos, (size_t)pai->cap_filhos * sizeof(No *));
    }
    pai->filhos[pai->num_filhos++] = filho;
}

static void imprimir_rec(const No *no, const char *prefixo, int ultimo) {
    if (!no) return;

    printf("%s%s", prefixo, ultimo ? "+-- " : "|-- ");

    if (no->eh_folha) {
        printf("%s \"%s\"\n", no->rotulo, no->lexema);
        return;
    }

    printf("%s\n", no->rotulo);

    char proximo[256];
  size_t len = strlen(prefixo);
    if (len + 4 >= sizeof(proximo)) len = sizeof(proximo) - 5;
    memcpy(proximo, prefixo, len);
    proximo[len] = '\0';
    strcat(proximo, ultimo ? "    " : "|   ");

    for (int i = 0; i < no->num_filhos; i++) {
        imprimir_rec(no->filhos[i], proximo, i == no->num_filhos - 1);
    }
}

void ast_imprimir_texto(const No *raiz, int profundidade) {
    (void)profundidade;
    if (!raiz) return;
    printf("\nArvore sintatica (formato em arvore):\n");
    imprimir_rec(raiz, "", 1);
}

static void dot_atribuir_ids(No *no) {
    if (!no) return;
    no->id_dot = g_proximo_id++;
    for (int i = 0; i < no->num_filhos; i++) {
        dot_atribuir_ids(no->filhos[i]);
    }
}

static void dot_escapar_label(const char *texto, char *saida, size_t tam) {
    size_t j = 0;
    for (size_t i = 0; texto[i] != '\0' && j + 2 < tam; i++) {
        if (texto[i] == '"' || texto[i] == '\\') {
            if (j + 1 < tam) saida[j++] = '\\';
        }
        saida[j++] = texto[i];
    }
    saida[j] = '\0';
}

static void dot_emitir_no(FILE *f, const No *no) {
    char label[256];
    char esc[512];

    if (no->eh_folha) {
        snprintf(label, sizeof(label), "%s\\n%s", no->rotulo, no->lexema);
    } else {
        snprintf(label, sizeof(label), "%s", no->rotulo);
    }
    dot_escapar_label(label, esc, sizeof(esc));
    if (no->eh_folha) {
        fprintf(f, "  n%d [label=\"%s\", shape=ellipse, style=filled, fillcolor=\"#FFF3B0\"];\n",
                no->id_dot, esc);
    } else {
        fprintf(f, "  n%d [label=\"%s\", style=filled, fillcolor=\"#CFE8FF\"];\n",
                no->id_dot, esc);
    }
}

static void dot_emitir_arvore(FILE *f, const No *no) {
    if (!no) return;
    dot_emitir_no(f, no);
    for (int i = 0; i < no->num_filhos; i++) {
        fprintf(f, "  n%d -> n%d;\n", no->id_dot, no->filhos[i]->id_dot);
        dot_emitir_arvore(f, no->filhos[i]);
    }
}

void ast_exportar_dot(const No *raiz, const char *arquivoDot) {
    if (!raiz || !arquivoDot) return;

    g_proximo_id = 0;
    dot_atribuir_ids((No *)raiz);

    FILE *f = fopen(arquivoDot, "w");
    if (!f) {
        fprintf(stderr, "Erro ao criar %s\n", arquivoDot);
        return;
    }

    fprintf(f, "digraph AST_MicroPascal {\n");
    fprintf(f, "  rankdir=TB;\n");
    fprintf(f, "  node [fontname=\"DejaVu Sans\", fontsize=10];\n");
    dot_emitir_arvore(f, raiz);
    fprintf(f, "}\n");
    fclose(f);
    printf("Arvore exportada para: %s\n", arquivoDot);
}

int ast_gerar_imagem(const char *arquivoDot, const char *arquivoPng, const char *arquivoPdf) {
    char cmd[1024];
    int ok = 0;

    snprintf(cmd, sizeof(cmd), "dot -Tpng \"%s\" -o \"%s\"", arquivoDot, arquivoPng);
    if (system(cmd) == 0) {
        printf("Imagem gerada: %s\n", arquivoPng);
        ok = 1;
    } else {
        fprintf(stderr, "Aviso: nao foi possivel gerar PNG (instale graphviz: dot).\n");
    }

    snprintf(cmd, sizeof(cmd), "dot -Tpdf \"%s\" -o \"%s\"", arquivoDot, arquivoPdf);
    if (system(cmd) == 0) {
        printf("PDF gerado: %s\n", arquivoPdf);
        ok = 1;
    }

    return ok;
}

void no_liberar(No *no) {
    if (!no) return;
    for (int i = 0; i < no->num_filhos; i++) {
        no_liberar(no->filhos[i]);
    }
    free(no->filhos);
    free(no);
}
