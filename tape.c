#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BLANK ' '
#define EMPTY " "


/**
 * Struct de uma fita.
*/
typedef struct TAPE
{
  char *vector;
  int index;
  int size;
  char head;
} TAPE;

/**
 * Função que inicializa uma nova fita.
*/
TAPE *initialize_tape(char *entry, int entry_size)
{
  TAPE *new_tape;
  int tape_index;
  char *tape;
  int i;
  int offset = 30; // Quantidade de espaços em branco a serem adicionados
                   // além da quantidade de caracteres da entrada.

  new_tape = (TAPE *)malloc(sizeof(TAPE));
  new_tape->index = offset / 2;
  // Aloca um vetor do tamanho da entrada mais 20 caracteres.
  new_tape->vector = (char*)malloc(sizeof(char) * entry_size + offset);
  new_tape->size = entry_size + offset;

  // Inicializa o vetor com espaços em branco
  memset(new_tape->vector, ' ', new_tape->size);

  tape_index = new_tape->index;
  tape = new_tape->vector;

  // Copia a entrada para a fita, deixando 10 caracteres atras e 10
  // caracteres na frente.
  for (i = 0; i < entry_size; i++)
  {
    tape[tape_index++] = entry[i];
  }

  tape_index = offset / 2;
  new_tape->head = tape[tape_index];

  return new_tape;
}

/**
 * Movimenta a fita para a direita e atualiza o valor do cabeçote 
 * para a direta.
*/
void move_right(TAPE *tape)
{
  int *tape_index = &tape->index;
  char *vector = tape->vector;
  if (*tape_index < tape->size - 1)
    tape->head = vector[++*tape_index];
}

/**
 * Movimenta a fita para a direita e atualiza o valor do cabeçote 
 * para a esquerda.
*/
void move_left(TAPE *tape)
{
  int *tape_index = &tape->index;
  char *vector = tape->vector;
  if (*tape_index > 0)
    tape->head = vector[--*tape_index];
}

/**
 * Atualiza o valor da célula da fita de acordo.
*/
void update_value(TAPE *tape, char new_val)
{
  int tape_index = tape->index;
  tape->vector[tape_index] = new_val;
  tape->head = new_val;
}

/**
 * Mostra a fita, passada por parâmetro, na tela.
 * 
 * Mostra sempre 21 células.
*/
void print_tape(TAPE *tape)
{
 
  int i;
  int tape_index = tape->index, blank_counter = 0;
  char value;
  char *vector = tape->vector;
  
  tape_index -= 12;

  while(++tape_index < 0)
  {
    printf("[%c ]", BLANK);
    blank_counter++;
  }
    
  for (i = tape_index; i < tape_index + 24 - blank_counter; i++)
  {
    value = vector[i];

    if (i == tape->index)
    {
      print_value(value, "\033[1;31m[ ", "]\033[0m");  
    }
    else if (i >= tape->size)
    {
      print_value(BLANK, "[", "]"); 
    }
    else
    {
      print_value(value, "[", "]");
    }
  }
}
