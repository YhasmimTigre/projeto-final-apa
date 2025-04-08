#ifndef AIRPORT_LOADER_H
#define AIRPORT_LOADER_H

using namespace std;

#include <vector>
#include <string>

struct Voo {
    int id, horario_prev, horario_real, duracao, penalidade;
};

extern vector<Voo> voos;
extern vector<vector<int>> tempo_espera;
extern int m;
extern int n;

void alocarVoos(vector<Voo>& voos, int m);
bool lerDados(const string& nome_arquivo);
void exibirDados();


#endif
