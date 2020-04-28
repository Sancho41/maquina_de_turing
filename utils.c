#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Recebe o arquivo através do STDIN e aloca
// Retorna o ponteiro de onde foi alocado
char *file_from_stdin()
{
  char c;
  char *file;
  int size = 0;

  while ((c = getchar()) != EOF)
  {
    size++;

    if (size == 1)
    {
      file = (char *)malloc(sizeof(char));
      *file = c;
    }
    else
    {
      if ((int)c != 13)
      {
        file = (char *)realloc(file, sizeof(char) * size);
        file[size - 1] = c;
      }
      else
      {
        size--;
      }
    }
  }

  return file;
}

// Recebe o array de caracteres e uma variável para definir o tamanho
// Retorna um array de inteiros da conversão da string
// Seta a variável tamanho.
int *file_to_int(char *file, int *tam)
{
  int *int_file;
  int tam_file = strlen(file);
  int i = 0;
  *tam = tam_file;

  int_file = (int *)malloc(sizeof(int) * tam_file);

  for (i = 0; i < tam_file; i++)
  {
    int_file[i] = (int)file[i];
  }
  return int_file;
}