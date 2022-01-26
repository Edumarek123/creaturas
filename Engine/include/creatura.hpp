//
// Created by Edumarek on 21/01/2022.
//

#ifndef CREATURA_HPP
#define CREATURA_HPP
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstdlib>
#include "rede_neural_lib.hpp"
#include "matriz_lib.hpp"


struct Creatura{
    //cerebro
    RedeNeural* cerebro;
    //caracteristicas
    int x;	//centro da creatura
    int y;	//centro da creatura
    int top_x;
    int top_y;
    int bot_x;
    int bot_y;
    float massa;
    int tamanho;
    std::vector<int> cor;
    //atributos
    std::vector<int> direcao;
    float velocidade;
    float forca;

    Creatura()=delete;
    Creatura(const Creatura &c);
    Creatura(const Creatura &a, const Creatura &b);
    Creatura(int top_X, int top_Y, int bot_X, int bot_Y, float MASSA, std::vector<int> COR, float VELOCIDADE, float FORCA);
    ~Creatura();

    void calcula_tamanho(float m);
    void calcula_cor();
    void gera_spawn();
    void desenhar_creatura(SDL_Renderer* renderizador);
    void imprimi_creatura();
    void movimentar();
};

#endif //Creatura_HPP
