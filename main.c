#include <stdio.h>
#include "utils.c"
#include "machine.c"

int main()
{
  // int i = 0;
  // int tam = 0;
  // int *int_file;
  // char *file;

  // file = file_from_stdin();
  // int_file = file_to_int(file, &tam);

  // printf("%s\n", file);

  // printf("Quantidade de caracteres: %d\n", tam);

  // for (i = 0; i < strlen(file); i++)
  // {
  //   printf("%d\n", int_file[i]);
  // }

  // char *input;
  // TAPE *tape;
  // int input_size;

  // input = file_from_stdin();
  // input_size = strlen(input);

  // tape = intialize_tape(input, input_size);

  // do
  // {
  //   printf("[%c] ", tape->current->value);
  //   move_right(tape);
  // } while (tape->current->value != '~');
  // printf("[%c]\n", tape->current->value);

  // http://morphett.info/turing/turing.html

  MACHINE *machine;
  int step_counter = 0;

  char *input_value;

  printf("Loading program file: %s...\n", "programa.turing");
  machine = load_program("programa.turing");

  printf("Program loaded!\n");

  if (machine == NULL)
    return 0;

  printf("Type initial value of the tape (TYPE _ INSTEAD SPACE): ");
  scanf("%m[^\n]s", &input_value);
  load_tape(machine, input_value);

  printf("Tape initial state:\n");
  print_tape(machine->tape);

  printf("Executing program...\n");

  char *current_state;
  current_state = machine->current_state->state;

  while (strcmp(current_state, "halt") && strcmp(current_state, "halt-accept") && strcmp(current_state, "halt-reject"))
  {
    next_step(machine);
    current_state = machine->current_state->state;
    step_counter++;
  }

  printf("\nTape final state:\n");
  print_tape(machine->tape);
  printf("Last state: %s\n", current_state);
  printf("Steps taken: %d\n", step_counter);

  return 0;
}