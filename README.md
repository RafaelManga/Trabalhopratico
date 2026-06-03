# Analisador Léxico e Sintático — MicroPascal

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

Implementação de um **analisador léxico e sintático para a linguagem MicroPascal**, desenvolvido em linguagem C.

## Requisitos

- `gcc` ou outro compilador C compatível
- `make`
- `dot` / Graphviz (opcional, para gerar imagens da AST em `*.png` e `*.pdf`)

## Etapa Léxica (AFD)

O analisador léxico é responsável por:
- Reconhecer todos os tokens válidos da linguagem (palavras-chave, identificadores, números, operadores e símbolos)
- Detectar erros léxicos (caracteres inválidos e comentários não fechados)
- Manter uma Tabela de Símbolos com palavras reservadas e identificadores
- Gerar três arquivos de saída: `.lex`, `.ts` e `.err`

## Etapa Sintática (Parser)

O analisador sintático:
- Implementa a gramática do MicroPascal com descida recursiva
- Utiliza `CasaToken` para validar tokens esperados vs tokens correntes
- Imprime a sequência de regras de produção e constrói uma árvore sintática (AST)
- Exporta a AST para `*.dot` e tenta gerar `*.png` e `*.pdf` via Graphviz

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

Para salvar toda a saída em arquivo e facilitar capturas de tela:

```bash
./analisador ok1.pas > saidas/ok1.output 2>&1
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

Para cada arquivo `.pas` analisado, o programa gera:

### Saídas Léxicas (sempre)

| Arquivo | Conteúdo |
|---|---|
| `.lex` | Lista de todos os tokens reconhecidos, com linha e coluna |
| `.ts`  | Tabela de Símbolos completa (palavras reservadas + identificadores) |
| `.err` | Lista de erros léxicos encontrados, com tipo, linha, coluna e detalhe |

### Saídas Sintáticas (somente se não houver erro léxico)

| Arquivo | Conteúdo |
|---|---|
| `.sint` | Sequência de regras de produção usadas na análise sintática |
| `.dot`  | Árvore sintática (AST) exportada para Graphviz |
| `.png`  | Imagem da AST (se `dot`/Graphviz estiver instalado) |
| `.pdf`  | PDF da AST (se `dot`/Graphviz estiver instalado) |

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

echo ""
echo "===== TESTES SINTATICOS COM ERRO ====="
./analisador sint_erro1.pas
./analisador sint_erro2.pas
./analisador sint_erro3.pas
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

echo.
echo ===== TESTES SINTATICOS COM ERRO =====
analisador.exe sint_erro1.pas
analisador.exe sint_erro2.pas
analisador.exe sint_erro3.pas
```

### Descrição dos testes

| Arquivo | Tipo | O que cobre |
|---|---|---|
| `ok1.pas` | Correto | `if/else`, atribuição, `+`, `-`, `*`, `/`, `>` |
| `ok2.pas` | Correto | `while/do`, `begin/end` aninhado, `NUM_REAL`, variável `real` |
| `ok3.pas` | Correto | Case-insensitive (`PROGRAM`, `VAR`, `INTEGER`), relacionais `<>`, `>=`, `<=` |
| `erro1.pas` | Erro léxico | Caractere inválido `@` |
| `erro2.pas` | Erro léxico | Múltiplos caracteres inválidos `$` e `%` no mesmo arquivo |
| `erro3.pas` | Erro léxico | Comentário aberto com `{` sem `}` de fechamento (EOF) |
| `sint_erro1.pas` | Erro sintático | Falta de `;` entre comandos em `begin/end` |
| `sint_erro2.pas` | Erro sintático | `if` sem `then` |
| `sint_erro3.pas` | Erro léxico (capturado antes do sintático) | Comentário `{` aberto sem `}` — EOF chega no meio do bloco, detectado já na fase léxica |

---

## Gerar o Diagrama do AFD

Com o [Graphviz](https://graphviz.org/download/) instalado:

```bash
dot -Tpng afd_micropascal.dot -o diagrama_afd.png
```

---

## Visualizar a Árvore Sintática (AST)

Para cada programa com sucesso léxico, o parser gera:
- `*.dot` (árvore em Graphviz)
- `*.png` e `*.pdf` (se o Graphviz `dot` estiver instalado)

Você também pode gerar manualmente com:
```bash
dot -Tpng ok1.dot -o ok1_ast.png
dot -Tpdf ok1.dot -o ok1_ast.pdf
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

---

## Referencias

https://github.com/luizguilhermecm/analisador-sintatico-pascal

https://github.com/Tsukalos/syntax-analysis

https://github.com/kszinhu/syntactic-analysis

https://github.com/fischly/Pascal-Recursive-Descent 
