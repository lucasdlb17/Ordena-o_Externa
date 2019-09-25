
int contaCaractere(FILE* arq){
    int i;
    char c='0';
    for(i=0;c!=EOF; c=getc(arq)){
            if(c!='\n' && c!='\0' && c!=' ')
                i++;
    }
    return i;
}

int calculaPassadas(int fitas, int numreg, int bufsize){
    return ((log(numreg/bufsize))/(log(fitas/2)));
}

int vazioBuffer(char* buffer){
    if((buffer[0]==EOF) && (buffer[1]==EOF) && (buffer[2]==EOF) )
        return 1;
    return 0;
}

char* ordenaBuffer(char* buffer){
    int i, j;
    char aux;
    for(i = 1; i < 3; i++){
        for(j = 0; j < 3 - i; j++){
            if(buffer[j] > buffer[j + 1]){
                aux = buffer[j];
                buffer[j] = buffer[j+1];
                buffer[j+1] = aux;
            }
        }
    }
    return buffer;
}

void gravaBuffer(FILE* f, char* buffer){
    int i;
    for(i=0; i<3; i++){
        fprintf(f, "%c", buffer[i]);
    }
    fprintf(f, "%c", '\0');
}

int PosMenorBuffer(char* buffer){
    int i, posicao;
    char minimo=' ';

    for(i=0; i<3; i++){
        if( (buffer[i]!=EOF) && (buffer[i]!='\0') &&  (buffer[i]!=' ')){
            if( (buffer[i]<minimo) || (minimo<'0') ){
                minimo = buffer[i];
                posicao=i;
            }
        }
    }
    if(minimo<'0')
        return -1;
    return posicao;
}

void fprintBloco(char* buffer, FILE* fitaO1, FILE* fitaO2, FILE* fitaO3, FILE* fitaD){
    int x;
    while( (x = PosMenorBuffer(buffer)) != -1 ){
        fprintf(fitaD, "%c", buffer[x]);
        if(x == 0){
            buffer[x] = getc(fitaO1);
        }else if(x == 1){
            buffer[x] = getc(fitaO2);
        }else if(x == 2){
            buffer[x] = getc(fitaO3);
        }
        else{
            printf("Erro! Posicao de vetor invalida.\n");
            exit(1);
        }
    }
    fprintf(fitaD, "%c",'\0');
}

void separaBloco(FILE* fita){
    FILE* arq;
    arq = fopen("fitaOrdenada.txt", "w");
    char c = ' ';
    while(c!=EOF){
        c=getc(fita);
        if(c>='0'){
            fprintf(arq, "%c", c);
            fprintf(arq, "%c", '\n');
        }
    }
}

int preProcessamento(FILE* fp, FILE* fita1, FILE* fita2, FILE* fita3, char* buffer){
    //Se Arquivo estiver vazio saia da função
    int i=0, fita=1, registros=0;
    char ch;

    //Enquanto meu caractere não for nulo (EOF)
    while( (ch = fgetc(fp)) != EOF ){
        if(ch != '\n'){
            buffer[i] = ch;
            registros++;

    //se buffer estiver cheio
            if(i == 2){
            buffer = ordenaBuffer(buffer);
                switch(fita){
                    case 1:
                        gravaBuffer(fita1, buffer);
                        fita++;
                    break;
                    case 2:
                        gravaBuffer(fita2, buffer);
                        fita++;
                    break;
                    case 3:
                        gravaBuffer(fita3, buffer);
                        fita = 1;
                    break;
                    default:
                        printf("Erro no pre-processamento!\n");
                        return -1;
                }
                i = 0;
            }
            else i++;
        }
    }
    return registros;
}



void posProcessamento(FILE* fitaO1, FILE* fitaO2, FILE* fitaO3, FILE* fitaD1, FILE* fitaD2, FILE* fitaD3, char* buffer){
    int fita=1;

    buffer[0] = ' ';
    buffer[1] = ' ';
    buffer[2] = ' ';

    while(!vazioBuffer(buffer)){
        //Buffer recebe proximos dados dos blocos se já não estiverem no final
        if(buffer[0]!=EOF)
            buffer[0] = getc(fitaO1);
        if(buffer[1]!= EOF)
            buffer[1] = getc(fitaO2);
        if(buffer[2]!= EOF)
            buffer[2] = getc(fitaO3);

        switch(fita){
        //------------------------------------------------------------------------------------------------------------------------------------
            case 1:
                //Enquanto bloco das fitas não acabarem, coloque no bloco da fitaD1
                fprintBloco(buffer, fitaO1, fitaO2, fitaO3, fitaD1);
                fita++;
            break;
        //------------------------------------------------------------------------------------------------------------------------------------

            case 2:
                //Enquanto bloco das fitas não acabarem, coloque no bloco da fitaD2
                fprintBloco(buffer, fitaO1, fitaO2, fitaO3, fitaD2);
                fita++;
            break;

        //------------------------------------------------------------------------------------------------------------------------------------

            case 3:
                //Enquanto bloco das fitas não acabarem, coloque no bloco da fitaD3
                fprintBloco(buffer, fitaO1, fitaO2, fitaO3, fitaD3);
                fita=1;
            break;
            default:
                printf("Erro! fita%i nao encontrada.\n", fita);
        }
    }
}

void adminMenu(){
    int cmd, i;
    char buffer[3], arqNome[100];
    FILE* arq;

    while(cmd!=0){
        printf("======================================================MENU ADMINISTRADOR======================================================\n");
        printf("\n(1.Ver Comandos)\n");
        printf("Digite seu comando: ");
        scanf("%i", &cmd);

        switch(cmd){
            case 0:
            break;
            case 1:
                printf("2.Excluir Arquivo\n3.Identificar Menor no Buffer\n4.Ler Número de registros em arquivo\n\n");
            break;
            case 2:
                printf("\nDigite o nome do arquivo a ser excluido: ");
                scanf("%s", arqNome);
                strcat(arqNome, ".txt");
                if(remove(arqNome))
                    printf("\nArquivo %s foi excluido com sucesso.\n", arqNome);
                else
                    printf("Erro! Não foi possível deletar o arquivo.\n");
            break;
            case 3:
                printf("\nPreencha o vetor: ");
                for(i=0; i<3; i++){
                    printf("\nPosição %i: ", i);
                    scanf("%c", &buffer[i]);
                }
                printf("\nPosição com menor valor: %i", PosMenorBuffer(buffer));
            break;
            case 4:
                printf("\nDigite o nome do arquivo a ser lido: ");
                scanf("%s", arqNome);
                strcat(arqNome, ".txt");
                arq=fopen(arqNome, "r");
                printf("Número de caracteres: %i\n", contaCaractere(arq));
            break;
            default:
                printf("\nErro! Comando Inválido.\n");
        }
        system("pause");
        system("cls");
    }

}
