#ifndef ERROS_H
#define ERROS_H

void registrarErro(const char *tipo, int linha, int coluna, const char *detalhe);

void salvarErros(const char *nomeArquivo);


int obterTotalErros();

#endif
