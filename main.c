#include <stdio.h>
#include "machine_multitape.c"
#include <unistd.h>

/**
 * Struct para armazenamento de configurações
 * de execução.
*/
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

/**
 * Prototipações.
*/
void clear_terminal();
CONFIG initialize_config();
void load_greetings();
void show_config(CONFIG config);
void load_args(int argc, char *argv[], CONFIG *config);
void configure(CONFIG *config);
void load_tapes_from_user(CONFIG *config, MACHINE *machine);
void print_computation(MACHINE *machine, int step);
void compute(CONFIG *config, MACHINE *machine);
void menu(CONFIG *config, MACHINE *machine);

/**
 * Função principal
*/
int main(int argc, char *argv[])
{
  // Inicia uma instância de configuração.
  CONFIG config = initialize_config();

  // Carrega os argumentos de execução para as configurações.
  load_args(argc, argv, &config);

  // Cria uma nova máquina
  MACHINE *machine;
  machine = create_machine();

  // Inicia a execução chamando o menu.
  menu(&config, machine);

  return 0;
}

/** 
 * Função para limpar a Tela
*/
void clear_terminal()
{
  system("clear");
}

/**
 * Inicializa o Struct de configuração
 * com as configurações padrões.
*/
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

/**
 * Mostra na tela as saudações carregadas do arquivo
 * greetings.txt na raiz do projeto. (Funciona utilizando o cat do
 * linux).
*/
void load_greetings()
{
  system("clear");
  system("cat greetings.txt");
}

/**
 * Monstra as configurações setadas na tela.
*/
void show_config(CONFIG config)
{
  printf("========= Configurações =========\n");
  printf("Passo a Passo (manual): %s\n", config.step_by_step ? "Sim" : "Não");
  printf("Break Point           : %d\n", config.break_point);
  printf("Delay                 : %fs\n", config.delay);
  printf("Programa selecionado  : %s\n", config.selected_program);
  printf("=================================\n");
}

/**
 * Adiciona as configurações adiquiridas através dos argumentos
 * passados para o programa na hora da execução.
 * 
 * -b <passo> : Adiciona uma parada no passo <passo>
 * -d : Faz a execução direta, ignorando o tempo de delay
 * -s : Faz com que a execução seja passada manualmente apertando Enter
 * para cada passo.
 * -i <caminho>: Carrega o arquivo do caminho <caminho> como input da primeira
 * fita (Será ignorado o menu de entrada da fita).
 * -n : Ignora todo o menu e vai direto para a execução.
*/
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

/**
 * Menu para o usuário poder alterar as configurações de execução
 * em tempo de execução do programa.
*/
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

      /**
       * Se a entrada do usuário corresponder a somente 1 caracter, 
       * será feito a tentativa de combinar com algum programa pré
       * selecionado.
      */
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

/**
 * Carrega a fita de input de acordo com a entrada do usuário,
 * podendo aceitar também um caminho de arquivo para ser usado
 * como entrada.
*/
void load_tapes_from_user(CONFIG *config, MACHINE *machine)
{
  char input[50];
  char *file;
  int i;
  char op;
  TAPE *tape;

  clear_terminal();

  // Carrega o programa e verifica quantas fitas o programa pede.
  load_program(machine, config->selected_program);

  printf("Programa carregado: %s\n", machine->program_loaded);
  printf("Quantidade de fitas: %d\n", machine->qtd_tapes);

  // Inicializa os ponteiros para as fitas das máquinas
  machine->tapes = (TAPE **)malloc(sizeof(TAPE *) * machine->qtd_tapes);

  // Se o input já for passado por parâmetro, não será solicitada a entrada
  // por parte do usuário.
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
      // Carrega o arquivo passado como input.
      printf("Digite o caminho do arquivo de input:\n");
      scanf("%s", input);
      file = file_to_array(input);
      int tam = strlen(file);
      tape = initialize_tape(file, tam);
    }
    else
    {
      // Carrega o que foi escrito como input.
      printf("Digite o conteudo da fita: \n");
      scanf("%*c%[^\n]s%*c", input);
      tape = initialize_tape(input, strlen(input));
    }
  }

  // Adiciona na máquina a primeira fita.
  machine->tapes[0] = tape;

  // Inicializa o resto das fitas em branco.
  for (i = 1; i < machine->qtd_tapes; i++)
  {
    machine->tapes[i] = initialize_tape(EMPTY, 0);
  }

  // Mostra as fitas na tela.
  printf("Fitas carregadas: \n");

  for (i = 0; i < machine->qtd_tapes; i++)
  {
    print_tape(machine->tapes[i]);
    printf("\n");
  }
}

/**
 * Imprime na tela o estado atual de computação da máquina,
 * mostrando as fitas, os cabeçotes, o estado e o passo atual
 * da máquina.
*/
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

/**
 * Inicia o laço de computação da máquina.
*/
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

  // Enquanto a máquina não entrar em estado de parada, será continuado
  // a contagem de passos.
  while (!machine->halted)
  {
    // Próximo passo da máquina
    next_step(machine);
    step_counter++;

    // Mostra o estado atual da máquina na tela
    print_computation(machine, step_counter);

    // Se foi configurado o parâmetro direct, será ignorado o delay.
    if (!config->direct)
      usleep(config->delay * 1000000);

    // Se configurado step_by_step será solicitado entrada do usuário
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

  // Mostra estados finais da máquina.
  printf("\nEstado final: %s\n", machine->end_state != NULL ? machine->end_state : "HALT-REJECT");
  printf("Quantidade de passos: %d\n", step_counter);
  for (i = 0; i < sep_size; i++)
    printf("=");
  printf("\n");
  printf("Pressione enter para sair...\n");
  getchar();
}

/**
 * Mostra o primeiro menu do Programa.
*/
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
}
