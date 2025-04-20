#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>

#include "ils.h"
#include "vnd.h"

using namespace std;

void imprimirSolucao(const Airport* airport) {
    for (const auto& voo : airport->voos) {
        cout << "Voo " << voo.id
             << ": pista = " << voo.pista_alocada
             << ", horário real = " << voo.horario_real
             << ", penalidade = " << voo.penalidade
             << "\n";
    }
    cout << "-----------------------\n";
}

void perturbar(Airport* airport, int tamanhoBloco = 2) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> pistaDist(0, airport->num_pistas - 1);

    int pistaA, pistaB;
    do {
        pistaA = pistaDist(gen);
        pistaB = pistaDist(gen);
    } while (pistaA == pistaB || 
             airport->pistas[pistaA].size() < tamanhoBloco || 
             airport->pistas[pistaB].size() < tamanhoBloco);

    uniform_int_distribution<> idxA(0, airport->pistas[pistaA].size() - tamanhoBloco);
    uniform_int_distribution<> idxB(0, airport->pistas[pistaB].size() - tamanhoBloco);

    int startA = idxA(gen);
    int startB = idxB(gen);

    // Troca os blocos de voos entre as pistas
    vector<int> blocoA(airport->pistas[pistaA].begin() + startA, 
                      airport->pistas[pistaA].begin() + startA + tamanhoBloco);
    vector<int> blocoB(airport->pistas[pistaB].begin() + startB, 
                      airport->pistas[pistaB].begin() + startB + tamanhoBloco);

    // Realiza a troca
    for (int i = 0; i < tamanhoBloco; i++) {
        airport->pistas[pistaA][startA + i] = blocoB[i];
        airport->pistas[pistaB][startB + i] = blocoA[i];
        
        // Atualiza as pistas alocadas
        airport->voos[blocoB[i]].pista_alocada = pistaA;
        airport->voos[blocoA[i]].pista_alocada = pistaB;
    }

    // Atualiza horários e predecessores
    auto atualizarPista = [airport](int pista) {
        int anterior = -1;
        int tempoAtual = 0;

        for (int id : airport->melhor_pistas[pista]) {
            airport->voos[id].voo_anterior = anterior;

            if (anterior == -1) {
                airport->voos[id].horario_real = airport->voos[id].horario_prev;
            } else {
                int espera = airport->tempo_espera[anterior][id];
                airport->voos[id].horario_real = max(
                    airport->voos[anterior].horario_real + 
                    airport->voos[anterior].duracao + 
                    espera, 
                    airport->voos[id].horario_prev);
            }
            anterior = id;
        }
    };

    atualizarPista(pistaA);
    atualizarPista(pistaB);

    airport->calcularMultas();

    cout << "[Perturbação] Troca de blocos entre pistas " << pistaA << " e " << pistaB << " concluída.\n";
}

void ILS(Airport* airport, int maxIter) {
    // Executa VND inicial
    VND(airport);
    airport->salvarMelhorSolucao();
    int melhorCusto = airport->custo_melhor;

    cout << "\n--- Iniciando ILS ---\n";
    cout << "Custo inicial: " << melhorCusto << "\n";

    for (int iter = 0; iter < maxIter; iter++) {
        cout << "\nIteração " << iter + 1 << "/" << maxIter << endl;

        // Cria cópia da solução atual
        Airport* copia = new Airport(*airport);

        // Aplica perturbação
        perturbar(copia);

        // Refina com VND
        VND(copia);

        // Avalia a solução
        int custoAtual = copia->calcularCustoTotal();
        if (custoAtual < melhorCusto) {
            cout << "Nova melhor solução encontrada: " << custoAtual << " (anterior: " << melhorCusto << ")\n";
            *airport = *copia;  // Copia a solução melhorada
            airport->salvarMelhorSolucao();
            melhorCusto = custoAtual;
        }

        delete copia;
    }

    // Restaura a melhor solução encontrada
    airport->restaurarMelhorSolucao();
    cout << "\n--- Fim do ILS ---\n";
    cout << "Melhor custo final: " << melhorCusto << "\n";
    cout << "Melhor alocação:\n";
    imprimirSolucao(airport);
}