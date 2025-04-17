#ifndef AIRPORT_LOADER_H 
#define AIRPORT_LOADER_H



#include <vector>
#include <string>

struct Voo {
    int id, horario_prev, horario_real, duracao, penalidade, multa, pista_alocada, voo_anterior;
};

extern std::vector<Voo> voos;
extern std::vector<vector<int>> pistas;
extern std::vector<vector<int>> tempo_espera;
extern int n, m; //num de voos e num de pistas

bool lerDados(const std::string& arquivo);
void calcularMultas();
int calcularCustoTotal();
void exibirDados();
void escreverSolucao(const std::string& arquivo, int custo_total);

#endif
