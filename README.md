# Máquina de Turing
Implementação de uma máquina de turing como reconhecedor de linguagem

# Como executar
### O programa foi desenvolvido e testado inteiramente em ambiente **linux**, sendo assim, não há garantias que funcione corretamente no **windows**.
- Abra o terminal na pasta raiz do projeto e execute o arquivo `programa` com o comando:

```shell
$ ./programa
```

- Todas as instruções de para execução da computação da máquina, se encontra nos menus do programa.
- Para a execução do programa que reconhece a linguagem C (que já está pré-carregada na execução do programa) existe um arquivo preparado para testes `input.c`, não faz parte da execução do programa e pode ser alterado para fazer testes do programa. Utilize ele como entrada da fita (selecionando a opção de carregar a entrada a partir de um arquivo).

---
### Argumentos de execução

```
-b <numero_do_passo> (break_point)  : Pausa a execução do programa no passo <numero_do_passo>

-d (no_delay) : Executa o programa sem delay

-s (step_by_step) : Executa o programa passo a passo apertando [ENTER]

-i <input_file> (loaded_file): Carrega o arquivo <input_file> previamente

-n (no_menu) : Ignora o menu e vai direto para a execução
```