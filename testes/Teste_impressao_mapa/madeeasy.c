// Inclui o cabeçalho da bibilioteca Allegro 5
#include <allegro5/allegro.h>
 
// Inclui o cabeçalho do add-on para uso de imagens
#include <allegro5/allegro_image.h>


#define LARGURA_TELA 800
#define ALTURA_TELA 600

int main()
{
    ALLEGRO_DISPLAY *display;

    enum direction {DOWN, LEFT,RIGHT,UP};

    const float FPS = 60.0;
    const float frameFPS = 15.0;

    if(!al_init())
        puts("Fala ao iniciar allegro\n");

    display = al_create_display(LARGURA_TELA,ALTURA_TELA);

    if(!display)
        puts("Fala ao iniciar display.\n");

    al_set_window_position(display,200,200);

    bool done = false, draw = true, active = false;
    float x = 10, y =10, moveSpeed = 5;
    int dir = DOWN, sourceX = 32, sourceY = 0;

    al_install_keyboard();
    al_init_image_addon();

    ALLEGRO_BITMAP *player = al_load_bitmap("Josias.png");
    ALLEGRO_BITMAP *background = al_load_bitmap("mapaDef.png");

    ALLEGRO_KEYBOARD_STATE keyState;

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
                if(al_key_down(&keyState, ALLEGRO_KEY_UP))
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
                    dir = UP;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT))
                {
                    x -= moveSpeed;
                    dir = LEFT;
                }
                else
                    active = false;

            }

            else if(events.timer.source == frameTimer)
            {
                if(active)
                    sourceX += al_get_bitmap_width(player)/3;
                else
                    sourceX = 32;

                if(sourceX >= al_get_bitmap_width(player))
                    sourceX = 0;

                sourceY = dir;
            }

            draw = true;

        }

        if(draw)
        {
            ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap(player, sourceX, sourceY*32,32,32);
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