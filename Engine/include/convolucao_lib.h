//
// Created by Edumarek on 10/08/2021.
//

#ifndef CONVOLUCAO_LIB_H
#define CONVOLUCAO_LIB_H

#include "rede_neural_lib.h"
#include "matriz_lib.h"
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <assert.h>
#include <vector>

struct Bloco{
	int linhas;
	int colunas;
	int profundidade;
	std::vector<Matriz*> bloco;

	Bloco();
	~Bloco();
	Bloco(int lin, int col, int prof);
	Bloco(const Bloco &b);

	void imprimir_bloco();
	void inserir(float valor, bool aleatorio);

	void conv(Bloco* filtros, bool padding);
	void maxpool(int stride_l, int stride_c);

};

Matriz Convolucao(Bloco entrada);

#endif //CONVOLUCAO_LIB_H
