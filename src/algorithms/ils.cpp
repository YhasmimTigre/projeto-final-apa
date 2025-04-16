#include <vector>
#include <limits>
#include <iostream>
#include <algorithm>
#include <random>

#include "../core/airport_loader.h"
#include "alocador.h"
#include "guloso.h"
#include "vnd.h"

using namespace std;


void perturbar(vector<Voo>* solucao, int k = 2) { // k é a mudança aleatória
    int n = solucao->size();

    random_device rd; //gera num aleatórios
    mt19937 gen(rd());
    uniform_int_distribution<> vooDist(0, n - 1); // voos aleatórios
    uniform_int_distribution<> pistaDist(0, m - 1); // pistas aleatórias

    for (int i = 0; i < k; i++) {
        int idx = vooDist(gen);
        int novaPista = pistaDist(gen);
        (*solucao)[idx].pista_alocada = novaPista;
    }
}


vector<Voo> ILS(int maxIter, int& melhorCustoFinal) {
    // 1. Geração da solução inicial e refinamento
    vector<Voo> solucaoAtual = Guloso();
    solucaoAtual = VND();
    int custoAtual = calcularCustoTotal();

    vector<Voo> melhorSolucao = solucaoAtual;
    melhorCustoFinal = custoAtual;

    for (int iter = 0; iter < maxIter; iter++) {
        // 2. Perturba a solução atual
        vector<Voo> solucaoPerturbada = perturbar(solucaoAtual);

        // 3. Aplica VND na solução perturbada
        vector<Voo> solucaoLocal = VND();
        int custoLocal = calcularCustoTotal();

        // 4. Aceita se for melhor
        if (custoLocal < custoAtual) {
            solucaoAtual = solucaoLocal;
            custoAtual = custoLocal;

            if (custoAtual < melhorCustoFinal) {
                melhorSolucao = solucaoAtual;
                melhorCustoFinal = custoAtual;
                cout << "Iteração " << iter + 1 << ": nova melhor solução com custo " << melhorCustoFinal << endl;
            }
        }
    }

    return melhorSolucao;
}
