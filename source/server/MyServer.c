//#include "ACore.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "Player.h"
#include "ServerRule.h"
#include "server.h"

//ALA CHAT:
//  é possivel falarmos por aqui também qualquer coisa
//  bora primeiro ver se compila
//  eu n sei se o problema é no codigo ou no makeclient, pq tipo aparentemente ta tudo certinho aqui
//  taca printf em tudo. Em algum lugar tem q pegar alguns
// o makefile consertei aparetemente, ele chegou a commpilar, mas ao rodar ele dava core dumped
// gcc teste.c -o dale -lm -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

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
    ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_TIMER *decrementTimer = NULL;
    
int main() {

    char mapMatrix[][44] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,1,1,1,0,0,1,1,1,0,0,0,0,1,1,0,1,1,0,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    char str_buffer[BUFFER_SIZE], aux_buffer[BUFFER_SIZE];
    Player_Data players[2];
    char serverState = WAITING_CON;

    char houses[] = {1,2,4,5,6,7,8,17,19,20,21,22,23,24,40,42,43,44,45,46,48,50,60,61,62,64,66,68,69,71,72,73,74,75,76,77,88};
    

    puts("JOJOSUE SERVER IS UP, MAAAN\n");
    
    
    //printf("sizeof(Player_Data)=%d\n",sizeof(Player_Data));
    //char client_names[MAX_CHAT_CLIENTS][LOGIN_MAX_SIZE];
    serverInit(MAX_CLIENTS);
    puts("Server is running!!\n");
    
    if(!al_init())
       puts("Falha ao iniciar allegro\n");

    timer = al_create_timer(6.0); //bota sempre .0 nos numeros itneiros, pra ele interpretar como double
    if(!timer) puts("timer == NULL");
    else puts("Timer funfou");
    decrementTimer = al_create_timer(1.0);
    if(!decrementTimer) puts("decrementTimer == NULL");
    else puts("decrementTimer funfou");
    eventQueue = al_create_event_queue();


    //char mapMatrix[30][44];
    //readMap(mapMatrix);
    printf("OLA PROGRAMADOR\n");
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
                    players[id].HP = 10;
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
                    enemy.HP = 10;
                    strcpy(enemy.nome,players[i].nome);
                    sendMsgToClient((Enemy_Data *)&enemy,sizeof(Enemy_Data),(i==0?1:0));
                }
                serverState = IN_GAME;
                broadcast((char *)&GAME_START,1);
            }
        }
        char typeOfChange;
        printf("Estado IN_GAME!\n");

        al_start_timer(timer);
        al_start_timer(decrementTimer);

        while(serverState == IN_GAME){

            struct msg_ret_t ret = recvMsg(&typeOfChange);

            if(ret.status==MESSAGE_OK){
                mapMatrix[players[0].posY][players[0].posX] = '0';
                mapMatrix[players[1].posY][players[1].posX] = '0';
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

                else if(typeOfChange == ACTION_MESSAGE){
                    //printf("ENTROU!!!\n");
                    //printf("x = %d ---- y = %d\n");
                    if((players[ret.client_id].posX==21&&players[ret.client_id].posY==11)||(players[ret.client_id].posX==21&&players[ret.client_id].posY==16)){

                        if(players[ret.client_id].HP<10 && players[ret.client_id].money >= 100){
                            players[ret.client_id].HP += 3;
                            players[ret.client_id].money -= 100;
                            if(players[ret.client_id].HP>10) players[ret.client_id].HP = 10;
                        }

                    }
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));

                }

                else if(typeOfChange == ITEM1_BUTTON){

                    if(players[ret.client_id].itemArray[0] != NO_ITEM){    

                        char typeOfItem;

                        switch(players[ret.client_id].itemArray[0]){

                            case SHURICARD: if(players[ret.client_id].face == UP){
                                                for(int i = players[ret.client_id].posY; mapMatrix[i][players[ret.client_id].posX]!='1'&&(players[ret.client_id].posY - i < 7);i--){
                                                    if(players[!(ret.client_id)].posY == i && players[!(ret.client_id)].posX == players[ret.client_id].posX){
                                                         players[!(ret.client_id)].HP -= 1;
                                                         break;
                                                         }
                                                }
                                            }
                                            else if(players[ret.client_id].face == DOWN){
                                                for(int i = players[ret.client_id].posY; mapMatrix[i][players[ret.client_id].posX]!='1'&&(i - players[ret.client_id].posY < 7);i++){
                                                    if(players[!(ret.client_id)].posY == i && players[!(ret.client_id)].posX == players[ret.client_id].posX){
                                                         players[!(ret.client_id)].HP -= 1;
                                                         break;
                                                         }
                                                }
                                            }
                                            else if(players[ret.client_id].face == RIGHT){
                                                for(int i = players[ret.client_id].posX; mapMatrix[players[ret.client_id].posY][i]!='1'&&(i - players[ret.client_id].posX < 7);i++){
                                                    if(players[!(ret.client_id)].posY == players[ret.client_id].posY && players[!(ret.client_id)].posX == i){
                                                         players[!(ret.client_id)].HP -= 1;
                                                         break;
                                                         }
                                                }
                                            }
                                            else{
                                                for(int i = players[ret.client_id].posX; mapMatrix[players[ret.client_id].posY][i]!='1'&&(players[ret.client_id].posX - i < 7);i--){
                                                    if(players[!(ret.client_id)].posY == players[ret.client_id].posY && players[!(ret.client_id)].posX == i){
                                                         players[!(ret.client_id)].HP -= 1;
                                                         break;
                                                         }
                                                }
                                            }

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
                        broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                    }

                }

                else if(typeOfChange == ITEM2_BUTTON){

                    if(players[ret.client_id].itemArray[1] != NO_ITEM){    

                        char typeOfItem;

                        switch(players[ret.client_id].itemArray[1]){

                            case SHURICARD: if(players[ret.client_id].face == UP){
                                                for(int i = players[ret.client_id].posY; mapMatrix[i][players[ret.client_id].posX]!='1'&&(players[ret.client_id].posY - i < 7);i--){
                                                    if(players[!(ret.client_id)].posY == i && players[!(ret.client_id)].posX == players[ret.client_id].posX){
                                                         players[!(ret.client_id)].HP -= 1;
                                                         break;
                                                         }
                                                }
                                            }
                                            else if(players[ret.client_id].face == DOWN){
                                                for(int i = players[ret.client_id].posY; mapMatrix[i][players[ret.client_id].posX]!='1'&&(i - players[ret.client_id].posY < 7);i++){
                                                    if(players[!(ret.client_id)].posY == i && players[!(ret.client_id)].posX == players[ret.client_id].posX){
                                                         players[!(ret.client_id)].HP -= 1;
                                                         break;
                                                         }
                                                }
                                            }
                                            else if(players[ret.client_id].face == RIGHT){
                                                for(int i = players[ret.client_id].posX; mapMatrix[players[ret.client_id].posY][i]!='1'&&(i - players[ret.client_id].posX < 7);i++){
                                                    if(players[!(ret.client_id)].posY == players[ret.client_id].posY && players[!(ret.client_id)].posX == i){
                                                         players[!(ret.client_id)].HP -= 1;
                                                         break;
                                                         }
                                                }
                                            }
                                            else{
                                                for(int i = players[ret.client_id].posX; mapMatrix[players[ret.client_id].posY][i]!='1'&&(players[ret.client_id].posX - i < 7);i--){
                                                    if(players[!(ret.client_id)].posY == players[ret.client_id].posY && players[!(ret.client_id)].posX == i){
                                                         players[!(ret.client_id)].HP -= 1;
                                                         break;
                                                         }
                                                }
                                            }

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
                        broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                    }

                }

                else if(typeOfChange == ITEM3_BUTTON){

                    if(players[ret.client_id].itemArray[2] != NO_ITEM){    

                        char typeOfItem;

                        switch(players[ret.client_id].itemArray[2]){

                            case SHURICARD: if(players[ret.client_id].face == UP){
                                                for(int i = players[ret.client_id].posY; mapMatrix[i][players[ret.client_id].posX]!='1'&&(players[ret.client_id].posY - i < 7);i--){
                                                    if(players[!(ret.client_id)].posY == i && players[!(ret.client_id)].posX == players[ret.client_id].posX){
                                                         players[!(ret.client_id)].HP -= 1;
                                                         break;
                                                         }
                                                }
                                            }
                                            else if(players[ret.client_id].face == DOWN){
                                                for(int i = players[ret.client_id].posY; mapMatrix[i][players[ret.client_id].posX]!='1'&&(i - players[ret.client_id].posY < 7);i++){
                                                    if(players[!(ret.client_id)].posY == i && players[!(ret.client_id)].posX == players[ret.client_id].posX){
                                                         players[!(ret.client_id)].HP -= 1;
                                                         break;
                                                         }
                                                }
                                            }
                                            else if(players[ret.client_id].face == RIGHT){
                                                for(int i = players[ret.client_id].posX; mapMatrix[players[ret.client_id].posY][i]!='1'&&(i - players[ret.client_id].posX < 7);i++){
                                                    if(players[!(ret.client_id)].posY == players[ret.client_id].posY && players[!(ret.client_id)].posX == i){
                                                         players[!(ret.client_id)].HP -= 1;
                                                         break;
                                                         }
                                                }
                                            }
                                            else{
                                                for(int i = players[ret.client_id].posX; mapMatrix[players[ret.client_id].posY][i]!='1'&&(players[ret.client_id].posX - i < 7);i--){
                                                    if(players[!(ret.client_id)].posY == players[ret.client_id].posY && players[!(ret.client_id)].posX == i){
                                                         players[!(ret.client_id)].HP -= 1;
                                                         break;
                                                         }
                                                }
                                            }

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
                        broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
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
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                }

                else if(typeOfChange == BUY2){
                    if(players[ret.client_id].money >= 75){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = TRAP1;
                                players[ret.client_id].money -= 75;
                                break;
                            }
                        }
                    }
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                }

                else if(typeOfChange == BUY3){
                    if(players[ret.client_id].money >= 100){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = TRAP2;
                                players[ret.client_id].money -= 100;
                                break;
                            }
                        }
                    }
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                }

                else if(typeOfChange == BUY4){
                    if(players[ret.client_id].money >= 150){
                       // printf("%d\n",players[ret.client_id].money);
                        for(int i=0;i<3;i++){
                            printf("%d\n",players[ret.client_id].itemArray[i]);
                            if(players[ret.client_id].itemArray[i] == NO_ITEM){
                                players[ret.client_id].itemArray[i] = CATIORO;
                                players[ret.client_id].money -= 150;
                                break;
                            }
                        }
                    }
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
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
            
            if(!al_is_event_queue_empty(eventQueue)){

                ALLEGRO_EVENT event;
                al_wait_for_event(eventQueue, &event);

                if(event.type == ALLEGRO_EVENT_TIMER){
                    
                    if(event.timer.source == decrementTimer){
                        for(int j = 0;j<2;j++){
                            for(int i = 0;i<5;i++){
                                if(players[j].boxArray[i].type != NO_BOX && players[j].boxArray[i].timeLast >0) players[j].boxArray[i].timeLast -= 1;
                                
                                if(players[j].boxArray[i].type != NO_BOX && players[j].boxArray[i].timeLast == 0){
                                    players[j].boxArray[i].type = NO_BOX;
                                    players[j].reputation -= 10;
                                }
                            }
                        }
                    }

                    else if(event.timer.source == timer){
                        int randomizer = rand()%4;
                        if(randomizer == 0) randomizer = 1;
                        switch(randomizer){

                            case 1: for(int i = 0; i<2; i++){
                                        for(int j = 0; j< 5; j++){
                                            if(players[i].boxArray[j].type == NO_BOX){
                                                players[i].boxArray[j].type = PAC;
                                                players[i].boxArray[j].timeLast = 80;
                                                break;
                                            }
                                        }
                                    }
                                    break; 

                            case 2: for(int i = 0; i<2; i++){
                                        for(int j = 0; j< 5; j++){
                                            if(players[i].boxArray[j].type == NO_BOX){
                                                players[i].boxArray[j].type = SEDEX;
                                                players[i].boxArray[j].timeLast = 60;
                                                break;
                                            }
                                        }
                                    }
                                    break; 
                            case 3: for(int i = 0; i<2; i++){
                                        for(int j = 0; j< 5; j++){
                                            if(players[i].boxArray[j].type == NO_BOX){
                                                players[i].boxArray[j].type = EXPRESS;
                                                players[i].boxArray[j].timeLast = 40;
                                                break;
                                            }
                                        }
                                    }
                                    break; 

                        }
                    }

                }

                for(int i = 0; i<2; i++){
                    broadcast((Player_Data *)&players[i],sizeof(Player_Data));
                }

            }
            
            //11 e 21
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
    al_destroy_timer(timer);
    al_destroy_timer(decrementTimer);
}


