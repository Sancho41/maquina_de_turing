#include <stdio.h>
#include <stdlib.h>

// Arquivo de teste

int main()
{

  int i = 0;
  int j = 0;
  int tam = 10;
  int *array;

  scanf("%d", &i);
  i = i * 2;
  printf("\n");

  array = (int *)malloc(sizeof(int) * tam);

  for (j = 0; j < 10; j++)
  {
    array[j] = j + i;
  }

  i = 0;

  while (i < tam)
  {
    printf("%d\n", array[i]);
    i++;
  }

  i = tam - 1;
  printf("\n");

  do
  {
    printf("%d\n", array[i] * 2);
  } while (i--);

  return 0;
}
