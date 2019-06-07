#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <Player.h>
#include <client.h>
#define LARGURA_TELA 1600
#define ALTURA_TELA 900
#define FPS 60.0
//dale dbv
//-lallegro_ttf -lallegro_font -lallegro
//-lallegro -lallegro_audio -lallegro_acodec
    ALLEGRO_BITMAP *botao_jogar = NULL;
    ALLEGRO_BITMAP *botao_tutorial = NULL;
    ALLEGRO_BITMAP *botao_leaderboard = NULL;
    ALLEGRO_BITMAP *botao_opcoes = NULL;
    ALLEGRO_BITMAP *botao_sair = NULL;
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_BITMAP *fundo = NULL;
    ALLEGRO_SAMPLE *coin = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *folha_1_sprite = NULL;
    ALLEGRO_BITMAP *folha_2_sprite = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos_timer = NULL;
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_FONT *fonte_opcoes = NULL;
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

    coin = al_load_sample("coin.wav");
    if (!coin) {
        printf( "Audio nao carregado");
        return 0;
    }


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

    fonte_opcoes = al_load_font("pressStart.ttf", 10, 0);
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
typedef enum{
    nome, skin, ip
}status_opcoes;
void opcoes(){
    //digitar IP, id e escolher o personagem para jogo
    bool sair = false;
    double x = LARGURA_TELA/2, y = 200;
    status_opcoes status;
    Player_Data player;
    enum skin boneco;
    ALLEGRO_EVENT_QUEUE *fila_eventos_opcoes = NULL;
    fila_eventos_opcoes = al_create_event_queue();
    if (!fila_eventos_opcoes) {
        fprintf(stderr, "Falha ao criar fila de eventos de opções.\n");
        return;
    }
    al_register_event_source(fila_eventos_opcoes, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos_opcoes, al_get_keyboard_event_source());
    status = nome;
    int tamanho=0; //tamanho = tamanho do nome do player
    while(!sair){
        al_draw_scaled_bitmap(fundo,
        0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo),
        0, 0, LARGURA_TELA, ALTURA_TELA, 0);
        al_flip_display();
        while(!al_is_event_queue_empty(fila_eventos_opcoes)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos_opcoes, &evento);
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(evento.keyboard.keycode){
                    case ALLEGRO_KEY_ESCAPE: sair=true; break;
                    case ALLEGRO_KEY_DOWN:
                    {
                        y+=300;
                        if(status == ip) status = nome;
                        else status++;
                        break;
                    }
            }
            if(status == nome) {
                char ch = getch();
                al_draw_text(fonte_opcoes, al_map_rgb(255, 255, 0), LARGURA_TELA/2, 100, ALLEGRO_ALIGN_CENTRE, "DIGITE SEU NOME: ");
                while(1){
                    if(ch=='\n') {
                        status++;
                        if(tamanho<20) player.nome[tamanho++] = '\0';
                        break;
                    }
                    else if(ch == 127 || ch == 8){ //apagar o nome
                        if(tamanho>0){
                            --tamanho;
                            player.nome[tamanho] = '\0';
                        }
                    }
                    else {
                        if(tamanho<20 && ch!=NO_KEY_PRESSED) {
                            player.nome[tamanho] = ch;
                            tamanho++;
                            player.nome[tamanho] = '\0';
                        }
                    }
                }
            }
            else if(status == skin){

            }
            else if(status == ip){
                //ip = 127.20.4.X
                char ch = getch();
                al_draw_text(fonte_opcoes, al_map_rgb(255, 255, 0), LARGURA_TELA/2, 400, ALLEGRO_ALIGN_CENTRE, "DIGITE O IP: ");
                al_draw_text(fonte_opcoes, al_map_rgb(255, 255, 0), LARGURA_TELA/2, 450, ALLEGRO_ALIGN_CENTRE, "172.20.4.");
                while(1){
                    if(ch=='\n') {
                        status = nome;
                        
                        break;
                    }
                    else if(ch == 127 || ch == 8){ //apagar o nome
                        
                    }
                    else {
                        if( && ch!=NO_KEY_PRESSED){
                            
                        }
                    }
                }
            }
        }
    }
    }
    al_destroy_event_queue(fila_eventos_opcoes);
}
int main()
{

    int desenha = 1;
    int acesso_ao_jogo = 0; //so eh possivel jogar se antes o player acessar a parte de opcoes e botar seus dados.
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
    int sair = 0;
    if(!inicializar()) {
        exit(-1);
    }

    inicializa_botoes_menu();

    estados estado;

    while(!sair) {
        while(!al_is_event_queue_empty(fila_eventos)) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
            //al_wait_for_event(fila_eventos, &evento_timer);
            sair = 0;
            if(evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogar) / 2 - 57 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_jogar) / 2 + 53 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 - 40 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_jogar) / 2 - 28) {
                    al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    estado = em_jogar;
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 - 70 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_tutorial) / 2 + 78 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_tutorial) / 2 + 10 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_tutorial) / 2 + 20) {
                    al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    estado = em_tutorial;
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_leaderboard) / 2 - 125 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_leaderboard) / 2 + 135 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_leaderboard) / 2  + 110 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_leaderboard) / 2 + 122) {
                    al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    estado = em_leaderboard;
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_opcoes) / 2 - 65 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_opcoes) / 2 + 60 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_opcoes) / 2 + 60 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_opcoes) / 2 + 90) {
                    al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    estado = em_opcoes;
                }
                else if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 - 25 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_sair) / 2 + 30 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_sair) / 2 + 160 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_sair) / 2 + 173) {
                    al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    estado = em_sair;
                }
                else {
                    estado = em_nenhum;
                }
                sair = 0;
            }
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) sair = 1;
            else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 - 25 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_sair) / 2 + 30 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_sair) / 2 + 160 &&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_sair) / 2 + 173) {
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
                if (evento.mouse.x >= LARGURA_TELA / 2 - al_get_bitmap_width(botao_opcoes) / 2 - 60 &&
                evento.mouse.x <= LARGURA_TELA / 2 + al_get_bitmap_width(botao_opcoes) / 2 + 60 &&
                evento.mouse.y >= ALTURA_TELA / 2 - al_get_bitmap_height(botao_opcoes) / 2 + 60&&
                evento.mouse.y <= ALTURA_TELA / 2 + al_get_bitmap_height(botao_opcoes) / 2 + 90){
                    sair = 0;
                    opcoes();
                    acesso_ao_jogo = 1;
                    //atualiza a tela com os opcoes
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
        if (estado == em_jogar) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_jogar) / 2 + 45,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 - 40, ALLEGRO_ALIGN_CENTER, "JOGAR");
        }
        
        al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 + 80,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 10, ALLEGRO_ALIGN_CENTER, "TUTORIAL");
        if(estado == em_tutorial) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_tutorial) / 2 + 80,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 10, ALLEGRO_ALIGN_CENTER, "TUTORIAL");
        }

        al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_leaderboard) / 2 + 90,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 110, ALLEGRO_ALIGN_CENTER, "LEADERBOARD");
        if(estado == em_leaderboard) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_leaderboard) / 2 + 90,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 110, ALLEGRO_ALIGN_CENTER, "LEADERBOARD");
        }

        al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_opcoes) / 2 + 55,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 60, ALLEGRO_ALIGN_CENTER, "OPÇÕES");
        if(estado == em_opcoes) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_opcoes) / 2 + 55,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 60, ALLEGRO_ALIGN_CENTER, "OPÇÕES");
        }

        al_draw_text(fonte, al_map_rgb(255, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 + 55,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 160, ALLEGRO_ALIGN_CENTER, "SAIR");
        if(estado == em_sair) {
            al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2 - al_get_bitmap_width(botao_sair) / 2 + 55,
        ALTURA_TELA / 2 - al_get_bitmap_height(botao_jogar) / 2 + 160, ALLEGRO_ALIGN_CENTER, "SAIR");
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
    al_destroy_sample(coin);
    al_destroy_event_queue(fila_eventos);
    return 0;
}