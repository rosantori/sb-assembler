# SB ASSEMBLER

    Autora: Rosana Santos Ribeiro
    Matricula: 160047269

## Tabela de Conteúdo
- SB ASSEMBLER
    - Tabela de Conteúdo
    - Informações
        - Resumo
        - Escolha do trabalho
        - Ambiente de desenvolvimento
    - Instalação
    - Execução
    - Considerações
    - Hierarquia dos Arquivos

## Informacoes

### Resumo
sb-assembler é o primeiro trabalho pratico da disciplina CIC0104 - SOFTWARE BASICO - TB no semestre 2020/1 e visa
a implementacao de um montador que traduz uma linguagem hipotética de Assembly vista em sala de aula para uma representação de codigo objeto.

### Escolha do trabalho

 A opcao escolhida para o desenvolvimento foi de expansao de MACROS.

### Ambiente de desenvolvimento

O projeto foi desenvolvido no ambiente descrito a seguir:

    - Linux 4.15.0-58-generic #64~16.04.1-Ubuntu [...] x86_64 x86_64 x86_64 GNU/Linux
    - C++ (versão 11)
    - g++ (gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.12))
    - GNU Make 4.1

## Instalacao

Ir ate a pasta do projeto no computador

```
$ cd ~/caminho/para/sb-assembler/
```

Compilar com o comando
```
$ make
```

Caso queira a compilacao no modo de debug, executar

```
$ make debug
```

Para trocar o modo de compilacao, antes de executar um novo "make"  execute o comando
```
$ make clean
```

### Execucao
Ainda na pasta do projeto

Para a opcao de pre processamento

```
$  ./montador -p path/to/file.asm
```
Para a opcao de montagem

```
$ ./montador - o path/to/file.pre
```

    Observacao: o caminho para o arquivo de entrada pode ser o relativo ou o absoluto e o arquivo de saida sera gerado com o mesmo caminho.

## Consideracoes 

- A escolha pela implementacao das MACROS exclui a possibilidade de haver erros lexicos, semanticos ou sintaticos no codigo de origem. Portanto, mesmo sendo capaz de identificar alguns erros, esse projeto nao visa a correcao ou identificacao dos mesmos.

- Neste projeto, o pre processamento realizara as seguintes acoes:
    - Retirada de comentarios
    - Tratamento de tabulacoes
    - Retirada de espacos e de quebras de linhas extras
    - Retificacao de rotulos sozinhos em uma linha
    - Execucao das diretivas de pre processamento
        - Expansao de MACROS
        - Tratamentos de EQUs
        - Tratamentos de IFs
- Neste projeto, a montagem correra durante duas passadas:
    - A primeira passada sera para criar uma tabela dos rotulos com seus respectivos enderecos
    - A segunda passada sera para traduzir as instrucoes e diretivas no codigo objeto solicitado
    
- Mais informacoes sobre o roteiro do trabalho devem ser encontradas no arquivo "Trabalho1.pdf" na pasta do projeto

## Hierarquia dos Arquivos

        (sb-assembler)
        |--- examples
        |       |--- fatorial.asm
        |       |--- bin.asm
        |       |--- bin.obj
        |       |--- test1.asm
        |--- include
        |       |--- assembler.h
        |       |--- file.h
        |       |--- Inst.h
        |       |--- macro.h
        |       |--- pre.h
        |--- src
        |       |--- assembler.cpp
        |       |--- file.cpp
        |       |--- Inst.cpp
        |       |--- macro.cpp
        |       |--- main.cpp
        |       |--- pre.cpp
        |--- Makefile
        |--- Trabalho1.pdf
        |--- ./gitignore
        |--- README.md
