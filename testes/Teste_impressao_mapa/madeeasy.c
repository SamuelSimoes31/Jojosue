// Inclui o cabeçalho da bibilioteca Allegro 5
//#include "ACore.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "Player.h"
#include <ctype.h>

#define LARGURA_TELA 1500
#define ALTURA_TELA 800

ALLEGRO_BITMAP *player_sprite;
ALLEGRO_BITMAP *enemy_sprite;
ALLEGRO_BITMAP *background;
ALLEGRO_BITMAP *heart_carta;
ALLEGRO_BITMAP *item_bar;
ALLEGRO_BITMAP *box_bar;
ALLEGRO_BITMAP *icon_shuricarta;
ALLEGRO_BITMAP *icon_trap;
ALLEGRO_BITMAP *icon_bomb;
ALLEGRO_BITMAP *icon_dog;
ALLEGRO_BITMAP *icon_boxes;
ALLEGRO_FONT *fonte_timer;
ALLEGRO_FONT *fonte_jogo;
ALLEGRO_SAMPLE *shurikarta;
ALLEGRO_SAMPLE *armadilhaDamage = NULL;
ALLEGRO_SAMPLE *armadilhaPlaced;

//int posX=1, posY=1;
//PLAY_SCREEN

Enemy_Data enemy;
Player_Data player;
Player_Data auxPlayer;

char nickname[NICKNAME_MAX_SIZE + 4];
char skin;

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

int state;

float cameraPosition[2] = {0,0}, scale = 3.0;
void cameraUpdate(float* cameraPosition, float x, float y, int width, int height ){
    int bordaX = al_get_bitmap_width(background) - LARGURA_TELA/scale;
    int bordaY = al_get_bitmap_height(background) - ALTURA_TELA/scale;
    cameraPosition[0] = -(LARGURA_TELA/2/scale) + (x + width/2);
    cameraPosition[1] = -(ALTURA_TELA/2/scale) + (y + height/2);
    if(cameraPosition[0] < 0) cameraPosition[0] = 0;
    else if(cameraPosition[0] > bordaX) cameraPosition[0] = bordaX;
    if(cameraPosition[1] < 0) cameraPosition[1] = 0;
    else if(cameraPosition[1] > bordaY) cameraPosition[1] = bordaY;
    
    //if(cameraPosition[0] > ALTURA_TELA/); cameraPosition[0] = ALTURA_TELA;
    //printf("al_get_bitmap_width(background)=%d - %d\n",al_get_bitmap_width(background),LARGURA_TELA/scale);
    //printf("x=%g y=%g cameraPosition[0]=%g cameraPosition[1]=%g scale=%g \n",x,y,cameraPosition[0],cameraPosition[1],scale);
}

enum conn_ret_t tryConnect() {
	char server_ip[30];
	
	// char server_ID[5];
	// strcpy(server_ip,"172.20.4.");
	// printf("Please enter the server ID: ");
	// scanf(" %s", server_ID);
	// getchar();
	// strcat(server_ip,server_ID);
	
	printf("Please enter the server IP: ");
	scanf(" %s", server_ip);
	getchar();
	return connectToServer(server_ip);
	//return connectToServer("127.0.0.1");
}

void assertConnection() {
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
	
	printf("Please enter your nickname (limit = %d): ", NICKNAME_MAX_SIZE );
	scanf(" %[^\n]", nickname);
	getchar();
	int len = (int)strlen(nickname);
	sendMsgToServer(nickname, len + 1);
	printf("Please enter your skin 0 1 2:");
	
	scanf(" %d",&skin);
	getchar();
	sendMsgToServer(&skin,1);
    //player.skin = skin;
    switch(skin){
        case JOSUE:
            player_sprite = al_load_bitmap("source/resources/images/characters/Josue(com limite).png");
            if(!player_sprite){
                puts("Errou ao carregar Josue, muito pesado ele.");
                state = ENDGAME;
            }
            break;
        case JOSIAS:
            player_sprite = al_load_bitmap("source/resources/images/characters/Josias(com limite).png");
            if(!player_sprite){
                puts("Errou ao carregar Josias, muito alto ele.");
                state = ENDGAME;
            }
            break;
        case MATIAS:
            player_sprite = al_load_bitmap("source/resources/images/characters/Matias(com limite).png");
            if(!player_sprite){
                puts("Errou ao carregar Matias, muito rápido ele.");
                state = ENDGAME;
            }
            break;
        default:
            player_sprite = al_load_bitmap("source/resources/images/characters/Al Fredo(com limite).png");
            if(!player_sprite){
                puts("Errou ao carregar Al Fredo, muito secreto ele.");
                state = ENDGAME;
            }
            break;
    }
}

