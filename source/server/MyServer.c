#include "ACore.h"
#include "server.h"
#include <stdio.h>
#include <string.h>
#include "Player.h"
#include "ServerRule.h"

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 13
#define MAX_CLIENTS 2



const char GAME_START = 99;
enum Game_state{
    WAITING_CON,
    IN_GAME,
    ENDGAME
};

int main() {
    puts("JOJOSUE SERVER IS UP, MAAAN\n");
    char serverState = WAITING_CON;
    Player_Data players[2];
    printf("sizeof(Player_Data)=%d\n",sizeof(Player_Data));
    //char client_names[MAX_CHAT_CLIENTS][LOGIN_MAX_SIZE];
    char str_buffer[BUFFER_SIZE], aux_buffer[BUFFER_SIZE];
    serverInit(MAX_CLIENTS);
    puts("Server is running!!\n");
    
    char mapMatrix[][44] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,1,1,1,0,0,1,1,1,0,0,0,0,1,1,0,1,1,0,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    //char mapMatrix[30][44];
    //readMap(mapMatrix);
    //printf("OLA\n");
    char players_connected=0;
    while (serverState != ENDGAME) {
        while(serverState == WAITING_CON){
            int id = acceptConnection();
            if (id != NO_CONNECTION) {
                if(players_connected++ < 2){
                    recvMsgFromClient(players[id].nome, id, WAIT_FOR_IT);
                    recvMsgFromClient(&players[id].skin, id, WAIT_FOR_IT);
                    printf("%s connected id = %d players_connected=%d\n", players[id].nome, id,players_connected);
                    players[id].ID = id;
                    players[id].HP = 3;
                    players[id].reputation = 100;
                    players[id].face = (id==0 ? DOWN:UP);
                    players[id].money = 1000;
                    players[id].posX = (id==0 ? 1:42);
                    players[id].posY = (id==0 ? 1:27);
                    int i;
                    for(i=0;i<3;i++) players[id].itemArray[i] = NO_ITEM;
                    for(i=0;i<5;i++) {
                        players[id].boxArray[i].type = NO_BOX;
                        players[id].boxArray[i].timeLast = 0;
                    }
                    players[id].holdingBoxes[0] = players[id].holdingBoxes[1] = -1;

                    sendMsgToClient((Player_Data *)&players[id],sizeof(Player_Data),id);
                }
            }
            struct msg_ret_t msg_ret = recvMsg(aux_buffer);
            if(msg_ret.status == DISCONNECT_MSG){
                --players_connected;
                printf("%s disconnected id: %d\n",players[msg_ret.client_id].nome,msg_ret.client_id);
                printf("players_connected: %d\n",players_connected);
            }

            if(players_connected==2){
                int i;
                Enemy_Data enemy;
                for(i=0;i<2;i++){
                    enemy.posX = players[i].posX;
                    enemy.posY = players[i].posY;
                    enemy.face = players[i].face;
                    enemy.skin = players[i].skin;
                    enemy.HP = 3;
                    strcpy(enemy.nome,players[i].nome);
                    sendMsgToClient((Enemy_Data *)&enemy,sizeof(Enemy_Data),(i==0?1:0));
                }
                serverState = IN_GAME;
                broadcast((char *)&GAME_START,1);
            }
        }
        char typeOfChange;
        printf("Estado IN_GAME!\n");
        while(serverState == IN_GAME){

            struct msg_ret_t ret = recvMsg(&typeOfChange);

            if(ret.status==MESSAGE_OK){
                mapMatrix[players[0].posY][players[0].posX] = '+';
                mapMatrix[players[1].posY][players[1].posX] = '*';
                //printf("%d\n",typeOfChange);
                if(typeOfChange == UP_ARROW){
                    if(players[ret.client_id].posY-1>=0 && mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX]!=1){
                        players[ret.client_id].posY--;
                        players[ret.client_id].face = UP;
                        if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'X') players[ret.client_id].HP -= 1;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Y') players[ret.client_id].HP -= 3;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Z') players[ret.client_id].HP -= 5;
                        broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                    }
                }

                else if(typeOfChange == DOWN_ARROW){
                    if(players[ret.client_id].posY+1<30 && mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX]!=1){
                        players[ret.client_id].posY++;
                        players[ret.client_id].face = DOWN;
                        if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'X') players[ret.client_id].HP -= 1;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Y') players[ret.client_id].HP -= 3;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Z') players[ret.client_id].HP -= 5;
                        broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                    }
                }

                else if(typeOfChange == LEFT_ARROW){
                    if(players[ret.client_id].posX-1>=0 && mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1]!=1){
                        players[ret.client_id].posX--;
                        players[ret.client_id].face = LEFT;
                        if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'X') players[ret.client_id].HP -= 1;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Y') players[ret.client_id].HP -= 3;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Z') players[ret.client_id].HP -= 5;
                        broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                    }
                }

                else if(typeOfChange == RIGHT_ARROW){
                    if(players[ret.client_id].posY+1<44 && mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1]!=1){
                        players[ret.client_id].posX++;
                        players[ret.client_id].face = RIGHT;
                        if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'X') players[ret.client_id].HP -= 1;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Y') players[ret.client_id].HP -= 3;
                        else if(mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX] == 'Z') players[ret.client_id].HP -= 5;
                        broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                    }
                }

                else if(typeOfChange == PACKAGE_BUTTON){}

                else if(typeOfChange == ITEM1_BUTTON){

                    if(players[ret.client_id].itemArray[0] != NO_ITEM){    

                        char typeOfItem;

                        switch(players[ret.client_id].itemArray[0]){

                            case SHURICARD: 

                                            break;

                            default: if(players[ret.client_id].itemArray[0]==TRAP1) typeOfItem = 'X';
                                     else if(players[ret.client_id].itemArray[0]==TRAP2) typeOfItem = 'Y';
                                     else typeOfItem = 'Z';
                                     break;

                        }

                        if(players[ret.client_id].itemArray[0]!=SHURICARD){
                            if(players[ret.client_id].face == UP){
                                if((mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX] != 1) && (players[ret.client_id].posY>0)){
                                mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX] = typeOfItem; 
                                }
                            }

                            else if(players[ret.client_id].face == DOWN){
                                if((mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX] != 1) && (players[ret.client_id].posY<30)){
                                    mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX] = typeOfItem;
                                }
                            }

                            else if(players[ret.client_id].face == LEFT){
                                if((mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1] != 1) && (players[ret.client_id].posX>0)){
                                mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1] = typeOfItem; 
                                }
                            }

                            else{
                                if((mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1] != 1) && (players[ret.client_id].posX<44)){
                                mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1] = typeOfItem; 
                                }
                            }
                        }

                        players[ret.client_id].itemArray[0] = NO_ITEM;
                        sendMsgToClient(&players[ret.client_id],sizeof(Player_Data),ret.client_id);
                    }

                }

                else if(typeOfChange == ITEM2_BUTTON){

                    if(players[ret.client_id].itemArray[1] != NO_ITEM){    

                        char typeOfItem;

                        switch(players[ret.client_id].itemArray[1]){

                            case SHURICARD: printf("EL ninja\n");

                                            break;

                            default: if(players[ret.client_id].itemArray[1]==TRAP1) typeOfItem = 'X';
                                     else if(players[ret.client_id].itemArray[1]==TRAP2) typeOfItem = 'Y';
                                     else typeOfItem = 'Z';
                                     break;

                        }

                        if(players[ret.client_id].itemArray[1]!=SHURICARD){
                            if(players[ret.client_id].face == UP){
                                if((mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX] != 1) && (players[ret.client_id].posY>0)){
                                mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX] = typeOfItem; 
                                }
                            }

                            else if(players[ret.client_id].face == DOWN){
                                if((mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX] != 1) && (players[ret.client_id].posY<30)){
                                    mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX] = typeOfItem;
                                }
                            }

                            else if(players[ret.client_id].face == LEFT){
                                if((mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1] != 1) && (players[ret.client_id].posX>0)){
                                mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1] = typeOfItem; 
                                }
                            }

                            else{
                                if((mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1] != 1) && (players[ret.client_id].posX<44)){
                                mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1] = typeOfItem; 
                                }
                            }
                        }

                        players[ret.client_id].itemArray[1] = NO_ITEM;
                        sendMsgToClient(&players[ret.client_id],sizeof(Player_Data),ret.client_id);
                    }

                }

                else if(typeOfChange == ITEM3_BUTTON){

                    if(players[ret.client_id].itemArray[2] != NO_ITEM){    

                        char typeOfItem;

                        switch(players[ret.client_id].itemArray[2]){

                            case SHURICARD: printf("EL ninja\n");

                                            break;

                            default: if(players[ret.client_id].itemArray[2]==TRAP1) typeOfItem = 'X';
                                     else if(players[ret.client_id].itemArray[2]==TRAP2) typeOfItem = 'Y';
                                     else typeOfItem = 'Z';
                                     break;

                        }

                        if(players[ret.client_id].itemArray[2]!=SHURICARD){
                            if(players[ret.client_id].face == UP){
                                if((mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX] != 1) && (players[ret.client_id].posY>0)){
                                mapMatrix[players[ret.client_id].posY-1][players[ret.client_id].posX] = typeOfItem; 
                                }
                            }

                            else if(players[ret.client_id].face == DOWN){
                                if((mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX] != 1) && (players[ret.client_id].posY<30)){
                                    mapMatrix[players[ret.client_id].posY+1][players[ret.client_id].posX] = typeOfItem;
                                }
                            }

                            else if(players[ret.client_id].face == LEFT){
                                if((mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1] != 1) && (players[ret.client_id].posX>0)){
                                mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX-1] = typeOfItem; 
                                }
                            }

                            else{
                                if((mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1] != 1) && (players[ret.client_id].posX<44)){
                                mapMatrix[players[ret.client_id].posY][players[ret.client_id].posX+1] = typeOfItem; 
                                }
                            }
                        }

                        players[ret.client_id].itemArray[2] = NO_ITEM;
                        sendMsgToClient(&players[ret.client_id],sizeof(Player_Data),ret.client_id);
                    }

                }

                else if(typeOfChange == BUY1){
                    if(players[ret.client_id].money >= 50){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = SHURICARD;
                                players[ret.client_id].money -= 50;
                                break;
                            }
                        }
                    }
                    sendMsgToClient(&players[ret.client_id],sizeof(Player_Data),ret.client_id);
                }

                else if(typeOfChange == BUY2){
                    if(players[ret.client_id].money >= 75){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = SHURICARD;
                                players[ret.client_id].money -= 75;
                                break;
                            }
                        }
                    }
                    sendMsgToClient(&players[ret.client_id],sizeof(Player_Data),ret.client_id);
                }

                else if(typeOfChange == BUY3){
                    if(players[ret.client_id].money >= 100){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = SHURICARD;
                                players[ret.client_id].money -= 100;
                                break;
                            }
                        }
                    }
                    sendMsgToClient(&players[ret.client_id],sizeof(Player_Data),ret.client_id);
                }

                else if(typeOfChange == BUY4){
                    if(players[ret.client_id].money >= 150){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = SHURICARD;
                                players[ret.client_id].money -= 150;
                                break;
                            }
                        }
                    }
                    sendMsgToClient(&players[ret.client_id],sizeof(Player_Data),ret.client_id);
                }

                else {
                    printf("Invalid message!\nret.status=%d\n%d\n",ret.status,typeOfChange);
                }
                mapMatrix[players[0].posY][players[0].posX] = '+';
                mapMatrix[players[1].posY][players[1].posX] = '*';
                printMap(mapMatrix);
            }
            else if(ret.status==DISCONNECT_MSG){
                printf("PLAYER(%d) DESCONECTOU\n",ret.client_id);
            }
            
            //serverState = ENDGAME;
        }

        struct msg_ret_t msg_ret = recvMsg(aux_buffer);
        if (msg_ret.status == MESSAGE_OK) {
            sprintf(str_buffer, "%s-%d: %s", players[msg_ret.client_id].nome,
                msg_ret.client_id, aux_buffer);
            broadcast(str_buffer, (int)strlen(str_buffer) + 1);
        } else if (msg_ret.status == DISCONNECT_MSG) {
            sprintf(str_buffer, "%s disconnected", players[msg_ret.client_id].nome);
            printf("%s disconnected, id = %d is free\n",
                players[msg_ret.client_id].nome, msg_ret.client_id);
            broadcast(str_buffer, (int)strlen(str_buffer) + 1);
        }
  }
  
}


