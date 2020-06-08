#include <stdio.h>
#include "machine_multitape.c"
#include <unistd.h>

typedef struct CONFIG
{
  int debug;
  int step_by_step;
  int break_point;
  int direct;
  int no_menu;
  double delay;
  char selected_program[50];
  char *input;
} CONFIG;

void clear_terminal()
{
  system("clear");
}

CONFIG initialize_config()
{
  CONFIG new_config;

  new_config.step_by_step = 0;
  new_config.break_point = 0;
  new_config.direct = 0;
  new_config.delay = 0.2;
  new_config.input = NULL;
  new_config.no_menu = 0;
  strcpy(new_config.selected_program, "programs/reconhecedor.turing");
  return new_config;
}

void load_greetings()
{
  system("clear");
  system("cat greetings.txt");
}

void show_config(CONFIG config)
{
  printf("========= Configurações =========\n");
  printf("Passo a Passo (manual): %s\n", config.step_by_step ? "Sim" : "Não");
  printf("Break Point           : %d\n", config.break_point);
  printf("Delay                 : %fs\n", config.delay);
  printf("Programa selecionado  : %s\n", config.selected_program);
  printf("=================================\n");
}

void load_args(int argc, char *argv[], CONFIG *config)
{
  int i;
  char aux;
  for (i = 1; i < argc; i++)
  {
    aux = argv[i][0];
    if (aux == '-')
    {
      aux = argv[i][1];

      switch (aux)
      {
      case 'b':
        config->break_point = atoi(argv[++i]);
        break;
      case 'd':
        config->direct = 1;
        break;
      case 's':
        config->step_by_step = 1;
        break;
      case 'i':
        config->input = argv[++i];
        break;
      case 'n':
        config->no_menu = 1;
        break;
      }
    }
  }
}

void configure(CONFIG *config)
{
  int op, aux;
  double seconds;
  char program[50];

  do
  {
    clear_terminal();
    printf("Menu de configuração:\n");
    printf("1. Passo a Passo (manual): %s\n", config->step_by_step ? "Sim" : "Não");
    printf("2. Break Point           : %d\n", config->break_point);
    printf("3. Delay                 : %fs\n", config->delay);
    printf("4. Sem Delay             : %s\n", config->direct ? "Sim" : "Não");
    printf("5. Programa Selecionado  : %s\n", config->selected_program);
    printf("0. Sair\n");
    scanf("%d", &op);

    switch (op)
    {
    case 0:
      clear_terminal();
      show_config(*config);
      printf("Saindo das configurações...\n");
      break;
    case 1:
      config->step_by_step = !config->step_by_step;
      break;
    case 2:
      printf("Valor atual: %d\n", config->break_point);
      printf("Digite o valor para parar: ");
      scanf("%d", &config->break_point);
      break;
    case 3:
      printf("Valor atual: %lf\n", config->delay);
      printf("Digite o valor para o delay (double): ");
      scanf("%lf", &config->delay);
      break;
    case 4:
      config->direct = !config->direct;
      break;
    case 5:
      printf("Valor atual: %s\n", config->selected_program);
      printf("Selecione um programa ou digite um caminho:\n");
      printf("1. Reconhecedor de código C\n");
      printf("2. Reconhecedor de palavra\n");
      printf("3. Palindrome Fita única\n");
      printf("4. Padindrome Fita dupla\n");
      scanf("%s", program);

      if (strlen(program) == 1)
      {
        switch (program[0])
        {
        case '1':
          strcpy(config->selected_program, "programs/reconhecedor.turing");
          break;
        case '2':
          strcpy(config->selected_program, "programs/palavra.turing");
          break;
        case '3':
          strcpy(config->selected_program, "programs/palindrome1.turing");
          break;
        case '4':
          strcpy(config->selected_program, "programs/palindrome2.turing");
          break;
        default:
          break;
        }
      }
      else
      {
        strcpy(config->selected_program, program);
      }

      op = 5;
      break;

    default:
      op = 0;
      break;
    }

  } while (op != 0);
}

void load_tapes_from_user(CONFIG *config, MACHINE *machine)
{
  char input[50];
  char *file;
  int i;
  char op;
  TAPE *tape;

  clear_terminal();
  load_program(machine, config->selected_program);
  printf("Programa carregado: %s\n", machine->program_loaded);
  printf("Quantidade de fitas: %d\n", machine->qtd_tapes);

  machine->tapes = (TAPE **)malloc(sizeof(TAPE *) * machine->qtd_tapes);

  if (config->input)
  {
    file = file_to_array(config->input);
    tape = initialize_tape(file, strlen(file));
  }
  else
  {
    printf("Deseja carregar o input de um arquivo? (s, n) ");
    scanf("%*c");
    scanf("%c", &op);
    if (op == 's' || op == 'S')
    {
      printf("Digite o caminho do arquivo de input:\n");
      scanf("%s", input);
      file = file_to_array(input);
      int tam = strlen(file);
      tape = initialize_tape(file, tam);
    }
    else
    {
      printf("Digite o conteudo da fita: \n");
      scanf("%*c%[^\n]s%*c", input);
      tape = initialize_tape(input, strlen(input));
    }
  }

  machine->tapes[0] = tape;

  for (i = 1; i < machine->qtd_tapes; i++)
  {
    machine->tapes[i] = initialize_tape(EMPTY, 0);
  }

  printf("Fitas carregadas: \n");

  for (i = 0; i < machine->qtd_tapes; i++)
  {
    print_tape(machine->tapes[i]);
    printf("\n");
  }
}

