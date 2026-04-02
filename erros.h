#ifndef ERROS_H
#define ERROS_H

/* Registra um erro léxico com detalhes, linha e coluna */
void registrarErro(const char *tipo, int linha, int coluna, const char *detalhe);

/* Salva a lista de erros no arquivo especificado */
void salvarErros(const char *nomeArquivo);

/* Retorna o total de erros encontrados até o momento */
int obterTotalErros();

#endif