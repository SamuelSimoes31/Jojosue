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
    int i;
    for(i=0;i<=10;i++){
        imprimirSessaoDoMapa(i,0);
        al_flip_display();
        al_rest(0.2);
    }
    for(i=0;i<=10;i++){
        imprimirSessaoDoMapa(10,i);
        al_flip_display();
        al_rest(0.2);
    }

    al_destroy_display(janela);
 
    return 0;
}
