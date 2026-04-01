===========================================
  ANALISADOR LÉXICO - MicroPascal
  Linguagens Formais, Autômatos e Compiladores
  UCB - 2026
===========================================

-- O QUE É ISSO --

Este programa lê um arquivo de código escrito em MicroPascal (.pas)
e gera três arquivos de saída com os resultados da análise léxica.


-- O QUE VOCÊ PRECISA TER INSTALADO --

- GCC (compilador C)
  Windows: baixe o mingw caso nao tenha, e adicione a pasta /bin no PATH
  Linux:   sudo apt install gcc
  Mac:     xcode-select --install




-- COMO COMPILAR --

rode esse comando quando ja tiver todos os codigos

  gcc -Wall -g main.c tabela.c lexico.c erros.c -o analisador

No Windows o executável gerado será analisador.exe


-- COMO EXECUTAR --

  ./analisador teste.pas          (Linux/Mac)
  analisador.exe teste.pas        (Windows)


-- ARQUIVOS DE SAÍDA --

  .lex  -> lista de todos os tokens reconhecidos
           formato: <NOME_TOKEN, lexema> linha coluna
           exemplo: <KW_PROGRAM, program> 1 1

  .ts   -> tabela de símbolos completa ao final da análise
           contém todas as palavras reservadas e identificadores encontrados

  .err  -> lista de erros léxicos encontrados
           contém tipo do erro, linha e coluna de cada ocorrência
           (arquivo vazio significa que não houve erros)


-- EXTENSÕES DE ARQUIVO --

  .pas  -> arquivo de código fonte MicroPascal (entrada)
  .lex  -> tokens reconhecidos (saída)
  .ts   -> tabela de símbolos (saída)
  .err  -> erros léxicos (saída)
  .c    -> código fonte em C
  .h    -> cabeçalho C (define structs e funções usadas entre arquivos)
  .o    -> arquivo objeto gerado na compilação (pode apagar depois)


-- ESTRUTURA DO PROJETO --

  token.h    -> define o que é um Token e os tipos existentes
  tabela.h   -> interface da Tabela de Símbolos
  tabela.c   -> implementação da Tabela de Símbolos
  main.c     -> ponto de entrada, lê o arquivo e gera as saídas
  lexico.c   -> reconhece os tokens (pessoa 2 e 3)
  erros.c    -> registra e salva os erros léxicos (pessoa 4)
