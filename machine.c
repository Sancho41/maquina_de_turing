#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tape.c"

typedef struct MACHINE
{
  struct STATE *state_list;
  struct STATE *current_state;
  struct TAPE *tape;
} MACHINE;

typedef struct STATE
{
  char *state;
  struct STATE *next;
  struct INSTRUCTION *instruction_list;
} STATE;

typedef struct INSTRUCTION
{
  char cur_symbol;
  char new_symbol;
  char direction;
  char *new_state;
  struct INSTRUCTION *next;
} INSTRUCTION;

MACHINE *create_machine()
{
  MACHINE *machine;
  machine = (MACHINE *)malloc(sizeof(machine));
  machine->state_list = NULL;
  machine->current_state = NULL;
  return machine;
}

STATE *create_state(char *state_name)
{
  STATE *state;
  state = (STATE *)malloc(sizeof(STATE));
  state->state = state_name;
  state->instruction_list = NULL;
  state->next = NULL;
  return state;
}

INSTRUCTION *create_instruction(char cur_sy, char new_sy, char dir, char *new_st)
{
  INSTRUCTION *new;
  new = (INSTRUCTION *)malloc(sizeof(INSTRUCTION));
  new->cur_symbol = cur_sy;
  new->new_symbol = new_sy;
  new->direction = dir;
  new->new_state = new_st;
  new->next = NULL;
  return new;
}

STATE *add_or_find_state(MACHINE *machine, char *state_name)
{
  STATE *aux;
  aux = machine->state_list;

  if (aux == NULL)
  {
    machine->state_list = create_state(state_name);
    aux = machine->state_list;
  }
  else
  {
    while (aux->next != NULL && strcmp(aux->state, state_name))
    {
      aux = aux->next;
    }

    if (strcmp(aux->state, state_name))
    {
      aux->next = create_state(state_name);
      aux = aux->next;
    }
  }

  return aux;
}

int add_instruction(MACHINE *machine, char *line)
{
  char *cur_st = NULL;
  char *new_st = NULL;
  char dir, cur_sy, new_sy;
  int check;
  STATE *aux_state;
  INSTRUCTION *new, *aux_instruction;

  check = sscanf(line, "%ms %c %c %c %ms", &cur_st, &cur_sy, &new_sy, &dir, &new_st);

  new = create_instruction(cur_sy, new_sy, dir, new_st);

  if (check != 5)
    return 0;

  aux_state = add_or_find_state(machine, cur_st);

  aux_instruction = aux_state->instruction_list;

  if (aux_instruction == NULL)
  {
    aux_state->instruction_list = new;
  }
  else
  {
    while (aux_instruction->next != NULL)
    {
      aux_instruction = aux_instruction->next;
    }

    aux_instruction->next = new;
  }

  return 1;
}

STATE *find_state(MACHINE *machine, char *state_name)
{
  STATE *aux;
  aux = machine->state_list;

  if (aux == NULL)
    return NULL;

  while (aux->next != NULL && strcmp(aux->state, state_name))
    aux = aux->next;

  if (strcmp(aux->state, state_name))
    return NULL;

  return aux;
}

INSTRUCTION *find_instruction(STATE *state, char cur_sy)
{
  INSTRUCTION *aux;
  INSTRUCTION *any = NULL;
  aux = state->instruction_list;

  if (aux == NULL)
    return NULL;

  while (aux->next != NULL && aux->cur_symbol != cur_sy)
  {
    if (aux->cur_symbol == '*')
      any = aux;

    aux = aux->next;
  }

  if (aux->cur_symbol == '*')
    any = aux;

  if (aux->cur_symbol != cur_sy)
  {
    if (any != NULL)
      return any;
    else
      return NULL;
  }

  return aux;
  printf("symbol: %c\n", cur_sy);
}

MACHINE *load_program(char *name)
{
  FILE *file = NULL;
  char *line;
  MACHINE *machine = NULL;

  machine = create_machine();

  file = fopen(name, "r");

  if (file == NULL)
  {
    return NULL;
  }

  while (fscanf(file, "%m[^\n]\n", &line) != EOF)
  {
    if (!blank_line(line) && line[0] != ';')
    {
      if (!add_instruction(machine, line))
      {
        printf("Invalid program\n");
        return NULL;
      }
    }
  }

  machine->current_state = machine->state_list;

  return machine;
}

void load_tape(MACHINE *machine, char *input)
{

  TAPE *tape;
  int input_size;

  input_size = strlen(input);
  tape = initialize_tape(input, input_size);
  machine->tape = tape;
}

int next_step(MACHINE *machine)
{
  TAPE *tape = machine->tape;
  STATE *state = machine->current_state;
  CELL *head = tape->head;

  INSTRUCTION *instruction;
  char head_value = head->value;

  STATE *new_state;

  instruction = find_instruction(state, head_value);

  new_state = find_state(machine, instruction->new_state);

  if (new_state == NULL)
    new_state = create_state(instruction->new_state);

  if (instruction->new_symbol != '*')
    head->value = instruction->new_symbol;

  if (instruction->direction == 'r')
    move_right(tape);
  else if (instruction->direction == 'l')
    move_left(tape);

  machine->current_state = new_state;
  return 1;
}
