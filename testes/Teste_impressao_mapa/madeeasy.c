// Inclui o cabeçalho da bibilioteca Allegro 5
#include <allegro5/allegro.h>
 
// Inclui o cabeçalho do add-on para uso de imagens
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "client.h"
#include "Player.h"

#define LARGURA_TELA 1600
#define ALTURA_TELA 900

ALLEGRO_BITMAP *player;
ALLEGRO_BITMAP *background;

int posX=1, posY=1;

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

char assertConnection() {
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

enum conn_ret_t tryConnect() {
	char server_ip[30];
	
	//char server_ID[5];
	//strcpy(server_ip,"172.20.4.");
	//printf("Please enter the server ID: ");
	//scanf(" %s", server_ID);
	//getchar();
	//strcat(server_ip,server_ID);
	
	printf("Please enter the server IP: ");
	scanf(" %s", server_ip);
	getchar();
	return connectToServer(server_ip);
	//return connectToServer("172.20.4.24");
}

int main()
{
    int state = WAITING_ENEMY;
    ALLEGRO_DISPLAY *display;

    enum direction {DOWN, UP, LEFT, RIGHT};

    const float FPS = 60.0;
    const float frameFPS = 10.0;

    if(!al_init())
        puts("Fala ao iniciar allegro\n");

    display = al_create_display(LARGURA_TELA,ALTURA_TELA);

    if(!display)
        puts("Fala ao iniciar display.\n");

    //al_set_window_position(display,200,200);

    bool done = false, draw = true, active = false;
    float x = 128, y = 192, moveSpeed = 32/frameFPS, moveCounter = 0;
    int dir = DOWN, sourceX = 32, sourceY = 0;
    // / moveSpeed = 32/frameFPS,

    al_install_keyboard();
    al_init_image_addon();

    player = al_load_bitmap("source/resources/images/characters/Josue.png");
    if(!player){
        puts("Errou ao carregar Josue, muito pesado ele.");
        exit(0);
    }
    background = al_load_bitmap("source/resources/images/backgrounds/Mapa.png");
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

    while(!done)
    {
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);
        al_get_keyboard_state(&keyState);

        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        else if(events.type == ALLEGRO_EVENT_TIMER)
        {
            if(events.timer.source == timer)
            {
                if(active == false){
                    active = true;
                    if(al_key_down(&keyState, ALLEGRO_KEY_DOWN)){
                        //y += moveSpeed;
                        dir = DOWN;
                    }
                    else if(al_key_down(&keyState, ALLEGRO_KEY_UP)){
                        //y -= moveSpeed;
                        dir = UP;
                    }
                    else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT)){
                        //x += moveSpeed;
                        dir = RIGHT;
                    }
                    else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT)){
                        //x -= moveSpeed;
                        dir = LEFT;
                    }
                    else{
                        active = false;
                    }
                }
                else{
                //if(active){
                    if(dir == DOWN) y += moveSpeed;
                    else if(dir == UP) y -= moveSpeed;
                    else if(dir == RIGHT) x += moveSpeed;
                    else if(dir == LEFT) x -= moveSpeed;

                    moveCounter += moveSpeed;
                    if(moveCounter >= 32) {
                        moveCounter = 0;
                        active = false;
                        // posX = (int)(x/32 - 3);
                        // posY = (int)(y/32 - 5);
                        if(dir == DOWN) posY++;
                        else if(dir == UP) posY--;
                        else if(dir == RIGHT) posX++;
                        else if(dir == LEFT) posX--;
                        printf("x=%g y=%g\n",x,y);
                        printf("posX=%d posY=%d\n",posX,posY);
                    }

                    cameraUpdate(cameraPosition,x,y,32,32);
                    al_identity_transform(&camera);
                    al_translate_transform(&camera, -cameraPosition[0],-cameraPosition[1]);
                    al_scale_transform(&camera,scale,scale);
                    al_use_transform(&camera);
                }

                if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)){
                    done = true;
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
                if(active){
                    sourceX += al_get_bitmap_width(player) / 4;
                }
                else{
                    sourceX = 0;
                }

                if(sourceX >= al_get_bitmap_width(player)) sourceX = 0;
                sourceY = dir;

            }

            draw = true;

        }

        if(draw)
        {
            ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap(player, sourceX, sourceY*32, 32, 32);
            al_draw_bitmap(background,0,0,0);
            al_draw_bitmap(subBitmap,x,y,0);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
            al_destroy_bitmap(subBitmap);
        }
    }
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_timer(frameTimer);
    al_destroy_bitmap(player);
    al_destroy_bitmap(background);
    al_destroy_event_queue(event_queue);

    return 0;    
}