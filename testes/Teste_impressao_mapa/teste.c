// Inclui o cabeçalho da bibilioteca Allegro 5
#include <allegro5/allegro.h>
 
// Inclui o cabeçalho do add-on para uso de imagens
#include <allegro5/allegro_image.h>

#define LARGURA_TELA 1600
#define ALTURA_TELA 900
#define PIXEL_BASE_X 398
#define PIXEL_BASE_Y 180
#define LARGURA_AREA_DE_IMPRESSAO 576
#define ALTURA_AREA_DE_IMPRESSAO 324

    // Variável representando a janela principal
    ALLEGRO_DISPLAY *janela = NULL;
 
    // Variável representando a imagem
    ALLEGRO_BITMAP *imagem = NULL;

    ALLEGRO_TRANSFORM camera;

float cameraPosition[2] = {0,0};

void cameraUpdate(float* cameraPosition, float x, float y, int width, int height ){
    printf("x=%g y=%g\n",x,y);
    cameraPosition[0] = -(LARGURA_TELA/2) + (x + width/2);
    cameraPosition[1] = -(ALTURA_TELA/2) + (y + height/2);
    //if(cameraPosition[0] < 0) cameraPosition[0] = 0;
    //if(cameraPosition[1] < 0) cameraPosition[1] = 0;
}

void imprimirSessaoDoMapa(int x,int y){
    al_draw_scaled_bitmap(imagem,
                        (x*32) + PIXEL_BASE_X - (LARGURA_AREA_DE_IMPRESSAO/2), (y*32) + PIXEL_BASE_Y - (ALTURA_AREA_DE_IMPRESSAO/2),
                        LARGURA_AREA_DE_IMPRESSAO,ALTURA_AREA_DE_IMPRESSAO,
                        0,0,LARGURA_TELA,ALTURA_TELA,0);
}

int main(void)
{

 
    // Inicializa a Allegro
    al_init();
 
    // Inicializa o add-on para utilização de imagens
    al_init_image_addon();
 
    // Configura a janela
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
 
    // Carrega a imagem
    imagem = al_load_bitmap("mapadef.png");
 
    // Desenha a imagem na tela
    
    //al_draw_bitmap(imagem,0,0,NULL);
    int i;
    for(i=0;i<=10;i++){
        //imprimirSessaoDoMapa(i,0);
        cameraUpdate(cameraPosition,PIXEL_BASE_X + i*32,PIXEL_BASE_Y,32,32);
        al_identity_transform(&camera);
        al_translate_transform(&camera,-cameraPosition[0],cameraPosition[1]);
        al_scale_transform(&camera,5.0,5.0);
        al_use_transform(&camera);
        al_draw_bitmap(imagem,0,0,NULL);
        al_flip_display();
        al_rest(0.5);
    }
    for(i=0;i<=10;i++){
        //imprimirSessaoDoMapa(10,i);
        cameraUpdate(cameraPosition,PIXEL_BASE_X + 320,PIXEL_BASE_Y + i*32,32,32);
        al_identity_transform(&camera);
        al_translate_transform(&camera,-cameraPosition[0],cameraPosition[1]);
        al_scale_transform(&camera,5.0,5.0);
        al_use_transform(&camera);
        al_draw_bitmap(imagem,0,0,NULL);
        al_flip_display();
        al_rest(0.5);
    }

    al_destroy_display(janela);
 
    return 0;
}
