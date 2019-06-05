#include "Player.h"
#include "ACore.h"
#include "server.h"
#include <stdio.h>
#include <string.h>

unsigned char map[][1] = {{0},{0}};

enum message_status{
    OK,
    NOT_OK
};

void validateData(){
    Player_Data recieved_player;
    struct msg_ret_t ret = recvMsg(&recieved_player);


}