#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#define LARGURA_TELA 1600
#define ALTURA_TELA 900
#define FPS 60.0
//dale dbv
    ALLEGRO_BITMAP *botao_jogar = NULL;
    ALLEGRO_BITMAP *botao_tutorial = NULL;
    ALLEGRO_BITMAP *botao_leaderboard = NULL;
    ALLEGRO_BITMAP *botao_opcoes = NULL;
    ALLEGRO_BITMAP *botao_sair = NULL;
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_BITMAP *fundo = NULL;
    ALLEGRO_BITMAP *cartinha = NULL;
    ALLEGRO_BITMAP *cartinha2 = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_FONT *fonte = NULL;
    typedef enum {
        em_jogar,
        em_tutorial,
        em_leaderboard,
        em_opcoes,
        em_sair,
        em_nenhum 
    } estados;

int inicializar() {
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return 0;
    }
    al_init_image_addon();
    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar add-on allegro_primitives.\n");
        return 0;
    }
    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_primitives.\n");
        return 0;
    }

    al_init_font_addon();
    
    fundo = al_load_bitmap("background.png");
    if(!fundo) {
        fprintf(stderr, "Falha ao criar fundo.\n");
        return 0;
    }

    // Inicialização do add-on para uso de fontes True Type
    if (!al_init_ttf_addon()){
        printf("Falha ao inicializar add-on allegro_ttf");
        return 0;
    }


    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela) {
        fprintf(stderr, "Falha ao criar janela.\n");
        return 0;
    }

    
    /*imagem = al_load_bitmap("cart.jpeg");
    if (!imagem) {
        printf("Falha ao carregar o arquivo de imagem");
        al_destroy_display(janela);
        return 0;
    }*/


    fonte = al_load_font("pressStart.ttf", 18, 0);
    if (!fonte){
        al_destroy_display(janela);
        printf("Falha ao carregar fonte");
        return -1;
    }

    al_set_window_title(janela, "Beta Menu");
 
    fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return 0;
    }
    
    if (!al_install_mouse()) {
        printf("Falha ao inicializar o mouse");
        al_destroy_display(janela);
        return 0;
    }

    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)) {
        printf("Falha ao atribuir ponteiro do mouse");
        al_destroy_display(janela);
        return 0;
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    return 1;
}

void inicializa_botoes_menu() {


    botao_jogar = al_create_bitmap(90, 25);
    if (!botao_jogar) {
        printf("Falha ao criar bitmap");
        al_destroy_display(janela);
        exit(0);
    }
    botao_tutorial = al_create_bitmap(151, 25);
    if(!botao_tutorial) {
        printf("Falha ao criar bitmap");
        al_destroy_display(janela);
        exit(0);
    }
    botao_opcoes = al_create_bitmap(110, 25);
    if(!botao_opcoes) {
        printf("Falha ao criar bitmap");
        al_destroy_display(janela);
        exit(0);
    }
    botao_leaderboard = al_create_bitmap(170, 25);
    if(!botao_leaderboard) {
        printf("Falha ao criar bitmap");
        al_destroy_display(janela);
        exit(0);
    }
    botao_sair = al_create_bitmap(100, 25);
    if(!botao_sair) {
        printf("Falha ao criar bitmap");
        al_destroy_display(janela);
        exit(0);
    }
}

