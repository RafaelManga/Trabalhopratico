#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura do Token conforme especificado 
typedef struct {
    char nome[20];
    char lexema[100];
    int linha;
    int coluna;
} Token;

// --- MOCK DA TABELA DE SÍMBOLOS (Feita por outro integrante) ---
// Estas funções devem ser substituídas pelas reais da TS do nosso grupo.
int ts_is_keyword(const char* lexema) {
    // Array simulando as palavras-chave já armazenadas [cite: 66]
    const char* keywords[] = {"program", "var", "integer", "real", "begin", "end", "if", "then", "else", "while", "do"};
    int num_keywords = 11;
    for(int i = 0; i < num_keywords; i++) {
        if(strcmp(lexema, keywords[i]) == 0) return 1; // É palavra reservada
    }
    return 0; // É identificador normal
}

void inserir_ts(const char* lexema) {
    // Lógica de quem fez a outra parte para inserir o ID na TS (não duplicar )
}
// ---------------------------------------------------------------

// Função auxiliar para normalizar o lexema (Case-Insensitive) 
void normalizar_lexema(char* lexema) {
    for (int i = 0; lexema[i]; i++) {
        lexema[i] = tolower(lexema[i]); // Tudo em minúsculo
    }
}

// Variáveis globais para controle de posição no arquivo
int linha_atual = 1;
int coluna_atual = 1;

// Função principal de responsabilidade da PARTE 2: Reconhecer IDs e Palavras-chave
Token reconhecer_id_ou_palavra_chave(FILE* arquivo, char caractere_inicial) {
    Token t;
    t.linha = linha_atual;
    t.coluna = coluna_atual;
    
    int i = 0;
    t.lexema[i++] = caractere_inicial;
    
    char c = fgetc(arquivo);
    coluna_atual++;
    
    // O AFD repete enquanto for letra ou dígito 
    while (isalnum(c)) {
        if (i < 99) { // Evita overflow do buffer
            t.lexema[i++] = c;
        }
        c = fgetc(arquivo);
        coluna_atual++;
    }
    t.lexema[i] = '\0';
    
    // O caractere extra lido precisa ser devolvido ao buffer do arquivo
    if (c != EOF) {
        ungetc(c, arquivo);
        coluna_atual--; // Ajusta a coluna, pois "devolvemos" o char
    }
    
    // Normalização Case-Insensitive [cite: 30]
    normalizar_lexema(t.lexema);
    
    // Consulta a Tabela de Símbolos para diferenciar ID de Palavra-chave
    if (ts_is_keyword(t.lexema)) {
        // Se a TS acusa que é palavra-chave, atribuímos um nome genérico ou específico
        // O trabalho não obriga um nome padrão único, usaremos KW_<LEXEMA_MAIUSCULO>
        sprintf(t.nome, "KW_%s", t.lexema);
        // Exemplo: se lexema é "program", o token fica "KW_PROGRAM" [cite: 106]
        for(int j=3; t.nome[j]; j++) t.nome[j] = toupper(t.nome[j]);
    } else {
        strcpy(t.nome, "ID");
        // Somente IDs e palavras reservadas vão para a TS 
        // Se não é keyword, é ID. Pedimos para a TS armazenar (a TS deve barrar duplicatas )
        inserir_ts(t.lexema); 
    }
    
    return t;
}

// Exemplo da casca da função principal do analisador léxico
Token obter_token(FILE* arquivo) {
    Token t;
    char c = fgetc(arquivo);
    coluna_atual++;
    
    // Pula espaços em branco, tabs e quebras de linha (não são tokens [cite: 22])
    while (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
        if (c == '\n') {
            linha_atual++;
            coluna_atual = 1;
        }
        c = fgetc(arquivo);
        if (c != '\n') coluna_atual++;
    }
    
    if (c == EOF) {
        strcpy(t.nome, "EOF");
        strcpy(t.lexema, "");
        return t;
    }
    
    // --- INTEGRAÇÃO DA PARTE 2: Se começar com Letra ---
    if (isalpha(c)) {
        return reconhecer_id_ou_palavra_chave(arquivo, c);
    }
    
    // (O restante do código para operadores, números, chaves e outros símbolos 
    // será feito pelos demais colegas usando lógicas parecidas).
    
    // Fallback para caracteres não reconhecidos (Tratamento de erro pelo outro colega)
    strcpy(t.nome, "ERRO");
    t.lexema[0] = c;
    t.lexema[1] = '\0';
    t.linha = linha_atual;  
    t.coluna = coluna_atual;
    return t;
}

int main() {
    FILE* f = fopen("teste.pas", "r");
    if(!f) { printf("Erro ao abrir arquivo.\n"); return 1; }

    Token tk = obter_token(f);
    while(strcmp(tk.nome, "EOF") != 0) {
        printf("<%s, %s> %d %d\n", tk.nome, tk.lexema, tk.linha, tk.coluna); // Padrão de saída [cite: 104]
        tk = obter_token(f);
    }
    fclose(f);
    return 0;
}