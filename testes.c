#include <stdio.h>
#include <stdlib.h>

// Arquivo de teste

int main(int argc, char *argv[])
{

	int i = 0;
	int j = 0;
	int tam = 10;
	int *array;

	scanf("%m[^\n]s%c*", &i);
	i = i * 2;
	printf("\n");

	array = malloc(sizeof(int) * tam);

	for (j = 0; j < 10; j++)
	{
		if (j + 1 * (((i + j) + 2) / 3) % 2 && 1 || 3)
		{
			array[j] = j + i;
		}
	}

	i = 0;

	while (i < tam)
	{
		printf("%d\n", array[i]);
		i++;
	}

	do
	{
		i = tam - 1;
		printf("\n");
	}while (i);

	return 0;
}
