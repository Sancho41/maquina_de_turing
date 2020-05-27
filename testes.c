#include <stdio.h>
#include <stdlib.h>

// Arquivo de teste

int main(int argc, char **argv)
{

	int i = 0;
	int j = 0;
	int tam = 10;
	int *array;
	char *a;

	scanf("%m[^\n]s%*c", &a);
	i = i * 2;
	printf("\n");

	array = malloc(sizeof(int) * tam);

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

	i = 0;

	while (j + 1 * (((i + j) + 2) / 3) % 2 && 1 || 3)
	{
		printf("%d\n", array[i]);
		i++;
	}

	do
	{
		printf("%d\n", i);
	}while (j + 1 * (((i + j) + 2) / 3) % 2 && 1 || 3);


	i = tam - 1;
	printf("\n");

	return 0;
}

// Não verifica:
// Colchetes de indices de array: i[1]
// i++ ++i --i i--
// Toda a estrutura do switch
// Cast
// Se a variável foi declarada anteriormente
// Struct

// Verifica
// Estrutura de um IF
// Estrutura de um ELSE
// Estrutura de um IF ELSE
// Estrutura de um WHILE
// Estrutura de um DO WHILE
// Estrutura de um FOR
// Declaração de função e seus atributos
// Declaração de variável
// Atribuição de valores
// Expreções matemáticas
// Identação
// Máximo de linhas em branco (2)
// Formatações de boas práticas
