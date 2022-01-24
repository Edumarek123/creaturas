//
// Created by Edumarek on 27/06/2021.
//

#ifndef REDE_NEURAL_LIB_HPP
#define REDE_NEURAL_LIB_HPP
#include "matriz_lib.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <assert.h>
#include <chrono>

struct RedeNeural{
	float LEARNING_RATE;
	float score;
	int numero_layers;
	int tam_layer_x;
	int tam_layer_y;
	std::vector<int> tam_layers;

	std::vector<Matriz*> weights_layers;
	std::vector<Matriz*> bias_layers;
	std::vector<Matriz*> erros;
	std::vector<Matriz*> saidas;

	RedeNeural(int tam_Lx, int tam_Ly, std::vector<int> tams_hl, float alfa_learn);
	RedeNeural(const RedeNeural &r);
	RedeNeural()=delete;
	~RedeNeural();

	void imprimir_rede();
	void salvar_rede();
	void carregar_rede(const std::string path);

	void FeedFoward(Matriz input_layer);
	void Mutacao();
	void Calcular_Score(int i);
};

void covolucao_saidas(Matriz* m);
int covolucao_saidasToDecimal(Matriz* m);
RedeNeural* cruzamento(RedeNeural* a, RedeNeural* b);

#endif //REDE_NEURAL_LIB_HPP
