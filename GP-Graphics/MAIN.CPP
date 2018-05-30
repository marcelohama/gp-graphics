#include <allegro.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

/******************************************\
| Matriz pra guardar todos os valores f(x) |
\******************************************/
float valy[480],zoom=0;

/***************************************\
| Função pra inicializar o modo gráfico |
\***************************************/
void inicia_allegro()
{
    // inicia a allegro
	allegro_init();
	// habilita o manipulador de teclado
	install_keyboard();
	// define a profundidade de cores
	set_color_depth(16);
	// define o modo gráfico 480x480 sem buffer reserva
	set_gfx_mode(GFX_AUTODETECT, 480, 480, 0, 0);
}

/**************************\
| Desenha o menu principal |
\**************************/
void abertura()
{
    // limpa o bitmap screen
	clear_bitmap(screen);
	// escreve no bitmap screen
	textout(screen, font, "GP-2005 GRAFICOS POLINOMIAIS", 120, 249, makecol(255,0,0));
	textout(screen, font, "Comandos", 10, 425, makecol(255,255,255));
	textout(screen, font, "<C> para comecar", 10, 434, makecol(255,255,255));
	textout(screen, font, "<S> para sair", 10, 443, makecol(255,255,255));
	textout(screen, font, "<I> para informacoes", 10, 452, makecol(255,255,255));
	textout(screen, font, "<A> para ajuda sobre o GP-2005", 10, 461, makecol(255,255,255));
}

/**************************************************\
| Função para abrir arquivos texto da info e ajuda |
\**************************************************/
void texto(char arq[])
{
    // variáveis normais e de controle
    FILE *fp;
    char caracter;
    int i=0,j=0;
    // abrindo o arquivo com o nome-parâmetro passado
	fp=fopen(arq,"r");
	// limpa o bitmap screen
	clear_bitmap(screen);
	// looping para transporte do texto
	while(caracter!=EOF)
	{
	    // recebendo um caracter do arquivo
	    caracter=fgetc(fp);
	    // se não for new line, retorno de carro ou eof, escreve-o
	    if(caracter!=10&&caracter!=13&&caracter!=EOF)
	    {
	        textprintf(screen,font,10+i,10+j,makecol(255,255,i+j),"%c",caracter);
	        // aumentando 8 pixels na posição horizontal de escrita
            i+=8;
        }
        // se for new line
        if(caracter==10)
        {
            // aumenta 10 pixels na posição vertical de escrita 
            j+=10;
            // zera o contador horizontal
            i=0;
        }
    }
    // fecha a variável de controle de arquivo
    fclose(fp);
    // espera que uma tecla seja pressionada
	readkey();
}

/*******************************************\
| Função para receber os dados do polinômio |
\*******************************************/
void dados()
{
    // limpa a tela e põem o título
    system("cls");
	printf("|---------|\n| GP-2005 |\n|---------|\n\n");
    // variáveis
	int j,i,grau;
	float coef[100];
	// recebe o nível de zoom e verifica sua validez
	do{
		printf("Nivel de Zoom(positivo): "); scanf("%f",&zoom);
		if(zoom<=0) printf("Erro de entrada!\n");
	}while(zoom<=0);
	// recebe o grau da equação e verifica sua validez
	do{
		printf("Grau da equacao(inteiro positivo): "); scanf("%d",&grau);
		if(grau<0) printf("Erro de entrada!\n");
	}while(grau<0);
	// recebe os coeficientes do polinômio
	printf("Coeficientes(real): \n");
	for(i=0;i<=grau;i++)
	{
		printf("De x elevado a %d   ",grau-i);
		scanf("%f",&coef[i+1]);
	}
	// calcula os f(x) em função do pixel mínimo até o máximo e põem na matriz global
	for(i=-239;i<=240;i++)
		for(j=1;j<=grau+1;j++)
			valy[i+240] += coef[j]*pow((double)(i-1)/10/zoom,(double)grau+1-j);
}

