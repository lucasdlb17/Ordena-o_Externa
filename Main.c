#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include "Bilioteca.h"
#define TAM 3


int main()
{
    //traduz
    setlocale(LC_ALL, "Portuguese");
    //variaveis de arquivos
    FILE* fp;
    FILE* fita1;
    FILE* fita2;
    FILE* fita3;
    FILE* fita4;
    FILE* fita5;
    FILE* fita6;


    int inverte=0, passadas, regTotal;
    char buffer[TAM];   //buffer
    char arqNome[100];  //nome do arquivo

    printf("==================================================ORDENAÇÃO DE ARQUIVO==================================================\n");
    printf("Digite o nome do arquivo: ");
    scanf("%s", arqNome);
    if(strcmp(arqNome, "admin")==0){
        system("cls");
        adminMenu();
        printf("\nDigite o nome do arquivo a ser lido: ");
        scanf("%s", arqNome);
    }
    strcat(arqNome, ".txt");
    //abrindo arquivos para preProcessamento
    fp = fopen(arqNome, "r");
    if(fp == NULL){
        printf("Erro! Arquivo nao encontrado");
        exit(1);
    }
    printf("\nCARREGANDO ARQUIVO...\n");
    fita1 = fopen("fita1.txt", "w+");
    fita2 = fopen("fita2.txt", "w+");
    fita3 = fopen("fita3.txt", "w+");
    //função carrega dados em blocos de 3 para fitas e retorna a quantidades de registros
    regTotal = preProcessamento(fp, fita1, fita2, fita3, buffer);
    printf("\nPre-Processamento Finalizado.\n");
    printf("%i Registros Lidos.\n\n", regTotal);
    //fecha arquivos utilizados
    fclose(fita1);
    fclose(fita2);
    fclose(fita3);
    fclose(fp);
    printf("PROCESSANDO...\n");

    for(passadas=0; passadas<=calculaPassadas(6,regTotal, 3); passadas++){
        if(inverte==0){
            //abrindo arquivos para posProcessamento
            fita1 = fopen("fita1.txt", "r");
            fita2 = fopen("fita2.txt", "r");
            fita3 = fopen("fita3.txt", "r");
            fita4 = fopen("fita4.txt", "w");
            fita5 = fopen("fita5.txt", "w");
            fita6 = fopen("fita6.txt", "w");
            posProcessamento(fita1, fita2, fita3, fita4, fita5, fita6, buffer);
            fclose(fita1);
            fclose(fita2);
            fclose(fita3);
            fclose(fita4);
            fclose(fita5);
            fclose(fita6);
            inverte++;
        }
        else if(inverte==1)
        {
	    //abrindo arquivos para posProcessamento
            fita1 = fopen("fita1.txt", "w");
            fita2 = fopen("fita2.txt", "w");
            fita3 = fopen("fita3.txt", "w");
            fita4 = fopen("fita4.txt", "r");
            fita5 = fopen("fita5.txt", "r");
            fita6 = fopen("fita6.txt", "r");
            posProcessamento(fita4, fita5, fita6, fita1, fita2, fita3, buffer);
            fclose(fita1);
            fclose(fita2);
            fclose(fita3);
            fclose(fita4);
            fclose(fita5);
            fclose(fita6);
            inverte--;
        }
        else
        {
            printf("\nErro! Falha na inverção de fitas\n");
            exit(1);
        }
    }
    if(inverte==0){
        fita1=fopen("fita1.txt", "r");
        separaBloco(fita1);
        fclose(fita1);
    }
    else if(inverte==1){
        fita4=fopen("fita4.txt", "r");
        separaBloco(fita4);
        fclose(fita4);
    }
    printf("\nOrdenação Finalizada.\n");
    printf("Arquivo Ordenado na fitaOrdenada.\n");

    return 0;
}


