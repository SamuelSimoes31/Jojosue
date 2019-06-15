#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>

#define DOWN_ARROW 's'
#define UP_ARROW 'w'
#define LEFT_ARROW 'a'
#define RIGHT_ARROW 'd'
#define ACTION_MESSAGE ' '
#define ITEM1_BUTTON '1'
#define ITEM2_BUTTON '2'
#define ITEM3_BUTTON '3'
#define BUY1 'i'
#define BUY2 'o'
#define BUY3 'p'
#define BUY4 '0'

enum flagAccept{
    YES,
    NO
};

enum BoxType{
  NO_BOX,
  PAC,
  SEDEX,
  EXPRESS
};

typedef struct{
    unsigned short int timeLast;
    char type;
    unsigned char addIndex;
}Box;

enum Items{
   NO_ITEM,
   SHURICARD,
   TRAP1,
   TRAP2,
   CATIORO  
};

enum skin{
    JOSUE,
    JOSIAS,
    MATIAS
};
/*
enum MsgIdentifier{
    DOWN_ARROW = 48,
    UP_ARROW,
    LEFT_ARROW,
    RIGHT_ARROW,
    PACKAGE_BUTTON,
    ITEM1_BUTTON,
    ITEM2_BUTTON,
    ITEM3_BUTTON
};
*/
enum playerFace{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct{
    unsigned char ID;
    char HP;
    unsigned int money;
    unsigned char reputation;
    unsigned short posX, posY;
    Box boxArray[5];
    char holdingBoxes[2];//indica qual o index das caixas que o jogador está carregando(baseado no vetor de caixas para entregar);
    unsigned char itemArray[3];
    unsigned char skin;
    unsigned char face;
    char nome[20];
}Player_Data; 

typedef struct{
    unsigned short posX, posY;
    unsigned char face;
    unsigned char skin;
    char nome[20];
    char HP;
    
}Enemy_Data;

void readMap(char matrix[][44]);
void printMap(char matrix[][44]);

#endif