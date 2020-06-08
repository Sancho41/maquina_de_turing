#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *buffer[50];

/**
 * Verifica se uma linha está em branco.
*/
int blank_line(char *line)
{
  int i;
  int blank_char = 0;
  int size = strlen(line);
  // Percorre toda a string e verifica se os caracteres são espaços.
  for (i = 0; i < size; i++)
    blank_char += line[i] == ' ' || line[i] == 13 || line[i] == 10;

  return blank_char == size;
}

/**
 * Converte uma string paraum char correspondete.
*/
char *convert_string_to_format(char *entry)
{
  if (entry[0] != '\\' || strlen(entry) == 1)
    return entry;

  if (entry[1] == 'n')
    return "\n";

  if (entry[1] == 't')
    return "\t";

  if (entry[1] = 's')
    return " ";
}

/**
 * Printa na tela o caracter, e altera para um outro caso seja caracter de
 * formatação (\n, \t, [espaço]).
*/
int print_value(char entry, char *before, char *after)
{

  printf("%s", before);
  switch (entry)
  {
  case '\n':
    printf("\\n");
    break;

  case '\t':
    printf("\\t");
    break;
  
  default:
    printf("%c ", entry);
    break;
  }
  printf("%s", after);
}


/**
 * Divide uma string em várias sub strings.
*/
char **split(char *string, int qtd_strings)
{
  // char **string_vector;
  char *token;
  char *aux;
  int index = 0;

  token = strtok(string, " :\n\r");

  // string_vector = (char **)malloc(sizeof(char *) * qtd_strings);

  while (token != NULL)
  {
    aux = (char *)malloc(sizeof(char) * strlen(token));
    strcpy(aux, token);

    buffer[index] = convert_string_to_format(aux);

    token = strtok(NULL, " :\n\r");
    index++;
  }
  return buffer;
}

/**
 * Comparação entre a cabeça da máquina e as instruções.
*/
int compare(char *a, char *b)
{
  int len, i, comp;
  len = strlen(a);
  comp = len;

  for (i = 0; i < len; i++)
  {
    if (a[i] == b[i] || a[i] == '~')
      comp--;
  }
  return comp;
}

/**
 * Recebe um caminho de um arquivo por parâmetro, coloca em vetor
 * e retorna o vetor alocado.
*/
char *file_to_array(char *path)
{
  char *buffer = NULL;
  size_t size = 0;

  FILE *fp = fopen(path, "r");

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  buffer = malloc((size + 1) * sizeof(*buffer));

  fread(buffer, size, 1, fp);

  buffer[size] = '\0';

  return buffer;
}