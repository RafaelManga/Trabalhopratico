#ifndef ERROS_H
#define ERROS_H

/* Registra um erro léxico encontrado durante a análise */
void registrarErro(const char *tipo, int linha, int coluna, const char *detalhe);

/* Salva todos os erros registrados no arquivo .err */
void salvarErros(const char *nomeArquivo);

#endif