void print_computation(MACHINE *machine, int step)
{
  int i, qtd_tapes;
  qtd_tapes = machine->qtd_tapes;

  for (i = 0; i < qtd_tapes + 3; i++)
    printf("\033[F");

  printf("Estado: %s                    \n", machine->current_state->state);
  printf("Passo: %d                     \n\n", step);

  for (i = 0; i < qtd_tapes; i++)
  {
    print_tape(machine->tapes[i]);
    printf("\n");
  }
}

void compute(CONFIG *config, MACHINE *machine)
{

  int i;
  int step_counter = 0;
  int qtd_tapes;
  int sep_size = 98;
  char exit;

  clear_terminal();

  for (i = 0; i < sep_size; i++)
    printf("=");
  printf("\n");

  printf("Programa: %s\n", machine->program_loaded);
  for (i = 0; i < machine->qtd_tapes + 3; i++)
    printf("\n");

  while (!machine->halted)
  {
    next_step(machine);
    step_counter++;

    print_computation(machine, step_counter);

    if (!config->direct)
      usleep(config->delay * 1000000);

    if (config->step_by_step && step_counter >= config->break_point)
    {
      exit = getchar();
      printf("\033[F");
      if (exit == '0')
      {
        config->step_by_step = 0;
        config->direct = 0;
      }

      if (exit == '1')
      {
        config->step_by_step = 0;
        config->direct = 1;
      }
    }
  }
  printf("\nEstado final: %s\n", machine->end_state != NULL ? machine->end_state : "HALT-REJECT");
  printf("Quantidade de passos: %d\n", step_counter);
  for (i = 0; i < sep_size; i++)
    printf("=");
  printf("\n");
  printf("Pressione enter para sair...\n");
  getchar();
}

void menu(CONFIG *config, MACHINE *machine)
{

  int op = 0;
  char selected_program[29] = "programs/reconhecedor.turing";

  if (config->no_menu)
    op = 3;

  do
  {
    load_greetings();
    printf("Selecione uma opção:\n");
    printf("1. Mostrar as configurações da máquina.\n");
    printf("2. Alterar as configurações da máquina.\n");
    printf("3. Carregar fitas e iniciar computação.\n");

    if (op != 3)
      scanf("%d", &op);

    switch (op)
    {
    case 1:
      clear_terminal();
      show_config(*config);
      printf("Pressione enter para voltar.\n");
      scanf("%*c%*c");
      break;
    case 2:
      clear_terminal();
      configure(config);
      clear_terminal();
      break;
    case 3:
      load_tapes_from_user(config, machine);
      if (!config->no_menu)
      {
        printf("Pressione ENTER para iniciar a computação...\n");
        scanf("%*c%*c");
      }
      compute(config, machine);
      op = 0;
      break;
    default:
      op = 0;
      break;
    }
  } while (op);

  // load_program(machine, "programs/reconhecedor.turing");
  // printf("Programa '%s' carregado\n", machine->program_loaded);
}

int main(int argc, char *argv[])
{
  CONFIG config = initialize_config();

  load_args(argc, argv, &config);
  // show_config(config);
  // configure(&config);

  MACHINE *machine;
  machine = create_machine();

  menu(&config, machine);

  // printf("Carregando fitas (quantidade de fitas: %d)...\n\n", machine->qtd_tapes);

  // load_tapes(machine, input, EMPTY);

  return 0;

  // int debug = 0, step = 0, direct = 0;
  // char exit;

  // if (argc >= 2)
  // {
  //   debug = strcmp(argv[1], "debug") == 0;
  //   if (argc > 2 && debug)
  //     step = atoi(argv[2]);

  //   direct = strcmp(argv[argc - 1], "direct") == 0;
  // }

  // char *input;

  // input = file_to_array("testes.c");

  // MACHINE *machine;
  // machine = create_machine();
  // int qtd_tapes, i, step_counter = 0;

  // load_program(machine, "programs/reconhecedor.turing");

  // printf("Programa '%s' carregado\n", machine->program_loaded);
  // printf("Carregando fitas (quantidade de fitas: %d)...\n\n", machine->qtd_tapes);

  // load_tapes(machine, input, EMPTY);
  // qtd_tapes = machine->qtd_tapes;

  // for (i = 0; i < qtd_tapes; i++)
  // {
  //   print_tape(machine->tapes[i]);
  //   printf("\n");
  // }

  // printf("\nIniciando computação:\n\n");

  // for (i = 0; i < qtd_tapes + 3; i++)
  //   printf("\n");

  return 0;
}
