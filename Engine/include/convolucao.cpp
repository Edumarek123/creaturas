//
// Created by Edumarek on 10/08/2021.
//
#include "convolucao_lib.h"

Bloco::Bloco(){
	linhas=0;
	colunas=0;
	profundidade=0;
	bloco=NULL;
}

Bloco::Bloco(int lin, int col, int prof){ 
	linhas=lin;
	colunas=col;
	profundidade=prof;

	bloco=(Bloco* )malloc((profundidade)*sizeof(Matriz*));
}

Bloco::Bloco(const Bloco &b){
	linhas=b.linhas;
	colunas=b.colunas;
	profundidade=b.profundidade;

	bloco=(Bloco* )malloc((profundidade)*sizeof(Matriz*));

	for(int i=0;i<profundidade;i++)
		bloco[i]=new Matriz(linhas, colunas);
}

Bloco::~Bloco(){
	for(int i=0;i<profundidade;i++)
		delete bloco[i];
	free(bloco);
}

void Bloco::conv(Bloco filtros, stride_l, stride_c){

}

void Bloco::maxpool(stride_l, stride_c){
	int max;

	assert(linhas%stride_l==0 && colunas%stride_c==0);
	Bloco resultado=new Bloco(linhas/stride_c, colunas/stride_c, profundidade);	

	for(int k=0;k<resultado.profundidade;k++)
		for(int i=0;i<resultado.linhas;i+=stride_l)
			for(int j=0;j<resultado.colunas;j+=stride_c){
				max=-100;
				for(int l=0;l<stride_l;l++)
					for(int c=0;c<stride_c;c++)
						if(bloco[k].matriz[l+i][c+j]>max)
							max=bloco[k].matriz[l+i][c+j];
				resultado.bloco[k].matriz[i][j]=max;
			}

	for(int k=0;k<profundidade;k++){
		delete bloco[k];
		bloco[k]=resultado.bloco[k];	

	}
	linhas=resultado.linhas;
	colunas=resultado.colunas;
}

Matriz Convolucao(Bloco entrada){
	Bloco* filtros=carrega_filtros();//?
	Bloco* paineis=new Bloco(entrada);

	//conv1
	//maxpool1 
	//conv2
	//maxpool2
	//conv3
	//maxpool3
	//convN
	//maxpoolN

	return paineis;
}


