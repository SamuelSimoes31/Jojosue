#include "ACore.h"
#include "server.h"
#include <stdio.h>
#include <string.h>
#include "Player.h"

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
    Enemy_Data enemy;
    //char client_names[MAX_CHAT_CLIENTS][LOGIN_MAX_SIZE];
    char str_buffer[BUFFER_SIZE], aux_buffer[BUFFER_SIZE];
    serverInit(MAX_CLIENTS);
    puts("Server is running!!");
    
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
                    players[id].HP = 5;
                    players[id].face = (id==0 ? DOWN:UP);
                    players[id].money = 0;
                    players[id].posX = (id==0 ? 0:100);
                    players[id].posY = (id==0 ? 0:100);
                    int i;
                    for(i=0;i<3;i++) players[id].itemArray[i] = NO_ITEM;
                    for(i=0;i<5;i++) {
                        players[id].boxArray[i].type = NO_BOX;
                        players[id].boxArray[i].timeLast = 0;
                    }
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
                for(i=0;i<2;i++){
                    enemy.posX = players[i].posX;
                    enemy.posY = players[i].posY;
                    enemy.face = players[i].face;
                    enemy.skin = players[i].skin;
                    strcpy(enemy.nome,players[i].nome);
                    sendMsgToClient((Enemy_Data *)&enemy,sizeof(Enemy_Data),(i==0?1:0));
                }
                serverState = IN_GAME;
                broadcast((char *)&GAME_START,1);
                //broadcast((Player_Data *),size);
            }
        }
        char typeOfChange;
        printf("Estado IN_GAME!\n");
        while(serverState == IN_GAME){
            Player_Data recieved_player;

            struct msg_ret_t ret = recvMsg(&typeOfChange);

            if(ret.status==MESSAGE_OK){

                if(typeOfChange == UP_ARROW){  
                    players[ret.client_id].posY--;
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                    printf("new pos of (user that id is:%d)is :%d\n",ret.client_id,players[ret.client_id].posY);
                }

                else if(typeOfChange == DOWN_ARROW){
                    players[ret.client_id].posY++;
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                    //sendMsgToClient((Player_Data *)&players[ret.client_id],sizeof(Player_Data),ret.client_id);
                    printf("new pos of (user that id is:%d)is :%d\n",ret.client_id,players[ret.client_id].posY);
                }

                else if(typeOfChange == LEFT_ARROW){
                    players[ret.client_id].posX--;
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                    //printf("Chegou aqui o enum!\n");
                    printf("new pos of (user that id is:%d)is :%d\n",ret.client_id,players[ret.client_id].posY);
                }

                else if(typeOfChange == RIGHT_ARROW){
                    players[ret.client_id].posX++;
                    broadcast((Player_Data *)&players[ret.client_id],sizeof(Player_Data));
                    printf("new pos of (user that id is:%d)is :%d\n",ret.client_id,players[ret.client_id].posY);
                }

                else if(typeOfChange == PACKAGE_BUTTON){}

                else if(typeOfChange == ITEM1_BUTTON){}

                else if(typeOfChange == ITEM1_BUTTON){}

                else if(typeOfChange == ITEM1_BUTTON){}


                else puts("Invalid message!");
            }
            else if(ret.status==DISCONNECT_MSG){
                                
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
