#include <stdio.h>
#include "utils.c"

int main()
{
  int i = 0;
  int tam = 0;
  int *int_file;
  char *file;

  file = file_from_stdin();
  int_file = file_to_int(file, &tam);

  printf("%s\n", file);

  printf("Quantidade de caracteres: %d\n", tam);

  for (i = 0; i < strlen(file); i++)
  {
    printf("%d\n", int_file[i]);
  }

  return 0;
}