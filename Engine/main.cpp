#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include"include/creatura.h"
#include<vector>
#include<cstdlib>


int main(int argc, char* argv[]){
	SDL_Window* janela=nullptr; //cria uma variavel do tipo Window
	SDL_Renderer* renderizador=nullptr; //cria uma variavel do tipo renderizador

	if(SDL_Init(SDL_INIT_EVERYTHING)<0){
		std::cout<<"Erro encontrado:  "<<SDL_GetError()<<std::endl;
	}else{
		std::cout<<"Video Iniciado"<<std::endl;
	}
	SDL_CreateWindowAndRenderer(700, 700, SDL_WINDOW_SHOWN, &janela, &renderizador);
	SDL_SetWindowTitle(janela, "Simulacao");

	std::vector<int> cor1={255, 0, 0};

	int tam_pop=200;
	std::vector<Creatura*> populacao;
	for(int i=0; i<tam_pop; i++){
		populacao.emplace_back(new Creatura(250, 250, rand()%15, cor1, rand()%20, 10));
	}
	populacao.shrink_to_fit();

	std::cout<<"Populacao: "<<populacao.size()<<std::endl;

	bool simulando=true;
	while(simulando){
		SDL_Event evento;
		while(SDL_PollEvent(&evento)){
			if(evento.type == SDL_QUIT)
				simulando=false;
		}
		SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 255);
		SDL_RenderClear(renderizador);
		
		for(int i=0; i<=20; i++){
			populacao[i]->movimentar();	
			populacao[i]->desenhar_creatura(renderizador);	
		}

		SDL_RenderPresent(renderizador);
		
		SDL_Delay(20);
	}
	SDL_DestroyWindow(janela);
	SDL_DestroyRenderer(renderizador);

	SDL_Quit();

	for(int i=0; i<=20; i++)
		delete populacao[i];
	
	return 0;
}
