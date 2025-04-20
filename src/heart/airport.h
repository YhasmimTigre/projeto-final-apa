#ifndef AIRPORT_H 
#define AIRPORT_H

#include <vector>
#include <string>

struct Voo {
    int id;
    int horario_prev;
    int horario_real = 0;
    int duracao;
    int penalidade;
    int multa = 0;
    int pista_alocada = -1;
	int voo_anterior = -1;
};

class Airport {
	public:
    	std::vector<Voo> voos;
		std::vector<std::vector<int>> pistas;
   		std::vector<std::vector<int>> tempo_espera;
		std::vector<Voo> melhor_voos;
		std::vector<std::vector<int>> melhor_pistas;
		int num_voos, num_pistas; //n e m
		int custo_total = 0;
		int custo_melhor = 0;

		bool executarAlocacao(const std::string& arquivo_entrada);
		void mostrarSolucaoNoTerminal(int custo_total);

		//vizinhanças
		bool inverterVoosConsecutivos(int pista, int posicao_voo);
		bool insertIntraPista(int id_voo, int pista, int posicao_voo);
		bool opt2IntraPista(int p, int i, int j, int max_gap = 5);


    	bool lerDados(const std::string& arquivo);		
		int calcularCustoTotal();
		void calcularMultas();
		void escreverSolucao(const std::string& arquivo);

		
		void salvarMelhorSolucao();
		void restaurarMelhorSolucao();
};

#endif