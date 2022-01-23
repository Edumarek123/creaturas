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
const int fps=60;

void update();
void input();
void draw();


std::vector<Creatura*> populacao;

int main(int argc, char* argv[]){
	int fps_timer, delta_fps;
	if(SDL_Init(SDL_INIT_EVERYTHING)<0){
		std::cout<<"Erro encontrado:  "<<SDL_GetError()<<std::endl;
	}else{
		std::cout<<"Video Iniciado"<<std::endl;
	}
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &janela, &renderizador);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	SDL_SetWindowTitle(janela, "Simulacao");
	
	std::vector<int> cor1={255, 0, 0};

	int tam_pop=1000;
	for(int i=0; i<tam_pop; i++){
		populacao.emplace_back(new Creatura(WIDTH/2, HEIGHT/2, rand()%15, cor1, rand()%15, 10));
	}

	populacao.shrink_to_fit();

	simulando=true;
	while(simulando){
		fps_timer=SDL_GetTicks();
		std::cout<<"FPS: "<<fps<<std::endl;
		
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

	for(int i=0; i<(int)populacao.size(); i++)
		delete populacao[i];
	
	return 0;
}


void update(){


}

void input(){
	SDL_Event evento;
		while(SDL_PollEvent(&evento)){
			if(evento.type == SDL_QUIT)
				simulando=false;
		}
}

void draw(){
	//reset screen
	SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 255);
	SDL_RenderClear(renderizador);	
	//draw
	for(int i=0; i<(int)populacao.size(); i++){
		populacao[i]->movimentar();	
		populacao[i]->desenhar_creatura(renderizador);	
	}



	//comit draws
	SDL_RenderPresent(renderizador);
}
