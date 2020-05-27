#include <stdio.h>
#include "machine_multitape.c"
#include <unistd.h>
#define PAUSE 0.2

int main(int argc, char *argv[])
{
  int debug = 0, step = 0, direct = 0;
  char exit;

  if (argc >= 2)
  {
    debug = strcmp(argv[1], "debug") == 0;
    if (argc > 2 && debug)
      step = atoi(argv[2]);
    
    direct = strcmp(argv[argc - 1], "direct") == 0;
  }

  char *input;

  // input = file_from_stdin();
  input = file_to_array("testes.c");

  MACHINE *machine;
  machine = create_machine();
  int qtd_tapes, i, step_counter = 0;

  load_program(machine, "programs/reconhecedor.turing");

  printf("Programa '%s' carregado\n", machine->program_loaded);
  printf("Carregando fitas (quantidade de fitas: %d)...\n\n", machine->qtd_tapes);

  load_tapes(machine,input, EMPTY);
  qtd_tapes = machine->qtd_tapes;

  for (i = 0; i < qtd_tapes; i++)
  {
    print_tape(machine->tapes[i]);
    printf("\n");
  }

  printf("\nIniciando computação:\n\n");

  for (i = 0; i < qtd_tapes + 3; i++)
    printf("\n");

  while (!machine->halted)
  {
    next_step(machine);
    step_counter++;

    for (i = 0; i < qtd_tapes + 3; i++)
      printf("\033[F");

    printf("Estado: %s                   \n", machine->current_state->state);
    printf("Passo: %d                     \n\n", step_counter);

    for (i = 0; i < qtd_tapes; i++)
    {
      print_tape(machine->tapes[i]);
      printf("\n");
    }

    fflush(stdout);

    if (!direct)
      usleep(PAUSE * 1000000);

    if (debug && step_counter >= step)
    {
      // usleep(PAUSE * 1000000);
      exit = getchar();
      printf("\033[F");
      if (exit == '0')
        debug = 0;
    }
  }

  printf("\nEstado final: %s\n", machine->end_state != NULL ? machine->end_state : "HALT-REJECT");
  printf("Quantidade de passos: %d\n", step_counter);

  return 0;
}
