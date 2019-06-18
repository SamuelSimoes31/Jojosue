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
#include <Player.h>

#define LARGURA_TELA 1600
#define ALTURA_TELA 900
#define FPS 60.0
#define NOME_MAX_SIZE 13
//dale dbv
//-lallegro_ttf -lallegro_font -lallegro
//-lallegro -lallegro_audio -lallegro_acodec
/*
source/resources/audio/musics
source/resources/audio/samples
source/resources/fonts
source/resources/images
source/resources/images/backgrounds
source/resources/images/characters
*/
    ALLEGRO_BITMAP *botao_conectar = NULL;
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
    ALLEGRO_BITMAP *fundo = NULL;
    ALLEGRO_BITMAP *titulo = NULL;
    ALLEGRO_SAMPLE *coin = NULL;
    ALLEGRO_SAMPLE *clicking = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *folha_1_sprite = NULL;
    ALLEGRO_BITMAP *folha_2_sprite = NULL;
    ALLEGRO_BITMAP *folha_3_sprite = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos_timer = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos_tut = NULL;
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_FONT *fonte_tut = NULL;
    ALLEGRO_AUDIO_STREAM *musica_menu = NULL;
    typedef struct {
        char nome[14];
        int pontuacao;
    } player_score;

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
        EM_IP,
        EM_VOLTAR,
        EM_CONECTAR
    } estados_game_menu;
