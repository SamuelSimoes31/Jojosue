#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
//gcc teste.c -o dale -lm -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

    //fps = frames per second = atualizacoes por segundo
    #define FPS 60.0
    #define LARGURA_TELA 640
    #define ALTURA_TELA 480
     
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    //declara o timer do programa
    ALLEGRO_TIMER *timer = NULL;
     
    void error_msg(char *text){
    	printf("%s\n",text);
    }
     
    int inicializar(){
        if (!al_init()){
            error_msg("Falha ao inicializar a Allegro");
            return 0;
        }
     
        //cria o timer com o intervalo de tempo que ele ira disparar
        timer = al_create_timer(1.0 / FPS);
        if(!timer) {
            error_msg("Falha ao criar temporizador");
            return 0;
        }
     
        janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
        if(!janela) {
            error_msg("Falha ao criar janela");
            al_destroy_timer(timer);
            return 0;
        }
     
        al_set_window_title(janela, "Utilizando timers");
     
        fila_eventos = al_create_event_queue();
        if(!fila_eventos) {
            error_msg("Falha ao criar fila de eventos");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            return 0;
        }
     
        al_register_event_source(fila_eventos, al_get_display_event_source(janela));
        //registra o timer na fila de eventos
        al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
        al_clear_to_color(al_map_rgb(0,0,0));
        al_flip_display();
        //inicia a contagem do timer
        al_start_timer(timer);
     
        return 1;
    }
     
    int main(void){
        //flag que determina quando a tela precisa ser atualizada
        int desenha = 1;
        //flag que determina quando devemos sair do loop principal
        int sair = 0;
        //vetor que guarda as componentes RGB que sera impresso no fundo da tela
        int cor[3]={0,0,0};
        //i= indice que acessamos o veror
        //inc= direcao que a cor incremente, ou ele sera +1 ou -1
        int i=0,inc=1;
     
     
        if (!inicializar()){
            return -1;
        }
     
        while(!sair){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
     
            /* -- EVENTOS -- */
            //se o tipo do evento for o disparo de um timer
            if(evento.type == ALLEGRO_EVENT_TIMER){
                cor[i]+=inc;
                if (cor[i]>=255)
                    inc*=-1;
                if (cor[i]<=0){
                    i=(i+1)%3;
                    inc*=-1;
                }
                desenha = 1;
            }
            else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                sair = 1;
            }
     
            /* -- ATUALIZA TELA -- */
            if(desenha && al_is_event_queue_empty(fila_eventos)) {
                al_clear_to_color(al_map_rgb(cor[0],cor[1],cor[2]));
                al_flip_display();
                //zera flag para nao entrar aqui no proximo loop
                desenha = 0;
            }
        }
     
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
     
        return 0;
    }