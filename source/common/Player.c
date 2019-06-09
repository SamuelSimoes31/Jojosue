#include "Player.h"
#include "errno.h"

void readMap(char matrix[][44]){
    char a;
    printf("entrou no readMap\n");
    FILE *arq = NULL;
    arq = fopen("mano.txt", "a+");
    if(arq==NULL){
        printf("n abriu o arq\n");
        printf("%s\n", strerror(errno));
        exit(1);
        }
    else printf("funfou");
    printf("OIIIII\n");
    for(int i = 0;i<30;i++){
        for(int j = 0;j<44;j++){
            fscanf(arq," %c",&a);
            matrix[i][j] = a-48;
        }
    }
    /*
    for(int i = 0;i<30;i++){
        for(int j = 0;j<44;j++){
            printf("%d",matrix[i][j]);
        }
        printf("\n");
    }*/
    fclose(arq);
}

//USADA PRA DEBUG NO LCIENT DE TESTE
void printMap(char matrix[][44]){
    system("clear");
    for(int i = 0;i<30;i++){
        for(int j = 0;j<44;j++){
            printf("%c",(matrix[i][j]<10)?matrix[i][j]+48:matrix[i][j]);
        }
        printf("\n");
    }
}


