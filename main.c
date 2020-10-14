#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para usar strings

// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel RGB
typedef struct
{
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB

typedef struct
{
    int width, height;
    RGB *img;
} Img;

// Protótipos
void load(char *name, Img *pic);
void toGrayscale(Img *pic);

// Carrega uma imagem para a struct Img
void load(char *name, Img *pic)
{
    int chan;
    pic->img = (unsigned char *)SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if (!pic->img)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

void toGrayscale(Img *pic)
{
    for (int i = 0; i < pic->width * pic->height; i++)
    {
        int cinza = 0.3 * pic->img[i].r + 0.59 * pic->img[i].g + 0.11 * pic->img[i].b;
        pic->img[i].r = cinza;
        pic->img[i].g = cinza;
        pic->img[i].b = cinza;
    }
}

void toHTML(int *medias, int tamMedias, int larMedias)
{
    FILE *arq = fopen("saida.html", "w"); // criar o arquivo: w
    if (arq == NULL)                      // 0, falso
    {
        printf("Erro abrindo arquivo de saída\n");
        exit(1);
    }

    fprintf(arq, "<html>\n");
    fprintf(arq, "<head>\n");
    fprintf(arq, "</head>\n");
    fprintf(arq, "<body style=\"background: black;\" leftmargin=0 topmargin=0>\n");
    fprintf(arq, "<style>");
    fprintf(arq, "pre{");
    fprintf(arq, "    color: white;");
    fprintf(arq, "    font-family: Courier;");
    fprintf(arq, "    font-size: 10px;}");
    fprintf(arq, "</style>\n");
    fprintf(arq, "<pre>\n");

    int largura = 0;
    for (int i = 0; i < tamMedias; i++)
    {
        if (medias[i] >= 255)
        {
            fprintf(arq, " ");
        }
        else if (medias[i] >= 223.125)
        {
            fprintf(arq, ".");
        }
        else if (medias[i] >= 191.25)
        {
            fprintf(arq, ":");
        }
        else if (medias[i] >= 159.375)
        {
            fprintf(arq, "c");
        }
        else if (medias[i] >= 127.5)
        {
            fprintf(arq, "o");
        }
        else if (medias[i] >= 95.625)
        {
            fprintf(arq, "C");
        }
        else if (medias[i] >= 63.75)
        {
            fprintf(arq, "O");
        }
        else if (medias[i] >= 31.875)
        {
            fprintf(arq, "8");
        }
        else
        {
            fprintf(arq, "@");
        }
        largura++;
        if (largura >= larMedias)
        {
            fprintf(arq, "\n");
            largura = 0;
        }
    }

    fprintf(arq, "</pre>\n");
    fprintf(arq, "</body>\n");

    fclose(arq);
}

int main(int argc, char **argv)
{
    int const ALT_BLOCO = 4;
    int const LAR_BLOCO = 2;

    Img pic;
    if (argc == 1)
    {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);

    printf(argv[2]); //fator de reducao

    if (pic.height < ALT_BLOCO || pic.width < LAR_BLOCO)
    {
        printf("Imagem muito pequena, tamanho mínimo de 4x5 pixels.");
        exit(1);
    }

    toGrayscale(&pic);

    // Exemplo: gravando um arquivo de saída com a imagem (não é necessário para o trabalho, apenas
    // para ver o resultado intermediário, por ex, da conversão para tons de cinza)
    SOIL_save_image("out.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, (const unsigned char *)pic.img);

    int qtdH = pic.width / LAR_BLOCO;
    int qtdV = pic.height / ALT_BLOCO;
    int *medias = malloc((qtdH * qtdV) * sizeof(int *));

    int indexMedia = 0;
    for (int i = 0; i <= pic.width * ((qtdV - 1) * ALT_BLOCO); i += pic.width * ALT_BLOCO)
    {
        for (int j = 0; j <= qtdH - 1; j++)
        {
            int soma = 0;
            int linhaAuxiliar = 0;
            int limite = (i + (j * LAR_BLOCO)) + ((LAR_BLOCO * pic.width) + (LAR_BLOCO - 1)); //Linha atual(i) + bloco atual(j)*largura do bloco + 4 linhas + 3 = ultimo numero do bloco
            for (int k = i + (j * LAR_BLOCO); k <= limite; k++)
            {
                soma += pic.img[k].r;

                linhaAuxiliar++;
                if (linhaAuxiliar >= ALT_BLOCO)
                {
                    linhaAuxiliar = 0;
                    k += pic.width;
                }
            }
            medias[indexMedia] = soma / (ALT_BLOCO * LAR_BLOCO);
            indexMedia++;
        }
    }

    toHTML(medias, qtdH * qtdV, qtdH);

    // Exemplo: gravando um arquivo saida.html
    free(pic.img);
}
