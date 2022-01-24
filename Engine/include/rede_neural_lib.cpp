//
// Created by Edumarek on 27/06/2021.
//

#include "rede_neural_lib.hpp"

//------------------CONSTRUTORES&DESTRUTORES-------------------//

RedeNeural::RedeNeural(int tam_Lx, int tam_Ly, std::vector<int> tams_hl,float alfa_learn){
	tam_layer_x=tam_Lx;
	tam_layer_y=tam_Ly;
	numero_layers=((int)tams_hl.size())+2;
	LEARNING_RATE=alfa_learn;
	score=0;

	tam_layers.insert(tam_layers.begin(), tam_layer_x);
	for(int i=1;i<numero_layers-1;i++)
		tam_layers.insert(tam_layers.begin()+i, tams_hl[i-1]);
	tam_layers.push_back(tam_layer_y);
	tam_layers.shrink_to_fit();

	for(int i=0;i<(numero_layers-1);i++)
		bias_layers.emplace_back(new Matriz(tam_layers[i+1],1));
	bias_layers.shrink_to_fit();

	for(int i=0;i<numero_layers-1;i++)
		weights_layers.emplace_back(new Matriz(tam_layers[i+1],tam_layers[i]));

	weights_layers.shrink_to_fit();

	for(int i=0;i<numero_layers-1;i++)
		erros.emplace_back(new Matriz(tam_layers[i+1], 1));
	erros.shrink_to_fit();

	for(int i=0;i<numero_layers-1;i++)
		saidas.emplace_back(new Matriz(tam_layers[i+1], 1));
	saidas.shrink_to_fit();
}

RedeNeural::RedeNeural(const RedeNeural &r){
	tam_layer_x=r.tam_layer_x;
	tam_layer_y=r.tam_layer_y;
	numero_layers=r.numero_layers;
	LEARNING_RATE=r.LEARNING_RATE;
	score=0;

	tam_layers=r.tam_layers;

	bias_layers=r.bias_layers;
	bias_layers.shrink_to_fit();

	weights_layers=r.weights_layers;
	weights_layers.shrink_to_fit();

	erros=r.erros;
	erros.shrink_to_fit();

	saidas=r.saidas;
	saidas.shrink_to_fit();
}

RedeNeural::~RedeNeural(){
	for(int i=0;i<numero_layers-1;i++){
		delete weights_layers[i];
		delete bias_layers[i];
		delete erros[i];
		delete saidas[i];
	}
}

//---------------------------METODOS---------------------------//

void RedeNeural::imprimir_rede() {
	std::cout << std::endl << "Neural Network Structure( I | Wn | O )" << std::endl << std::endl;

	std::cout<<"Tamanho layers"<<std::endl<<std::endl;
	std::cout<<"|  X  | ";
	for(int i=0;i<numero_layers-2;i++)
		std::cout<<"HL"<<i<<" | ";
	std::cout<<" Y  |"<<std::endl<<"| ";
	for(int i=0;i<numero_layers;i++)
		std::cout<<" "<<tam_layers[i]<<"  | ";
	std::cout<<std::endl<<std::endl;

	std::cout<<"Vizualizacao | Entradas | Pesos | Saidas |"<<std::endl<<std::endl;
	for (int i = 0; i < tam_layer_x; ++i)
		std::cout<<"X"<<std::endl;
	std::cout << std::endl;

	for (int i = 0; i < numero_layers-1; ++i) {
		weights_layers[i]->imprimir();
		std::cout << std::endl;
	}

	for (int i = 0; i < tam_layer_y; ++i)
		std::cout<<"Y"<<std::endl;
	std::cout << std::endl;

	std::cout << std::endl<< "Bias" << std::endl << std::endl;
	for (int i = 0; i < (numero_layers-1); ++i) {
		bias_layers[i]->imprimir();
		std::cout << std::endl;
	}
}

