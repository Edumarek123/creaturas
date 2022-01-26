//
// Created by Edumarek on 25/01/2022.
//

#include"gpx_lib.hpp"

#define WIDTH 1363
#define HEIGHT 763

void inicializacao_SDL(SDL_Window* janela, SDL_Renderer* renderizador){
	if(SDL_Init(SDL_INIT_EVERYTHING)<0)
		std::cout<<"Erro encontrado:  "<<SDL_GetError()<<std::endl;
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN, &janela, &renderizador);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	SDL_SetWindowBordered(janela, SDL_TRUE);
	SDL_SetRenderDrawBlendMode(renderizador, SDL_BLENDMODE_BLEND);
}

void encerramento_SDL(SDL_Window* janela, SDL_Renderer* renderizador){
	SDL_DestroyWindow(janela);
	SDL_DestroyRenderer(renderizador);
	SDL_Quit();
}
