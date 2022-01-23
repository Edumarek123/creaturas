//
// Created by Edumarek on 22/01/2022.
//
#include "creatura.hpp"


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
	cerebro=new RedeNeural(2, 8, lh, 1);

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
	int delta_x, delta_y;

	Matriz* entradas=new Matriz(2, 1);

	entradas->matriz[0][0]=x;	
	entradas->matriz[1][0]=y;	

	cerebro->FeedFoward(*entradas);

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
	}

	delta_x=x+(direcao[0]*velocidade);
	delta_y=y+(direcao[1]*velocidade);

	//std::cout<<"tamanho: "<<tamanho<<" | x = "<<x<<" | delta_x = "<<delta_x<<" | y = "<<y<<" | delta_y = "<<delta_y<<std::endl;

	if(delta_x>500-tamanho)
		x=500-tamanho;
	else if(delta_x<0)
		x=0;
	else
		x=delta_x;

	if(delta_y>500-tamanho)
		y=500-tamanho;
	else if(delta_y<0)
		y=0;
	else
		y=delta_y;
}
