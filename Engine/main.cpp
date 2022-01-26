#include<iostream>
#include<cstdlib>
#include<vector>
#include<string>
#include<iostream>
#include"include/creatura.hpp"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>


#define WIDTH 1360
#define HEIGHT 768
#define BORDA 6

#define gera_aleatorio populacao.emplace_back(new Creatura(BORDA, BORDA, WIDTH-BORDA, HEIGHT-BORDA, rand()%11, {1, 1, 1}, max_speed, 10));

SDL_Window* janela=nullptr; //cria uma variavel do tipo Window
SDL_Renderer* renderizador=nullptr; //cria uma variavel do tipo renderizador

bool simulando=true, display_time=false;
const int fps=60, steps_pgeracao=fps*5, numero_max_geracoes=300, tam_pop=600, max_speed=6;
int steps=0, geracao=0;

void inicializacao_SDL();
void encerramento_SDL();

void update();
void input();
void draw();

void cenario();

void gerar_nova_populacao();
int pool_selection(int max_score);

std::vector<Creatura*> populacao;
std::vector<Creatura*> backup_populacao;

int main(int argc, char* argv[]){
	int fps_timer, delta_fps;

	inicializacao_SDL();

	for(int i=0; i<tam_pop; i++)
		gera_aleatorio;
	populacao.shrink_to_fit();

	std::cout<<"Geracao numero: "<<geracao+1<<std::endl;
	while(simulando && geracao<=numero_max_geracoes-1){
		fps_timer=SDL_GetTicks();
		if(steps%fps==0 && display_time)
			std::cout<<"Tempo de Simulacao: "<<steps/fps<<".0 s"<<std::endl;

		update();
		input();
		draw();

		delta_fps=SDL_GetTicks()-fps_timer;
		if(1000/fps>delta_fps)
			SDL_Delay(1000/fps-delta_fps);
	}
	encerramento_SDL();

	for(int i=0;i<(int)populacao.size();i++)
		delete populacao[i];
	for(int i=0;i<(int)backup_populacao.size();i++)
		delete backup_populacao[i];

	return 0;
}

void inicializacao_SDL(){
	if(SDL_Init(SDL_INIT_EVERYTHING)<0)
		std::cout<<"Erro encontrado:  "<<SDL_GetError()<<std::endl;
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &janela, &renderizador);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	SDL_SetWindowBordered(janela, SDL_TRUE);
	SDL_SetRenderDrawBlendMode(renderizador, SDL_BLENDMODE_BLEND);
}

void encerramento_SDL(){
	SDL_DestroyWindow(janela);
	SDL_DestroyRenderer(renderizador);
	SDL_Quit();
}

void update(){
	if(steps==steps_pgeracao){
		geracao++;
		steps=0;
		if(geracao<=numero_max_geracoes){
			if(geracao<=numero_max_geracoes-1){
				gerar_nova_populacao();
				std::cout<<std::endl<<(int)populacao.size()<<")Geracao numero: "<<geracao+1<<std::endl;
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

	for(int i=0; i<(int)populacao.size(); i++){
		populacao[i]->movimentar();
	}
}

void draw(){
	//reset screen
	SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 255);
	SDL_RenderClear(renderizador);
	//draw
	cenario();

	for(int i=0; i<(int)populacao.size(); i++)
		populacao[i]->desenhar_creatura(renderizador);


	std::string t="Geracao numero: " + std::to_string(geracao) + " / " +std::to_string(numero_max_geracoes);
	const char* title= t.c_str();
	SDL_SetWindowTitle(janela, title);

	//comit draws
	SDL_RenderPresent(renderizador);
}

void cenario(){
	int alfa=35;
	SDL_Rect left, bordaE, bordaI;

	left.x=50;
	left.y=50;
	left.w=100;
	left.h=100;

	bordaE.x=0;
	bordaE.y=0;
	bordaE.w=WIDTH;
	bordaE.h=HEIGHT;

	bordaI.x=BORDA;
	bordaI.y=BORDA;
	bordaI.w=bordaE.w-(2*BORDA);
	bordaI.h=bordaE.h-(2*BORDA);

	SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderizador, &bordaE);
	SDL_RenderFillRect(renderizador, &bordaE);

	SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderizador, &bordaI);
	SDL_RenderFillRect(renderizador, &bordaI);


	SDL_SetRenderDrawColor(renderizador, 255, 255, 255, alfa);
	SDL_RenderDrawRect(renderizador, &left);
	SDL_RenderFillRect(renderizador, &left);
}

void gerar_nova_populacao(){
	int max_score=-1;

	for(int i=0;i<(int)populacao.size();i++){
		populacao[i]->cerebro->Calcular_Score(populacao[i]->x+populacao[i]->tamanho, populacao[i]->y+populacao[i]->tamanho, WIDTH-100, HEIGHT-100);
		if(populacao[i]->cerebro->score>max_score)
			max_score=populacao[i]->cerebro->score;
	}

	for(int i=0;i<tam_pop;i++){
		if(populacao[i]->cerebro->score>max_score-(max_score*0.01))
			backup_populacao.emplace_back(new Creatura(*populacao[i]));
	}
	backup_populacao.shrink_to_fit();

	for(int i=0;i<(int)populacao.size();i++)
		delete populacao[i];
	populacao.clear();

	int restantes=(int)backup_populacao.size();
	for(int i=0;i<tam_pop;i++){
		populacao.emplace_back(new Creatura(*backup_populacao[(int)(rand()%restantes)]));
		populacao[i]->cerebro->Mutacao();
	}
	populacao.shrink_to_fit();

	for(int i=0;i<(int)backup_populacao.size();i++)
		delete backup_populacao[i];
	backup_populacao.clear();
}

int pool_selection(int max_score){
	int indice;
	while(true){
		indice=rand()%((int)backup_populacao.size());
		if(rand()%max_score<backup_populacao[indice]->cerebro->score)
			return indice;
	}
}

/*
int a=pool_selection(max_score);
int b=pool_selection(max_score);
backup_populacao.emplace_back(new Creatura(*populacao[a], *populacao[b]));
}
backup_populacao.shrink_to_fit();

for(int i=0;i<(int)populacao.size();i++)
delete populacao[i];
populacao.clear();

for(int i=0;i<(int)backup_populacao.size();i++){
populacao.emplace_back(new Creatura(*backup_populacao[i]));
populacao[i]->cerebro->Mutacao();
}
*/
