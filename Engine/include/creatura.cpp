//
// Created by Edumarek on 22/01/2022.
//
#include "creatura.h"


const float massa_minima=10; //1.0kg
const float velocidade_minima=25; //1.0ms
const float forca_minima=1; //1.0N

//------------------CONSTRUTORES&DESTRUTORES-------------------//

Creatura::Creatura(){
	massa=massa_minima;
	velocidade=velocidade_minima;
	forca=forca_minima;

	direcao.push_back(0);
	direcao.push_back(0);
	
	calcula_tamanho(massa);
	calcula_cor();

	x=tamanho+1;
	y=tamanho+1;
}

Creatura::Creatura(int X, int Y, float MASSA, std::vector<int> COR, float VELOCIDADE, float FORCA){
	massa=MASSA;
	velocidade=VELOCIDADE;
	forca=FORCA;

	direcao.push_back(0);
	direcao.push_back(0);
	
	calcula_tamanho(massa);
	//cor=COR;
	calcula_cor();

	x=X;
	y=X;
	
	std::vector<int> lh={2, 2};
	cerebro=new RedeNeural(2, 4, lh, 1);

	//imprimi_creatura();
}

Creatura::~Creatura(){
	delete cerebro;
}


//--------------------------METODOS---------------------------//

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
	x+=(direcao[0]*velocidade);
	y+=(direcao[1]*velocidade);

	Matriz* entradas=new Matriz(2, 1);

	entradas->matriz[0][0]=x;	
	entradas->matriz[1][0]=y;	
	

	cerebro->FeedFoward(*entradas);
	for(int i=0; i<=3; i++)
		if(cerebro->saidas[2]->matriz[i][0]>0.4)
			cerebro->saidas[2]->matriz[i][0]=1;

	if(cerebro->saidas[2]->matriz[0][0]==1){
		direcao[0]=-1;
		direcao[1]=0;
	}
	if(cerebro->saidas[2]->matriz[1][0]==1){
		direcao[0]=0;
		direcao[1]=1;
	}

	if(cerebro->saidas[2]->matriz[2][0]==1){
		direcao[0]=1;
		direcao[1]=0;
	}

	if(cerebro->saidas[2]->matriz[3][0]==1){
		direcao[0]=0;
		direcao[1]=-1;
	}


	if(x>=701)
		x=0;
	if(x<=-1)
		x=700;
	if(y>=701)
		y=0;
	if(y<=-1)
		y=700;
}
