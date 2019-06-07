#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Player.h"

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 13
#define HIST_MAX_SIZE 200

enum conn_ret_t tryConnect() {
  char server_ip[30];
  char server_ID[3];
  strcpy(server_ip,"172.20.4.");
  printf("Please enter the server IP: ");
  scanf(" %s", server_ID);
  getchar();
  strcat(server_ip,server_ID);
  return connectToServer(server_ip);
  //return connectToServer("172.20.4.24");
}

void printHello() {
  puts("Hello! Welcome to simple chat example.");
  puts("We need some infos to start up!");
}

char assertConnection() {
  printHello();
  enum conn_ret_t ans = tryConnect();
  while (ans != SERVER_UP) {
    if (ans == SERVER_DOWN) {
      puts("Server is down!");
    } else if (ans == SERVER_FULL) {
      puts("Server is full!");
    } else if (ans == SERVER_CLOSED) {
      puts("Server is closed for new connections!");
    } else {
      puts("Server didn't respond to connection!");
    }
    printf("Want to try again? [Y/n] ");
    int res;
    while (res = tolower(getchar()), res != 'n' && res != 'y' && res != '\n'){
      puts("anh???");
    }
    if (res == 'n') {
      exit(EXIT_SUCCESS);
    }
    ans = tryConnect();
  }
  char login[LOGIN_MAX_SIZE + 4];
  printf("Please enter your login (limit = %d): ", LOGIN_MAX_SIZE);
  scanf(" %[^\n]", login);
  getchar();
  int len = (int)strlen(login);
  sendMsgToServer(login, len + 1);
  printf("Please enter your skin 0 1 2:");
  char skin;
  scanf(" %d",&skin);
  getchar();
  sendMsgToServer(&skin,1);

  return skin;
}

void runChat() {
  char str_buffer[BUFFER_SIZE], type_buffer[MSG_MAX_SIZE] = {0};
  char msg_history[HIST_MAX_SIZE][MSG_MAX_SIZE] = {{0}};
  int type_pointer = 0;
  
  while (1) {
    // LER UMA TECLA DIGITADA
    char ch = getch();
    if (ch == '\n') {
      type_buffer[type_pointer++] = '\0';
      int ret = sendMsgToServer((void *)type_buffer, type_pointer);
      if (ret == SERVER_DISCONNECTED) {
        return;
      }
      type_pointer = 0;
      type_buffer[type_pointer] = '\0';
    } else if (ch == 127 || ch == 8) {
      if (type_pointer > 0) {
        --type_pointer;
        type_buffer[type_pointer] = '\0';
      }
    } else if (ch != NO_KEY_PRESSED && type_pointer + 1 < MSG_MAX_SIZE) {
      type_buffer[type_pointer++] = ch;
      type_buffer[type_pointer] = '\0';
      
    }

    // LER UMA MENSAGEM DO SERVIDOR
    int ret = recvMsgFromServer(str_buffer, DONT_WAIT);
    if (ret == SERVER_DISCONNECTED) {
      return;
    } else if (ret != NO_MESSAGE) {
      int i;
      for (i = 0; i < HIST_MAX_SIZE - 1; ++i) {
        strcpy(msg_history[i], msg_history[i + 1]);
      }
      strcpy(msg_history[HIST_MAX_SIZE - 1], str_buffer);
    }

    // PRINTAR NOVO ESTADO DO CHAT
    system("clear");
    int i;
    for (i = 0; i < HIST_MAX_SIZE; ++i) {
      printf("%s\n", msg_history[i]);
    }
    printf("\nYour message: %s\n", type_buffer);
  }
}

int main() {
    int ret;
    Enemy_Data enemy;
    Player_Data player;
    player.skin = assertConnection();
    ret = recvMsgFromServer(&player, WAIT_FOR_IT);
    ret = recvMsgFromServer(&enemy, WAIT_FOR_IT);
    printf("Jogador: %s\nPosx = %d\nPosy = %d\n",player.nome,player.posX,player.posY);
    printf("Jogador: %s\nPosx = %d\nPosy = %d\n",enemy.nome,enemy.posX,enemy.posY);
    //getchar();

    int contagem = 0;
    
    strcpy(player.nome,"HAHaHA");
    char serverResponse;
    
    ret = recvMsgFromServer(&serverResponse, WAIT_FOR_IT);
    //while(serverResponse!=GAME_START) ret = recvMsgFromServer(&serverResponse, WAIT_FOR_IT);

    
    ret = sendMsgToServer(&player, sizeof(Player_Data));
    ret = sendMsgToServer(&player, sizeof(Player_Data));
    while(1){
    }
    /*
    strcpy(player.nome,"Josias");
    player.skin = JOSIAS;
    int ret = sendMsgToServer(&player, sizeof(Player_Data));
    printf("sizeof(Player_Data)=%d sizeof(player)=%d\n",sizeof(Player_Data),sizeof(player));
    printf("ret=%d nome=%s skin=%d",ret,player.nome,player.skin);
    */
    /*
    while(1) {
        runChat();
        printf("Server disconnected! Want to try again? [Y/N] ");
        int res;
        while (res = tolower(getchar()), res != 'n' && res != 'y' && res != '\n'){
        puts("anh???");
        }
        if (res == 'y' || res == '\n') {
        assertConnection();
        } else {
        break;
        }
    }
    */
  return 0;
}
