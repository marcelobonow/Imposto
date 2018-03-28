/// Programa para testar o reconhecimento de imagem no pc
/// O Windows guarda o programa em formato B G R na memória
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdint.h>

#include "types.h"

struct RGB
{
	uint8 R;
	uint8 G;
	uint8 B;
};

int main()
{
	setlocale(LC_ALL, "pt-BR");
	FILE* f = fopen("../linha.bmp", "rb");
	if (f == NULL)
	{
		printf("não existe arquivo: linha.bmp");
		getchar();
		exit(EXIT_FAILURE);
	}
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f);

	int totalSize = info[2] + (255 * info[3]) + (65535*info[4]) + (16777215 * info[5]);
	int width = info[18];
	int heigth = info[22];
	int bitsPerPixel = info[34];
	int imageSize = (bitsPerPixel/8) *width * heigth;
	printf("----- HEADER ------\n");
	printf("Tamanho: %d Bytes\n", totalSize);
	printf("Altura: %d\n", heigth);
	printf("Largura: %d\n", width);
	
	///Pega a imagem em si
	unsigned char* data = (unsigned char*) malloc(imageSize);
	if (data == NULL)
	{
		perror("Não foi possivel alocar o tamanho da imagem");
		getchar();
		exit(EXIT_FAILURE);
	}

	free(data);
	getchar();

}

