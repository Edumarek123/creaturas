//
// Created by Edumarek on 10/08/2021.
//
#include "convolucao_lib.h"

Bloco::Bloco(){
	linhas=0;
	colunas=0;
	profundidade=0;
}

Bloco::Bloco(int lin, int col, int prof){
	linhas=lin;
	colunas=col;
	profundidade=prof;

	for(int i=0;i<profundidade;i++)
		bloco.emplace_back(new Matriz(linhas, colunas));
	bloco.shrink_to_fit();
}

Bloco::Bloco(const Bloco &b){
	linhas=b.linhas;
	colunas=b.colunas;
	profundidade=b.profundidade;

	bloco.clear();
	for(int i=0;i<profundidade;i++)
		bloco.emplace_back(b.bloco[i]);
}

Bloco::~Bloco(){
	for(int i=0;i<profundidade;i++)
		delete bloco[i];
}

void Bloco::imprimir_bloco(){
	for(int i=0;i<profundidade;i++){
		bloco[i]->imprimir();
		std::cout<<std::endl;
	}
}

void Bloco::inserir(float valor, bool aleatorio){
	for(int i=0;i<profundidade;i++)
		bloco[i]->carrega_valores(valor, aleatorio);
}

void Bloco::conv(Bloco* filtros, bool padding){
	int stride_l, stride_c, r_lin, r_col;
	float produto_escalar;

	stride_l=filtros->linhas-1;
	stride_c=filtros->colunas-1;

	if(padding){
		assert((((linhas+1)%stride_l)==0 && ((colunas+1)%stride_c)==0) || (filtros->linhas==linhas+2 && filtros->colunas==colunas+2));
		
		Bloco* temp=new Bloco(linhas+2, colunas+2, profundidade);

		for(int k=0;k<temp->profundidade;k++)
			for(int i=0;i<temp->linhas;i++)
				for(int j=0;j<temp->colunas;j++){
					if(i==0 || j==0 || i==(temp->linhas-1) || j==(temp->colunas-1))
						temp->bloco[k]->matriz[i][j]=0;
					else
						temp->bloco[k]->matriz[i][j]=bloco[k]->matriz[i-1][j-1];
				}

		bloco.clear();
		bloco.swap(temp->bloco);

		linhas=temp->linhas;
		colunas=temp->colunas;

		delete temp;
	}
	else{
		assert((((linhas-1)%stride_l)==0 && ((colunas-1)%stride_c)==0) || (filtros->linhas==linhas && filtros->colunas==colunas));
	}

	r_lin=(linhas-1)/stride_l;
	r_col=(colunas-1)/stride_c;

	Bloco* resultado=new Bloco(r_lin, r_col, profundidade);

	for(int k=0;k<profundidade;k++)
		for(int i=0, y=0;i<linhas-1;i+=stride_l, y++)
			for(int j=0, x=0;j<colunas-1;j+=stride_c, x++){
				produto_escalar=0;
				for(int l=0;l<stride_l+1;l++)
					for(int c=0;c<stride_c+1;c++)
						produto_escalar+=(bloco[k]->matriz[l+i][c+j]*filtros->bloco[k]->matriz[l][c]);
				resultado->bloco[k]->matriz[y][x]=produto_escalar;
			}

	bloco.clear();
	bloco.swap(resultado->bloco);

	linhas=resultado->linhas;
	colunas=resultado->colunas;

	delete resultado;
}

void Bloco::maxpool(int stride_l,int stride_c){
	float max;

	assert(linhas%stride_l==0 && colunas%stride_c==0);
	Bloco* resultado=new Bloco(linhas/stride_c, colunas/stride_c, profundidade);

	for(int k=0;k<resultado->profundidade;k++)
		for(int i=0, y=0;i<linhas;i+=stride_l,y++)
			for(int j=0, x=0;j<colunas;j+=stride_c,x++){
				max=-100;
				for(int l=0;l<stride_l;l++)
					for(int c=0;c<stride_c;c++)
						if(bloco[k]->matriz[i+l][j+c]>max)
							max=bloco[k]->matriz[l+i][c+j];
				resultado->bloco[k]->matriz[y][x]=max;
			}
	bloco.clear();
	bloco.swap(resultado->bloco);

	linhas=resultado->linhas;
	colunas=resultado->colunas;

	delete resultado;
}

Matriz Convolucao(Bloco entrada){
	//Bloco* filtros=carrega_filtros();//?
	Bloco* paineis=new Bloco(entrada);

	//conv1
	//maxpool1
	//conv2
	//maxpool2
	//conv3
	//maxpool3
	//convN
	//maxpoolN
	std::cout<<std::endl;
	return *paineis->bloco[0];
}
