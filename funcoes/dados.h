#ifndef DADOS_H
#define DADOS_H

using namespace std;

#include <vector>
#include <string>

struct Voo {
    int id, horario, duracao, penalidade;
};

extern vector<Voo> voos;
extern vector<vector<int>> tempo_espera;
extern int m;
extern int n;

void alocarVoos(vector<Voo>& voos, int m);
bool lerDados(const string& nome_arquivo);
void exibirDados();


#endif
