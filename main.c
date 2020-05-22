#include <stdio.h>
#include "machine_multitape.c"
#include <unistd.h>
#define PAUSE 0.2

int main()
{
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

    printf("State: %s                   \n", machine->current_state->state);
    printf("Step: %d                     \n\n", step_counter);

    for (i = 0; i < qtd_tapes; i++)
    {
      print_tape(machine->tapes[i]);
      printf("\n");
    }

    fflush(stdout);
    // usleep(PAUSE * 1000000);
    if (step_counter >= 0)
    {
      // usleep(PAUSE * 1000000);
      getchar();
      printf("\033[F");
    }
  }

  printf("\nEnd state: %s\n", machine->end_state);
  printf("Step counter: %d\n", step_counter);

  return 0;
}
