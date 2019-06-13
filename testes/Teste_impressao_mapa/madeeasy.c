// Inclui o cabeçalho da bibilioteca Allegro 5
#include <allegro5/allegro.h>
 
// Inclui o cabeçalho do add-on para uso de imagens
#include <allegro5/allegro_image.h>
#include<stdio.h>

#define LARGURA_TELA 1600
#define ALTURA_TELA 900


float cameraPosition[2] = {0,0}, scale = 1.0;
void cameraUpdate(float* cameraPosition, float x, float y, int width, int height ){
    
    cameraPosition[0] = -(LARGURA_TELA/2/scale) + (x + width/2);
    cameraPosition[1] = -(ALTURA_TELA/2/scale) + (y + height/2);
    if(cameraPosition[0] < 0) cameraPosition[0] = 0;
    if(cameraPosition[1] < 0) cameraPosition[1] = 0;
    //if(cameraPosition[0] > ALTURA_TELA); cameraPosition[0] = ALTURA_TELA;
    //if(cameraPosition[1] > LARGURA_TELA); cameraPosition[1] = ALTURA_TELA;
    printf("x=%g y=%g cameraPosition[0]=%g cameraPosition[1]=%g scale=%g \n",x,y,cameraPosition[0],cameraPosition[1],scale);
}

int main()
{
    ALLEGRO_DISPLAY *display;

    enum direction {DOWN, UP, LEFT, RIGHT};

    const float FPS = 60.0;
    const float frameFPS = 15;

    if(!al_init())
        puts("Fala ao iniciar allegro\n");

    display = al_create_display(LARGURA_TELA,ALTURA_TELA);

    if(!display)
        puts("Fala ao iniciar display.\n");

    al_set_window_position(display,200,200);

    bool done = false, draw = true, active = false;
    float x = 10, y = 10, moveSpeed = 4;
    int dir = DOWN, sourceX = 32, sourceY = 0;
    

    al_install_keyboard();
    al_init_image_addon();

    ALLEGRO_BITMAP *player = al_load_bitmap("Josue.png");
    ALLEGRO_BITMAP *background = al_load_bitmap("mapadef.png");

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
                active = true;
                if(al_key_down(&keyState, ALLEGRO_KEY_DOWN))
                {
                    y += moveSpeed;
                    dir = DOWN;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_UP))
                {
                    y -= moveSpeed;
                    dir = UP;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
                {
                    x += moveSpeed;
                    dir = RIGHT;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT))
                {
                    x -= moveSpeed;
                    dir = LEFT;
                }
                else{
                    active = false;
                }

                if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
                {
                    done = true;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_Z))
                {
                    scale += 0.1;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_X))
                {
                    scale -= 0.1;
                }
                
                cameraUpdate(cameraPosition,x,y,32,32);

                al_identity_transform(&camera);
                al_translate_transform(&camera, -cameraPosition[0],-cameraPosition[1]);
                al_scale_transform(&camera,scale,scale);
                al_use_transform(&camera);

            }

            else if(events.timer.source == frameTimer)
            {
                if(active){
                    sourceX += al_get_bitmap_width(player) / 4;
                    //printf("if sourceX depois= %d \n",sourceX);
                }
                else{
                    sourceX = 0;
                    //printf("else sourceX = %d\n",sourceX);
                }

                if(sourceX >= al_get_bitmap_width(player)) sourceX = 0;
                sourceY = dir;
                //printf("sourceX = %d  sourceY = %d\n",sourceX,sourceY);
            }

            draw = true;

        }

        if(draw)
        {
            
            ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap(player, sourceX, sourceY*32, 32, 32);
            al_draw_bitmap(background,0,0,NULL);
            al_draw_bitmap(subBitmap,x,y,NULL);

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