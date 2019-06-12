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

enum Game_state{
    MAIN_MENU,
	PLAY_SCREEN,
	OPTION_SCREEN,
	WAITING_ENEMY,
	PRE_GAME,
	IN_GAME,
	RANKING_SCREEN,
	WIN_SCREEN,
	LOSE_SCREEN,
    ENDGAME
};

enum conn_ret_t tryConnect() {
	char server_ip[30];
	char server_ID[5];
	
	strcpy(server_ip,"172.20.4.");
	printf("Please enter the server ID: ");
	scanf(" %s", server_ID);
	getchar();
	strcat(server_ip,server_ID);
	
	// printf("Please enter the server IP: ");
	// scanf(" %s", server_ip);
	// getchar();
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


void attMatrix(char matrix[][44],int x, int y, char id){

}

int main() {

	int state = PLAY_SCREEN;
	//PRE_GAME
	
    char mapMatrix[][44] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,1,1,1,0,0,1,1,1,0,0,0,0,1,1,0,1,1,0,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    //char mapMatrix[30][44];
	//readMap(mapMatrix);
	char choice;
	char lastChoice = !choice;

	//PLAY_SCREEN
	Enemy_Data enemy;
	Player_Data player;
	Player_Data auxPlayer;

    while(state != ENDGAME){

		if(state == PLAY_SCREEN){
			int ret;
			player.skin = assertConnection();
			ret = recvMsgFromServer(&player, WAIT_FOR_IT);
			printf("Jogador: %s\nPosx = %d\nPosy = %d\n",player.nome,player.posX,player.posY);
			state=WAITING_ENEMY;
		}

		while(state == WAITING_ENEMY){
			printf("WAITING PLAYER\n");
			int ret = recvMsgFromServer(&enemy, WAIT_FOR_IT);
			printf("Jogador: %s\nPosx = %d\nPosy = %d\n",enemy.nome,enemy.posX,enemy.posY);

			char serverResponse;
			ret = recvMsgFromServer(&serverResponse, WAIT_FOR_IT);
			if(serverResponse==99) state = IN_GAME;
			printf("GAME_START RECEBIDO\n");		
		}

		while(state == PRE_GAME){
			//readMap(mapMatrix);
			mapMatrix[player.posY][player.posX] = '+'; //D
			mapMatrix[enemy.posY][enemy.posX] = '*'; //D
			state = IN_GAME;
		}
		//Debug
		//printMap(mapMatrix);

		while(state == IN_GAME){
			choice = getch();
			if(choice!=0){
				if(choice=='w'||choice=='a'||choice=='s'||choice=='d'||choice==ITEM1_BUTTON||choice==ITEM2_BUTTON||choice==ITEM3_BUTTON){
					sendMsgToServer((char *)&choice,1);
					//printf("%d\n",choice);
					lastChoice = choice;
				}
				else if(choice==BUY1||choice==BUY2||choice==BUY3){
					if(player.ID==0){
						if(player.posX==1&&player.posY==1){
							sendMsgToServer((char *)&choice,1);
							//printf("%d\n",choice);
							lastChoice = choice;
						}
					}
					else{
						if(player.posX==42&&player.posY==27){
							sendMsgToServer((char *)&choice,1);
							//printf("%d\n",choice);
							lastChoice = choice;
						}
					}
				}
			}
			int ret = recvMsgFromServer(&auxPlayer,DONT_WAIT);
			if(ret == SERVER_DISCONNECTED){
				printf("O SERVER MORREU VEY\n");
			}
			else if(ret != NO_MESSAGE){
				if(auxPlayer.ID == player.ID){ //se for a estrutura deste jogador
					mapMatrix[player.posY][player.posX] = 0;
					mapMatrix[auxPlayer.posY][auxPlayer.posX] = '+';
					//printf("%c - %d - %d\n",mapMatrix[auxPlayer.posY][auxPlayer.posX],auxPlayer.posY,auxPlayer.posX);
					player = auxPlayer;	
					//printf("Nova posicao do jogador: %d(x) %d(y)\n",player.posX,player.posY);
					if(player.HP <= 0){
						state = LOSE_SCREEN;
					}
				}
				else{ //se for a estrututura do cliente
					mapMatrix[enemy.posY][enemy.posX] = 0;
					mapMatrix[auxPlayer.posY][auxPlayer.posX] = '*';
					enemy.posX = auxPlayer.posX;
					enemy.posY = auxPlayer.posY;
					enemy.HP = auxPlayer.HP;
					//printf("-----------------%d\n",enemy.HP);
					//printf("Nova posicao do jogador inimigo: %d(x) %d(y)\n",enemy.posX,enemy.posY);
					if(enemy.HP <= 0){
						state = WIN_SCREEN;
					}
				}
			printMap(mapMatrix);
			printf("[%d][%d][%d] - HP: %d - TAOK's: %d\n",player.itemArray[0],player.itemArray[1],player.itemArray[2],player.HP,player.money);
			}
			

		}
		
		closeConnection();

		if(state == WIN_SCREEN){
			printf("TU GANHOU RAPA!\n");
		}
		else if(state == LOSE_SCREEN){
			printf("TU PERDEU RAPA!\n");
		}

	}
  return 0;
}
