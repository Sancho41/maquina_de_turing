#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BLANK ' '
#define EMPTY " "
#define MAX_TAPE_SIZE 800

typedef struct TAPE
{
  // struct CELL *head;
  char *vector;
  int index;
  int size;
  char head;
} TAPE;

TAPE *initialize_tape(char *entry, int entry_size)
{
  TAPE *new_tape;
  int tape_index;
  char *tape;
  int i;

  // if (entry_size > MAX_TAPE_SIZE)
  //   return NULL;

  new_tape = (TAPE *)malloc(sizeof(TAPE));
  new_tape->index = 10;
  new_tape->vector = (char*)malloc(sizeof(char) * entry_size + 20);
  new_tape->size = entry_size + 20;
  memset(new_tape->vector, ' ', MAX_TAPE_SIZE);

  tape_index = new_tape->index;
  tape = new_tape->vector;

  for (i = 0; i < entry_size; i++)
  {
    tape[tape_index++] = entry[i];
  }

  tape_index = 10;
  new_tape->head = tape[tape_index];

  return new_tape;
}

void move_right(TAPE *tape)
{
  int *tape_index = &tape->index;
  char *vector = tape->vector;
  if (*tape_index < MAX_TAPE_SIZE - 1)
    tape->head = vector[++*tape_index];
}

void move_left(TAPE *tape)
{
  int *tape_index = &tape->index;
  char *vector = tape->vector;
  if (*tape_index > 0)
    tape->head = vector[--*tape_index];
}

void update_value(TAPE *tape, char new_val)
{
  int tape_index = tape->index;
  tape->vector[tape_index] = new_val;
  tape->head = new_val;
}

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
