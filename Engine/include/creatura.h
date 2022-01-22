//
// Created by Edumarek on 21/01/2022.
//

#ifndef Creatura_H
#define Creatura_H
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstdlib>
#include "rede_neural_lib.h"
#include "matriz_lib.h"


struct Creatura{
    //cerebro
    RedeNeural* cerebro;
    //caracteristicas
    int x;	//centro da creatura
    int y;	//centro da creatura
    float massa;
    int tamanho;
    std::vector<int> cor;
    //atributos
    std::vector<int> direcao;
    float velocidade;
    float forca;
	
    Creatura();
    //Creatura(const Creatura &c);
    Creatura(int X, int Y, float MASSA, std::vector<int> COR, float VELOCIDADE, float FORCA);
    ~Creatura();

    void calcula_tamanho(float m);
    void calcula_cor();
    void desenhar_creatura(SDL_Renderer* renderizador);
    void imprimi_creatura();
    void movimentar();
};

#endif //Creatura_H
