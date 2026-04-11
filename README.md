# Analisador Léxico — MicroPascal

## Integrantes

- Matheus Pazetto  
- Rafael Mangabeira Souza  
- Paulo Miguel  
- Matheus Almeida  
- Marcos Turisco  

**Professor:** Marcelo Eustaquio Soares de Lima Junior  
**Disciplina:** Linguagens Formais, Autômatos e Compiladores  
**Instituição:** Universidade Católica de Brasília — Bacharelado em Ciência da Computação  
**Ano:** 2026  

---

## Descrição

Implementação de um **analisador léxico para a linguagem MicroPascal**, desenvolvido em linguagem C, baseado em um Autômato Finito Determinístico (AFD).

O analisador é responsável por:
- Reconhecer todos os tokens válidos da linguagem (palavras-chave, identificadores, números, operadores e símbolos)
- Detectar erros léxicos (caracteres inválidos e comentários não fechados)
- Manter uma Tabela de Símbolos com palavras reservadas e identificadores
- Gerar três arquivos de saída: `.lex`, `.ts` e `.err`

---

## Estrutura do Projeto

```
Trabalhopratico-master/
├── main.c                  → Controlador principal
├── lexico.c / lexico.h     → Motor do analisador léxico (AFD)
├── tabela.c / tabela.h     → Tabela de Símbolos
├── erros.c / erros.h       → Sistema de registro de erros
├── token.h                 → Definição dos tipos de token
├── Makefile                → Script de compilação
├── afd_micropascal.dot     → Diagrama do AFD (Graphviz)
├── diagrama_afd.png        → Imagem gerada do AFD
├── palavraChave/           → Módulo auxiliar de palavras-chave
├── numeros/                → Módulo auxiliar de números
├── operacoes/              → Módulo auxiliar de operadores
├── simbolos/               → Módulo auxiliar de símbolos
├── ok1.pas                 → Teste correto 1: if/else e operadores aritméticos
├── ok2.pas                 → Teste correto 2: while e números reais
├── ok3.pas                 → Teste correto 3: case-insensitive e operadores relacionais
├── erro1.pas               → Teste de erro 1: caractere inválido @
├── erro2.pas               → Teste de erro 2: múltiplos caracteres inválidos $ e %
└── erro3.pas               → Teste de erro 3: comentário não fechado
```

---

## Como Compilar

### Linux / macOS

Requisito: `gcc` e `make` instalados.

```bash
make
```

Para limpar os arquivos compilados:

```bash
make clean
```

### Windows

```bash
mingw32-make
```

No Windows com MinGW, o executável gerado será `analisador.exe`.

---

## Como Executar

### Linux / macOS

```bash
./analisador <arquivo.pas>
```

Exemplo:

```bash
./analisador ok1.pas
```

### Windows

```bash
analisador.exe <arquivo.pas>
```

Exemplo:

```bash
analisador.exe ok1.pas
```

---

## Arquivos de Saída

Para cada arquivo `.pas` analisado, o programa gera automaticamente três arquivos com o mesmo nome base:

| Arquivo | Conteúdo |
|---|---|
| `.lex` | Lista de todos os tokens reconhecidos, com linha e coluna |
| `.ts`  | Tabela de Símbolos completa (palavras reservadas + identificadores) |
| `.err` | Lista de erros léxicos encontrados, com tipo, linha, coluna e detalhe |

Formato do `.lex`:
```
<NOME_TOKEN, lexema> linha coluna
```

Exemplo:
```
<KW_PROGRAM, program> 1 1
<ID, calculadora> 1 10
<SMB_SEM, ;> 1 22
```

---

## Testes

### Executar todos os testes de uma vez

**Linux / macOS:**
```bash
echo "===== TESTES SEM ERRO ====="
./analisador ok1.pas
./analisador ok2.pas
./analisador ok3.pas

echo ""
echo "===== TESTES COM ERRO ====="
./analisador erro1.pas
./analisador erro2.pas
./analisador erro3.pas
```

**Windows:**
```bash
echo ===== TESTES SEM ERRO =====
analisador.exe ok1.pas
analisador.exe ok2.pas
analisador.exe ok3.pas

echo.
echo ===== TESTES COM ERRO =====
analisador.exe erro1.pas
analisador.exe erro2.pas
analisador.exe erro3.pas
```

### Descrição dos testes

| Arquivo | Tipo | O que cobre |
|---|---|---|
| `ok1.pas` | Correto | `if/else`, operadores `+`, `-`, `*`, `/`, `>`, múltiplas variáveis |
| `ok2.pas` | Correto | `while/do`, `begin/end` aninhado, `NUM_REAL`, variável `real` |
| `ok3.pas` | Correto | Case-insensitive (`PROGRAM`, `VAR`, `INTEGER`), operadores `<>`, `>=`, `<=`, `NUM_REAL` |
| `erro1.pas` | Erro | Caractere inválido `@` |
| `erro2.pas` | Erro | Múltiplos caracteres inválidos `$` e `%` no mesmo arquivo |
| `erro3.pas` | Erro | Comentário aberto com `{` sem `}` de fechamento (EOF) |

---

## Gerar o Diagrama do AFD

Com o [Graphviz](https://graphviz.org/download/) instalado:

```bash
dot -Tpng afd_micropascal.dot -o diagrama_afd.png
```

---

## Tokens Reconhecidos

| Categoria | Tokens |
|---|---|
| Palavras-chave | `KW_PROGRAM`, `KW_VAR`, `KW_INTEGER`, `KW_REAL`, `KW_BEGIN`, `KW_END`, `KW_IF`, `KW_THEN`, `KW_ELSE`, `KW_WHILE`, `KW_DO` |
| Identificador | `ID` |
| Números | `NUM_INT`, `NUM_REAL` |
| Operadores relacionais | `OP_EQ`, `OP_NE`, `OP_LT`, `OP_LE`, `OP_GT`, `OP_GE` |
| Operadores aritméticos | `OP_AD`, `OP_MIN`, `OP_MUL`, `OP_DIV` |
| Atribuição | `OP_ASS` |
| Símbolos | `SMB_SEM`, `SMB_COM`, `SMB_OPA`, `SMB_CPA`, `SMB_COL`, `SMB_DOT` |