void RedeNeural::salvar_rede(){
	std::ofstream arquivo;
	arquivo.open("rede_neural.txt");

	if(arquivo.is_open()) {
		for (int i = 0; i < numero_layers - 1; i++)
			for (int j = 0; j < weights_layers[i]->linhas; j++)
				for (int k = 0; k < weights_layers[i]->colunas; k++)
					arquivo << weights_layers[i]->matriz[j][k] << " ";

		for (int i = 0; i < numero_layers - 1; i++)
			for (int j = 0; j < bias_layers[i]->linhas; j++)
				for (int k = 0; k < bias_layers[i]->colunas; k++)
					arquivo << bias_layers[i]->matriz[j][k] << " ";
	}else{
		assert(arquivo.is_open());
	}

	arquivo.close();
}

void RedeNeural::carregar_rede(const std::string path){
	std::fstream arquivo;
	arquivo.open(path, std::ios::in);
	std::string valor, linha;

	if(arquivo.is_open()) {
		getline(arquivo, linha);
		std::istringstream iss(linha);

		for (int i = 0; i < numero_layers - 1; i++)
			for (int j = 0; j < weights_layers[i]->linhas; j++)
				for (int k = 0; k < weights_layers[i]->colunas; k++){
					std::getline(iss, valor, ' ');
					weights_layers[i]->matriz[j][k]=std::stof(valor);
				}

		for (int i = 0; i < numero_layers - 1; i++)
			for (int j = 0; j < bias_layers[i]->linhas; j++)
				for (int k = 0; k < bias_layers[i]->colunas; k++){
					std::getline(iss, valor, ' ');
					bias_layers[i]->matriz[j][k]=std::stof(valor);
				}
	}else{
		assert(arquivo.is_open());
	}

	arquivo.close();
}

void RedeNeural::FeedFoward(Matriz input_layer){
	//z_l=W_l.X+B
	//z_l=W_l.a_(l-1)+B
	//a_l=6z_l

	for (int i = 0; i < (numero_layers-1); i++)
		delete saidas[i];

	saidas[0]=new Matriz(multiplicar(*weights_layers[0], input_layer));
	saidas[0]->somar(*bias_layers[0]);
	saidas[0]->map("sigmoide");

	for (int i = 1; i < (numero_layers-1); i++) {
		saidas[i]=new Matriz(multiplicar(*weights_layers[i], *saidas[i-1]));
		saidas[i]->somar(*bias_layers[i]);
		saidas[i]->map("sigmoide");
	}
	saidas.shrink_to_fit();
}

void RedeNeural::Mutacao(){
	float coeficiente_mutacao=0.0001;
	
	for (int i = 0; i < numero_layers - 1; i++)
		for (int j = 0; j < weights_layers[i]->linhas; j++)		
			for (int k = 0; k < weights_layers[i]->colunas; k++)
				if((rand()%100000)/100000<=coeficiente_mutacao)
					weights_layers[i]->matriz[j][k]=((rand()%2001)-1000)/10;

	for (int i = 0; i < numero_layers - 1; i++)
		for (int j = 0; j < bias_layers[i]->linhas; j++)
			for (int k = 0; k < bias_layers[i]->colunas; k++)
				if((rand()%1000)/1000<=coeficiente_mutacao)
					bias_layers[i]->matriz[j][k]=((rand()%2001)-1000)/10;
}

void RedeNeural::Calcular_Score(int pos_x){
	score=pos_x/5; //Score de 0 -> 100 baseado no quao perto da borda direita estou
	if(score<=0.1)
		score=0;
}

//---------------------------FUNCOES---------------------------//

void covolucao_saidas(Matriz* m){ //editar
	float max_value=-1, posicao=0;

	for(int i=0;i<m->linhas;i++)
		if(m->matriz[i][0]>max_value){
			max_value=m->matriz[i][0];
			posicao=i;
		}

	for(int i=0;i<m->linhas;i++) {
		if (i == posicao)
			m->matriz[i][0] = 1;
		else
			m->matriz[i][0] = 0;
	}
}

int covolucao_saidasToDecimal(Matriz* m){
	covolucao_saidas(m);
	int cont=0, i=0, resultado;
	while(m->matriz[i][0]!=1){
		cont++;
		i++;
	}
	resultado=pow(2, cont);

	return	resultado;
}

RedeNeural* cruzamento(RedeNeural* a, RedeNeural* b){

	return b;
}

//----------------------FUNCOES AUXILIARES---------------------//
