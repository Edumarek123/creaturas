# creaturas

Modifying my previous developed Neural Network I made this simulation in SDL2&C++, the main objectve was to test how the librarie would response on different kinds of implementations. In this case, the Neural Network librarie was modfied to a NEAT(NeuroEvolution of Augmenting Topologies) librarie, spoiler: after seeing the final tests I think it was a successfull!!


The Simulation:
Mission = Get Inside of the square with center in (100, 100)
Population Size = 600
Time/Generation = 5 s

Population(creaturas) caracteristics:
-Little squares with random sizes < limiter
-Ramdom colors
-Ramdom velocity
-Ramdom spawn position
-All genetic information is passed for the next generation
-Can moviment to all 8 directions(wind rose) or stop

Neural Network topology:
Inputs = 2
Outputs = 9
Hidden layers = 24 , 24
2 | 24 | 24 | 9

Modificando a minha antiga biblioteca de Redes Neurais desenvolvi esta simulação utilizando SDL2&C++, o principal objetivo foi estudar o comportamento desta biblioteca em diferentes tipos de implementacoes. Neste projeto a biblioteca de Redes Neurais foi adaptada para uma NEAT(NeuroEvolution of Augmenting Topologies), spoiler: fiquei muito satisfeito com os resultados obtidos.

----------------------------------------------------------------------------------------------------------------------------------------------------------

A Simulacao:
Missão = Ir/Permanecer dentro do quadrado com centro na posição (100, 100)
Tamanho da População = 600
Tempo / geração = 5 s

População(creaturas) características:
-Quadradinhos com tamanhos aleatórios < limitador
-Cores aleatórias
-Velocidade aleatória
-Toda informacao genética é passada para a próxima geração
-Podem se mover para 8 direções seguindo o princípio de uma rosa dos ventos

Topologia da Rede Neural:
Entradas = 2
Saidas = 9
Camadas Ocultas = 24 , 24
2 | 24 | 24 | 9

----------------------------------------------------------------------------------------------------------------------------------------------------------

Criador: Eduardo Marek
