#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "tape.c"
#include "utils.c"
#include <unistd.h>

typedef struct MACHINE
{
  struct STATE *state_list;
  struct STATE *current_state;
  struct TAPE **tapes;
  int qtd_tapes;
  char *program_loaded;
  char *initial_state;
  char *end_state;
  int halted;
} MACHINE;

typedef struct STATE
{
  char *state;
  struct STATE *next;
  struct INSTRUCTION *instruction_list;
} STATE;

typedef struct INSTRUCTION
{
  char *cur_symbols;
  char *new_symbols;
  char *directions;
  char *new_state;
  struct INSTRUCTION *next;
} INSTRUCTION;

MACHINE *create_machine()
{
  MACHINE *machine;
  machine = (MACHINE *)malloc(sizeof(MACHINE));
  machine->state_list = NULL;
  machine->current_state = NULL;
  machine->tapes = NULL;
  machine->qtd_tapes = 0;
  machine->initial_state = NULL;
  machine->end_state = NULL;
  machine->halted = 0;
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

INSTRUCTION *create_instruction()
{
  INSTRUCTION *new;
  new = (INSTRUCTION *)malloc(sizeof(INSTRUCTION));
  new->cur_symbols = NULL;
  new->new_symbols = NULL;
  new->directions = NULL;
  new->new_state = NULL;
  new->next = NULL;
  return new;
}

// Carrega as configurações da linha na máquina
// config <Tapes_Quantity> <Initial_state> <program_name>
int load_config(MACHINE *machine, char *config_line)
{
  int qtd_tapes, check;
  char *initial_state, *program_name;

  check = sscanf(config_line, "%*s %d %ms %ms", &qtd_tapes, &initial_state, &program_name);

  if (check != 3)
  {
    return 0;
  }

  machine->qtd_tapes = qtd_tapes;
  machine->initial_state = initial_state;
  machine->program_loaded = program_name;

  return 1;
}

STATE *find_state(MACHINE *machine, char *state_name)
{
  STATE *aux;
  aux = machine->state_list;

  while (aux != NULL)
  {
    if (strcmp(state_name, aux->state) == 0)
      break;
    aux = aux->next;
  }

  return aux;
}

STATE *find_or_create_state(MACHINE *machine, char *state_name)
{
  STATE *aux;
  aux = machine->state_list;

  // Cria um novo estado se a lista estiver vazia
  if (aux == NULL)
  {
    machine->state_list = create_state(state_name);
    aux = machine->state_list;
  }
  else
  {
    // Percorre a lista de estados
    while (aux->next != NULL && strcmp(aux->state, state_name) != 0)
    {
      aux = aux->next;
    }

    // Se a variavel auxiliar for diferente do estado que está procurando
    // Cria um novo estado
    if (strcmp(aux->state, state_name) != 0)
    {
      aux->next = create_state(state_name);
      aux = aux->next;
    }
  }

  return aux;
}

INSTRUCTION *find_instruction(STATE *sate, char *head)
{
  INSTRUCTION *aux;
  aux = sate->instruction_list;

  while (aux != NULL)
  {
    if (strcmp(aux->cur_symbols, head) == 0)
      break;
    aux = aux->next;
  }

  return aux;
}

int load_instruction(MACHINE *machine, char *line)
{
  char *aux;
  char **string_vector;
  char *current_state;
  int string_vector_size;
  int qtd_instructions;
  int i, j;
  STATE *state;
  INSTRUCTION *new_instruction;

  // Cria uma nova instrução vazia
  new_instruction = create_instruction();

  qtd_instructions = machine->qtd_tapes;
  string_vector_size = (3 * qtd_instructions) + 2;

  // Separa a string de comando em substrings e coloca em um vetor
  // de ponteiros para strings
  string_vector = split(line, string_vector_size);

  for (i = 0; i < 3; i++)
  {

    // Junta as intruções de cada fita em uma string só
    aux = (char *)malloc(sizeof(char) * qtd_instructions);
    for (j = 0; j < qtd_instructions; j++)
    {
      aux[j] = string_vector[i + (j * 3) + 1][0];
    }

    switch (i)
    {
    case 0:
      new_instruction->cur_symbols = aux;
      break;

    case 1:
      new_instruction->new_symbols = aux;
      break;

    case 2:
      new_instruction->directions = aux;
      break;
    }
  }

  new_instruction->new_state = string_vector[string_vector_size - 1];

  // Procura um estado, se não econtrar cria um
  current_state = string_vector[0];
  state = find_or_create_state(machine, current_state);

  // Adiciona a nova instrução no inicio da lista
  new_instruction->next = state->instruction_list;
  state->instruction_list = new_instruction;

  return 1;
}

// CARREGA
int load_tapes(MACHINE *machine, ...)
{
  char *input;
  int qtd_tapes, i, input_size;
  va_list ap;
  TAPE *tape;

  qtd_tapes = machine->qtd_tapes;

  va_start(ap, machine);

  machine->tapes = (TAPE **)malloc(sizeof(TAPE *) * qtd_tapes);

  for (i = 0; i < qtd_tapes; i++)
  {
    input = (char *)va_arg(ap, char *);
    if (input != NULL)
    {
      input_size = strlen(input);
      tape = initialize_tape(input, input_size);
    }
    else
    {
      tape = initialize_tape(EMPTY, 0);
    }

    machine->tapes[i] = tape;
  }

  return 1;
}

int load_program(MACHINE *machine, char *program_dir)
{
  char *line, *aux;
  int found_config = 0;
  int line_counter = 0;
  size_t len = 0;
  FILE *file = NULL;

  file = fopen(program_dir, "r");

  if (file == NULL)
  {
    return 0;
  }

  // Le linha por linha do arquivo
  while (getline(&line, &len, file) != EOF)
  {
    line_counter++;

    // Ignora as linhas de comentários e linhas em branco
    if (!blank_line(line) && line[0] != ';')
    {

      // Verifica se ainda não achou a linha de configuração.
      if (!found_config)
      {
        // Verifica se a primiera linha de comando é a configuração da máquina
        // Caso contrário termina execução.
        sscanf(line, "%ms", &aux);
        if (strcmp(aux, "config") == 0)
        {
          found_config = 1;
          load_config(machine, line);
        }
        else
        {
          printf("Expected config line on line %d found \"%s\".\n", line_counter, aux);
          return 1;
        }
      }
      else
      {
        load_instruction(machine, line);
      }
    }
  }

  // Seta o estado inicial da máquina
  machine->current_state = find_state(machine, machine->initial_state);

  return 1;
}

void show_states(MACHINE *machine)
{
  STATE *auxs;
  INSTRUCTION *auxi;

  auxs = machine->state_list;

  while (auxs != NULL)
  {
    printf("State %s:\n", auxs->state);
    auxi = auxs->instruction_list;

    while (auxi != NULL)
    {
      printf("---\n");
      printf("cur_symbols: %s\n", auxi->cur_symbols);
      printf("new_symbols: %s\n", auxi->new_symbols);
      printf("directions: %s\n", auxi->directions);
      printf("new_state: %s\n", auxi->new_state);
      auxi = auxi->next;
    }
    printf("\n");

    auxs = auxs->next;
  }
}

int next_step(MACHINE *machine)
{
  char *heads;
  char *current_state;
  char direction;
  int qtd_tapes, i;
  TAPE **tapes;
  STATE *state, *new_state;
  INSTRUCTION *instruction;
  CELL *head;

  state = machine->current_state;
  tapes = machine->tapes;
  qtd_tapes = machine->qtd_tapes;

  heads = get_tapes_heads(tapes, qtd_tapes);

  instruction = find_instruction(state, heads);

  if (instruction != NULL)
  {
    for (i = 0; i < qtd_tapes; i++)
    {
      head = tapes[i]->head;
      head->value = instruction->new_symbols[i];

      direction = instruction->directions[i];

      if (direction == 'r')
        move_right(tapes[i]);
      else if (direction == 'l')
        move_left(tapes[i]);
    }

    new_state = find_state(machine, instruction->new_state);
    if (new_state == NULL)
    {
      machine->halted = 1;
      machine->end_state = instruction->new_state;
    }
    else
    {
      machine->current_state = new_state;
      current_state = new_state->state;
    }
  }
  else
  {
    machine->halted = 1;
    return 0;
  }
}

// Temporário
int main()
{
  MACHINE *machine;
  machine = create_machine();
  int qtd_tapes, i, step_counter = 0;

  load_program(machine, "programs/string_comparator_2_tapes.turing");

  printf("Programa carregado\n");
  printf("Carregando fitas...\n\n");

  load_tapes(machine, "abba_abba", EMPTY);
  qtd_tapes = machine->qtd_tapes;

  for (i = 0; i < qtd_tapes; i++)
  {
    print_tape(machine->tapes[i]);
    printf("\n");
  }

  printf("\nIniciando computação:\n\n");

  for (i = 0; i < qtd_tapes; i++)
    printf("\n");

  while (!machine->halted)
  {
    next_step(machine);
    step_counter++;

    for (i = 0; i < qtd_tapes; i++)
      printf("\033[F");

    for (i = 0; i < qtd_tapes; i++)
    {
      print_tape(machine->tapes[i]);
      printf("\n");
    }

    fflush(stdout);
    usleep(100000);
  }

  printf("\nEnd state: %s\n", machine->end_state);
  printf("Step counter: %d\n", step_counter);

  return 0;
}
