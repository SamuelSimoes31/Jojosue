#include "ACore.h"
#include "server.h"
#include <stdio.h>
#include <string.h>
#include "Player.h"

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 13
#define MAX_CHAT_CLIENTS 2

enum Game_states{
    WAITING_CON,
    IN_GAME,
    ENDGAME
};

int main() {
    puts("JOJOSUE SERVER IS UP, MAAAN\n");
    char serverState = WAITING_CON;
    Player_Data players[2];
    char client_names[MAX_CHAT_CLIENTS][LOGIN_MAX_SIZE];
    char str_buffer[BUFFER_SIZE], aux_buffer[BUFFER_SIZE];
    serverInit(MAX_CHAT_CLIENTS);
    puts("Server is running!!");
    
    char players_connected=0;
    while (serverState != ENDGAME) {

        while(serverState == WAITING_CON){
            int id = acceptConnection();
            if (id != NO_CONNECTION) {
                if(++players_connected == 1) serverState = IN_GAME;
                recvMsgFromClient(client_names[id], id, WAIT_FOR_IT);
                printf("%s connected id = %d\n", client_names[id], id);
            }
        }

        while(serverState == IN_GAME){
            Player_Data recieved_player;
            struct msg_ret_t ret = recvMsg(&recieved_player);
            
            if(ret.status==MESSAGE_OK){
                
                printf("Nome : %s\nSkin : %d\n",recieved_player.nome,recieved_player.skin);
                printf("Ret status = %d\nRet id = %d\nRet size = %d\n",ret.status, ret.client_id, ret.size);
                }
            
            //serverState = ENDGAME;
        }

        struct msg_ret_t msg_ret = recvMsg(aux_buffer);
        if (msg_ret.status == MESSAGE_OK) {
            sprintf(str_buffer, "%s-%d: %s", client_names[msg_ret.client_id],
                msg_ret.client_id, aux_buffer);
            broadcast(str_buffer, (int)strlen(str_buffer) + 1);
        } else if (msg_ret.status == DISCONNECT_MSG) {
            sprintf(str_buffer, "%s disconnected", client_names[msg_ret.client_id]);
            printf("%s disconnected, id = %d is free\n",
                client_names[msg_ret.client_id], msg_ret.client_id);
            broadcast(str_buffer, (int)strlen(str_buffer) + 1);
        }
  }
}
