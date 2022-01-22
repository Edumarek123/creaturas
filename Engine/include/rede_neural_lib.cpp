//
// Created by Edumarek on 27/06/2021.
//

#include "rede_neural_lib.h"

//------------------CONSTRUTORES&DESTRUTORES-------------------//

RedeNeural::RedeNeural(int tam_Lx, int tam_Ly, std::vector<int> tams_hl,float alfa_learn){
	tam_layer_x=tam_Lx;
	tam_layer_y=tam_Ly;
	numero_layers=((int)tams_hl.size())+2;
	LEARNING_RATE=alfa_learn;

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

void RedeNeural::Calcula_Erros(Matriz saidas_esperadas) {
	//e_L = (A_L - Y) 0 d6(z_L)
	//e_l = ((W_(l+1))T(erro_(l+1))) 0 d6(z_l)

	delete erros[numero_layers-2];
	erros[numero_layers-2]=new Matriz(subtrair(saidas_esperadas, *saidas[numero_layers-2]));
	erros[numero_layers-2]->multiplicar_Hadamard(map(*saidas[numero_layers-2], "Dsigmoide"));

	for (int i=numero_layers-3; i>=0; i--){
		delete erros[i];
		erros[i]=new Matriz(multiplicar(transposta(*weights_layers[i+1]), *erros[i+1]));
		erros[i]->multiplicar_Hadamard(map(*saidas[i], "Dsigmoide"));
	}
}

void RedeNeural::BackPropagation(Matriz entradas){
	//∇Cw_l = a_(l-1) * e_l
	//∇Cb_l = e_l
	for(int i=numero_layers-2;i>=1;i--){
		weights_layers[i]->somar(multiplicar_escalar((multiplicar(*erros[i], transposta(*saidas[i-1]))), LEARNING_RATE));
		bias_layers[i]->somar(multiplicar_escalar(*erros[i], LEARNING_RATE));
	}
	weights_layers[0]->somar(multiplicar_escalar((multiplicar(*erros[0], transposta(entradas))), LEARNING_RATE));
	bias_layers[0]->somar(multiplicar_escalar(*erros[0], LEARNING_RATE));
}


void RedeNeural::TreinarRede(const std::string entradas,const std::string saidas_esperadas, int porcentagem_testes, float threshold){
	int num_treinos, num_testes, num_dados, cont_treinos=0;
	float porcentagem_acerto, LEARNING_RATEo, decaimento=0.02;

	std::fstream arquivo_x(entradas, std::ios::in);
	std::fstream arquivo_y(saidas_esperadas, std::ios::in);
	assert(arquivo_x.is_open());
	assert(arquivo_y.is_open());

	num_dados=contagem_dados(entradas);
	assert(num_dados==contagem_dados(saidas_esperadas));

	std::string linha_x;
	std::string linha_y;

	num_testes=num_dados*porcentagem_testes/100;
	num_treinos=num_dados-num_testes;

	assert(num_testes+num_treinos==num_dados);

	std::cout<<std::endl<<std::endl<<"--Dados: "<<num_dados<<" Treinos: "<<num_treinos<<" Testes: "<<num_testes<<"--"<<std::endl<<std::endl;

	LEARNING_RATEo=LEARNING_RATE;
	do{
		LEARNING_RATE=LEARNING_RATEo*(1/(1+(decaimento*cont_treinos)));

		std::cout<<std::endl<<std::endl<<"|INICIANDO TREINAMENTO: "<<cont_treinos+1<<" |"<<std::endl;

		auto start_time=std::chrono::high_resolution_clock::now();
		for(int i=0;i<num_treinos;i++){
			getline(arquivo_x, linha_x);
			getline(arquivo_y, linha_y);

			auto dados_x=separa_dados(linha_x);
			auto dados_y=separa_dados(linha_y);

			linha_x= " ";
			linha_y= " ";


			FeedFoward(*dados_x);
			Calcula_Erros(*dados_y);
			BackPropagation(*dados_x);

			delete dados_x;
			delete dados_y;
		}
		auto stop_time=std::chrono::high_resolution_clock::now();
		auto duration=std::chrono::duration_cast<std::chrono::seconds>(stop_time-start_time);
		std::cout<<"Tempo de Execucao: "<<duration.count()<<" s"<<std::endl;

		std::cout<<std::endl<<std::endl<<"|INICIANDO TESTES: "<<cont_treinos+1<<" |"<<std::endl;

		assert(arquivo_x.is_open());
		assert(arquivo_y.is_open());

		porcentagem_acerto=0;
		for(int i=num_treinos;i<num_treinos+num_testes;i++){
			getline(arquivo_x, linha_x);
			getline(arquivo_y, linha_y);

			auto dados_x=separa_dados(linha_x);
			auto dados_y=separa_dados(linha_y);

			linha_x= " ";
			linha_y= " ";

			FeedFoward(*dados_x);
			covolucao_saidas(saidas[numero_layers-2]);

			if(vericar_igualdade(*saidas[numero_layers-2], *dados_y))
				porcentagem_acerto++;

			delete dados_x;
			delete dados_y;
		}
		porcentagem_acerto=(porcentagem_acerto/num_testes)*100;

		std::cout<<"PORCENTAGEM DE ACERTO DOS TESTES: "<<porcentagem_acerto<<"%"<<std::endl<<std::endl;

		salvar_rede();

		arquivo_x.clear();
		arquivo_x.seekg(0, std::ios::beg);
		arquivo_y.clear();
		arquivo_y.seekg(0, std::ios::beg);

		cont_treinos++;
	}while(porcentagem_acerto<threshold);
	arquivo_x.close();
	arquivo_y.close();
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

//----------------------FUNCOES AUXILIARES---------------------//

int contagem_dados(std::string path_arquivo){
	int cont=0;
	std::fstream arquivo(path_arquivo, std::ios::in);
	std::string linha;

	assert(arquivo.is_open());

	do{
		getline(arquivo,linha);
		cont++;
	}while(arquivo);

	return cont-1;
}

Matriz* separa_dados(std::string linha) {
	std::istringstream iss(linha);
	std::string valor;
	std::vector<float> dados;
	int i = 0;

	while (std::getline(iss, valor, ' ')) {
		dados.push_back(std::stof(valor));
		i++;
	}
	dados.shrink_to_fit();
	Matriz* coleta=new Matriz(i, 1);

	for(int j=0;j<i;j++)
		coleta->matriz[j][0]=dados[j];

	return coleta;
}
