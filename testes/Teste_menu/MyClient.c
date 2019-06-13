#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#define LARGURA_TELA 1600
#define ALTURA_TELA 900
#define FPS 60.0
#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 13
#define HIST_MAX_SIZE 200
//dale dbv
//-lallegro_ttf -lallegro_font -lallegro
//-lallegro -lallegro_audio -lallegro_acodec
    ALLEGRO_BITMAP *botao_jogar = NULL;
    ALLEGRO_BITMAP *botao_tutorial = NULL;
    ALLEGRO_BITMAP *botao_leaderboard = NULL;
    ALLEGRO_BITMAP *botao_sair = NULL;
    ALLEGRO_BITMAP *name_button = NULL;
    ALLEGRO_BITMAP *matias_button = NULL;
    ALLEGRO_BITMAP *josue_button = NULL;
    ALLEGRO_BITMAP *josias_button = NULL;
    ALLEGRO_BITMAP *ip_button = NULL;
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_BITMAP *fundo = NULL;
    ALLEGRO_SAMPLE *coin = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *folha_1_sprite = NULL;
    ALLEGRO_BITMAP *folha_2_sprite = NULL;
    ALLEGRO_BITMAP *folha_3_sprite = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos_timer = NULL;
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_FONT *fonte_pregame = NULL;
    typedef struct {
        char nome[14];
        char skin;
    } initial_data;

    typedef enum {
        EM_JOGAR,
        EM_TUTORIAL,
        EM_LEADERBOARD,
        EM_SAIR,
        EM_NENHUM 
    } estados_botao_menu;

    typedef enum {
        MAIN_MENU,
        GAME_MENU,
        IN_GAME,
        TUTORIAL_MENU,
        LEADERBOARD_MENU,
        ENDGAME   
    } estados_tela;

    typedef enum {
        EM_NAME,
        EM_MATIAS,
        EM_JOSUE,
        EM_JOSIAS,
        EM_IP
    } estados_game_menu;
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
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return 0;
    }

    al_init_font_addon();

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

    // coin = al_load_sample("coin.wav");
    // if (!coin) {
    //     printf( "Audio nao carregado");
    //     return 0;
    // }


    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        printf("Falha ao criar timer");
        return 0;
    }
    
    fundo = al_load_bitmap("background.png");
    if(!fundo) {
        fprintf(stderr, "Falha ao criar fundo.\n");
        return 0;
    }

    if (!al_init_ttf_addon()){
        printf("Falha ao inicializar add-on allegro_ttf");
        return 0;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela) {
        fprintf(stderr, "Falha ao criar janela.\n");
        return 0;
    }

    fonte = al_load_font("pressStart.ttf", 40, 0);
    if (!fonte){
        al_destroy_display(janela);
        printf("Falha ao carregar fonte\n");
        return -1;
    }


    al_set_window_title(janela, "Beta Menu");
 
    fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return 0;
    }

    fila_eventos_timer = al_create_event_queue();
    if (!fila_eventos_timer) {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return 0;
    }

    folha_1_sprite = al_load_bitmap("Josue.png");
    if(!folha_1_sprite) {
        printf("Falha ao carregar folha de sprite\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        return 0;
    }

    folha_2_sprite = al_load_bitmap("Josias.png");
    if(!folha_2_sprite) {
        printf("Falha ao carregar folha 2 de sprite\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        return 0;
    }

    folha_3_sprite = al_load_bitmap("Mathias.png");
    if(!folha_3_sprite) {
        printf("Falha ao carregar folha 3 de sprite\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
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
    al_register_event_source(fila_eventos_timer, al_get_timer_event_source(timer));
    al_start_timer(timer);

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
    name_button = al_create_bitmap(LARGURA_TELA, 100);
    if(!name_button) {
        printf("falha ao criar bitmap\n");
        al_destroy_display(janela);
        exit(0);
    }
    matias_button = al_create_bitmap(170, 170);
    if(!matias_button) {
        printf("falha ao criar bitmap\n");
        al_destroy_display(janela);
        exit(0);
    }
    josue_button = al_create_bitmap(170, 170);
    if(!josue_button) {
        printf("falha ao criar bitmap\n");
        al_destroy_display(janela);
        exit(0);
    }
    josias_button = al_create_bitmap(170, 170);
    if(!josias_button) {
        printf("falha ao criar bitmap\n");
        al_destroy_display(janela);
        exit(0);
    }
    ip_button = al_create_bitmap(LARGURA_TELA, 100);
    if(!ip_button) {
        printf("falha ao criar bitmap\n");
        al_destroy_display(janela);
        exit(0);
    }
}

int main()
{

    int desenha = 1;
    //largura e altura de cada sprite dentro da folha
    int altura_sprite = 64, largura_sprite = 64, altura_2_sprite = 74;
    //quantos sprites tem em cada linha da folha, e a atualmente mostrada
    int colunas_folha=4, coluna_atual=0;
    //quantos sprites tem em cada coluna da folha, e a atualmente mostrada
    int linha_atual = 0;
    //posicoes X e Y da folha de sprites que serao mostradas na tela
    int regiao_x_folha = coluna_atual * largura_sprite, regiao_y_folha = linha_atual * altura_sprite, regiao_y_2_folha = linha_atual * altura_2_sprite;
    //quantos frames devem se passar para atualizar para o proximo sprite
    int frames_sprite=9, cont_frames=0;
    //posicao X Y da janela em que sera mostrado o sprite
    int pos_x_sprite = LARGURA_TELA / 2 - 550, pos_y_sprite= ALTURA_TELA / 2 - 100;
    int pos_x_2_sprite = LARGURA_TELA / 2 + 200, pos_y_2_sprite = ALTURA_TELA / 2 - 100;
    if(!inicializar()) {
        exit(-1);
    }

    inicializa_botoes_menu();

    estados_botao_menu estado_botao;
    estados_tela estado_tela = MAIN_MENU;

    while(estado_tela == MAIN_MENU) {
        while(!al_is_event_queue_empty(fila_eventos)) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
            //al_wait_for_event(fila_eventos, &evento_timer);
            if(evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogar) / 2 - 57 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_jogar) / 2 + 53 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 - 40 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_jogar) / 2 - 28) {
                    //al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    estado_botao = EM_JOGAR;
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 - 70 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_tutorial) / 2 + 78 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_tutorial) / 2 + 25 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_tutorial) / 2 + 35) {
                    //al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    estado_botao = EM_TUTORIAL;
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_leaderboard) / 2 - 125 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_leaderboard) / 2 + 135 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_leaderboard) / 2  + 95 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_leaderboard) / 2 + 107) {
                    //al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    estado_botao = EM_LEADERBOARD;
                }
                
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 - 25 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_sair) / 2 + 30 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_sair) / 2 + 160 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_sair) / 2 + 173) {
                    //al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    estado_botao = EM_SAIR;
                }
                else {
                    estado_botao = EM_NENHUM;
                }
            }
            else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 - 25 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_sair) / 2 + 30 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_sair) / 2 + 160 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_sair) / 2 + 173) {
                    estado_tela = ENDGAME;
                    //fecha o client
                }
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogar) / 2 - 57 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_jogar) / 2 + 53 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 - 40 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_jogar) / 2 - 28) {
                    estado_tela = GAME_MENU;
                    //atualiza a tela como o inicio do jogo
                }
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 - 70 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_tutorial) / 2 + 78 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_tutorial) / 2 + 25 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_tutorial) / 2 + 35){
                    estado_tela = TUTORIAL_MENU;
                    //atualiza a tela com a tela de tutorial
                }
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_leaderboard) / 2 - 125 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_leaderboard) / 2 + 135 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_leaderboard) / 2  + 95 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_leaderboard) / 2 + 107) {
                    estado_tela = LEADERBOARD_MENU;
                    //atualiza a tela com os ranking de melhores jogadores
                }
            }
        }
        
        ALLEGRO_EVENT evento_timer;
        al_wait_for_event(fila_eventos_timer, &evento_timer);

        if(evento_timer.type == ALLEGRO_EVENT_TIMER) {
            cont_frames += 1;

            if(cont_frames >= frames_sprite) {
                cont_frames = 0;
                coluna_atual += 1;
                if(coluna_atual >= colunas_folha) {
                    coluna_atual = 0;

                }
                regiao_x_folha = coluna_atual * largura_sprite;
            }
            pos_x_sprite += 0;
            pos_y_sprite += 0;
            pos_x_2_sprite += 0;
            pos_y_2_sprite += 0;

            desenha = 1;
        }

        if(desenha && al_is_event_queue_empty(fila_eventos_timer)) {
                al_draw_scaled_bitmap(folha_1_sprite,
                regiao_x_folha, regiao_y_folha,
                largura_sprite, altura_sprite,
                pos_x_sprite, pos_y_sprite, 350, 350, 0);

                al_draw_scaled_bitmap(folha_2_sprite,
                regiao_x_folha, regiao_y_2_folha,
                largura_sprite, altura_2_sprite,
                pos_x_2_sprite, pos_y_2_sprite, 350, 350, 0);
                desenha = 0;

                al_flip_display();
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
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 - 40, ALLEGRO_ALIGN_CENTER, "JOGAR");
        if (estado_botao == EM_JOGAR) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogar) / 2 + 45,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 - 40, ALLEGRO_ALIGN_CENTER, "JOGAR");
        }
        
        al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 + 80,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 25, ALLEGRO_ALIGN_CENTER, "TUTORIAL");
        if(estado_botao == EM_TUTORIAL) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 + 80,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 25, ALLEGRO_ALIGN_CENTER, "TUTORIAL");
        }

        al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_leaderboard) / 2 + 90,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 95, ALLEGRO_ALIGN_CENTER, "LEADERBOARD");
        if(estado_botao == EM_LEADERBOARD) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_leaderboard) / 2 + 90,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 95, ALLEGRO_ALIGN_CENTER, "LEADERBOARD");
        }

        al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 + 55,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 160, ALLEGRO_ALIGN_CENTER, "SAIR");
        if(estado_botao == EM_SAIR) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 + 55,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 160, ALLEGRO_ALIGN_CENTER, "SAIR");
        }

        //al_flip_display();
        //al_rest(0.005);
    }

    cont_frames = 0;
    coluna_atual = 0;
    desenha = 0;
    pos_x_sprite = 250;
    pos_y_sprite = 400;
    pos_x_2_sprite = 750;
    pos_y_2_sprite = 400;
    int pos_x_3_sprite = 1250;
    int pos_y_3_sprite = 400;
    int limite = 0;
    char skin;

    while(estado_tela == GAME_MENU) {
        estados_game_menu estado;
        while(!al_is_event_queue_empty(fila_eventos)) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                if(evento.mouse.x >= 0 && evento.mouse.x <= LARGURA_TELA &&
                evento.mouse.y >= 50 && evento.mouse.y <= 50 + al_get_bitmap_height(name_button)) {
                    estado = EM_NAME;
                }
                if(evento.mouse.x >= 0 && evento.mouse.x <= LARGURA_TELA &&
                evento.mouse.y >= 500 && evento.mouse.y <= 500 + al_get_bitmap_height(ip_button)) {
                    estado = EM_IP;
                }
                if(evento.mouse.x >= 50 && evento.mouse.x <= 50 + al_get_bitmap_width(matias_button) &&
                evento.mouse.y >= 450 && evento.mouse.y <= 450 + al_get_bitmap_height(matias_button)) {
                    estado = EM_MATIAS;
                }
                if(evento.mouse.x >= 250 && evento.mouse.x <= 250 + al_get_bitmap_width(josue_button) &&
                evento.mouse.y >= 450 && evento.mouse.y <= 450 + al_get_bitmap_height(josue_button)) {
                    estado = EM_JOSUE;
                }
                if(evento.mouse.x >= 450 && evento.mouse.x <= 450 + al_get_bitmap_width(josue_button) &&
                evento.mouse.y >= 450 && evento.mouse.y <= 450 + al_get_bitmap_height(josue_button)) {
                    estado = EM_JOSIAS;
                }
            }
        }

        ALLEGRO_EVENT evento_timer;
        al_wait_for_event(fila_eventos_timer, &evento_timer);

        if(evento_timer.type == ALLEGRO_EVENT_TIMER) {
            cont_frames += 1;

            if(cont_frames >= frames_sprite) {
                cont_frames = 0;
                coluna_atual += 1;
                if(coluna_atual >= colunas_folha) {
                    coluna_atual = 0;

                }
                regiao_x_folha = coluna_atual * largura_sprite;
            }
            pos_x_sprite += 0;
            pos_y_sprite += 0;
            pos_x_2_sprite += 0;
            pos_y_2_sprite += 0;
            pos_x_3_sprite += 0;
            pos_y_3_sprite += 0;

            desenha = 1;
        }

        if(desenha && al_is_event_queue_empty(fila_eventos_timer)) {
                al_draw_scaled_bitmap(folha_1_sprite,
                regiao_x_folha, regiao_y_folha,
                largura_sprite, altura_sprite,
                pos_x_sprite, pos_y_sprite, 150, 150, 0);

                al_draw_scaled_bitmap(folha_2_sprite,
                regiao_x_folha, regiao_y_2_folha,
                largura_sprite, altura_2_sprite,
                pos_x_2_sprite, pos_y_2_sprite, 150, 150, 0);

                al_draw_scaled_bitmap(folha_3_sprite,
                regiao_x_folha/2, regiao_y_folha/2,
                largura_sprite/2, altura_sprite/2,
                pos_x_3_sprite, pos_y_3_sprite, 150, 150, 0);

                desenha = 0;

                al_flip_display();
        }
        
        al_clear_to_color(al_map_rgb(0, 0, 0));
        
        al_draw_scaled_bitmap(fundo,
        0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo),
        0, 0, LARGURA_TELA, ALTURA_TELA, 0);

        al_draw_text(fonte, al_map_rgb(255, 255, 0), 700, 100, ALLEGRO_ALIGN_RIGHT, "DIGITE SEU NOME: ");
        if(estado == EM_NAME) {
            char str_buffer[BUFFER_SIZE], type_buffer[MSG_MAX_SIZE] = {0};
            char msg_history[HIST_MAX_SIZE][MSG_MAX_SIZE] = {{0}};
            int type_pointer = 0;

            while (1) {
                // LER UMA TECLA DIGITADA
                char ch = getch();
                if (ch == '\n') {
                    type_buffer[type_pointer++] = '\0';
                    //int ret = sendMsgToServer((void *)type_buffer, type_pointer);
                    limite++;
                /*if (ret == SERVER_DISCONNECTED) {
                    return;
                }*/
                    type_pointer = 0;
                    type_buffer[type_pointer] = '\0';
                    break;
                } 
                else if (ch == 127 || ch == 8) {
                    if (type_pointer > 0) {
                        --type_pointer;
                        type_buffer[type_pointer] = '\0';
                    }
                    al_draw_text(fonte, al_map_rgb(255, 255, 0), 900, 200, ALLEGRO_ALIGN_RIGHT, type_buffer);
                } 
                else if (ch != NO_KEY_PRESSED && type_pointer + 1 < MSG_MAX_SIZE) {
                    type_buffer[type_pointer++] = ch;
                    type_buffer[type_pointer] = '\0';
                    al_draw_text(fonte, al_map_rgb(255, 255, 0), 900, 200, ALLEGRO_ALIGN_RIGHT, type_buffer);
                }
            }
        }
        al_draw_text(fonte, al_map_rgb(255, 255, 0), 1100, 600, ALLEGRO_ALIGN_RIGHT, "DIGITE OS DIGITOS FINAIS DO SEU IP: ");
        if(estado == EM_IP) {
            char str_buffer[BUFFER_SIZE], type_buffer[MSG_MAX_SIZE] = {0};
            char msg_history[HIST_MAX_SIZE][MSG_MAX_SIZE] = {{0}};
            int type_pointer = 0;

            while (1) {
                // LER UMA TECLA DIGITADA
                char ch = getch();
                if (ch == '\n') {
                    type_buffer[type_pointer++] = '\0';
                    //int ret = sendMsgToServer((char *)type_buffer, type_pointer);
                /*if (ret == SERVER_DISCONNECTED) {
                    return;
                }*/
                    type_pointer = 0;
                    type_buffer[type_pointer] = '\0';
                    limite++;
                    break;
                } 
                else if (ch == 127 || ch == 8) {
                    if (type_pointer > 0) {
                        --type_pointer;
                        type_buffer[type_pointer] = '\0';
                    }
                    al_draw_text(fonte, al_map_rgb(255, 255, 0), 900, 200, ALLEGRO_ALIGN_RIGHT, type_buffer);
                } 
                else if (ch != NO_KEY_PRESSED && type_pointer + 1 < MSG_MAX_SIZE) {
                    type_buffer[type_pointer++] = ch;
                    type_buffer[type_pointer] = '\0';
                    al_draw_text(fonte, al_map_rgb(255, 255, 0), 900, 200, ALLEGRO_ALIGN_RIGHT, type_buffer);
                }
            }
        }

        al_draw_text(fonte, al_map_rgb(255, 255, 0), 750, 300, ALLEGRO_ALIGN_RIGHT, "ESCOLHA SUA SKIN: ");
        if(estado == EM_MATIAS) {
            skin = '2';
            limite++; 
        }
        if(estado == EM_JOSIAS) {
            skin = '1';
            limite++;
        }
        if(estado == EM_JOSUE) {
            skin = '0';
            limite++;
        }
        
        if(limite == 3) {
            int ret = sendMsgToServer(&skin, 1);
            estado_tela = IN_GAME;
        }


        //al_flip_display();

    }
    al_destroy_bitmap(botao_sair);
    al_destroy_bitmap(botao_jogar);
    al_destroy_bitmap(botao_tutorial);
    al_destroy_bitmap(botao_leaderboard);
    al_destroy_display(janela);
    al_destroy_font(fonte);
    al_destroy_font(fonte_pregame);
    al_destroy_sample(coin);
    al_destroy_event_queue(fila_eventos);
    return 0;
}