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


struct Bloco{
	int linhas;
	int colunas;
	int profundidade;
	Matriz* bloco;

	Bloco();
	~Bloco();
	Bloco(int lin, int col, int prof); 
	Bloco(const Bloco &b);

	void conv(Bloco filtros, int stride_l, int stride_c);
	void maxpool(int stride_l, int stride_c);

};

Matriz Convolucao(Bloco entrada);

#endif //CONVOLUCAO_LIB_H