void destruirBitmaps(){
    al_destroy_bitmap(player_sprite);
    al_destroy_bitmap(enemy_sprite);
    al_destroy_bitmap(background);
    al_destroy_bitmap(heart_carta);
    al_destroy_bitmap(icon_dog);
    al_destroy_bitmap(icon_boxes);
    al_destroy_bitmap(item_bar);
    al_destroy_bitmap(box_bar);    
    al_destroy_bitmap(icon_shuricarta);
    al_destroy_bitmap(icon_trap);
    al_destroy_bitmap(icon_bomb); 
}

int main()
{
    state = PLAY_SCREEN;
    ALLEGRO_DISPLAY *display;

    //enum direction {DOWN, UP, LEFT, RIGHT};

    const float FPS = 60.0;
    const float frameFPS = 12.0;

    if(!al_init()){
        puts("Fala ao iniciar allegro\n");
        exit(-1);
    }

    display = al_create_display(LARGURA_TELA,ALTURA_TELA);
    if(!display){
        puts("Fala ao iniciar display.\n");
        al_destroy_display(display);
        exit(-1);
    }

    if(!al_install_audio()){
        printf("Falha ao inicializar o audio");
        return 0;
    }
 
    //addon que da suporte as extensoes de audio
    if(!al_init_acodec_addon()){
        printf("Falha ao inicializar o codec de audio");
        return 0;
    }
 
    //cria o mixer (e torna ele o mixer padrao), e adciona 5 samples de audio nele
    if (!al_reserve_samples(5)){
        printf("Falha ao reservar amostrar de audio");
        return 0;
    }

    shurikarta = al_load_sample("source/resources/audio/samples/shurikarta.wav");
    if (!shurikarta) {
        printf( "Audio nao carregado.");
        return 0;
    }

    /*armadilhaDamage = al_load_sample("source/resources/audio/samples/armadilha.wav");
    if (!armadilhaDamage) {
        printf( "Audio nao carregado..");
        return 0;
    }*/

    armadilhaPlaced = al_load_sample("source/resources/audio/samples/armadilhaPlaced.wav");
    if (!armadilhaPlaced) {
        printf( "Audio nao carregado...");
        return 0;
    }

    
    //al_set_window_position(display,200,200);
    int passos=16, passosCounter=0, passosCounterEnemy=0;//quantos saltos de moveSpeed ele vai fazer até chegar em 32
    bool done = false, draw = true, animate = false, animateEnemy=false ,active = false/*,comprou=true*/;
    char comprou = NO_ITEM;
    float x=0, y=0, ENx=0, ENy=0, moveSpeed = 32/(float)passos;// moveCounter = 0;
    unsigned short oldPosX, oldPosY, oldPosEnemyX, oldPosEnemyY;
    int sourceX = 32, sourceY = 0, sourceEnemyX = 32, sourceEnemyY=0;
    char use=0;
    int cnt60=0, cnt240=0;
    char houses[] = {1,2,4,5,6,7,8,17,19,20,21,22,23,24,40,42,43,44,45,46,48,50,60,61,62,64,66,68,69,71,72,73,74,75,76,77,88,-1};
    //moveSpeed = 32/frameFPS,

    al_install_keyboard();
    al_init_image_addon();

    background = al_load_bitmap("source/resources/images/backgrounds/MapaComLimites.png");
    if(!background){
        puts("Errou ao carregar Mapa, prefiro o de 26.0 GB.");
        exit(0);
    }

    heart_carta = al_load_bitmap("source/resources/images/Life.png");
    if(!heart_carta){
        puts("Falha ao carregar heart_carta.\n");
        destruirBitmaps();
        exit(-1);
        state = ENDGAME;
    }

    item_bar = al_load_bitmap("source/resources/images/Barra_de_Itens.png");
    if(!item_bar){
        puts("Falha ao carregar Barra_de_Itens.\n");
        state = ENDGAME;
    }
    box_bar = al_load_bitmap("source/resources/images/Barra_de_Caixas.png");
    if(!box_bar){
        puts("Falha ao carregar Barra_de_Caixas.\n");
        state = ENDGAME;
    }

    icon_shuricarta = al_load_bitmap("source/resources/images/A_Shurikarta(com limites).png");
    if(!icon_shuricarta){
        puts("Falha ao carregar A_Shurikarta.\n");
        state = ENDGAME;
    }

    icon_trap = al_load_bitmap("source/resources/images/Armadilha_V1(com limites).png");
    if(!icon_trap){
        puts("Falha ao carregar Armadilha_V1.\n");
        state = ENDGAME;
    }
    icon_bomb = al_load_bitmap("source/resources/images/Armadilha_V2.png");
    if(!icon_bomb){
        puts("Falha ao carregar Armadilha_V2.\n");
        state = ENDGAME;
    }
    icon_dog = al_load_bitmap("source/resources/images/El_Catioro(com limites).png");
    if(!icon_dog){
        puts("Falha ao carregar El_Catioro.\n");
        state = ENDGAME;
    }
    icon_boxes = al_load_bitmap("source/resources/images/Caixas.png");
    if(!icon_boxes){
        puts("Falha ao carregar Caixas.\n");
        state = ENDGAME;
    }
    
    printf("Antes da fonte\n");
    al_init_font_addon();
    if (!al_init_ttf_addon()){
        printf("Falha ao inicializar add-on allegro_ttf");
        state = ENDGAME;
    }
    else{
        fonte_timer = al_load_font("source/resources/fonts/pressStart.ttf", 7, 0); 
        fonte_jogo = al_load_font("source/resources/fonts/pressStart.ttf", 10, 0);
        if (!fonte_jogo || !fonte_timer){
            printf("Falha ao carregar fonte\n");
            state = ENDGAME;
        }
    }
    printf("Depois da fonte\n");
    


    if(state == ENDGAME) {
        destruirBitmaps();
        al_destroy_display(display);
        exit(EXIT_FAILURE);
    }
    int larguraCarta = al_get_bitmap_width(heart_carta)/7;
    int alturaCarta = al_get_bitmap_height(heart_carta);

    ALLEGRO_KEYBOARD_STATE keyState;

    ALLEGRO_TRANSFORM camera;

    ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);
    ALLEGRO_TIMER *frameTimer = al_create_timer(1.0/frameFPS);

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue,al_get_timer_event_source(frameTimer));
    al_register_event_source(event_queue,al_get_display_event_source(display));
    al_register_event_source(event_queue,al_get_keyboard_event_source());

    al_start_timer(timer);
    al_start_timer(frameTimer);

    //cameraUpdate(cameraPosition,x,y,al_get_bitmap_width(player_sprite)/4,al_get_bitmap_height(player_sprite)/4);
    al_identity_transform(&camera);
    al_translate_transform(&camera, -cameraPosition[0],-cameraPosition[1]);
    al_scale_transform(&camera,scale,scale);
    al_use_transform(&camera);

    

    while(state != ENDGAME){

        if(state == PLAY_SCREEN){
			int ret;
			assertConnection();
			ret = recvMsgFromServer(&player, WAIT_FOR_IT);
            if(player.identifier==BOX_CHANGE) printf("BOX 1 - %s/%d - BOX 2 - %s/%d - BOX 3 - %s/%d - BOX 4 - %s/%d - BOX 5 - %s/%d\n",(player.boxArray[0].type==PAC?"PAC":(player.boxArray[0].type==SEDEX?"SEDEX":(player.boxArray[0].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[0].addIndex],(player.boxArray[1].type==PAC?"PAC":(player.boxArray[1].type==SEDEX?"SEDEX":(player.boxArray[1].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[1].addIndex],(player.boxArray[2].type==PAC?"PAC":(player.boxArray[2].type==SEDEX?"SEDEX":(player.boxArray[2].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[2].addIndex],(player.boxArray[3].type==PAC?"PAC":(player.boxArray[3].type==SEDEX?"SEDEX":(player.boxArray[3].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[3].addIndex],(player.boxArray[4].type==PAC?"PAC":(player.boxArray[4].type==SEDEX?"SEDEX":(player.boxArray[4].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[4].addIndex]);
			state=WAITING_ENEMY;
            x = (player.posX + 3)*32;
            y = (player.posY + 5)*32;
		}

        while(state == WAITING_ENEMY){
			printf("WAITING PLAYER\n");
			int ret = recvMsgFromServer(&enemy, WAIT_FOR_IT);
			printf("Jogador: %s\nPosx = %d\nPosy = %d\n",enemy.nome,enemy.posX,enemy.posY);

            ENx = (enemy.posX + 3)*32;
            ENy = (enemy.posY + 5)*32;
            oldPosEnemyX = enemy.posX;
            oldPosEnemyY = enemy.posY;
            switch(enemy.skin){
                case JOSUE:
                    enemy_sprite = al_load_bitmap("source/resources/images/characters/Josue(com limite).png");
                    if(!enemy_sprite){
                        puts("Errou ao carregar Josue no inimigo, muito pesado ele.");
                        state = ENDGAME;
                    }
                    break;
                case JOSIAS:
                    enemy_sprite = al_load_bitmap("source/resources/images/characters/Josias(com limite).png");
                    if(!enemy_sprite){
                        puts("Errou ao carregar Josias no inimigo, muito alto ele.");
                        state = ENDGAME;
                    }
                    break;
                case MATIAS:
                    enemy_sprite = al_load_bitmap("source/resources/images/characters/Matias(com limite).png");
                    if(!enemy_sprite){
                        puts("Errou ao carregar Matias no inimigo, muito rápido ele.");
                        state = ENDGAME;
                    }
                    break;
                default:
                    enemy_sprite = al_load_bitmap("source/resources/images/characters/Al Fredo(com limite).png");
                    if(!enemy_sprite){
                        puts("Errou ao carregar Al Fredo no inimigo, muito secreto ele.");
                        state = ENDGAME;
                    }
                    break;
            }

			char serverResponse;
			ret = recvMsgFromServer(&serverResponse, WAIT_FOR_IT);
            printf("------>%d\n",serverResponse);
			if(serverResponse == 99){ 
                state = IN_GAME;
			    printf("%d - GAME_START RECEBIDO\n",state);
            }		
		}

        while(state == PRE_GAME){
            // do stuff here
            state = IN_GAME;
        }

        while(state == IN_GAME){

            ALLEGRO_EVENT events;
            al_wait_for_event(event_queue, &events);
            al_get_keyboard_state(&keyState);

            int ret = recvMsgFromServer(&auxPlayer,DONT_WAIT);
			if(ret == SERVER_DISCONNECTED){
				printf("O SERVER MORREU VEY\n");
			}
			else if(ret != NO_MESSAGE){
				if(auxPlayer.ID == player.ID){ //se for a estrutura deste jogador
                    puts("Recebeu propria estrutura");
					player = auxPlayer;	
                    if(player.identifier == POSITION){
                        if(active) animate = true;
                    }
                    else if(player.identifier == BUY){
                        // al_get_keyboard_state(&keyState);
                        // if(comprou && ){

                        // }
                    }
					if(player.HP <= 0){
						state = LOSE_SCREEN;
					}

                    // oldPosX = player.posX;
                    // oldPosY = player.posY;
                    //player.posX + 3)*32;
                    //player.posY + 5)*32;
                    
				}
				else{ //se for a estrututura do inimigo
                    puts("Recebeu estrutura inimigo");
					enemy.posX = auxPlayer.posX;
					enemy.posY = auxPlayer.posY;
					enemy.HP = auxPlayer.HP;
                    enemy.identifier = auxPlayer.identifier;
                    enemy.face = auxPlayer.face;
                    if(enemy.identifier == POSITION){
                        animateEnemy = true;
                    }
					if(enemy.HP <= 0){
						state = WIN_SCREEN;
					}
				}
			    //printf("[%d][%d][%d] - HP: %d - TAOK's: %d\n",player.itemArray[0],player.itemArray[1],player.itemArray[2],player.HP,player.money);
			//printf("BOX 1 - %s/%d - BOX 2 - %s/%d - BOX 3 - %s/%d - BOX 4 - %s/%d - BOX 5 - %s/%d\n",(player.boxArray[0].type==PAC?"PAC":(player.boxArray[0].type==SEDEX?"SEDEX":(player.boxArray[0].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[0].addIndex],(player.boxArray[1].type==PAC?"PAC":(player.boxArray[1].type==SEDEX?"SEDEX":(player.boxArray[1].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[1].addIndex],(player.boxArray[2].type==PAC?"PAC":(player.boxArray[2].type==SEDEX?"SEDEX":(player.boxArray[2].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[2].addIndex],(player.boxArray[3].type==PAC?"PAC":(player.boxArray[3].type==SEDEX?"SEDEX":(player.boxArray[3].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[3].addIndex],(player.boxArray[4].type==PAC?"PAC":(player.boxArray[4].type==SEDEX?"SEDEX":(player.boxArray[4].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[4].addIndex]);
			}
			  
            if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                state = ENDGAME;
            }
            else if(events.type == ALLEGRO_EVENT_TIMER)
            {
                if(events.timer.source == timer) // 1/60
                {
		    if(cnt60++==40){
			comprou=0;
            use = 0;
			cnt60=0;
		    }
                    
                    if(active == false){
                        active = true;
                        
                        if(al_key_down(&keyState, ALLEGRO_KEY_DOWN)){
                            //puts("Entrou no KEY_DOWN");
                            char key = DOWN_ARROW;
                            oldPosX = player.posX;
                            oldPosY = player.posY;
                            sendMsgToServer((char *)&key,1);
                        }
                        else if(al_key_down(&keyState, ALLEGRO_KEY_UP)){
                            //puts("Entrou no KEY_UP");
                            char key = UP_ARROW;
                            oldPosX = player.posX;
                            oldPosY = player.posY;
                            sendMsgToServer((char *)&key,1);
                        }
                        else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT)){
                            //puts("Entrou no KEY_RIGHT");
                            char key = RIGHT_ARROW;
                            oldPosX = player.posX;
                            oldPosY = player.posY;
                            sendMsgToServer((char *)&key,1);
                        }
                        else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT)){
                            //puts("Entrou no KEY_LEFT");
                            char key = LEFT_ARROW;
                            oldPosX = player.posX;
                            oldPosY = player.posY;
                            sendMsgToServer((char *)&key,1);
                        }
                        else{
                            active = false;
                        }
                    }
                    //else{
                    if(animate){
                        passosCounter++;

                        if(oldPosX != player.posX || oldPosY != player.posY){
                            if(player.face == DOWN) y += moveSpeed;
                            else if(player.face == UP) y -= moveSpeed;
                            else if(player.face == RIGHT) x += moveSpeed;
                            else if(player.face == LEFT) x -= moveSpeed;
                            //printf("x=%g y=%g\n",x,y);
                        }
                        
                        if(passosCounter == passos) {
                            //printf("terimnou x=%g y=%g moveCounter=%g\n",x,y,moveCounter);
                            passosCounter = 0;
                            active = false;
                            animate = false;
                            // posX = (int)(x/32 - 3);
                            // posY = (int)(y/32 - 5);
                            x = (player.posX + 3)*32;
                            y = (player.posY + 5)*32;
                            //printf("x=%g y=%g\n",x,y);
                            //printf("posX=%d posY=%d x=%g y=%g\n",player.posX,player.posY,x,y);
                        }
                        
                    }
                    if(animateEnemy){
                        passosCounterEnemy++;
                        if(oldPosEnemyX != enemy.posX || oldPosEnemyY != enemy.posY){
                            if(enemy.face == DOWN) ENy += moveSpeed;
                            else if(enemy.face == UP) ENy -= moveSpeed;
                            else if(enemy.face == RIGHT) ENx += moveSpeed;
                            else if(enemy.face == LEFT) ENx -= moveSpeed;
                            //printf("ENx=%g ENy=%g\n",ENx,ENy);
                        }

                        if(passosCounterEnemy == passos) {
                            passosCounterEnemy = 0;
                            ENx = (enemy.posX + 3)*32;
                            ENy = (enemy.posY + 5)*32;
                            animateEnemy = false;
                            oldPosEnemyX = enemy.posX;
                            oldPosEnemyY = enemy.posY;
                            //printf("enemy.posY=%d enemy.posY=%d ENx=%g ENy=%g\n",enemy.posX,enemy.posY,ENx,ENy);
                        }

                    }

                    if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)){
                        state = ENDGAME;
                    }
                    else if(al_key_down(&keyState, ALLEGRO_KEY_Z)){
                        scale += 0.1;
                    }
                    else if(al_key_down(&keyState, ALLEGRO_KEY_X)){
                        scale -= 0.1;
                    }
                    else if(!comprou && al_key_down(&keyState, ALLEGRO_KEY_Q)){
                        puts("Comprei item 1");
                        comprou = SHURICARD;
                        char key = BUY1;
                        sendMsgToServer((char *)&key,1);
                    }
                    else if(!comprou && al_key_down(&keyState, ALLEGRO_KEY_W)){
                        comprou = TRAP;
                        char key = BUY2;
                        sendMsgToServer((char *)&key,1);
                    }
                    else if(!comprou && al_key_down(&keyState, ALLEGRO_KEY_E)){
                        comprou = BOMB;
                        char key = BUY3;
                        sendMsgToServer((char *)&key,1);
                    }
                    else if(!comprou && al_key_down(&keyState, ALLEGRO_KEY_R)){
                        comprou = DOG;
                        char key = BUY4;
                        sendMsgToServer((char *)&key,1);
                    }
                    else if(!use && al_key_down(&keyState, ALLEGRO_KEY_1)){
                        use = 1;
                        al_play_sample(shurikarta, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        puts("usei item 1");
                        char key = ITEM1_BUTTON;
                        sendMsgToServer((char *)&key,1);
                    }
                    else if(!use && al_key_down(&keyState, ALLEGRO_KEY_2)){
                        use = 1;
                        al_play_sample(armadilhaPlaced, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        char key = ITEM2_BUTTON;
                        sendMsgToServer((char *)&key,1);
                    }
                    else if(!use && al_key_down(&keyState, ALLEGRO_KEY_3)){
                        use = 1;
                        char key = ITEM3_BUTTON;
                        sendMsgToServer((char *)&key,1);
                    }
                    else if(!use && al_key_down(&keyState, ALLEGRO_KEY_SPACE)){
                        use = 1;
                        char key = ACTION_MESSAGE;
                        sendMsgToServer((char *)&key,1);
                    }
                    // else if(comprou != NO_ITEM && events.type == ALLEGRO_EVENT_KEY_UP){
                    //     comprou = NO_ITEM;
                    // }
                    // else if(comprou == TRAP && al_key_up(&keyState, ALLEGRO_KEY_W)){
                    //     comprou = NO_ITEM;
                    // }
                    // else if(comprou == BOMB && al_key_up(&keyState, ALLEGRO_KEY_E)){
                    //     comprou = NO_ITEM;
                    // }
                    // else if(comprou == DOG && al_key_up(&keyState, ALLEGRO_KEY_R)){
                    //     comprou = NO_ITEM;
                    // }
                    // else
                    // {
                    //     printf("comprou dnv\n");
                    //     comprou = true;
                    // }

                    if(draw)
                    {
                        cameraUpdate(cameraPosition,x,y,al_get_bitmap_width(player_sprite)/4,al_get_bitmap_height(player_sprite)/4);
                        al_identity_transform(&camera);
                        al_translate_transform(&camera, -cameraPosition[0],-cameraPosition[1]);
                        al_scale_transform(&camera,scale,scale);
                        al_use_transform(&camera);

                        ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap(player_sprite, sourceX, sourceY*al_get_bitmap_height(player_sprite)/4,al_get_bitmap_width(player_sprite)/4,al_get_bitmap_height(player_sprite)/4);
                        ALLEGRO_BITMAP *subBitmapEnemy = al_create_sub_bitmap(enemy_sprite, sourceEnemyX, sourceEnemyY*al_get_bitmap_height(enemy_sprite)/4,al_get_bitmap_width(enemy_sprite)/4,al_get_bitmap_height(enemy_sprite)/4);
                        al_draw_bitmap(background,0,0,0);
                        if(skin != JOSIAS) al_draw_bitmap(subBitmap,x,y,0);
                        else al_draw_bitmap(subBitmap,x,y-5,0);
                        if(enemy.skin != JOSIAS) al_draw_bitmap(subBitmapEnemy,ENx,ENy,0);
                        else al_draw_bitmap(subBitmapEnemy,ENx,ENy-5,0);

                        al_identity_transform(&camera);
                        al_translate_transform(&camera,0,0);
                        //al_scale_transform(&camera,scale,scale);
                        al_use_transform(&camera);

                        for(int i=0;i<player.HP;i++){
                            al_draw_bitmap_region(heart_carta, 0, 0, larguraCarta, alturaCarta, i*larguraCarta + 5, 20, 0);
                        }
                        al_draw_textf(fonte_jogo,al_map_rgb(0,255,0),32,alturaCarta +50,0,"%d Taoquei's",player.money);

                        al_draw_scaled_bitmap(item_bar,0,0,34,96,0,ALTURA_TELA/4,68,192,NULL);
                        al_draw_scaled_bitmap(box_bar,0,0,160,40,(LARGURA_TELA/2)-160,ALTURA_TELA-80,320,80,NULL);

                        for(int i=0;i<3;i++){
                            if(player.itemArray[i] != NO_ITEM){
                                switch (player.itemArray[i]){
                                    case SHURICARD:
                                        al_draw_scaled_bitmap(icon_shuricarta,0,0,32,32,0,ALTURA_TELA/4 + i*62,64,64,NULL);
                                        break;
                                    case TRAP:
                                        al_draw_scaled_bitmap(icon_trap,0,0,32,32,0,ALTURA_TELA/4 + i*62,64,64,NULL);
                                        break;
                                    case BOMB:
                                        al_draw_scaled_bitmap(icon_bomb,0,0,32,32,0,ALTURA_TELA/4 + i*62,64,64,NULL);
                                        break;
                                    case DOG:
                                        al_draw_scaled_bitmap(icon_dog,0,0,32,32,0,ALTURA_TELA/4 + i*62,64,64,NULL);
                                        break;
                                }
                            }
                        }

                        for(int i = 0; i<5; i++){
                            if(player.boxArray[i].type != NO_BOX){
                                switch (player.boxArray[i].type){
                                    case PAC:
                                        al_draw_scaled_bitmap(icon_boxes,0,0,32,32,(LARGURA_TELA/2)-160+(i*62),ALTURA_TELA-80,64,64,0);
                                        break;
                                    case SEDEX:
                                        al_draw_scaled_bitmap(icon_boxes,32,0,32,32,(LARGURA_TELA/2)-160+(i*62),ALTURA_TELA-80,64,64,0);
                                        break;
                                    case EXPRESS:
                                        al_draw_scaled_bitmap(icon_boxes,64,0,32,32,(LARGURA_TELA/2)-160+(i*62),ALTURA_TELA-80,64,64,0);
                                        break;
                                }

                                al_draw_textf(fonte_timer,al_map_rgb(255,255,255),(LARGURA_TELA/2)-132+(i*62),ALTURA_TELA-11,0,"%d",player.boxArray[i].timeLast);
                                al_draw_textf(fonte_timer,al_map_rgb(255,255,255),(LARGURA_TELA/2)-150+(i*62),ALTURA_TELA-59,0,"Casa %d",houses[player.boxArray[i].addIndex]);

                            }
                        }

                        al_flip_display();
                        al_clear_to_color(al_map_rgb(0,0,0));
                        al_destroy_bitmap(subBitmap);
                        al_destroy_bitmap(subBitmapEnemy);
                    }

                }

                else if(events.timer.source == frameTimer) //i/
                {
                    //ESSE PLAYER
                    if(animate){
                        sourceX += al_get_bitmap_width(player_sprite) / 4;
                    }
                    else{
                        sourceX = 0;
                    }

                    if(sourceX >= al_get_bitmap_width(player_sprite)) sourceX = 0;
                    sourceY = player.face;

                    //INIMIGO
                    if(animateEnemy){
                        sourceEnemyX += al_get_bitmap_width(enemy_sprite) / 4;
                    }
                    else{
                        sourceEnemyX = 0;
                    }

                    if(sourceEnemyX >= al_get_bitmap_width(enemy_sprite)) sourceEnemyX = 0;
                    sourceEnemyY = enemy.face;

                }

                draw = true;

            }
            /*else if(events.type == ALLEGRO_EVENT_KEY_UP){
                printf("ENTROU AQUI %d %c\n",events.keyboard.keycode,events.keyboard.keycode + 'A' -1);
                if((events.keyboard.keycode == ALLEGRO_KEY_Q && comprou == SHURICARD)||(events.keyboard.keycode == ALLEGRO_KEY_W && comprou == TRAP)||(events.keyboard.keycode == ALLEGRO_KEY_E && comprou == BOMB)||(events.keyboard.keycode == ALLEGRO_KEY_R && comprou == DOG)) comprou = NO_ITEM;
            }*/
            // else if(events.type == ALLEGRO_EVENT_KEY_UP && !comprou){
            //     comprou = true;
            //     printf("pode comprou\n");
            // }
            
        }
    }

    
    destruirBitmaps();
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_timer(frameTimer);
    al_destroy_event_queue(event_queue);

    return 0;    
}

