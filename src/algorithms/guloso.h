//Para receber as funções a serem usadas no VND

#ifndef GULOSO_H
#define GULOSO_H

#include <vector>
#include "../core/airport_loader.h"

void Guloso(vector<Voo>& voos, int m, const vector<vector<int>>& tempo_espera);

/**
 int calcularPenalidade(pistas, t);

struct Pista {
    int id;
    int tempo_liberacao;
    vector<Voos> voos_alocados;
};

struct Problema {
    vector<Pista> pistas;
    int solucao;
}
**/

#endif