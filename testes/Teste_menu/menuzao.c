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

#define LARGURA_TELA 1500
#define ALTURA_TELA 800
#define FPS 60.0
#define frameFPS 12.0
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

Enemy_Data enemy;
Player_Data player;
Player_Data auxPlayer;

int larguraCarta;
int alturaCarta;

#define passos 16.0

int passosCounter=0, passosCounterEnemy=0;//quantos saltos de moveSpeed ele vai fazer até chegar em 32
bool done = false, draw = true, animate = false, animateEnemy=false, animateTRAPDMG=false, animateDMG=false, animateATK=false, animateTRAP=false ,active = false/*,comprou=true*/;
char comprou = NO_ITEM;
float x=0, y=0, ENx=0, ENy=0, moveSpeed = 32/passos;// moveCounter = 0;
unsigned short oldPosX, oldPosY, oldPosEnemyX, oldPosEnemyY;
int sourceX = 32, sourceY = 0, sourceEnemyX = 32, sourceEnemyY=0, sourceHPX = 0, sourceHPY=0, sourceATKX = 0, sourceATKY=0, sourceTRAPX = 0, sourceTRAPY = 0, sourceTRAPDMGX = 0, sourceTRAPDMGY = 0;
char use=0;
int cnt60=0, cnt240=0;
char houses[] = {1,2,4,5,6,7,8,17,19,20,21,22,23,24,40,42,43,44,45,46,48,50,60,61,62,64,66,68,69,71,72,73,74,75,76,77,88,-1};
char dmg;
char tempface;
char item_used;
char trapdmg;
bool boool=0;

