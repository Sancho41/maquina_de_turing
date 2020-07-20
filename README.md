# Máquina de Turing
![Página incial do app](/docs/images/maquina_de_turing.png)
# Projeto integrador 1 IESB
## Integrantes da equipe:
- [Gabriel Sancho](https://github.com/sancho41)
- [Matheus Rocha](https://github.com/rochadasilvamatheus)

# Objetivo do projeto
Implementar uma máquina de turing como reconhecedor de linguagem. Teve como objetivo principal reconhecer um dado código escrito na linguagem `C` era correto sintaticamente e se seguia corretamente um padrão de estilo.

---
# Objetivos alcaçados
- Foi feita a implemetação da máquina com a possibilidade de reprogramação;
- É possível utilizar com uma ou mais fitas;
- Foi criado o programa que essa máquina interpreta, [clique aqui](/programs/reconhecedor.turing) para acessar, com ela foi possível validar os seguintes aspéctos da linguagem `C`:
  - Estruturas de blocos de código, tais como:
    - condicionais `if`, `else if` e `else`;
    - laços de repetição `for`, `while` e `do while`;
  - declarações de variáveis e funções;
  - invocações de funções e atribuições de variáveis
  - expressões matemáticas;
  - identação de código.
  
---
# Objetivos não alcançados
- Verificação da estrutura `switch` e declaração de `struct`;
- Limite de caracteres do programa e do input.
  
---
# Como executar
> O programa foi desenvolvido e testado inteiramente em ambiente **linux**, sendo assim, não há garantias que funcione corretamente no **windows**.
Neste repositório já exites um arquivo pré compilado de nome `programa`, para executálo basta digitar no terminal o comando:

```shell
$ ./programa
```
---
Você também pode compilar. Para isso, é necessário ter o `gcc` instalado. Para compilar é só executar o commando:
```shell
$ gcc main.c -o programa
```
Após isso, basta executar o arquivo `programa` gerado com o mesmo comando acima;

Todas as instruções de para execução da computação da máquina, se encontra nos menus do programa.
  
Para a execução do programa que reconhece a linguagem C (que já está pré-carregada na execução) existe um arquivo preparado para testes `input.c`, não faz parte da execução e pode ser alterado para fazer testes do programa. Utilize ele como entrada da fita (selecionando a opção de carregar a entrada a partir de um arquivo).

---
### Argumentos de execução

```
-b <numero_do_passo> (break_point)  : Pausa a execução do programa no passo <numero_do_passo>

-d (no_delay) : Executa o programa sem delay

-s (step_by_step) : Executa o programa passo a passo apertando [ENTER]

-i <input_file> (loaded_file): Carrega o arquivo <input_file> previamente

-n (no_menu) : Ignora o menu e vai direto para a execução
```
