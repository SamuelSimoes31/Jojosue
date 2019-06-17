// Inclui o cabeçalho da bibilioteca Allegro 5
#include <allegro5/allegro.h>
 
// Inclui o cabeçalho do add-on para uso de imagens
#include <allegro5/allegro_image.h>
#include "client.h"
#include "Player.h"
#include <ctype.h>

#define LARGURA_TELA 1600
#define ALTURA_TELA 900

ALLEGRO_BITMAP *player_sprite;
ALLEGRO_BITMAP *background;

//int posX=1, posY=1;
//PLAY_SCREEN

Enemy_Data enemy;
Player_Data player;
Player_Data auxPlayer;



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
	//char server_ip[30];
	
	//char server_ID[5];
	//strcpy(server_ip,"172.20.4.");
	//printf("Please enter the server ID: ");
	//scanf(" %s", server_ID);
	//getchar();
	//strcat(server_ip,server_ID);
	
	// printf("Please enter the server IP: ");
	// scanf(" %s", server_ip);
	// getchar();
	// return connectToServer(server_ip);
	return connectToServer("127.0.0.1");
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
	char nickname[NICKNAME_MAX_SIZE + 4];
	printf("Please enter your nickname (limit = %d): ", NICKNAME_MAX_SIZE );
	scanf(" %[^\n]", nickname);
	getchar();
	int len = (int)strlen(nickname);
	sendMsgToServer(nickname, len + 1);
	printf("Please enter your skin 0 1 2:");
	char skin;
	scanf(" %d",&skin);
	getchar();
	sendMsgToServer(&skin,1);
    player.skin = skin;
    switch(skin){
        case JOSIAS:
        case JOSUE:
            player_sprite = al_load_bitmap("source/resources/images/characters/Josue(com limite).png");
            if(!player_sprite){
                puts("Errou ao carregar Josue, muito pesado ele.");
                state = ENDGAME;
            }
            break;
        // case JOSIAS:
        //     player_sprite = al_load_bitmap("source/resources/images/characters/Josue.png");
        //     if(!player_sprite){
        //         puts("Errou ao carregar Josue, muito pesado ele.");
        //         state = ENDGAME;
        //     }
        //     break;
        case MATIAS:
            player_sprite = al_load_bitmap("source/resources/images/characters/Matias(com limite).png");
            if(!player_sprite){
                puts("Errou ao carregar Matias, muito rápido ele.");
                state = ENDGAME;
            }
            break;
    }
}



