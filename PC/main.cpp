///Programa para testar o reconhecimento de imagem no pc
#include <stdio.h>

int main()
{
	FILE* f = fopen("../linha.bmp", "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f);

	int size = info[2] + (255 * info[3]) + (65535*info[4]) + (16777215 * info[5]);
	printf("Tamanho: %d Bytes\n", size);
	for (int i = 0; i < 54; i++)
	{
		printf("Valor: %d\t No endereco: %x\n", info[i],i);

	}

	getchar();

}