/**************************************\
| Função que desenha o gráfico na tela |
\**************************************/
void graficos()
{
    // cria um bitmap chamado tela
	BITMAP *tela = create_bitmap(SCREEN_W, SCREEN_H);
	// variáveis
	int i,x,y,xo,yo;
	// pinta o bitmap tela todo de branco
	rectfill(tela, 0, 0, 479, 479, makecol(255,255,255));
	// desenha os quadriculados que serão usados na escala
	for(i=239;i<=479;i+=10)
	{
		line(tela, i, 0, i, 479, makecol(220,220,220));
		line(tela, 478-i, 0, 478-i, 479, makecol(220,220,220));
		line(tela, 0, 478-i, 479, 478-i, makecol(220,220,220));
		line(tela, 0, i, 479, i, makecol(220,220,220));
	}
	// atualiza as variáveis para o centro do bitmap
	xo=239,yo=239;
	// cria retas de ponto em ponto, que fazem parte de f(x) e em função do bitmap
	for(i=239;i<479;i++)
	{
		(int)(y=239-(valy[i])*10*zoom);
		(int)(x=i-1);
		if((xo!=239)||(yo!=239))
		line(tela, xo, yo, x, y, makecol(255,0,0));
		// recebendo os pontos anteriores
		yo=y;
		xo=x;
	}
	// atualiza variáveis para o centro
	xo=239; yo=239;
	// também cria retas, mas agora do outro lado do eixo y
	for(i=239;i>0;i--)
	{
		(int)(y=239-(valy[i])*10*zoom);
		(int)(x=i-2);
		if((xo!=239)||(yo!=239))
		line(tela, xo, yo, x, y, makecol(255,0,0));
		// recebendo os pontos anteriores
		yo=y;
		xo=x;
	}
	// desenha os eixos cartesianos
	line(tela, 239, 0, 239, 479, makecol(0,0,0));
	line(tela, 0, 239, 479, 239, makecol(0,0,0));
	// desenha na parte de baixo da tela um retângulo preto
	rectfill(tela, 230, 472, 160, 479, makecol(0,0,0));
	// coloca em cima do retângulo as mensagens da escala e do botão a ser pressionado
	textprintf(tela, font, 230, 472, makecol(255,255,255), "escala do quadriculado = %.4f", 1/zoom);
	textout(tela, font, "pressione qualquer tecla", 0, 472, makecol(255,255,255));
	// desenha o bitmap tela no bitmap screen
	blit(tela, screen, 0, 0, 0, 0, tela->w, tela->h);
	// espera uma tecla
	readkey();
	// destroi o bitmap tela
	destroy_bitmap(tela);
	// zera a matriz para possível nova consulta
	for(i=1;i<=480;i++)
		valy[i]=0;
}

/*****************************************************\
| Função que dá o roteiro a ser seguido pelo programa |
\*****************************************************/
void programa()
{
    // sai do modo gráfico
	allegro_exit();
	// recebe os dados do polinômio
	dados();
	// entra no modo gráfico
	inicia_allegro();
	// desenha o gráfico do polinômio
	graficos();
}

/************************************************************\
| A main() administra as demais funções em função do usuário |
\************************************************************/
int main(int argc, char *argv[])
{
    // inicia o modo gráfico
	inicia_allegro();
	// variáveis
	char resp;
	// looping principal do programa
	for(;;)
	{
	    // chama a funçao da abertura
	    abertura();
	    // sincroniza o tempo dos elétrons do monitor
	    vsync();
	    // recebe a tecla de escolha
	    resp=readkey();
	    // chama determinada função em função da escolha
	    switch(resp)
	    {
	         case 'c': programa(); break;
	         case 's': exit(0); allegro_exit(); break;
	         case 'a': texto("ajuda.txt"); break;
	         case 'i': texto("info.txt"); break;
	         default : ;
        }
	}
	// retorna zero
	return 0;
}
// isso não sei pra que serve... só sei que precisa
END_OF_MAIN();
