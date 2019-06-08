#include "Player.h"

void readMap(char matrix[][44]){
    char a;
    printf("OIIII\n");
    FILE *file = NULL;
    file = fopen("Matriz.txt","rt");
    if(file==NULL)exit(-5);
    printf("OIIIII\n");
    for(int i = 0;i<30;i++){
        for(int j = 0;j<44;j++){
            fscanf(file," %c",&a);
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
    fclose(file);
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


