//
// Created by kamil on 05/04/2025.
//

#ifndef DADOS_H
#define DADOS_H

using namespace std;

#include <vector>
#include <string>

struct Voo {
    int id_voo, horario, duracao, penalidade;
};

extern vector<Voo> voos;
extern vector<vector<int>> tempo_espera;

bool lerDados(const string& nome_arquivo);
void exibirDados();

#endif
