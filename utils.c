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

int blank_line(char *line)
{
  int i;
  int blank_char = 0;
  int size = strlen(line);
  for (i = 0; i < size; i++)
    blank_char += line[i] == ' ' || line[i] == 13 || line[i] == 10;

  return blank_char == size;
}

char **split(char *string, int qtd_strings)
{
  char **string_vector;
  char *token;
  char *aux;
  int index = 0;

  token = strtok(string, " :\n\r");

  string_vector = (char **)malloc(sizeof(char *) * qtd_strings);

  while (token != NULL)
  {
    aux = (char *)malloc(sizeof(char) * strlen(token));
    strcpy(aux, token);

    string_vector[index] = aux;

    token = strtok(NULL, " :\n\r");
    index++;
  }
  return string_vector;
}