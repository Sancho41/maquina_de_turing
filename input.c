#include <stdio.h>
#include <stdlib.h>
#define MAX 100

// Arquivo de teste

int main(int argc, char **argv)
{

	int i = 0;
	int j = 0;
	int tam = 10;
	int *array;
	char *a;
	while (j + 1 * (((i + j) + 2) / 3) % 2 && 1 || 3)
	{
		printf("%d\n", array[i]);
		i++;

		do
		{
			printf("%d\n", i);
			for (j = 0; j < 10; j++)
			{
				if (j + 1 * (((i + j) + 2) / 3) % 2 && 1 || 3)
				{
					array[j] = j + i;
				}
				else if (j + 1 * (((i + j) + 2) / 3) % 2 && 1 || 3)
				{
					i = 0;
				}
				else
				{
					// TESTE
				}
			}
		}while (j + 1 * (((i + j) + 2) / 3) % 2 && 1 || 3);
	}

	scanf("%m[^\n]s%*c", &a);
	i = i * 2;
	printf("\n");

	array = malloc(sizeof(int) * tam);
	printf("\n");

	return 0;
}