char skin = 'J';
char type_buffer_name[NOME_MAX_SIZE] = {"nick\0"};
char type_buffer_ip[NOME_MAX_SIZE] = {"127.0.0.1"};


    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_TRANSFORM camera;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
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
    ALLEGRO_TIMER *frameTimer = NULL;
    ALLEGRO_BITMAP *folha_1_sprite = NULL;
    ALLEGRO_BITMAP *folha_2_sprite = NULL;
    ALLEGRO_BITMAP *folha_3_sprite = NULL;
    ALLEGRO_BITMAP *folha_4_sprite = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos_timer = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos_tut = NULL;
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_FONT *fonte_tut = NULL;
    ALLEGRO_FONT *fonte_timer = NULL;
    ALLEGRO_FONT *fonte_jogo = NULL;
    ALLEGRO_AUDIO_STREAM *musica_menu = NULL;
    ALLEGRO_BITMAP *game_icon = NULL;
    ALLEGRO_BITMAP *anim_trap = NULL;
    ALLEGRO_BITMAP *anim_bomb = NULL;
    ALLEGRO_BITMAP *anim_dog = NULL;
    ALLEGRO_BITMAP *atk_throw = NULL;
    ALLEGRO_BITMAP *store_menu = NULL;
    ALLEGRO_BITMAP *heart_carta = NULL;
    ALLEGRO_BITMAP *item_bar = NULL;
    ALLEGRO_BITMAP *box_bar = NULL;
    ALLEGRO_BITMAP *icon_shuricarta = NULL;
    ALLEGRO_BITMAP *icon_trap = NULL;
    ALLEGRO_BITMAP *icon_bomb = NULL;
    ALLEGRO_BITMAP *icon_dog = NULL;
    ALLEGRO_BITMAP *icon_boxes = NULL;
    ALLEGRO_BITMAP *enemy_sprite = NULL;
    ALLEGRO_BITMAP *player_sprite = NULL;
    ALLEGRO_SAMPLE *shurikarta = NULL;
    ALLEGRO_SAMPLE *armadilhaDamage = NULL;
    ALLEGRO_SAMPLE *armadilhaPlaced = NULL;
    ALLEGRO_SAMPLE *bombPlaced = NULL;
    ALLEGRO_SAMPLE *cachorro = NULL;
    ALLEGRO_SAMPLE *caixaobtida = NULL;
    ALLEGRO_SAMPLE *dinheiroObtido = NULL;
    ALLEGRO_SAMPLE *dinheiroDropado = NULL;
    ALLEGRO_SAMPLE *explosao = NULL;
    ALLEGRO_SAMPLE *hospitalcura = NULL;
    ALLEGRO_SAMPLE *hospitalPorta = NULL;
    ALLEGRO_SAMPLE *josiasDano = NULL;
    ALLEGRO_SAMPLE *nani = NULL;
    ALLEGRO_SAMPLE *somfalha = NULL;
    ALLEGRO_SAMPLE *dano = NULL;
    ALLEGRO_SAMPLE *loja = NULL;


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
        WAITING_PLAYER,
        PRE_GAME,
        IN_GAME,
        TUTORIAL_MENU,
        LEADERBOARD_MENU,
        WIN_SCREEN,
        LOSE_SCREEN,
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

     shurikarta = al_load_sample("source/resources/audio/samples/shurikarta.wav");
    if (!shurikarta) {
        printf( "Audio nao carregado1.");
        return 0;
    }
    bombPlaced= al_load_sample("source/resources/audio/samples/bombPlaced.wav");
    if (!bombPlaced) {
        printf( "Audio nao carregado2.");
        return 0;
    }
    cachorro = al_load_sample("source/resources/audio/samples/cachurro.wav");
    if (!cachorro) {
        printf( "Audio nao carregado3.");
        return 0;
    }
    caixaobtida = al_load_sample("source/resources/audio/samples/caixaobtida.wav");
    if (!caixaobtida) {
        printf( "Audio nao carregado4.");
        return 0;
    }
    dinheiroDropado = al_load_sample("source/resources/audio/samples/dinheiroDropado.wav");
    if (!dinheiroDropado) {
        printf( "Audio nao carregado5.");
        return 0;
    }
    dano = al_load_sample("source/resources/audio/samples/dano.wav");
    if (!dano) {
        printf( "Audio nao carregado6.");
        return 0;
    }
    dinheiroObtido = al_load_sample("source/resources/audio/samples/dinheiroObtido.wav");
    if (!dinheiroObtido) {
        printf( "Audio nao carregado7.");
        return 0;
    }
    explosao = al_load_sample("source/resources/audio/samples/explosao.wav");
    if (!explosao) {
        printf( "Audio nao carregado8.");
        return 0;
    }
    hospitalcura = al_load_sample("source/resources/audio/samples/hospitalCura.wav");
    if (!hospitalcura) {
        printf( "Audio nao carregado9.");
        return 0;
    }
    hospitalPorta = al_load_sample("source/resources/audio/samples/hospitalPorta.wav");
    if (!hospitalPorta) {
        printf( "Audio nao carregado10.");
        return 0;
    }
   josiasDano = al_load_sample("source/resources/audio/samples/josiasDano.wav");
    if (!josiasDano) {
        printf( "Audio nao carregado11.");
        return 0;
    }
    nani = al_load_sample("source/resources/audio/samples/nani.wav");
    if (!nani) {
        printf( "Audio nao carregado.12");
        return 0;
    }
    somfalha = al_load_sample("source/resources/audio/samples/som de falha.wav");
    if (!somfalha) {
        printf( "Audio nao carregado13.");
        return 0;
    }
    armadilhaDamage = al_load_sample("source/resources/audio/samples/armadilha.wav");
    if (!armadilhaDamage) {
        printf( "Audio nao carregado14..");
        return 0;
    }

    armadilhaPlaced = al_load_sample("source/resources/audio/samples/armadilhaPlaced.wav");
    if (!armadilhaPlaced) {
        printf( "Audio nao carregado.15..");
        return 0;
    }
    loja = al_load_sample("source/resources/audio/samples/shopDoorBell.wav");
    if (!loja) {
        printf( "Audio nao carregado.16..");
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
    
    frameTimer = al_create_timer(1.0/frameFPS);
    if(!frameTimer) {
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

    store_menu = al_load_bitmap("source/resources/images/backgrounds/Menu de Compras.png");
    if(!store_menu){
        puts("Errou ao carregar Menu de Compras.");
        return 0;
    }

    atk_throw = al_load_bitmap("source/resources/images/ATK.png");
    if(!atk_throw){
        puts("Falha ao carregar ATK.\n");
        return 0;
    }
    anim_dog = al_load_bitmap("source/resources/images/El_Catioro_bmap.png");
    if(!anim_dog){
        puts("Falha ao carregar El_Catioro_bmap.\n");
        return 0;
    }
    anim_bomb = al_load_bitmap("source/resources/images/Armadilha_V2_bmap.png");
    if(!anim_bomb){
        puts("Falha ao carregar Armadilha_V2_bmap.\n");
        return 0;
    }
    anim_trap = al_load_bitmap("source/resources/images/Armadilha_V1_bmap.png");
    if(!anim_trap){
        puts("Falha ao carregar Armadilha_V1_bmap.\n");
        return 0;
    }

    heart_carta = al_load_bitmap("source/resources/images/Life.png");
    if(!heart_carta){
        puts("Falha ao carregar heart_carta.\n");
        return 0;
    }

    larguraCarta = al_get_bitmap_width(heart_carta)/7;
    alturaCarta = al_get_bitmap_height(heart_carta);

    item_bar = al_load_bitmap("source/resources/images/Barra_de_Itens.png");
    if(!item_bar){
        puts("Falha ao carregar Barra_de_Itens.\n");
        return 0;
    }
    box_bar = al_load_bitmap("source/resources/images/Barra_de_Caixas.png");
    if(!box_bar){
        puts("Falha ao carregar Barra_de_Caixas.\n");
        return 0;
    }

    icon_shuricarta = al_load_bitmap("source/resources/images/A_Shurikarta.png");
    if(!icon_shuricarta){
        puts("Falha ao carregar A_Shurikarta.\n");
        return 0;
    }

    icon_trap = al_load_bitmap("source/resources/images/Armadilha_V1.png");
    if(!icon_trap){
        puts("Falha ao carregar Armadilha_V1.\n");
        return 0;
    }
    icon_bomb = al_load_bitmap("source/resources/images/Armadilha_V2.png");
    if(!icon_bomb){
        puts("Falha ao carregar Armadilha_V2.\n");
        return 0;
    }
    icon_dog = al_load_bitmap("source/resources/images/El_Catioro(com limites).png");
    if(!icon_dog){
        puts("Falha ao carregar El_Catioro.\n");
        return 0;
    }
    icon_boxes = al_load_bitmap("source/resources/images/Caixas.png");
    if(!icon_boxes){
        puts("Falha ao carregar Caixas.\n");
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
    fonte_timer = al_load_font("source/resources/fonts/pressStart.ttf", 7, 0); 
    if (!fonte_timer){
        printf("Falha ao carregar fonte\n");
        return -1;
    }
    fonte_jogo = al_load_font("source/resources/fonts/pressStart.ttf", 10, 0);
    if (!fonte_jogo){
        printf("Falha ao carregar fonte\n");
        return -1;
    }

    al_set_window_title(janela, "JOJOSUE'S BIZARRE ADVENTURE");
 
    fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return 0;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
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
    game_icon = al_create_sub_bitmap(folha_1_sprite,32,0,32,32);
    al_set_display_icon(janela, game_icon);

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

    folha_4_sprite = al_load_bitmap("source/resources/images/characters/Al Fredo.png");
    if(!folha_4_sprite) {
        printf("Falha ao carregar folha 4 de sprite\n");
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

float cameraPosition[2] = {0,0}, scale = 3.0;
void cameraUpdate(float* cameraPosition, float x, float y, int width, int height){
    int bordaX = al_get_bitmap_width(fundo) - LARGURA_TELA/scale;
    int bordaY = al_get_bitmap_height(fundo) - ALTURA_TELA/scale;
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
estados_tela estado_tela = MAIN_MENU;
enum conn_ret_t tryConnect() {
	char server_ip[30];
	char server_ID[5];
	
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

void conecta(){
    enum conn_ret_t ret = connectToServer(type_buffer_ip);
    if(ret != SERVER_UP) {
        al_play_sample(coin, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		if (ret == SERVER_DOWN) {
			puts("Server is down!");
		} else if (ret == SERVER_FULL) {
			puts("Server is full!");
		} else if (ret == SERVER_CLOSED) {
			puts("Server is closed for new connections!");
		} else {
			puts("Server didn't respond to connection!");
		}

  	}
    else{
        int len = strlen(type_buffer_name);
        sendMsgToServer((char *)type_buffer_name, len +1);
        sendMsgToServer((char *)&skin,1);
        int ans = recvMsgFromServer(&player, WAIT_FOR_IT);
        estado_tela = WAITING_PLAYER;
        x = (player.posX + 3)*32;
        y = (player.posY + 5)*32;
        fadeout(10);
    }
    
}

void destroi(){
    al_destroy_bitmap(botao_sair);
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
        int linha_atual_waiting = 3;
        int coluna_atual_waiting = 0;
        int pos_x_sprite_waiting = -10;
        int pos_y_sprite_waiting = 700;
        int velocidade = 8;
        int TRABALHO = 0;
        int regiao_y_2_folha_waiting = linha_atual_waiting * altura_2_sprite;
        int regiao_x_folha_waiting = coluna_atual_waiting * largura_sprite, regiao_y_folha_waiting = linha_atual_waiting * altura_sprite;
        while(estado_tela == WAITING_PLAYER) {
            
            // printf("WAITING PLAYER\n");
			// int ret = recvMsgFromServer(&enemy, WAIT_FOR_IT);
            

            printf("WAITING GAME\n");
            al_clear_to_color(al_map_rgb(0, 0, 0));
        
            al_draw_scaled_bitmap(fundo,
            0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo),
            0, 0, LARGURA_TELA, ALTURA_TELA, 0);

            al_draw_text(fonte, al_map_rgb(255, 0, 0), 800, 300 + TRABALHO, ALLEGRO_ALIGN_CENTRE, "WAITING FOR ENEMY");
            TRABALHO++;
            if(TRABALHO == 15) {
                TRABALHO = -15;
            }

            while(!al_is_event_queue_empty(fila_eventos)) {
                ALLEGRO_EVENT evento;
                al_wait_for_event(fila_eventos, &evento);

                if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    estado_tela = ENDGAME;
                }
                else if(evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                    if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                        //disconecta o player e volta para o game_menu
                        closeConnection();
                        estado_tela = GAME_MENU;
                    }
                }
            }

            ALLEGRO_EVENT evento_timer;
            al_wait_for_event(fila_eventos_timer, &evento_timer);
            if(evento_timer.type == ALLEGRO_EVENT_TIMER) {
                cont_frames += 1;

                if(cont_frames >= frames_sprite) {
                    cont_frames = 0;
                    coluna_atual_waiting += 1;
                    if(coluna_atual_waiting >= colunas_folha) {
                        coluna_atual_waiting = 0;

                    }
                    regiao_x_folha_waiting = coluna_atual_waiting * largura_sprite;
                }
                pos_x_sprite_waiting += velocidade;
                pos_y_sprite_waiting += 0;
                if(pos_x_sprite_waiting >= LARGURA_TELA) pos_x_sprite_waiting = -10;

                desenha = 1;
            }           
            

            if(desenha && al_is_event_queue_empty(fila_eventos_timer)) {
                    switch(skin) {
                        case JOSUE:
                            al_draw_scaled_bitmap(folha_1_sprite,
                            regiao_x_folha_waiting, regiao_y_folha_waiting,
                            largura_sprite, altura_sprite,
                            pos_x_sprite_waiting, pos_y_sprite_waiting, 100, 100, 0);
                        break;

                        case JOSIAS:
                            al_draw_scaled_bitmap(folha_2_sprite,
                            regiao_x_folha_waiting, 3*37,
                            largura_sprite, altura_2_sprite,
                            pos_x_sprite_waiting, pos_y_sprite_waiting, 100, 100, 0);
                        break;

                        case MATIAS:
                            al_draw_scaled_bitmap(folha_3_sprite,
                            regiao_x_folha_waiting, regiao_y_folha_waiting,
                            largura_sprite, altura_sprite,
                            pos_x_sprite_waiting, pos_y_sprite_waiting, 100, 100, 0);
                        break;

                        default:
                            al_draw_scaled_bitmap(folha_4_sprite,
                            regiao_x_folha_waiting, regiao_y_folha_waiting,
                            largura_sprite, altura_sprite,
                            pos_x_sprite_waiting, pos_y_sprite_waiting, 100, 100, 0);
                        break;
                    }

                    desenha = 0;

                    al_flip_display();
            }
            int ret = recvMsgFromServer(&enemy, DONT_WAIT);
            if(ret >= 0) {
                estado_tela =  PRE_GAME;
                ENx = (enemy.posX + 3)*32;
                ENy = (enemy.posY + 5)*32;
                oldPosEnemyX = enemy.posX;
                oldPosEnemyY = enemy.posY;
                al_clear_to_color(al_map_rgb(0,0,0));
                al_flip_display();
                fadein(fundo,20);
            }
        }
        while(estado_tela == PRE_GAME) {
            al_register_event_source(event_queue, al_get_timer_event_source(timer));
            al_register_event_source(event_queue,al_get_timer_event_source(frameTimer));
            al_register_event_source(event_queue,al_get_display_event_source(janela));
            al_register_event_source(event_queue,al_get_keyboard_event_source());
            
            al_start_timer(frameTimer);

            al_draw_scaled_bitmap(fundo,
            0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo),
            0, 0, LARGURA_TELA, ALTURA_TELA, 0);
            al_draw_text(fonte,al_map_rgb(0,0,255),LARGURA_TELA/2-400 - 128,ALTURA_TELA/2,ALLEGRO_ALIGN_CENTER,type_buffer_name);
            switch(skin){
                case JOSUE:
                    player_sprite = folha_1_sprite;
                    al_draw_scaled_bitmap(folha_1_sprite,0,0,32,32,LARGURA_TELA/2-400,ALTURA_TELA/2 - 100, 256,256,0);
                    break;
                case JOSIAS:
                    player_sprite = folha_2_sprite;
                    al_draw_scaled_bitmap(folha_2_sprite,0,0,32,37,LARGURA_TELA/2-400,ALTURA_TELA/2 - 100, 256,256,0);
                    break;
                case MATIAS:
                    player_sprite = folha_3_sprite;
                    al_draw_scaled_bitmap(folha_3_sprite,0,0,32,32,LARGURA_TELA/2-400,ALTURA_TELA/2 - 100, 256,256,0);
                    break;
                default:
                    player_sprite = folha_4_sprite;
                    al_draw_scaled_bitmap(folha_4_sprite,0,0,32,32,LARGURA_TELA/2-400,ALTURA_TELA/2 - 100, 256,256,0);
                    break;
            }

            al_draw_text(fonte,al_map_rgb(0,0,255),LARGURA_TELA/2+400 + 128,ALTURA_TELA/2,ALLEGRO_ALIGN_CENTER,enemy.nome);
            switch(enemy.skin){
                case JOSUE:
                    enemy_sprite = folha_1_sprite;
                    al_draw_scaled_bitmap(folha_1_sprite,0,0,32,32,LARGURA_TELA/2+400 - 256,ALTURA_TELA/2 - 100, 256,256,0);
                    break;
                case JOSIAS:
                    enemy_sprite = folha_2_sprite;
                    al_draw_scaled_bitmap(folha_2_sprite,0,0,32,37,LARGURA_TELA/2+400 - 256,ALTURA_TELA/2 - 100, 256,256,0);
                    break;
                case MATIAS:
                    enemy_sprite = folha_3_sprite;
                    al_draw_scaled_bitmap(folha_3_sprite,0,0,32,32,LARGURA_TELA/2+400 - 256,ALTURA_TELA/2 - 100, 256,256,0);
                    break;
                default:
                    enemy_sprite = folha_4_sprite;
                    al_draw_scaled_bitmap(folha_4_sprite,0,0,32,32,LARGURA_TELA/2+400 - 256,ALTURA_TELA/2 - 100, 256,256,0);
                    break;
            }

            al_draw_text(fonte,al_map_rgb(255,255,0),LARGURA_TELA/2,200,ALLEGRO_ALIGN_CENTER,"CARTEIROS ASSEMBLE");

            al_flip_display();
            
            char serverResponse;
            int ret = recvMsgFromServer(&serverResponse, WAIT_FOR_IT);
            if(serverResponse == 99){
                al_rest(3);
                estado_tela = IN_GAME;
                fadeout(5);

                al_identity_transform(&camera);
                al_translate_transform(&camera, -cameraPosition[0],-cameraPosition[1]);
                al_scale_transform(&camera,scale,scale);
                al_use_transform(&camera);
            }
        }
        while(estado_tela == IN_GAME){

            ALLEGRO_EVENT events;
            al_wait_for_event(event_queue, &events);
            al_get_keyboard_state(&keyState);

            int ret = recvMsgFromServer(&auxPlayer,DONT_WAIT);
			if(ret == SERVER_DISCONNECTED){
				printf("O SERVER MORREU VEY\n");
			}
			else if(ret != NO_MESSAGE){
				if(auxPlayer.ID == player.ID){ //se for a estrutura deste jogador
                    puts("Recebeu propria estrutura");
                    if(auxPlayer.identifier == DAMAGE || auxPlayer.identifier == POSITION){
                        if(player.HP>auxPlayer.HP){
                            animateDMG = 1;
                            
                            dmg = player.HP - auxPlayer.HP;
                            trapdmg = dmg;
                            if(dmg > 1) animateTRAPDMG = 1;
                            if(skin==JOSIAS){
                                al_play_sample(josiasDano, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            }
                            else {
                                al_play_sample(dano, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);    
                            }
                        }
                    }
                    if(auxPlayer.identifier == ITEM_USAGE){
                        for(int i = 0; i<3; i++){
                            if(player.itemArray[i]!=SHURICARD&&player.itemArray[i]!=NO_ITEM){
                                if(player.itemArray[i]!=auxPlayer.itemArray[i]){
                                    item_used = player.itemArray[i];
                                    animateTRAP = 1;
                                    break;
                                }
                            }
                        }
                    }
                    if(player.identifier == BOX_CHANGE){
                        for(int i = 0; i<5; i++){
                            if(player.boxArray[i].type < auxPlayer.boxArray[i].type){
                                al_play_sample(caixaobtida, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            }
                        }
                    }
					player = auxPlayer;	
                    if(player.identifier == POSITION){
                        boool=1;
                        if(active) animate = true;
                    }
                    else if(player.identifier == BUY){
                        al_play_sample(dinheiroDropado, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }
					if(player.identifier == LOSE){
					    estado_tela = ENDGAME;
                        break;
					}
                    else if(enemy.identifier == LOSE){
                        estado_tela = ENDGAME;
                        break;
                    }
                    // oldPosX = player.posX;
                    // oldPosY = player.posY;
                    //player.posX + 3)*32;
                    //player.posY + 5)*32;
                    
				}
				else{ //se for a estrututura do inimigo
                    puts("Recebeu estrutura inimigo");
					enemy.posX = auxPlayer.posX;
					enemy.posY = auxPlayer.posY;
					enemy.HP = auxPlayer.HP;
                    enemy.identifier = auxPlayer.identifier;
                    enemy.face = auxPlayer.face;
                    if(enemy.identifier == POSITION){
                        animateEnemy = true;
                    }
					if(player.identifier == WIN||enemy.identifier == LOSE){
					    estado_tela = WIN_SCREEN;
                        break;
					}
                    else if(player.identifier == LOSE||enemy.identifier == WIN){
                        estado_tela = LOSE_SCREEN;
                        break;
                    }
				}
			    //printf("[%d][%d][%d] - HP: %d - TAOK's: %d\n",player.itemArray[0],player.itemArray[1],player.itemArray[2],player.HP,player.money);
			//printf("BOX 1 - %s/%d - BOX 2 - %s/%d - BOX 3 - %s/%d - BOX 4 - %s/%d - BOX 5 - %s/%d\n",(player.boxArray[0].type==PAC?"PAC":(player.boxArray[0].type==SEDEX?"SEDEX":(player.boxArray[0].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[0].addIndex],(player.boxArray[1].type==PAC?"PAC":(player.boxArray[1].type==SEDEX?"SEDEX":(player.boxArray[1].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[1].addIndex],(player.boxArray[2].type==PAC?"PAC":(player.boxArray[2].type==SEDEX?"SEDEX":(player.boxArray[2].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[2].addIndex],(player.boxArray[3].type==PAC?"PAC":(player.boxArray[3].type==SEDEX?"SEDEX":(player.boxArray[3].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[3].addIndex],(player.boxArray[4].type==PAC?"PAC":(player.boxArray[4].type==SEDEX?"SEDEX":(player.boxArray[4].type==EXPRESS?"EXPRESSO":"SEM CAIXA"))),houses[player.boxArray[4].addIndex]);
			}
			  
            if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                estado_tela = ENDGAME;
            }
            else if(events.type == ALLEGRO_EVENT_TIMER)
            {
                if(events.timer.source == timer) // 1/60
                {
                    if(cnt60++==40){
                    comprou=0;
                    use = 0;
                    cnt60=0;
                    }
                    
                    if(active == false && !animateTRAP && !animateTRAPDMG){
                        active = true;
                        
                        if(al_key_down(&keyState, ALLEGRO_KEY_DOWN)){
                            //puts("Entrou no KEY_DOWN");
                            //if(!animateTRAP){
                            char key = DOWN_ARROW;
                            oldPosX = player.posX;
                            oldPosY = player.posY;
                            sendMsgToServer((char *)&key,1);//}
                        }
                        else if(al_key_down(&keyState, ALLEGRO_KEY_UP)){
                            //puts("Entrou no KEY_UP");
                            //if(!animateTRAP){
                            char key = UP_ARROW;
                            oldPosX = player.posX;
                            oldPosY = player.posY;
                            sendMsgToServer((char *)&key,1);//}
                        }
                        else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT)){
                            //puts("Entrou no KEY_RIGHT");
                            //if(!animateTRAP){
                            char key = RIGHT_ARROW;
                            oldPosX = player.posX;
                            oldPosY = player.posY;
                            sendMsgToServer((char *)&key,1);//}
                        }
                        else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT)){
                            //puts("Entrou no KEY_LEFT");
                            //if(!animateTRAP){
                            char key = LEFT_ARROW;
                            oldPosX = player.posX;
                            oldPosY = player.posY;
                            sendMsgToServer((char *)&key,1);//}
                        }
                        else{
                            active = false;
                        }
                    }
                    //else{
                    if(animate){
                        passosCounter++;

                        if(oldPosX != player.posX || oldPosY != player.posY){
                            if(player.face == DOWN) y += moveSpeed;
                            else if(player.face == UP) y -= moveSpeed;
                            else if(player.face == RIGHT) x += moveSpeed;
                            else if(player.face == LEFT) x -= moveSpeed;
                            //printf("x=%g y=%g\n",x,y);
                        }
                        
                        if(passosCounter == passos) {
                            //printf("terimnou x=%g y=%g moveCounter=%g\n",x,y,moveCounter);
                            passosCounter = 0;
                            active = false;
                            animate = false;
                            // posX = (int)(x/32 - 3);
                            // posY = (int)(y/32 - 5);
                            x = (player.posX + 3)*32;
                            y = (player.posY + 5)*32;
                            //printf("x=%g y=%g\n",x,y);
                            //printf("posX=%d posY=%d x=%g y=%g\n",player.posX,player.posY,x,y);
                        }
                        
                    }
                    if(animateEnemy){
                        passosCounterEnemy++;
                        if(oldPosEnemyX != enemy.posX || oldPosEnemyY != enemy.posY){
                            if(enemy.face == DOWN) ENy += moveSpeed;
                            else if(enemy.face == UP) ENy -= moveSpeed;
                            else if(enemy.face == RIGHT) ENx += moveSpeed;
                            else if(enemy.face == LEFT) ENx -= moveSpeed;
                            //printf("ENx=%g ENy=%g\n",ENx,ENy);
                        }

                        if(passosCounterEnemy == passos) {
                            passosCounterEnemy = 0;
                            ENx = (enemy.posX + 3)*32;
                            ENy = (enemy.posY + 5)*32;
                            animateEnemy = false;
                            oldPosEnemyX = enemy.posX;
                            oldPosEnemyY = enemy.posY;
                            //printf("enemy.posY=%d enemy.posY=%d ENx=%g ENy=%g\n",enemy.posX,enemy.posY,ENx,ENy);
                        }

                    }

                    if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)){
                        estado_tela = ENDGAME;
                    }
                    else if(al_key_down(&keyState, ALLEGRO_KEY_Z)){
                        scale += 0.1;
                    }
                    else if(al_key_down(&keyState, ALLEGRO_KEY_X)){
                        scale -= 0.1;
                    }
                    else if(!comprou && al_key_down(&keyState, ALLEGRO_KEY_Q)){
                        puts("Comprei item 1");
                        comprou = SHURICARD;
                        char key = BUY1;
                        sendMsgToServer((char *)&key,1);
                    }
                    else if(!comprou && al_key_down(&keyState, ALLEGRO_KEY_W)){
                        comprou = TRAP;
                        char key = BUY2;
                        sendMsgToServer((char *)&key,1);
                    }
                    else if(!comprou && al_key_down(&keyState, ALLEGRO_KEY_E)){
                        comprou = BOMB;
                        char key = BUY3;
                        sendMsgToServer((char *)&key,1);
                    }
                    else if(!comprou && al_key_down(&keyState, ALLEGRO_KEY_R)){
                        comprou = DOG;
                        char key = BUY4;
                        sendMsgToServer((char *)&key,1);
                    }
                    else if(!use && al_key_down(&keyState, ALLEGRO_KEY_1)){
                        use = 1;
                        //al_play_sample(shurikarta, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        puts("usei item 1");
                        char key = ITEM1_BUTTON;
                        if(player.itemArray[0]==SHURICARD){
                            animateATK=1;
                            tempface = player.face;
                        }
                        sendMsgToServer((char *)&key,1);
                    }
                    else if(!use && al_key_down(&keyState, ALLEGRO_KEY_2)){
                        use = 1;
                        //al_play_sample(armadilhaPlaced, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        char key = ITEM2_BUTTON;
                        sendMsgToServer((char *)&key,1);
                        if(player.itemArray[1]==SHURICARD){
                            animateATK=1;
                            tempface = player.face;
                        }
                    }
                    else if(!use && al_key_down(&keyState, ALLEGRO_KEY_3)){
                        use = 1;
                        char key = ITEM3_BUTTON;
                        sendMsgToServer((char *)&key,1);
                        if(player.itemArray[2]==SHURICARD){
                            animateATK=1;
                            tempface = player.face;
                        }
                    }
                    else if(!use && al_key_down(&keyState, ALLEGRO_KEY_SPACE)){
                        use = 1;
                        char key = ACTION_MESSAGE;
                        sendMsgToServer((char *)&key,1);
                    }
                    // else if(comprou != NO_ITEM && events.type == ALLEGRO_EVENT_KEY_UP){
                    //     comprou = NO_ITEM;
                    // }
                    // else if(comprou == TRAP && al_key_up(&keyState, ALLEGRO_KEY_W)){
                    //     comprou = NO_ITEM;
                    // }
                    // else if(comprou == BOMB && al_key_up(&keyState, ALLEGRO_KEY_E)){
                    //     comprou = NO_ITEM;
                    // }
                    // else if(comprou == DOG && al_key_up(&keyState, ALLEGRO_KEY_R)){
                    //     comprou = NO_ITEM;
                    // }
                    // else
                    // {
                    //     printf("comprou dnv\n");
                    //     comprou = true;
                    // }

                    if(draw)
                    {
                        cameraUpdate(cameraPosition,x,y,al_get_bitmap_width(player_sprite)/4,al_get_bitmap_height(player_sprite)/4);
                        al_identity_transform(&camera);
                        al_translate_transform(&camera, -cameraPosition[0],-cameraPosition[1]);
                        al_scale_transform(&camera,scale,scale);
                        al_use_transform(&camera);

                        ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap(player_sprite, sourceX, sourceY*al_get_bitmap_height(player_sprite)/4,al_get_bitmap_width(player_sprite)/4,al_get_bitmap_height(player_sprite)/4);
                        ALLEGRO_BITMAP *subBitmapEnemy = al_create_sub_bitmap(enemy_sprite, sourceEnemyX, sourceEnemyY*al_get_bitmap_height(enemy_sprite)/4,al_get_bitmap_width(enemy_sprite)/4,al_get_bitmap_height(enemy_sprite)/4);
                        al_draw_bitmap(fundo,0,0,0);
                        if(animateATK){
                            if(tempface == UP){
                                    al_draw_bitmap_region(atk_throw, sourceATKX, 32, 32, 32,x, y-32, 0);
                                    al_play_sample(shurikarta, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            }
                        }
                        else if(animateTRAP){
                            if(item_used == TRAP){ 
                                switch(player.face){
                                    case DOWN: al_draw_bitmap_region(anim_trap, sourceTRAPX, 0, 32, 32,x, y+32, 0);
                                    al_play_sample(armadilhaPlaced, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    break;
                                    case UP: al_draw_bitmap_region(anim_trap, sourceTRAPX, 0, 32, 32,x, y-32, 0);
                                    al_play_sample(armadilhaPlaced, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);                                                        
                                    break;
                                    case LEFT: al_draw_bitmap_region(anim_trap, sourceTRAPX, 0, 32, 32,x-32, y, 0);
                                    al_play_sample(armadilhaPlaced, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);                                    
                                    break;
                                    case RIGHT: al_draw_bitmap_region(anim_trap, sourceTRAPX, 0, 32, 32,x+32, y, 0);
                                    al_play_sample(armadilhaPlaced, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    break;
                                }
                                //al_draw_bitmap_region(anim_trap, sourceTRAPX, 32, 32, 32,x, y-32, 0);
                            }
                            else if(item_used == BOMB){
                                switch(player.face){
                                    case DOWN: al_draw_bitmap_region(anim_bomb, sourceTRAPX, 0, 32, 32,x, y+32, 0);
                                    al_play_sample(bombPlaced, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    break;
                                    case UP: al_draw_bitmap_region(anim_bomb, sourceTRAPX, 0, 32, 32,x, y-32, 0);
                                    al_play_sample(bombPlaced, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    break;
                                    case LEFT: al_draw_bitmap_region(anim_bomb, sourceTRAPX, 0, 32, 32,x-32, y, 0);
                                    al_play_sample(bombPlaced, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    break;
                                    case RIGHT: al_draw_bitmap_region(anim_bomb, sourceTRAPX, 0, 32, 32,x+32, y, 0);
                                    al_play_sample(bombPlaced, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    break;
                                }
                                 //al_draw_bitmap_region(anim_bomb, sourceTRAPX, 32, 32, 32,x, y-32, 0);
                                 }
                            else if(item_used == DOG){
                                switch(player.face){
                                    case DOWN: al_draw_bitmap_region(anim_dog, sourceTRAPX, 0, 32, 32,x, y+32, 0);
                                    al_play_sample(cachorro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    break;
                                    case UP: al_draw_bitmap_region(anim_dog, sourceTRAPX, 0, 32, 32,x, y-32, 0);
                                    al_play_sample(cachorro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    break;
                                    case LEFT: al_draw_bitmap_region(anim_dog, sourceTRAPX, 0, 32, 32,x-32, y, 0);
                                    al_play_sample(cachorro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    break;
                                    case RIGHT: al_draw_bitmap_region(anim_dog, sourceTRAPX, 0, 32, 32,x+32, y, 0);
                                    al_play_sample(cachorro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    break;
                                }
                                 //al_draw_bitmap_region(anim_dog, sourceTRAPX, 32, 32, 32,x, y-32, 0);
                            }
                        }

                        if(skin != JOSIAS) al_draw_bitmap(subBitmap,x,y,0);
                        else al_draw_bitmap(subBitmap,x,y-5,0);
                        if(enemy.skin != JOSIAS) al_draw_bitmap(subBitmapEnemy,ENx,ENy,0);
                        else al_draw_bitmap(subBitmapEnemy,ENx,ENy-5,0);

                        if(animateTRAPDMG){
                            switch(trapdmg){
                                case 2:
                                    al_draw_bitmap_region(anim_trap, sourceTRAPDMGX, 32, 32, 32,(player.posX + 3)*32, (player.posY + 5)*32, 0);
                                    al_play_sample(armadilhaDamage, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);                                    
                                break;
                                case 3:
                                    al_draw_bitmap_region(anim_bomb, sourceTRAPDMGX, 32, 32, 32,(player.posX + 3)*32, (player.posY + 5)*32, 0);
                                    al_play_sample(explosao, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                break;
                                case 5:
                                    al_draw_bitmap_region(anim_dog, sourceTRAPDMGX, 32, 32, 32,(player.posX + 3)*32, (player.posY + 5)*32, 0);
                                    al_play_sample(nani, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                break;
                            }
                        }

                        if(animateATK){
                            switch(tempface){

                                case DOWN:
                                    al_play_sample(shurikarta, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    al_draw_bitmap_region(atk_throw, sourceATKX, 0, 32, 32,x, y+22, 0);
                                    break;
                                case LEFT:
                                    al_play_sample(shurikarta, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    al_draw_bitmap_region(atk_throw, sourceATKX, 64, 32, 32,x-22, y, 0);
                                    break;
                                case RIGHT:
                                    al_play_sample(shurikarta, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    al_draw_bitmap_region(atk_throw, sourceATKX, 92, 32, 32,x+22, y, 0);
                                    break;

                            }
                        }
                        al_identity_transform(&camera);
                        al_translate_transform(&camera,0,0);
                        //al_scale_transform(&camera,scale,scale);
                        al_use_transform(&camera);

                        for(int i=0;i<player.HP;i++){
                            al_draw_bitmap_region(heart_carta, 0, 0, larguraCarta, alturaCarta, i*larguraCarta + 5, 20, 0);
                        }
                        if(animateDMG){
                            for(int i=0; i<dmg; i++){
                                //puts("OPLES");
                                al_draw_bitmap_region(heart_carta, sourceHPX, 0, 32, 32,(i+player.HP)*larguraCarta + 5, 20, 0);
                            }
                        }
                        
                        al_draw_textf(fonte_jogo,al_map_rgb(0,255,0),32,alturaCarta +50,0,"%d Taoquei's",player.money);
                        al_draw_textf(fonte_jogo,al_map_rgb((player.reputation<70? 255: 0),(player.reputation>30? 255: 0),0),32,alturaCarta +80,0,"Reputacao: %d%%",player.reputation);
                        al_draw_scaled_bitmap(item_bar,0,0,34,96,0,ALTURA_TELA/4,68,192,NULL);
                        al_draw_scaled_bitmap(box_bar,0,0,160,40,(LARGURA_TELA/2)-160,ALTURA_TELA-80,320,80,NULL);

                        for(int i=0;i<3;i++){
                            if(player.itemArray[i] != NO_ITEM){
                                switch (player.itemArray[i]){
                                    case SHURICARD:
                                        al_draw_scaled_bitmap(icon_shuricarta,0,0,32,32,0,ALTURA_TELA/4 + i*62,64,64,NULL);
                                        break;
                                    case TRAP:
                                        al_draw_scaled_bitmap(icon_trap,0,0,32,32,0,ALTURA_TELA/4 + i*62,64,64,NULL);
                                        break;
                                    case BOMB:
                                        al_draw_scaled_bitmap(icon_bomb,0,0,32,32,0,ALTURA_TELA/4 + i*62,64,64,NULL);
                                        break;
                                    case DOG:
                                        al_draw_scaled_bitmap(icon_dog,0,0,32,32,0,ALTURA_TELA/4 + i*62,64,64,NULL);
                                        break;
                                }
                            }
                        }

                        for(int i = 0; i<5; i++){
                            if(player.boxArray[i].type != NO_BOX){
                                switch (player.boxArray[i].type){
                                    case PAC:
                                        al_draw_scaled_bitmap(icon_boxes,0,0,32,32,(LARGURA_TELA/2)-160+(i*62),ALTURA_TELA-80,64,64,0);
                                        break;
                                    case SEDEX:
                                        al_draw_scaled_bitmap(icon_boxes,32,0,32,32,(LARGURA_TELA/2)-160+(i*62),ALTURA_TELA-80,64,64,0);
                                        break;
                                    case EXPRESS:
                                        al_draw_scaled_bitmap(icon_boxes,64,0,32,32,(LARGURA_TELA/2)-160+(i*62),ALTURA_TELA-80,64,64,0);
                                        break;
                                }
                                    
                                al_draw_textf(fonte_timer,al_map_rgb(255,(player.boxArray[i].timeLast>10?255:0),(player.boxArray[i].timeLast>20?255:0)),(LARGURA_TELA/2)-132+(i*62),ALTURA_TELA-11,0,"%d",player.boxArray[i].timeLast);
                                al_draw_textf(fonte_timer,al_map_rgb(255,255,255),(LARGURA_TELA/2)-150+(i*62),ALTURA_TELA-59,0,"Casa %d",houses[player.boxArray[i].addIndex]);
                                
                                if(player.boxArray[i].timeLast<=5){
                                    al_play_sample(somfalha, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                }

                            }

                        }

                        
                        if(player.ID==0){
                            if(player.posX==1 && player.posY==1){
                                al_draw_bitmap(store_menu,(LARGURA_TELA/2)-400,(ALTURA_TELA/2)-225,0);
                                
                                if(boool){
                                    al_play_sample(loja, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    boool = 0;
                                }
                            }                            
                        }
                        else{
                            if(player.posX==42 && player.posY==27){
                                al_draw_bitmap(store_menu,(LARGURA_TELA/2)-400,(ALTURA_TELA/2)-225,0);
                                if(boool){
                                    al_play_sample(loja, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    boool = 0;
                                }
                            }
                        }
                        

                        al_flip_display();
                        al_clear_to_color(al_map_rgb(0,0,0));
                        al_destroy_bitmap(subBitmap);
                        al_destroy_bitmap(subBitmapEnemy);
                    }

                }

                else if(events.timer.source == frameTimer) //i/
                {
                    //ESSE PLAYER
                    if(animate){
                        sourceX += al_get_bitmap_width(player_sprite) / 4;
                    }
                    else{
                        sourceX = 0;
                    }

                    if(sourceX >= al_get_bitmap_width(player_sprite)) sourceX = 0;
                    sourceY = player.face;

                    //INIMIGO
                    if(animateEnemy){
                        sourceEnemyX += al_get_bitmap_width(enemy_sprite) / 4;
                    }
                    else{
                        sourceEnemyX = 0;
                    }

                    if(sourceEnemyX >= al_get_bitmap_width(enemy_sprite)) sourceEnemyX = 0;
                    sourceEnemyY = enemy.face;

                    //VIDA
                    if(animateDMG){
                        sourceHPX += 32;
                        if(sourceHPX==224) animateDMG=0;
                        
                    }
                    else{
                        sourceHPX = 0;
                        dmg = 0;
                    }

                    //TRAP ATIVANDO
                    if(animateTRAPDMG){
                        sourceTRAPDMGX += 32;
                        sourceTRAPDMGY = 1;
                        if(trapdmg == 2){ if(sourceTRAPDMGX>=96){ animateTRAPDMG=0;}}
                        else if(trapdmg == 3){ if(sourceTRAPDMGX>=256){ animateTRAPDMG=0;}}
                        else if(trapdmg == 5){ if(sourceTRAPDMGX>=384){ animateTRAPDMG=0;}}
                    }
                    else{
                        sourceTRAPDMGX = 0;
                        trapdmg = 0;
                    }

                    //ATAQUE DA SHURIKARTA
                    if(animateATK){
                        sourceATKX += 32;
                        sourceATKY = enemy.face;
                        if(sourceATKX==160) animateATK=0;
                    }
                    else{
                        sourceATKX = 0;

                    }

                    //IMPLANTAÇÃO DAS TRAP
                    if(animateTRAP){
                        sourceTRAPX += 32;
                        sourceTRAPY = 0;
                        if(item_used == TRAP){ if(sourceTRAPX>=320){ animateTRAP=0; item_used=0;}}
                        else if(item_used == BOMB){ if(sourceTRAPX>=256){ animateTRAP=0;item_used=0;}}
                        else if(item_used == DOG){ if(sourceTRAPX>=384){ animateTRAP=0;item_used=0;}}
                    }
                    if(!animateTRAP){
                        sourceTRAPX=0;
                    }
                    

                }

                draw = true;

            }
            /*else if(events.type == ALLEGRO_EVENT_KEY_UP){
                printf("ENTROU AQUI %d %c\n",events.keyboard.keycode,events.keyboard.keycode + 'A' -1);
                if((events.keyboard.keycode == ALLEGRO_KEY_Q && comprou == SHURICARD)||(events.keyboard.keycode == ALLEGRO_KEY_W && comprou == TRAP)||(events.keyboard.keycode == ALLEGRO_KEY_E && comprou == BOMB)||(events.keyboard.keycode == ALLEGRO_KEY_R && comprou == DOG)) comprou = NO_ITEM;
            }*/
            // else if(events.type == ALLEGRO_EVENT_KEY_UP && !comprou){
            //     comprou = true;
            //     printf("pode comprou\n");
            // }
            
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
        
        int type_pointer_name = 0;
        int type_pointer_ip = 0;
        // char inicial_name[12] = "mercadinho";
        // char inicial_ip[5] = "321";
        int meajuda = 0, meajuda2 = 0;
        int concluido_name = 0;
        int concluido_ip = 0;
        int pisca_name = 0;
        int pisca_ip = 0;
        int pisca_josias = 0;
        int pisca_josue = 0;
        int pisca_matias = 0;

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
                    if(evento.mouse.x >= LARGURA_TELA - 420 && evento.mouse.x <= LARGURA_TELA - 300 + al_get_text_width(fonte, "CONECTAR") && evento.mouse.y >= ALTURA_TELA - 150 && evento.mouse.y <= ALTURA_TELA - 150 + al_get_font_line_height(fonte)) {
                        //estado = EM_CONECTAR;
                        conecta();
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
                    if(estado == EM_JOSIAS) {
                        if(pisca_josias++ > 40) pisca_josias = 0;
                    }
                    else pisca_josias = 0;
                    if(pisca_josias < 25) {
                        al_draw_scaled_bitmap(folha_2_sprite,
                        regiao_x_folha, regiao_y_2_folha,
                        largura_sprite, altura_2_sprite,
                        750, pos_y_2_sprite, 150, 150, 0);
                    }
                    al_draw_text(fonte_tut, al_map_rgb(0, 0, 255), 750 + 70, pos_y_2_sprite - 30, ALLEGRO_ALIGN_CENTRE, "JOSIAS");

                    if(estado == EM_JOSUE) {
                        if(pisca_josue++ > 40) pisca_josue = 0;
                    }
                    else pisca_josue = 0;
                    if(pisca_josue < 25) {
                        al_draw_scaled_bitmap(folha_1_sprite,
                        regiao_x_folha, regiao_y_folha,
                        largura_sprite, altura_sprite,
                        pos_x_sprite, pos_y_sprite, 150, 150, 0);
                    }
                    al_draw_text(fonte_tut, al_map_rgb(0, 0, 255), pos_x_sprite + 70, pos_y_sprite - 30, ALLEGRO_ALIGN_CENTRE, "JOSUE");
                    
                    if(estado == EM_MATIAS) {
                        if(pisca_matias++ > 40) pisca_matias = 0;
                    }
                    else pisca_matias = 0;
                    if(pisca_matias < 25) {
                        al_draw_scaled_bitmap(folha_3_sprite,
                        regiao_x_folha, regiao_y_folha,
                        largura_sprite, altura_sprite,
                        pos_x_3_sprite, pos_y_3_sprite, 150, 150, 0);
                    }
                    al_draw_text(fonte_tut, al_map_rgb(0, 0, 255), pos_x_3_sprite + 70, pos_y_3_sprite - 30, ALLEGRO_ALIGN_CENTRE, "MATIAS");
                    
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
                skin = 2; 
                printf("skin==%d", skin);
            }
            if(estado == EM_JOSIAS) {
                puts("estado == EM_JOSIAS ");
                skin = 1;
                printf("skin==%d", skin);
            }
            if(estado == EM_JOSUE) {
                puts("estado == EM_JOSUE ");
                skin = 0;
                printf("skin==%d", skin);
            }

            // if(estado == EM_CONECTAR){
            //         //fadeout(3);
            //         conecta();
            // }
            
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
        while(estado_tela == WIN_SCREEN){
            ALLEGRO_BITMAP *vitoria = NULL;
            vitoria = al_load_bitmap("source/resources/images/Vitoria.png");
            //ALLEGRO_BITMAP *derrota = NULL;
            fadein(vitoria,5);

           al_rest(5);
           estado_tela = MAIN_MENU;
        }
        while(estado_tela == LOSE_SCREEN){
            ALLEGRO_BITMAP *derrota = NULL;
            derrota = al_load_bitmap("source/resources/images/Derrota.jpg");
            //ALLEGRO_BITMAP *derrota = NULL;
            fadein(derrota,5);

           al_rest(5);
           estado_tela = MAIN_MENU;
        }

    }
    fadeout(7);
    destroi();
    return 0;
}