#include <stdlib.h>
#define BLANK '_'
#define EMPTY "_"

typedef struct TAPE
{
  struct CELL *head;
} TAPE;

typedef struct CELL
{
  char value;
  struct CELL *next;
  struct CELL *prev;
} CELL;

CELL *new_cell(char value)
{
  CELL *new = (CELL *)malloc(sizeof(CELL));
  new->value = value;
  new->next = NULL;
  new->prev = NULL;
  return new;
}

char *get_tapes_heads(TAPE **tapes, int qtd)
{
  char *heads;
  int i;
  heads = (char *)malloc(sizeof(char) * qtd);

  for (i = 0; i < qtd; i++)
  {
    heads[i] = tapes[i]->head->value;
  }
  return heads;
}

TAPE *initialize_tape(char *entry, int entry_size)
{
  int i;
  CELL *aux = NULL;
  CELL *new = NULL;
  TAPE *new_tape = (TAPE *)malloc(sizeof(TAPE));

  CELL *last_cell = new_cell(BLANK);
  CELL *first_cell = new_cell(BLANK);

  // Adiciona a entrada na fita
  for (i = 0; i < entry_size; i++)
  {
    new = new_cell(entry[i]);
    if (aux != NULL)
    {
      aux->next = new;
      new->prev = aux;
    }
    else
    {
      new_tape->head = new;
    }

    // Adiciona o caractere branco no final da fita BLANK
    if (i == entry_size - 1)
    {
      new->next = last_cell;
      last_cell->prev = new;
    }

    aux = new;
  }

  // Adiciona o caractere branco no inicio da fita BLANK
  if (entry_size > 0)
  {
    first_cell->next = new_tape->head;
    new_tape->head->prev = first_cell;
  }
  else
  {
    first_cell->next = last_cell;
    last_cell->prev = first_cell;
    new_tape->head = first_cell;
  }

  return new_tape;
}

void move_right(TAPE *t)
{
  CELL *head = t->head;
  CELL *aux = NULL;

  if (head->next == NULL)
  {
    aux = new_cell(BLANK);
    aux->prev = head;
    head->next = aux;
  }

  t->head = head->next;
}

void move_left(TAPE *t)
{
  CELL *head = t->head;
  CELL *aux = NULL;

  if (head->prev == NULL)
  {
    aux = new_cell(BLANK);
    aux->next = head;
    head->prev = aux;
  }

  t->head = head->prev;
}

void print_tape(TAPE *tape)
{
  CELL *start = tape->head;
  CELL *aux;

  while (start->prev != NULL)
    start = start->prev;
  aux = start;

  while (aux != NULL)
  {
    if (aux == tape->head)
    {
      printf("\033[1;31m");
      printf("[%c]", aux->value);
      printf("\033[0m");
    }
    else
    {
      printf("[%c]", aux->value);
    }

    aux = aux->next;
  }
}