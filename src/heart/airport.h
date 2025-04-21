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
		Airport() : num_voos(0), num_pistas(0), custo_total(0), bkp_custo(0) {}

    	std::vector<Voo> voos;
		std::vector<std::vector<int>> pistas;
   		std::vector<std::vector<int>> tempo_espera;
		std::vector<Voo> bkp_voos;
		std::vector<std::vector<int>> bkp_pistas;
		int num_voos, num_pistas; //n e m
		int custo_total = 0;
		int bkp_custo = 0;

		bool executarAlocacao(const std::string& arquivo_entrada);
		Voo* encontrarVooPorId(int id);

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
		int calcularBKPCustoTotal();
		void calcularBKPMultas();

		//copias para ils
		Airport(const Airport& other) {

			voos = other.voos;
			pistas = other.pistas;
			tempo_espera = other.tempo_espera;
			num_voos = other.num_voos;
			num_pistas = other.num_pistas;
			custo_total = other.custo_total;
			bkp_custo = other.bkp_custo;
			bkp_voos = other.bkp_voos;
			bkp_pistas = other.bkp_pistas;

		}	
		//operador de atribuição
		Airport& operator=(const Airport& other) {
			if (this != &other) {

				voos = other.voos;
				pistas = other.pistas;
				tempo_espera = other.tempo_espera;
				num_voos = other.num_voos;
				num_pistas = other.num_pistas;
				custo_total = other.custo_total;
				bkp_custo = other.bkp_custo;
				bkp_voos = other.bkp_voos;
				bkp_pistas = other.bkp_pistas;
			
			}

			return *this;
		}

};

#endif