int inicializar() {
    if (!al_init()) {
        printf("Falha ao inicializar Allegro.\n");
        return 0;
    }
    al_init_image_addon();
    if (!al_init_image_addon()) {
        printf("Falha ao inicializar add-on allegro_primitives.\n");
        return 0;
    }
    if (!al_init_primitives_addon())
    {
        printf("Falha ao inicializar add-on allegro_primitives.\n");
        return 0;
    }
    if (!al_install_keyboard())
    {
        printf("Falha ao inicializar o teclado.\n");
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

    coin = al_load_sample("source/resources/audio/samples/coin.wav");
    if (!coin) {
        printf( "Audio nao carregado");
        return 0;
    }

    clicking = al_load_sample("source/resources/audio/samples/menuNavegacao.wav");
    if (!clicking) {
        printf( "Audio nao carregado");
        return 0;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        printf("Falha ao criar timer");
        return 0;
    }
    
    fundo = al_load_bitmap("source/resources/images/backgrounds/Mapa.png");
    if(!fundo) {
        printf("Falha ao criar fundo.\n");
        return 0;
    }

    titulo = al_load_bitmap("source/resources/images/Titulo_do_jogo.png");
    if(!titulo) {
        printf("falha ao carregar titulo\n");
        return 0;
    }

    if (!al_init_ttf_addon()){
        printf("Falha ao inicializar add-on allegro_ttf");
        return 0;
    }

    musica_menu = al_load_audio_stream("source/resources/audio/musics/Carteiro(musica menu).ogg", 4, 1024);
    if (!musica_menu)
    {
        printf( "Audio nao carregado" );
        return 0;
    }

    al_attach_audio_stream_to_mixer(musica_menu, al_get_default_mixer());
    al_set_audio_stream_playmode(musica_menu, ALLEGRO_PLAYMODE_LOOP);

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela) {
        fprintf(stderr, "Falha ao criar janela.\n");
        return 0;
    }

    fonte = al_load_font("source/resources/fonts/pressStart.ttf", 40, 0);
    if (!fonte){
        al_destroy_display(janela);
        printf("Falha ao carregar fonte\n");
        return -1;
    }

    fonte_tut = al_load_font("source/resources/fonts/pressStart.ttf", 20, 0);
    if (!fonte_tut){
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

    fila_eventos_tut = al_create_event_queue();
    if (!fila_eventos_tut) {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return 0;
    }

    folha_1_sprite = al_load_bitmap("source/resources/images/characters/Josue.png");
    if(!folha_1_sprite) {
        printf("Falha ao carregar folha de sprite\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        return 0;
    }

    folha_2_sprite = al_load_bitmap("source/resources/images/characters/Josias.png");
    if(!folha_2_sprite) {
        printf("Falha ao carregar folha 2 de sprite\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        return 0;
    }

    folha_3_sprite = al_load_bitmap("source/resources/images/characters/Matias.png");
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
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos_timer, al_get_timer_event_source(timer));
    al_start_timer(timer);

    return 1;
}

void fadeout(int velocidade)
{
    ALLEGRO_BITMAP *buffer = NULL;
    buffer = al_create_bitmap(LARGURA_TELA, ALTURA_TELA);
    al_set_target_bitmap(buffer);
    al_draw_bitmap(al_get_backbuffer(janela), 0, 0, 0);
    al_set_target_bitmap(al_get_backbuffer(janela));
 
    if (velocidade <= 0)
    {
        velocidade = 1;
    }
    else if (velocidade > 15)
    {
        velocidade = 15;
    }
 
    int alfa;
    for (alfa = 0; alfa <= 255; alfa += velocidade)
    {
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        al_draw_tinted_bitmap(buffer, al_map_rgba(255 - alfa, 255 - alfa, 255 - alfa, alfa), 0, 0, 0);
        al_flip_display();
        
    }
 
    al_destroy_bitmap(buffer);
}
 
void fadein(ALLEGRO_BITMAP *imagem, int velocidade)
{
    if (velocidade < 0)
    {
        velocidade = 1;
    }
    else if (velocidade > 15)
    {
        velocidade = 15;
    }
 
    int alfa;
    for (alfa = 0; alfa <= 255; alfa += velocidade)
    {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_tinted_bitmap(imagem, al_map_rgba(alfa, alfa, alfa, alfa), 0, 0, 0);
        al_flip_display();
    }
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

    botao_conectar = al_create_bitmap(100, 25);
    if(!botao_conectar){
        printf("Falha ao criar bitmap\n");
        al_destroy_bitmap(janela);
        exit(0);
    }
}

void printa(ALLEGRO_EVENT evento, char* str)
{
    // if (evento.type == ALLEGRO_EVENT_KEY_CHAR)
    // {
        if (strlen(str) < NOME_MAX_SIZE)
        {
            char temp[] = {evento.keyboard.unichar, '\0'};
            if (evento.keyboard.unichar == ' ')
            {
                strcat(str, temp);
                printf("str==%s\n", str);
            }
            else if (evento.keyboard.unichar >= '!' &&
                     evento.keyboard.unichar <= '?')
            {
                strcat(str, temp);
                printf("str==%s\n", str);
            }
            else if (evento.keyboard.unichar >= 'A' &&
                     evento.keyboard.unichar <= 'Z')
            {
                strcat(str, temp);
                printf("str==%s\n", str);
            }
            else if (evento.keyboard.unichar >= 'a' &&
                     evento.keyboard.unichar <= 'z')
            {
                strcat(str, temp);
                printf("str==%s\n", str);
            }
        }
 
        if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0)
        {
            str[strlen(str) - 1] = '\0';
            printf("str==%s\n", str);
        }
    //}
}

void conecta(){
    bool sair = false;
    while(!sair){

    }
}

void destroi(){
    al_destroy_bitmap(botao_sair);
    al_destroy_bitmap(botao_conectar);
    al_destroy_bitmap(botao_jogar);
    al_destroy_bitmap(botao_tutorial);
    al_destroy_bitmap(botao_leaderboard);
    al_destroy_bitmap(fundo);
    al_destroy_display(janela);
    al_destroy_font(fonte);
    al_destroy_font(fonte_tut);
    al_destroy_sample(coin);
    al_destroy_event_queue(fila_eventos);
    al_destroy_event_queue(fila_eventos_timer);
    al_destroy_timer(timer);
    al_destroy_bitmap(name_button);
    al_destroy_bitmap(matias_button);
    al_destroy_bitmap(josias_button);
    al_destroy_bitmap(josue_button);
    al_destroy_bitmap(ip_button);
    al_destroy_bitmap(folha_1_sprite);
    al_destroy_bitmap(folha_2_sprite);
    al_destroy_bitmap(folha_3_sprite);
    al_destroy_audio_stream(musica_menu);
}

int main()
{
    int desenha = 1;
    //largura e altura de cada sprite dentro da folha
    int altura_sprite = 32, largura_sprite = 32, altura_2_sprite = 37;
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
    int TRAB = 1;
    while(estado_tela != ENDGAME){
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
                        al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        estado_botao = EM_JOGAR;
                    }
                    else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 - 70 &&
                    evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_tutorial) / 2 + 78 &&
                    evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_tutorial) / 2 + 25 &&
                    evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_tutorial) / 2 + 35) {
                        al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        estado_botao = EM_TUTORIAL;
                    }
                    else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_leaderboard) / 2 - 125 &&
                    evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_leaderboard) / 2 + 135 &&
                    evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_leaderboard) / 2  + 95 &&
                    evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_leaderboard) / 2 + 107) {
                        al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        estado_botao = EM_LEADERBOARD;
                    }
                    
                    else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 - 25 &&
                    evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_sair) / 2 + 30 &&
                    evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_sair) / 2 + 160 &&
                    evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_sair) / 2 + 173) {
                        al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        estado_botao = EM_SAIR;
                    }
                    else {
                        estado_botao = EM_NENHUM;
                    }
                }
                else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) estado_tela = ENDGAME;
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
                        al_play_sample(clicking, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        estado_tela = GAME_MENU;
                        //atualiza a tela como o inicio do jogo
                    }
                    if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 - 70 &&
                    evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_tutorial) / 2 + 78 &&
                    evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_tutorial) / 2 + 25 &&
                    evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_tutorial) / 2 + 35){
                        al_play_sample(clicking, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        estado_tela = TUTORIAL_MENU;
                        //atualiza a tela com a tela de tutorial
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
        
            al_draw_scaled_bitmap(fundo,
            0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo),
            0, 0, LARGURA_TELA, ALTURA_TELA, 0);

            al_draw_scaled_bitmap(titulo,
            0, 0, al_get_bitmap_width(titulo), al_get_bitmap_height(titulo),
            al_get_bitmap_width(titulo) + 185, al_get_bitmap_height(titulo) - 100 + TRAB, al_get_bitmap_width(titulo) + 200, al_get_bitmap_height(titulo) + 100, 0);
            
            TRAB++;
            if(TRAB == 15) {
                TRAB = -15;
            }
            

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
        pos_x_2_sprite = LARGURA_TELA / 2 + 200;
        pos_y_2_sprite = 400;
        int pos_x_3_sprite = 1250;
        int pos_y_3_sprite = 400;
        int limite = 0;
        char skin = '-1';
        char type_buffer_name[NOME_MAX_SIZE] = {"nick\0"};
        char type_buffer_ip[NOME_MAX_SIZE] = {"127.0.0.1"};
        int type_pointer_name = 0;
        int type_pointer_ip = 0;
        char inicial_name[12] = "mercadinho";
        char inicial_ip[5] = "321";
        int meajuda = 0, meajuda2 = 0;
        int concluido_name = 0;
        int concluido_ip = 0;
        int pisca_name = 0;
        int pisca_ip = 0;

        estados_game_menu estado = 99;

        while(estado_tela == GAME_MENU) {
            
            while(!al_is_event_queue_empty(fila_eventos)) {
                ALLEGRO_EVENT evento;
                al_wait_for_event(fila_eventos, &evento);

                if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                    if(evento.mouse.x >= 0 && evento.mouse.x <= LARGURA_TELA &&
                    evento.mouse.y >= 50 && evento.mouse.y <= 50 + al_get_bitmap_height(name_button)) {
                        al_play_sample(clicking, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        estado = EM_NAME;
                        //concluido_ip = 1;
                    }
                    if(evento.mouse.x >= 0 && evento.mouse.x <= LARGURA_TELA &&
                    evento.mouse.y >= 600 && evento.mouse.y <= 600 + al_get_bitmap_height(ip_button)) {
                        al_play_sample(clicking, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        //concluido_name = 1;
                        estado = EM_IP;
                    }
                    
                    if(evento.mouse.x >= 1250 && evento.mouse.x <= 1250 + al_get_bitmap_width(matias_button) &&
                    evento.mouse.y >= 400 && evento.mouse.y <= 400 + al_get_bitmap_height(matias_button)) {
                        al_play_sample(clicking, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        estado = EM_MATIAS;
                    }
                    if(evento.mouse.x >= 250 && evento.mouse.x <= 250 + al_get_bitmap_width(josue_button) &&
                    evento.mouse.y >= 400 && evento.mouse.y <= 400 + al_get_bitmap_height(josue_button)) {
                        al_play_sample(clicking, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        estado = EM_JOSUE;
                    }
                    if(evento.mouse.x >= 750 && evento.mouse.x <= 750 + al_get_bitmap_width(josias_button) &&
                    evento.mouse.y >= 400 && evento.mouse.y <= 400 + al_get_bitmap_height(josias_button)) {
                        al_play_sample(clicking, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        estado = EM_JOSIAS;
                    }
                    if(evento.mouse.x >= LARGURA_TELA - al_get_bitmap_width(botao_conectar) - 200 && evento.mouse.x <=LARGURA_TELA && evento.mouse.y >= ALTURA_TELA - 300 && evento.mouse.y <= ALTURA_TELA){
                        estado = EM_CONECTAR;
                    }
                }
                else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    estado_tela = MAIN_MENU;
                }
                else if(evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                        if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                            estado_tela = MAIN_MENU;
                        }
                }
                else if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
                    if(estado == EM_NAME) printa(evento,type_buffer_name);
                    else if(estado == EM_IP) printa(evento,type_buffer_ip);
                }
                if(estado == EM_CONECTAR){
                    fadeout(3);
                    conecta();
                    estado_tela = MAIN_MENU;
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
                    750, pos_y_2_sprite, 150, 150, 0);

                    al_draw_scaled_bitmap(folha_3_sprite,
                    regiao_x_folha, regiao_y_folha,
                    largura_sprite, altura_sprite,
                    pos_x_3_sprite, pos_y_3_sprite, 150, 150, 0);

                    if(estado == EM_NAME){
                        if(pisca_name++ > 40) pisca_name = 0; 
                    }
                    else pisca_name = 0;
                    if(pisca_name < 25) al_draw_text(fonte, al_map_rgb(255, 0, 0), 1250, 100, ALLEGRO_ALIGN_RIGHT, type_buffer_name);
                    if(estado == EM_IP){
                        if(pisca_ip++ > 40) pisca_ip = 0; 
                    }
                    else pisca_ip = 0;
                    if(pisca_ip < 25) al_draw_text(fonte, al_map_rgb(255, 0, 0), 1450, 600, ALLEGRO_ALIGN_RIGHT, type_buffer_ip);

                    desenha = 0;
                    al_flip_display();
            }
            
            al_clear_to_color(al_map_rgb(0, 0, 0));
            
            al_draw_scaled_bitmap(fundo,
            0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo),
            0, 0, LARGURA_TELA, ALTURA_TELA, 0);
            al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA - 100, ALTURA_TELA - 150, ALLEGRO_ALIGN_RIGHT, "CONECTAR");
            al_draw_text(fonte, al_map_rgb(255, 255, 0), 745, 100, ALLEGRO_ALIGN_RIGHT, "DIGITE SEU NOME: ");
            if(estado == EM_NAME) {
                
                //puts("estado == EM_NAME");
                printf("NAME==%s\n", type_buffer_name);
                printf("ip==%s\n", type_buffer_ip);
                // if(!al_is_event_queue_empty(fila_eventos)) {
                //     ALLEGRO_EVENT evento_nome;
                //     al_wait_for_event(fila_eventos, &evento_nome);

                //     //if (!concluido_name) {
                //     printa(evento_nome, type_buffer_name);
                        
 
                //         /*if (evento_nome.type == ALLEGRO_EVENT_KEY_DOWN && evento_nome.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                //             concluido_name = true;
                //         }
                //     }*/
                // }
            }

            

            al_draw_text(fonte, al_map_rgb(255, 255, 0), 985, 600, ALLEGRO_ALIGN_RIGHT, "DIGITE O IP DO SERVER: ");
            if(estado == EM_IP) {
                //puts("estado == EM_IP");
                printf("name==%s\n", type_buffer_name);
                printf("IP==%s\n", type_buffer_ip);
                // if(!al_is_event_queue_empty(fila_eventos)) {
                //     ALLEGRO_EVENT evento_ip;
                //     al_wait_for_event(fila_eventos, &evento_ip);

                //     //if (!concluido_ip) {
                //         printa(evento_ip, type_buffer_ip);
                //         //printf("ip==%s\n", type_buffer_ip);
 
                //        /* if (evento_ip.type == ALLEGRO_EVENT_KEY_DOWN && evento_ip.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                //             concluido_ip = true;
                //         }
                //     }*/
                // }
            }
            //al_draw_text(fonte_voltar, al_map_rgb(255, 255, 0), 100, 10, ALLEGRO_ALIGN_RIGHT, "VOLTAR");
            al_draw_text(fonte, al_map_rgb(255, 255, 0), 785, 300, ALLEGRO_ALIGN_RIGHT, "ESCOLHA SUA SKIN: ");
            if(estado == EM_MATIAS) {
                puts("estado == EM_MATIAS ");
                skin = '2'; 
                printf("skin==%c", skin);
            }
            if(estado == EM_JOSIAS) {
                puts("estado == EM_JOSIAS ");
                skin = '1';
                printf("skin==%c", skin);
            }
            if(estado == EM_JOSUE) {
                puts("estado == EM_JOSUE ");
                skin = '0';
                printf("skin==%c", skin);
            }

            if(skin != '-1' && strcmp(type_buffer_name, inicial_name) != 0 && strcmp(type_buffer_ip, inicial_ip) != 0) {
                //puts("DEU CERTO KRAI");
                int ret = sendMsgToServer((char) &skin, 1);
                int ret2 = sendMsgToServer((char *) type_buffer_name, type_pointer_name);

            }
            
            /*if(limite == 3) {
                int ret = sendMsgToServer(&skin, 1);
                estado_tela = IN_GAME;
            }*/


            //al_flip_display();

        }

        if(estado_tela == TUTORIAL_MENU) {
            fadein(fundo, 3);
        }

        while(estado_tela == TUTORIAL_MENU) {

            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 150, ALLEGRO_ALIGN_LEFT, "  Vivemos em um mundo onde pessoas encomen-");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 175, ALLEGRO_ALIGN_LEFT, "dam produtos compulsivamente a cada minuto. ");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 200, ALLEGRO_ALIGN_LEFT, "Neste mundo, apenas Josias, Josue, Matias e ");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 225, ALLEGRO_ALIGN_LEFT, "Al fredo podem salvar o planeta de um cola-");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 250, ALLEGRO_ALIGN_LEFT, "pso no espaço-tempo. Porém, esses entregado-");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 275, ALLEGRO_ALIGN_LEFT, "res vivem ameaçados por uma possível demis-");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 300, ALLEGRO_ALIGN_LEFT, "são sem justa causa. Assim, sob o seu coman-");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 325, ALLEGRO_ALIGN_LEFT, "do, caro jogador, estes trabalhadores serão");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 350, ALLEGRO_ALIGN_LEFT, "responsáveis por evitar que o espaço seja o-");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 375, ALLEGRO_ALIGN_LEFT, "cupado por encomendas geradas rapidamente na");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 400, ALLEGRO_ALIGN_LEFT, "cidade, de modo que não sejam demitidos. En-");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 425, ALLEGRO_ALIGN_LEFT, "tão, sua função no jogo resume-se em entre");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 450, ALLEGRO_ALIGN_LEFT, "Então, sua função no jogo consiste em entre-");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 475, ALLEGRO_ALIGN_LEFT, "gar mais encomendas do que seu oponente com ");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 500, ALLEGRO_ALIGN_LEFT, "o objetivo de não ficar desempregado. ");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 525, ALLEGRO_ALIGN_LEFT, "Para isso, você pode utilizar armadilhas pa-");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 550, ALLEGRO_ALIGN_LEFT, "ra atrapalhar seu inimigo, podendo até matá-");
            al_draw_text(fonte_tut, al_map_rgb(255, 255, 0), 350, 575, ALLEGRO_ALIGN_LEFT, "-lo. Use as setas para se mover pelo mapa.");

            al_flip_display();

            while(!al_is_event_queue_empty(fila_eventos)) {
                ALLEGRO_EVENT evento;
                al_wait_for_event(fila_eventos, &evento);

                if(evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                        if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                            estado_tela = MAIN_MENU;
                        }
                }
                else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    estado_tela = ENDGAME;
                }
            }

        }

    }
    fadeout(7);
    destroi();
    return 0;
}