int main()
{
    float raio = 30.0;
    float posx = raio;
    float posy = raio;
    int dir_x = 1, dir_y = 0;

    int sair = 0;
    if(!inicializar()) {
        exit(-1);
    }

    inicializa_botoes_menu();

    estados estado;

    while(!sair) {
        al_draw_filled_circle(posx, 100, raio, al_map_rgb(255, 0, 0));
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
            posx += 10.0 * dir_x;
            posy += 1.0 * dir_y;        
 
            if (posx >= LARGURA_TELA - raio)
            {
                dir_x = -1;
                posx = LARGURA_TELA - raio;
            } else if (posx <= raio) {
                dir_x = 1;
                posx = raio;
            }
        while(!al_is_event_queue_empty(fila_eventos)) {
            ALLEGRO_EVENT evento;
            //ALLEGRO_EVENT evento_timer;
            al_wait_for_event(fila_eventos, &evento);
            //al_wait_for_event(fila_eventos, &evento_timer);
            sair = 0;
            if(evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_jogar) / 2) {
                    estado = em_jogar;
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 + 10 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_tutorial) / 2 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_tutorial) / 2 + 30 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_tutorial) / 2 + 30) {
                    estado = em_tutorial;
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_leaderboard) / 2 - 9 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_leaderboard) / 2 + 20 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_leaderboard) / 2  + 90 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_leaderboard) / 2 + 90) {
                    estado = em_leaderboard;
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_opcoes) / 2 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_opcoes) / 2 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_opcoes) / 2 + 50 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_opcoes) / 2 + 50) {
                    estado = em_opcoes;
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 + 18 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_sair) / 2 - 8 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_sair) / 2 + 120 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_sair) / 2 + 120) {
                    estado = em_sair;
                }
                else {
                    estado = em_nenhum;
                }
                sair = 0;
            }
            else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_sair) / 2 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_sair) / 2 + 120 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_sair) / 2 + 120) {
                    sair = 1;
                    //fecha o client
                }
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogar) / 2 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_jogar) / 2 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_jogar) / 2) {
                    sair = 0;
                    //atualiza a tela como o inicio do jogo
                }
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_tutorial) / 2 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_tutorial) / 2 + 30 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_tutorial) / 2 + 30){
                    sair = 0;
                    //atualiza a tela com a tela de tutorial
                }
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_leaderboard) / 2 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_leaderboard) / 2 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_leaderboard) / 2 + 90 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_leaderboard) / 2 + 90) {
                    sair = 0;
                    //atualiza a tela com os ranking de melhores jogadores
                }
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_opcoes) / 2 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_opcoes) / 2 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_opcoes) / 2 + 60&&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_opcoes) / 2 + 60){
                    sair = 0;
                    //atualiza a tela com os opcoes
                }
            }
            
        }
        al_clear_to_color(al_map_rgb(0, 0, 0));
        //al_draw_bitmap(fundo, LARGURA_TELA/2, ALTURA_TELA/2, 0);
        /*al_draw_bitmap(fundo, 0, 0, 0);
        al_draw_bitmap(imagem, LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogar) / 2 - 100,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 - 200, 0);*/
        //al_draw_bitmap(fundo, 0, 0, 0);
        al_draw_scaled_bitmap(fundo,
        0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo),
        0, 0, LARGURA_TELA, ALTURA_TELA, 0);

        al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogar) / 2 + 45,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 5, ALLEGRO_ALIGN_CENTER, "JOGAR");
        if (estado == em_jogar) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogar) / 2 + 45,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 5, ALLEGRO_ALIGN_CENTER, "JOGAR");
        }
        /*else {
            al_clear_to_color(al_map_rgb(255, 255, 0));
        }*/
        al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 + 80,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 35, ALLEGRO_ALIGN_CENTER, "TUTORIAL");
        if(estado == em_tutorial) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 + 80,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 35, ALLEGRO_ALIGN_CENTER, "TUTORIAL");
        }

        al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_leaderboard) / 2 + 90,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 95, ALLEGRO_ALIGN_CENTER, "LEADERBOARD");
        if(estado == em_leaderboard) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_leaderboard) / 2 + 90,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 95, ALLEGRO_ALIGN_CENTER, "LEADERBOARD");
        }

        al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_opcoes) / 2 + 55,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 65, ALLEGRO_ALIGN_CENTER, "OPÇÕES");
        if(estado == em_opcoes) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_opcoes) / 2 + 55,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 65, ALLEGRO_ALIGN_CENTER, "OPÇÕES");
        }

        al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 + 55,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 125, ALLEGRO_ALIGN_CENTER, "SAIR");
        if(estado == em_sair) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 + 55,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 125, ALLEGRO_ALIGN_CENTER, "SAIR");
        }

        //al_flip_display();
        al_rest(0.005);
    }
    al_destroy_bitmap(botao_sair);
    al_destroy_bitmap(botao_jogar);
    al_destroy_bitmap(botao_tutorial);
    al_destroy_bitmap(botao_opcoes);
    al_destroy_bitmap(botao_leaderboard);
    al_destroy_display(janela);
    al_destroy_font(fonte);
    al_destroy_event_queue(fila_eventos);
    return 0;
}
