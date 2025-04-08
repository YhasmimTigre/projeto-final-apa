#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include "funcoes/dados.h"
#include "guloso.h"

using namespace std; // tem q declarar em todo arquivo? não basta estar em dados.h?

using Solucao = vector<vector<Voo>>; // vetor de pistas onde cada pista é uma lista de voos

// Gera uma solução inicial simples (PODE SER TROCADA PELO GULOSO)
Solucao solucaoInicial(const vector<Voo>& voos, int num_pistas) {
    Solucao pistas(num_pistas);
    for (size_t i = 0; i < voos.size(); ++i)
        pistas[i % num_pistas].push_back(voos[i]);
    return pistas;
}

// Vizinhança 1: troca dois voos entre pistas diferentes, caso reduz penalidade, aceita a mudança
bool vizinhanca1(Solucao& pistas, const vector<vector<int>>& t, int& custo) {
    for (int i = 0; i < pistas.size(); ++i) {
        for (int j = 0; j < pistas.size(); ++j) {
            if (i == j) continue;
            for (int a = 0; a < pistas[i].size(); ++a) {
                for (int b = 0; b < pistas[j].size(); ++b) {
                    swap(pistas[i][a], pistas[j][b]);
                    int nova_penalidade = calcularPenalidade(pistas, t);
                    if (nova_penalidade < custo) {
                        custo = nova_penalidade;
                        return true;
                    }
                    swap(pistas[i][a], pistas[j][b]); // desfaz
                }
            }
        }
    }
    return false;
}

// Vizinhança 2: mover um voo de uma pista para outra, caso reduz penalidade, aceita a mudança
bool vizinhanca2(Solucao& pistas, const vector<vector<int>>& t, int& custo) {
    for (int i = 0; i < pistas.size(); ++i) {
        for (int j = 0; j < pistas.size(); ++j) {
            if (i == j || pistas[i].empty()) continue;
            for (int k = 0; k < pistas[i].size(); ++k) {
                Voo temp = pistas[i][k];
                pistas[i].erase(pistas[i].begin() + k);
                pistas[j].push_back(temp);
                int nova_penalidade = calcularPenalidade(pistas, t);
                if (nova_penalidade < custo) {
                    custo = nova_penalidade;
                    return true;
                }
                pistas[j].pop_back();
                pistas[i].insert(pistas[i].begin() + k, temp);
            }
        }
    }
    return false;
}

// Vizinhança 3: inverter dois voos dentro da mesma pista, caso reduz penalidade, aceita a mudança
bool vizinhanca3(Solucao& pistas, const vector<vector<int>>& t, int& custo) {
    for (auto& pista : pistas) {
        for (int i = 0; i < pista.size(); ++i) {
            for (int j = i + 1; j < pista.size(); ++j) {
                swap(pista[i], pista[j]);
                int nova_penalidade = calcularPenalidade(pistas, t);
                if (nova_penalidade < custo) {
                    custo = nova_penalidade;
                    return true;
                }
                swap(pista[i], pista[j]);
            }
        }
    }
    return false;
}

// VND principal
void VND(vector<Voo>& voos, const vector<vector<int>>& t, int num_pistas) {
    Solucao pistas = solucaoInicial(voos, num_pistas);
    int custo = calcularPenalidade(pistas, t);

    int k = 1;
    const int k_max = 2;

    while (k <= k_max) {
        bool melhorou = false;
        if (k == 1) melhorou = vizinhanca1(pistas, t, custo);
        else if (k == 2) melhorou = vizinhanca2(pistas, t, custo);
        else if (k == 3) melhorou = vizinhanca3(pistas, t, custo);

        if (melhorou)
            k = 1;
        else
            ++k;
    }

    cout << "Melhor penalidade encontrada: " << custo << endl;
    for (int i = 0; i < pistas.size(); ++i) {
        cout << "Pista " << i + 1 << ": ";
        for (auto& v : pistas[i])
            cout << "V" << v.id + 1 << " ";
        cout << endl;
    }
}