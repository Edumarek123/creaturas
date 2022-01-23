//
// Created by Edumarek on 27/06/2021.
//

#ifndef REDE_NEURAL_LIB_H
#define REDE_NEURAL_LIB_H
#include "matriz_lib.h"
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
	int numero_layers;
	int tam_layer_x;
	int tam_layer_y;
	std::vector<int> tam_layers;

	std::vector<Matriz*> weights_layers;
	std::vector<Matriz*> bias_layers;
	std::vector<Matriz*> erros;
	std::vector<Matriz*> saidas;

	RedeNeural(int tam_Lx, int tam_Ly, std::vector<int> tams_hl, float alfa_learn);//ok
	RedeNeural()=delete;//ok
	~RedeNeural();//ok

	void imprimir_rede();//ok
	void salvar_rede();//ok
	void carregar_rede(const std::string path); //ok

	void FeedFoward(Matriz input_layer); //ok
	void mutacao();
};

void covolucao_saidas(Matriz* m);
int covolucao_saidasToDecimal(Matriz* m);
RedeNeural cruzamento(RedeNeural* a, RedeNeural* b);

#endif //REDE_NEURAL_LIB_H
