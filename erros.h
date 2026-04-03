#ifndef ERROS_H
#define ERROS_H

<<<<<<< HEAD
/* Registra um erro léxico com tipo, linha, coluna e detalhe */
=======
/* Registra um erro léxico encontrado durante a análise */
>>>>>>> 512399c2bafa95c9a69a997a171c02675ec43a0e
void registrarErro(const char *tipo, int linha, int coluna, const char *detalhe);

/* Salva todos os erros registrados no arquivo .err */
void salvarErros(const char *nomeArquivo);

<<<<<<< HEAD
/* Retorna o total de erros encontrados */
int obterTotalErros();

=======
>>>>>>> 512399c2bafa95c9a69a997a171c02675ec43a0e
#endif
