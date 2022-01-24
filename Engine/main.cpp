#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include"include/creatura.hpp"
#include<vector>
#include<cstdlib>


#define WIDTH 500
#define HEIGHT 500

SDL_Window* janela=nullptr; //cria uma variavel do tipo Window
SDL_Renderer* renderizador=nullptr; //cria uma variavel do tipo renderizador

bool simulando;
const int fps=60, steps_pgeracao=fps*3, numero_max_geracoes=5, tam_pop=200;
int steps=0, geracao=0;

void update();
void input();
void draw();

void cenario();

void gerar_nova_populacao();

std::vector<Creatura*> populacao, backup_populacao;

int main(int argc, char* argv[]){
	int fps_timer, delta_fps;

	if(SDL_Init(SDL_INIT_EVERYTHING)<0)
		std::cout<<"Erro encontrado:  "<<SDL_GetError()<<std::endl;
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &janela, &renderizador);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	SDL_SetWindowTitle(janela, "Simulacao: Objetivo - Permanecer na area verde");
	SDL_SetRenderDrawBlendMode(renderizador, SDL_BLENDMODE_BLEND);	
	
	std::vector<int> cor1={255, 0, 0};
	int j=0;
	for(int i=0; i<tam_pop; i++, j+=5){
		if(j==HEIGHT)
			j=0;
		populacao.emplace_back(new Creatura(WIDTH/2, j, rand()%15, cor1, 1, 10));
	}
	populacao.shrink_to_fit();
	
	simulando=true;
	std::cout<<"Geracao numero: "<<geracao+1<<std::endl;
	while(simulando && geracao<=numero_max_geracoes-1){
		fps_timer=SDL_GetTicks();
		if(steps%fps==0)
			std::cout<<"Tempo de Simulacao: "<<steps/fps<<".0 s"<<std::endl;

		update();
		input();
		draw();

		delta_fps=SDL_GetTicks()-fps_timer;
		if(1000/fps>delta_fps)
			SDL_Delay(1000/fps-delta_fps);

	}
	SDL_DestroyWindow(janela);
	SDL_DestroyRenderer(renderizador);

	SDL_Quit();

	populacao.clear();
	backup_populacao.clear();
	
	return 0;
}


void update(){
	if(steps==steps_pgeracao){
		geracao++;
		steps=0;
		if(geracao<=numero_max_geracoes){
			int sobreviventes=0;
			for(int i=0;i<(int)populacao.size();i++)
				if(populacao[i]->x>WIDTH/2)
					sobreviventes++;
			std::cout<<"Lado Esquerdo: "<<(int)populacao.size()-sobreviventes<<" | Lado Direito: "<<sobreviventes<<std::endl;
			if(geracao<=numero_max_geracoes-1){
				gerar_nova_populacao();
				std::cout<<std::endl<<"Geracao numero: "<<geracao+1<<" Integrantes: "<<(int)populacao.size()<<std::endl;
			}
		}
	}
	steps++;
}

void input(){
	SDL_Event evento;
		while(SDL_PollEvent(&evento)){
			if(evento.type == SDL_QUIT)
				simulando=false;
		}

	for(int i=0; i<(int)populacao.size(); i++)
		populacao[i]->movimentar();	
}

void draw(){
	//reset screen
	SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 255);
	SDL_RenderClear(renderizador);	
	//draw
	cenario();

	for(int i=0; i<(int)populacao.size(); i++)
		populacao[i]->desenhar_creatura(renderizador);	



	//comit draws
	SDL_RenderPresent(renderizador);
}

void cenario(){
	int alfa=25;
	SDL_Rect left, right;
	left.x=0;
	left.y=0;
	left.w=WIDTH/2;
	left.h=HEIGHT;
	
	right.x=WIDTH/2;
	right.y=0;
	right.w=WIDTH;
	right.h=HEIGHT;

	SDL_SetRenderDrawColor(renderizador, 255, 0, 0, alfa);
	SDL_RenderDrawRect(renderizador, &left);
	SDL_RenderFillRect(renderizador, &left);

	SDL_SetRenderDrawColor(renderizador, 0, 255, 0, alfa);
	SDL_RenderDrawRect(renderizador, &right);
	SDL_RenderFillRect(renderizador, &right);
	
	SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderizador, (WIDTH/2)-1, 0, WIDTH/2, HEIGHT);
}

void gerar_nova_populacao(){
	int max_score=-1,escolhido, restantes, j=0;

	for(int i=0;i<(int)populacao.size();i++){
		populacao[i]->cerebro->Calcular_Score(populacao[i]->x+populacao[i]->tamanho);
		if(populacao[i]->cerebro->score>max_score)
			max_score=populacao[i]->cerebro->score;
	}
	for(int i=0;i<(int)populacao.size();i++){
		if(populacao[i]->cerebro->score<max_score || populacao[i]->x==0){	
			populacao.erase(populacao.begin() + i);
			i--;
		}
	}
	backup_populacao=populacao;
	restantes=(int)populacao.size();

	int aleatorios=0;
	if(restantes!=0){
		populacao.clear();
		for(int i=0;i<tam_pop-aleatorios;i++,j+=5){
			if(j==HEIGHT)
				j=0;
			escolhido=rand()%restantes;
			populacao.push_back(backup_populacao[escolhido]);
			populacao[i]->cerebro->Mutacao();
			//reposicionando
			populacao[i]->x=WIDTH/2;
			populacao[i]->y=j;
		}
		if(aleatorios>0)
			for(int i=0;i<aleatorios;i++, j+=5){
				if(j==HEIGHT)
					j=0;
				populacao.emplace_back(new Creatura(WIDTH/2, j, rand()%15, {1, 1, 1}, 1, 10));
			}


		populacao.shrink_to_fit();
		backup_populacao.clear();
	}
	else{
		for(int i=0; i<tam_pop; i++, j++){
			if(j==HEIGHT)
				j=0;
			populacao.emplace_back(new Creatura(WIDTH/2, j, rand()%15, {1, 1, 1}, 1, 10));
		}
	}
}