int main()
{
    state = PLAY_SCREEN;
    ALLEGRO_DISPLAY *display;

    //enum direction {DOWN, UP, LEFT, RIGHT};

    const float FPS = 60.0;
    const float frameFPS = 12.0;

    if(!al_init())
        puts("Fala ao iniciar allegro\n");

    display = al_create_display(LARGURA_TELA,ALTURA_TELA);

    if(!display)
        puts("Fala ao iniciar display.\n");

    //al_set_window_position(display,200,200);
    int passos=15, passosCounter=0;//quantos saltos de moveSpeed ele vai fazer até chegar em 32
    bool done = false, draw = true, animate = false ,active = false;
    float x=0, y=0, moveSpeed = 32/(float)passos;// moveCounter = 0;
    unsigned short oldPosX, oldPosY;
    int dir = DOWN, sourceX = 32, sourceY = 0;
    // / moveSpeed = 32/frameFPS,

    al_install_keyboard();
    al_init_image_addon();

    // player_sprite = al_load_bitmap("source/resources/images/characters/Josue.png");
    // if(!player_sprite){
    //     puts("Errou ao carregar Josue, muito pesado ele.");
    //     exit(0);
    // }
    background = al_load_bitmap("source/resources/images/backgrounds/MapaComLimites.png");
    if(!background){
        puts("Errou ao carregar Mapa, prefiro o de 26.0 GB.");
        exit(0);
    }

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

    cameraUpdate(cameraPosition,x,y,32,32);
    al_identity_transform(&camera);
    al_translate_transform(&camera, -cameraPosition[0],-cameraPosition[1]);
    al_scale_transform(&camera,scale,scale);
    al_use_transform(&camera);

    

    while(state != ENDGAME){
        
        if(state == PLAY_SCREEN){
			int ret;
			assertConnection();
			ret = recvMsgFromServer(&player, WAIT_FOR_IT);
			printf("Jogador: %s\nPosx = %d\nPosy = %d\n",player.nome,player.posX,player.posY);
			state=WAITING_ENEMY;
            x = (player.posX + 3)*32;
            y = (player.posY + 5)*32;
		}

        while(state == WAITING_ENEMY){
			printf("WAITING PLAYER\n");
			int ret = recvMsgFromServer(&enemy, WAIT_FOR_IT);
			printf("Jogador: %s\nPosx = %d\nPosy = %d\n",enemy.nome,enemy.posX,enemy.posY);

			char serverResponse;
			ret = recvMsgFromServer(&serverResponse, WAIT_FOR_IT);
			if(serverResponse==99) state = PRE_GAME;
			printf("GAME_START RECEBIDO\n");		
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
					player = auxPlayer;	
					if(player.HP <= 0){
						state = LOSE_SCREEN;
					}
                    if(active) animate = true;
                    // oldPosX = player.posX;
                    // oldPosY = player.posY;
                    //player.posX + 3)*32;
                    //player.posY + 5)*32;
                    
				}
				else{ //se for a estrututura do inimigo
					enemy.posX = auxPlayer.posX;
					enemy.posY = auxPlayer.posY;
					enemy.HP = auxPlayer.HP;
					//printf("-----------------%d\n",enemy.HP);
					//printf("Nova posicao do jogador inimigo: %d(x) %d(y)\n",enemy.posX,enemy.posY);
					if(enemy.HP <= 0){
						state = WIN_SCREEN;
					}
				}
			    printf("[%d][%d][%d] - HP: %d - TAOK's: %d\n",player.itemArray[0],player.itemArray[1],player.itemArray[2],player.HP,player.money);
			//printf("BOX 1 - %s/%d - BOX 2 - %s/%d - BOX 3 - %s/%d - BOX 4 - %s/%d - BOX 5 - %s/%d\n",(player.boxArray[0].type==PAC?"PAC":(player.boxArray[0].type==SEDEX?"SEDEX":(player.boxArray[0].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[0].addIndex],(player.boxArray[1].type==PAC?"PAC":(player.boxArray[1].type==SEDEX?"SEDEX":(player.boxArray[1].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[1].addIndex],(player.boxArray[2].type==PAC?"PAC":(player.boxArray[2].type==SEDEX?"SEDEX":(player.boxArray[2].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[2].addIndex],(player.boxArray[3].type==PAC?"PAC":(player.boxArray[3].type==SEDEX?"SEDEX":(player.boxArray[3].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[3].addIndex],(player.boxArray[4].type==PAC?"PAC":(player.boxArray[4].type==SEDEX?"SEDEX":(player.boxArray[4].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[4].addIndex]);
			}
			  
            if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                state = ENDGAME;
            }
            else if(events.type == ALLEGRO_EVENT_TIMER)
            {
                if(events.timer.source == timer)
                {
                    if(active == false){
                        active = true;
                        
                        if(al_key_down(&keyState, ALLEGRO_KEY_DOWN)){
                            puts("Entrou no KEY_DOWN");
                            char key = DOWN_ARROW;
                            oldPosX = player.posX;
                            oldPosY = player.posY;
                            sendMsgToServer((char *)&key,1);
                            //y += moveSpeed;
                            //dir = DOWN;
                        }
                        else if(al_key_down(&keyState, ALLEGRO_KEY_UP)){
                            puts("Entrou no KEY_UP");
                            char key = UP_ARROW;
                            oldPosX = player.posX;
                            oldPosY = player.posY;
                            sendMsgToServer((char *)&key,1);
                            //y -= moveSpeed;
                            //dir = UP;
                        }
                        else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT)){
                            puts("Entrou no KEY_RIGHT");
                            char key = RIGHT_ARROW;
                            oldPosX = player.posX;
                            oldPosY = player.posY;
                            sendMsgToServer((char *)&key,1);
                            //x += moveSpeed;
                            //dir = RIGHT;
                        }
                        else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT)){
                            puts("Entrou no KEY_LEFT");
                            char key = LEFT_ARROW;
                            oldPosX = player.posX;
                            oldPosY = player.posY;
                            sendMsgToServer((char *)&key,1);
                            //x -= moveSpeed;
                            //dir = LEFT;
                        }
                        else{
                            active = false;
                        }
                    }
                    //else{
                    if(animate){
                        dir = player.face;
                        //moveCounter += moveSpeed;
                        passosCounter++;

                        if(oldPosX != player.posX || oldPosY != player.posY){
                            if(dir == DOWN) y += moveSpeed;
                            else if(dir == UP) y -= moveSpeed;
                            else if(dir == RIGHT) x += moveSpeed;
                            else if(dir == LEFT) x -= moveSpeed;
                            //printf("x=%g y=%g moveCounter=%g\n",x,y,moveCounter);
                        }

                        cameraUpdate(cameraPosition,x,y,32,32);
                        al_identity_transform(&camera);
                        al_translate_transform(&camera, -cameraPosition[0],-cameraPosition[1]);
                        al_scale_transform(&camera,scale,scale);
                        al_use_transform(&camera);
                        
                        if(passosCounter == passos) {
                            //printf("terimnou x=%g y=%g moveCounter=%g\n",x,y,moveCounter);
                            passosCounter = 0;
                            active = false;
                            animate = false;
                            // posX = (int)(x/32 - 3);
                            // posY = (int)(y/32 - 5);
                            
                            //printf("x=%g y=%g\n",x,y);
                            printf("posX=%d posY=%d\n",player.posX,player.posY);
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

                }

                else if(events.timer.source == frameTimer)
                {
                    if(animate){
                        sourceX += al_get_bitmap_width(player_sprite) / 4;
                    }
                    else{
                        sourceX = 0;
                    }

                    if(sourceX >= al_get_bitmap_width(player_sprite)) sourceX = 0;
                    sourceY = dir;

                }

                draw = true;

            }

            if(draw)
            {
                ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap(player_sprite, sourceX, sourceY*32, 32, 32);
                al_draw_bitmap(background,0,0,0);
                al_draw_bitmap(subBitmap,x,y,0);

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
                al_destroy_bitmap(subBitmap);
            }
        }
    }

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_timer(frameTimer);
    al_destroy_bitmap(player_sprite);
    al_destroy_bitmap(background);
    al_destroy_event_queue(event_queue);

    return 0;    
}