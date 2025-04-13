#ifndef AIRPORT_LOADER_H 
#define AIRPORT_LOADER_H

using namespace std;

#include <vector>
#include <string>

struct Voo {
    int id, horario_prev, horario_real, duracao, penalidade, multa, pista_alocada, voo_anterior;
};

extern vector<Voo> voos;
extern vector<vector<int>> tempo_espera;
extern int n, m; //num de voos e num de pistas

bool lerDados(const string& nome_arquivo);
void calcularMultas();
int calcularCustoTotal();
void exibirDados();
void escreverSolucao(const string& nome_arquivo, int custo_total);

#endif
