//
// Created by Edumarek on 22/01/2022.
//
#include "creatura.hpp"


const float massa_minima=10; //1.0kg
const float velocidade_minima=25; //1.0ms
const float forca_minima=1; //1.0N

//------------------CONSTRUTORES&DESTRUTORES-------------------//
Creatura::Creatura(const Creatura &c){
	cerebro=new RedeNeural(*c.cerebro);
	bot_x=c.bot_x;
	bot_y=c.bot_y;
	top_x=c.top_x;
	top_y=c.top_y;
	massa=c.massa;
	calcula_tamanho(massa);
	cor=c.cor;
	direcao=c.direcao;
	velocidade=c.velocidade;
	forca=c.forca;
	gera_spawn();
}

Creatura::Creatura(const Creatura &a, const Creatura &b){
	cerebro=new RedeNeural(Cruzamento(*a.cerebro, *b.cerebro));

	if(rand()%2)
		massa=a.massa;
	else
		massa=b.massa;
	if(rand()%2)
		velocidade=a.velocidade;
	else
		velocidade=b.velocidade;
	if(rand()%2)
		forca=a.forca;
	else
		forca=b.forca;

	for(int i=0;i<3;i++){
		if(rand()%2)
			cor.push_back(a.cor[i]);
		else
			cor.push_back(b.cor[i]);
	}
	cor.shrink_to_fit();

	calcula_tamanho(massa);

	direcao=a.direcao;
	bot_x=a.bot_x;
	bot_y=a.bot_y;
	top_x=a.top_x;
	top_y=a.top_y;

	gera_spawn();
}

Creatura::Creatura(int top_X, int top_Y, int bot_X, int bot_Y, float MASSA, std::vector<int> COR, float VELOCIDADE, float FORCA){
	massa=MASSA;
	velocidade=VELOCIDADE;
	forca=FORCA;

	direcao.push_back(0);
	direcao.push_back(0);

	calcula_tamanho(massa);
	//cor=COR;
	calcula_cor();

	bot_x=bot_X;
	bot_y=bot_Y;
	top_x=top_X;
	top_y=top_Y;

	gera_spawn();

	cerebro=new RedeNeural(2, 9, {24, 24}, 0.08);
}

Creatura::~Creatura(){
	delete cerebro;
}


//--------------------------METODOS---------------------------//
void Creatura::gera_spawn(){
	x=rand()%bot_x;
	y=rand()%bot_y;
}

void Creatura::calcula_tamanho(float m){
	tamanho=massa+2;
}

void Creatura::calcula_cor(){
	cor.push_back(rand()%256);
	cor.push_back(rand()%256);
	cor.push_back(rand()%256);
	cor.shrink_to_fit();
}

void Creatura::desenhar_creatura(SDL_Renderer* renderizador){
	SDL_Rect r;
	r.x=x;
	r.y=y;
	r.w=tamanho;
	r.h=tamanho;

	SDL_SetRenderDrawColor(renderizador, cor[0], cor[1], cor[2], 255);
	SDL_RenderDrawRect(renderizador, &r);
	SDL_RenderFillRect(renderizador, &r);
}

void Creatura::imprimi_creatura(){
	std::cout<<"x = "<<x<<" | y = "<<y<<" | tamanho = "<<tamanho<<std::endl;
	std::cout<<"R = "<<cor[0]<<" | G = "<<cor[1]<<" | B = "<<cor[2]<<std::endl;
}

void Creatura::movimentar(){
	int delta_x, delta_y;

	Matriz* entradas=new Matriz(2, 1);

	entradas->matriz[0][0]=x-100+tamanho;
	entradas->matriz[1][0]=y-100+tamanho;

	cerebro->FeedFoward(*entradas);

	delete entradas;

	int escolha=covolucao_saidasToDecimal(cerebro->saidas[2]);
	switch (escolha){
		case 1:
			direcao[0]=1;
			direcao[1]=0;
			break;
		case 2:
			direcao[0]=1;
			direcao[1]=-1;
			break;
		case 4:
			direcao[0]=0;
			direcao[1]=-1;
			break;
		case 8:
			direcao[0]=-1;
			direcao[1]=-1;
			break;
		case 16:
			direcao[0]=-1;
			direcao[1]=0;
			break;
		case 32:
			direcao[0]=-1;
			direcao[1]=1;
			break;
		case 64:
			direcao[0]=0;
			direcao[1]=1;
			break;
		case 128:
			direcao[0]=1;
			direcao[1]=1;
			break;
		default:
			velocidade=0;
			break;
	}

	delta_x=x+(direcao[0]*velocidade);
	delta_y=y+(direcao[1]*velocidade);

	if(delta_x>bot_x-tamanho)
		x=bot_x-tamanho;
	else if(delta_x<top_x)
		x=top_x;
	else
		x=delta_x;

	if(delta_y>bot_y-tamanho)
		y=bot_y-tamanho;
	else if(delta_y<top_y)
		y=top_y;
	else
		y=delta_y;
}
