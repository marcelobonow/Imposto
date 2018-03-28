///Programa para testar o reconhecimento de imagem no pc
#include <stdio.h>
#include <locale.h>

int main()
{
	setlocale(LC_ALL, "pt-BR");
	FILE* f = fopen("../linha.bmp", "rb");
	if (f == NULL)
	{
		printf("não existe arquivo: linha.bmp");
		getchar();
		return -1;
	}
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f);

	int totalSize = info[2] + (255 * info[3]) + (65535*info[4]) + (16777215 * info[5]);
	int width = info[18];
	int heigth = info[22];
	int pixelSize = 3 * width * heigth;
	printf("Tamanho: %d Bytes\n", totalSize);
	printf("Altura: %d\n", heigth);
	printf("Largura: %d\n", width);
	for (int i = 0; i < 54; i++)
	{
		printf("Valor: %d\t No endereco: %x\n", info[i],i);

	}

	getchar();

}

