#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <stdio.h> 
#define FPS 60.0
#define LARGURA_TELA 800
    #define ALTURA_TELA 380
    //setas do teclado
    #define tecla_cima 0
    #define tecla_baixo 1
    #define tecla_esquerda 2
    #define tecla_direita 3
    #define velY 4
    #define velX 4
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *folha_sprite = NULL;
    ALLEGRO_BITMAP *fundo = NULL;
    int inicializar(){
        if (!al_init()){
            printf("Falha ao inicializar a Allegro");
            return 0;
        }
     
        if (!al_init_image_addon()){
            printf("Falha ao inicializar o addon de imagens");
            return 0;
        }
        if(!al_install_keyboard()){
            printf("Falha ao inicializar o teclado!");
            return 0;
        }
     
        timer = al_create_timer(1.0 / FPS);
        if(!timer) {
            printf("Falha ao criar temporizador");
            return 0;
        }
     
        janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
        if(!janela) {
            printf("Falha ao criar janela");
            al_destroy_timer(timer);
            return 0;
        }
        al_set_window_title(janela, "Sprites");
     
     
        fila_eventos = al_create_event_queue();
        if(!fila_eventos) {
            printf("Falha ao criar fila de eventos");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            return 0;
        }

     
        //carrega a folha de sprites na variavel
        folha_sprite = al_load_bitmap("Josias.png");
        if (!folha_sprite){
            printf("Falha ao carregar sprites");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            return 0;
        }
        //usa a cor rosa como transparencia
        al_convert_mask_to_alpha(folha_sprite,al_map_rgb(255,0,255));
     
        //carrega o fundo
        fundo = al_load_bitmap("background.png");
        if (!fundo){
            printf("Falha ao carregar fundo");
            al_destroy_timer(timer);
            al_destroy_display(janela);
            al_destroy_event_queue(fila_eventos);
            al_destroy_bitmap(folha_sprite);
            return 0;
        }
     
        al_register_event_source(fila_eventos, al_get_display_event_source(janela));
        al_register_event_source(fila_eventos, al_get_keyboard_event_source());
        al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
        al_start_timer(timer);
     
        return 1;
    }
     
    int main(void){
        int sair = 0;
        int tecla = 3, tecla_anterior = tecla;
        //largura e altura de cada sprite dentro da folha
        int altura_sprite=74, largura_sprite=64;
        //quantos sprites tem em cada linha da folha, e a atualmente mostrada
        int colunas_folha=4, coluna_atual=0;
        //quantos sprites tem em cada coluna da folha, e a atualmente mostrada
        int linha_atual=3, linhas_folha=4;
        //posicoes X e Y da folha de sprites que serao mostradas na tela
        int regiao_x_folha=coluna_atual * largura_sprite, regiao_y_folha=linha_atual * altura_sprite;
        //quantos frames devem se passar para atualizar para o proximo sprite
        int frames_sprite=6, cont_frames=0;
        //posicao X Y da janela em que sera mostrado o sprite
        int pos_x_sprite=50, pos_y_sprite=200;
        //velocidade X Y que o sprite ira se mover pela janela
        double vel_x_sprite=velX, vel_y_sprite=velY;
     
        if (!inicializar()){
            return -1;
        }
     
        while(!sair){
            /* -- ATUALIZA TELA -- */
            //desenha o fundo na tela
            al_draw_bitmap_region(fundo,0,0,LARGURA_TELA,ALTURA_TELA,0,0,0);
            if(tecla = tecla_cima){
                al_draw_scaled_bitmap(folha_sprite,
                regiao_x_folha,regiao_y_folha,
                largura_sprite,altura_sprite,
                pos_x_sprite+largura_sprite,pos_y_sprite,
                largura_sprite,altura_sprite,0);
            }
            else if(tecla = tecla_baixo){
                al_draw_bitmap_region(folha_sprite,
                regiao_x_folha,regiao_y_folha,
                largura_sprite,altura_sprite,
                pos_x_sprite,pos_y_sprite,0);
            }
            else if(tecla = tecla_esquerda){
                al_draw_scaled_bitmap(folha_sprite,
                regiao_x_folha,regiao_y_folha,
                largura_sprite,altura_sprite,
                pos_x_sprite+largura_sprite,pos_y_sprite,
                largura_sprite,altura_sprite,0);
            }
            else if(tecla = tecla_direita){
                al_draw_bitmap_region(folha_sprite,
                regiao_x_folha,regiao_y_folha,
                largura_sprite,altura_sprite,
                pos_x_sprite,pos_y_sprite,0);
            }
            else{
                switch (tecla_anterior){
                    //regiao x da folha para mostrar o sprite parado
                    case tecla_cima:
                    {
                        al_draw_scaled_bitmap(folha_sprite,
                        0,regiao_y_folha,
                        largura_sprite,altura_sprite,
                        pos_x_sprite+largura_sprite,pos_y_sprite,
                        largura_sprite,altura_sprite,0);
                        break;
                    }
                    case tecla_baixo:
                    {
                        al_draw_bitmap_region(folha_sprite,
                        0,regiao_y_folha,
                        largura_sprite,altura_sprite,
                        pos_x_sprite,pos_y_sprite,0);
                        break;
                    }
                    case tecla_esquerda:
                    {
                        al_draw_scaled_bitmap(folha_sprite,
                        0,regiao_y_folha,
                        largura_sprite,altura_sprite,
                        pos_x_sprite+largura_sprite,pos_y_sprite,
                        largura_sprite,altura_sprite,0);
                        break;
                    }
                    case tecla_direita:
                    {
                        al_draw_bitmap_region(folha_sprite,
                        0,regiao_y_folha,
                        largura_sprite,altura_sprite,
                        pos_x_sprite,pos_y_sprite,0);
                        break;
                    }
                }
            }
            al_flip_display();
            while(!al_is_event_queue_empty(fila_eventos)) {
                ALLEGRO_EVENT evento;
                al_wait_for_event(fila_eventos, &evento);
                if(evento.type == ALLEGRO_EVENT_TIMER){
                    if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                        al_flush_event_queue(fila_eventos);
                        //verifica qual tecla foi pressionada
                        switch(evento.keyboard.keycode){
                        //seta para cima
                        case ALLEGRO_KEY_UP:
                            tecla = tecla_cima;
                            break;
                        //seta para baixo
                        case ALLEGRO_KEY_DOWN:
                            tecla = tecla_baixo;
                            break;
                        //seta para esquerda
                        case ALLEGRO_KEY_LEFT:
                            tecla = tecla_esquerda;
                            break;
                        //seta para direita.
                        case ALLEGRO_KEY_RIGHT:
                            tecla = tecla_direita;
                            break;
                        }
                        tecla_anterior = tecla;
                    }
                    else if(evento.type == ALLEGRO_EVENT_KEY_UP){
                        tecla = 4; //para ele ficar parado
                    }
                    switch(tecla){
                        case tecla_cima:
                        {
                            linha_atual = 1;
                            vel_y_sprite = -velY;
                            break;
                        }
                        case tecla_baixo:
                        {
                            linha_atual = 0;
                            vel_y_sprite = velY;
                            break;
                        }
                        case tecla_esquerda:
                        {
                            linha_atual = 2;
                            vel_x_sprite = -velX;
                            break;
                        }
                        case tecla_direita:
                        {
                            linha_atual = 3;
                            vel_x_sprite = velX;
                            break;
                        }
                    }
                    if (pos_x_sprite + largura_sprite > LARGURA_TELA - 70 || pos_x_sprite < 20 || pos_y_sprite < 40 || pos_y_sprite + altura_sprite > ALTURA_TELA - 70){ //para n deixar o sprite andar
                            tecla = 4;
                    }
                        if(tecla >=0 && tecla <=3){
                        //a cada disparo do timer, incrementa cont_frames
                        cont_frames++;
                        //se alcancou a quantidade de frames que precisa passar para mudar para o proximo sprite
                            if (cont_frames >= frames_sprite){
                                //reseta cont_frames
                                cont_frames=0;
                                //incrementa a coluna atual, para mostrar o proximo sprite
                                coluna_atual++;
                                //se coluna atual passou da ultima coluna
                                if (coluna_atual >= colunas_folha){
                                    //volta pra coluna inicial
                                    coluna_atual = 0;
                                    regiao_y_folha = linha_atual * altura_sprite;
                                }
                                //calcula a regiao X da folha que sera mostrada
                                regiao_x_folha = coluna_atual * largura_sprite;
                            }
                            //se o sprite estiver perto da borda direita ou esquerda da tela
                            //atualiza as posicoes X Y do sprite de acordo com a velocidade, positiva ou negativa
                                pos_x_sprite += vel_x_sprite;
                                pos_y_sprite += vel_y_sprite;
                        }
                        else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                            sair = 1;
                        }
                    }
                else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    sair = 1;
                }
            }
        }

        al_destroy_bitmap(folha_sprite);
        al_destroy_bitmap(fundo);
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
     
        return 0;
    }