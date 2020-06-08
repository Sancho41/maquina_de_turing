#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "utils.c"
#include "tape.c"
#include <unistd.h>

/**
 * Struct da máquina.
*/
typedef struct MACHINE
{
  struct STATE *state_list;
  struct STATE *current_state;
  struct TAPE **tapes;
  int qtd_tapes;
  char *program_loaded;
  char *initial_state;
  char *end_state;
  char *heads;
  int halted;
} MACHINE;

/**
 * Estrutura dos estados, utilizando listas ligadas.
*/
typedef struct STATE
{
  char *state;
  struct STATE *next;
  struct INSTRUCTION *instruction_list;
} STATE;

/**
 * Estrutura das instruções, utilizando listas ligadas.
*/
typedef struct INSTRUCTION
{
  char *cur_symbols;
  char *new_symbols;
  char *directions;
  char *new_state;
  struct INSTRUCTION *next;
} INSTRUCTION;

/**
 * Função que inicializa uma nova máquina.
*/
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
  machine->heads = NULL;
  return machine;
}

/**
 * Função que incializa um novo estado.
*/
STATE *create_state(char *state_name)
{
  STATE *state;
  state = (STATE *)malloc(sizeof(STATE));
  state->state = state_name;
  state->instruction_list = NULL;
  state->next = NULL;
  return state;
}

/**
 * Função que inicializa uma nova instrução.
*/
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

/**
 * Carrega as configurações da linha da máquina.
 * A linha deve conter o seguinte padrão:
 * config <quantidad de fitas> <estado incial> <nome do programa>
*/
int load_config(MACHINE *machine, char *config_line)
{
  int qtd_tapes, check;
  char *initial_state, *program_name;

  check = sscanf(config_line, "%*s %d %ms %ms", &qtd_tapes, &initial_state, &program_name);

  if (check != 3)
  {
    return 0;
  }

  machine->heads = (char *)malloc(sizeof(char) * qtd_tapes);
  machine->qtd_tapes = qtd_tapes;
  machine->initial_state = initial_state;
  machine->program_loaded = program_name;

  return 1;
}

/**
 * Procura algum estado correspondente na lista da máquina.
*/
STATE *find_state(MACHINE *machine, char *state_name)
{
  STATE *aux;
  aux = machine->state_list;

  // Uma busca simples de uma lista ligada.
  while (aux != NULL)
  {
    if (strcmp(state_name, aux->state) == 0)
      break;
    aux = aux->next;
  }

  return aux;
}

/**
 * Procura um estado correspondente na máquina, se não encontrado
 * criasse um novo estado.
*/
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
    // cria um novo estado
    if (strcmp(aux->state, state_name) != 0)
    {
      aux->next = create_state(state_name);
      aux = aux->next;
    }
  }

  return aux;
}

/**
 * Procura uma instrução correspondente dado o estado e a leitura
 * das cabeças da máquina.
 * 
 * Para as instruções foi utilizado, como forma de simplificação de 
 * escrita de programas, uma forma de representação de caracter genérico,
 * que poderia representar qualquer caracter da tabela asccii. Porém, se
 * para um mesmo estado for definido um caracter genérico e um outro caracter
 * específico, toma se como prioritário o caracter específico. Se não for 
 * compatível com o caracter específico, poderá se utilizar do caracter
 * genérico.
 * 
 * 
 * Por essa abordagem foi definido um método de comparação de instrução,
 * utilizado na função compare, onde, quanto mais próximo de 0 a função retornar
 * mais compatível com a instrução.
 * 
 * Para que essa abordagem seja efetiva, e necessário adicionar a instrução
 * mais genérica em primeiro lugar, uma vez que ela será a primeira a ser
 * verificada.
 * 
*/
INSTRUCTION *find_instruction(STATE *sate, char *head)
{
  INSTRUCTION *aux, *found;
  aux = sate->instruction_list;
  int min = 99999;
  int i;
  int j;

  // Percorre a lista de instruções.
  while (aux != NULL)
  {
    // Verifica o nível de comparação da instrução.
    i = compare(aux->cur_symbols, head);

    // Se o nível de comparação for menor que o armazenado, será
    // utilizado essa nova instrução.
    if (i < min)
    {
      min = i;
      found = aux;
    }
    aux = aux->next;
  }

  if (min == strlen(head))
  {
    return NULL;
  }

  // Verifica se a instrução encontrada é compatível.
  for (j = 0; j < strlen(head); j++)
  {
    if (found->cur_symbols[j] != head[j])
      if (found->cur_symbols[j] != '~')
        return NULL;
  }

  return found;
}

/**
 * Carrega uma nova instrução para a máquina.
*/
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

/**
 * Carrega as fitas na máquina, passando o conteúdo das fitas 
 * como parâmetro.
*/
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

/**
 * Carrega um programa passado o seu cominho como parâmetro.
*/
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

/**
 * Função para mostrar os estado possíveis da máquina.
*/
void show_states(MACHINE *machine)
{
  STATE *auxs;
  INSTRUCTION *auxi;

  auxs = machine->state_list;

  // Percorre a lista ligada de estados da máquina.
  while (auxs != NULL)
  {
    printf("State %s:\n", auxs->state);
    auxi = auxs->instruction_list;

    // Percorre a lista ligada de instruções de cada estado da máquina.
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

/**
 * Atualiza na estrutura da máquina os valores dos cabeçotes da máquina.
*/
void get_tapes_heads(MACHINE *machine)
{
  TAPE **tapes;
  TAPE *tape;
  int qtd_tapes;
  int i;

  tapes = machine->tapes;
  qtd_tapes = machine->qtd_tapes;

  // Percorre pelas fitas da máquina, pegando seus valores e armazenando
  // no vetor de caracteres que indica os valores dos cabeçotes.
  for (i = 0; i < qtd_tapes; i++)
  {
    tape = tapes[i];
    machine->heads[i] = tape->head;
  }
}

/**
 * Executa um novo passo na máquina.
*/
int next_step(MACHINE *machine)
{
  char *heads;
  char *current_state;
  char direction;
  int qtd_tapes, i;
  TAPE **tapes;
  TAPE *tape;
  STATE *state, *new_state;
  INSTRUCTION *instruction;

  state = machine->current_state;
  tapes = machine->tapes;
  qtd_tapes = machine->qtd_tapes;

  // Atualiza os valores dos cabeçotes na máquina.
  get_tapes_heads(machine);

  heads = machine->heads;

  // Encontra uma nova instrução de acordo com o estado e com os valores
  // no cabeçote da máquina.
  instruction = find_instruction(state, heads);

  // Verifica se foi encontrado uma instrução, caso contrário
  // a máquina entrara em estado de parada.
  if (instruction != NULL)
  {

    // Executa a intrução encontrada para cada fita.
    for (i = 0; i < qtd_tapes; i++)
    {
      // Atuliza o valor da célula de acordo com a instrução.
      if (instruction->new_symbols[i] != '~')
      {
        tape = tapes[i];
        update_value(tape, instruction->new_symbols[i]);
      }

      direction = instruction->directions[i];
      
      // Movimenta o cabeçote.
      // r : direta
      // l : esquerda
      // ~ : não movimenta
      if (direction == 'r')
        move_right(tapes[i]);
      else if (direction == 'l')
        move_left(tapes[i]);
    }

    // Procura o novo estado da máquina.
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
