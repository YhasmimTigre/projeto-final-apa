#include <vector>
#include <iostream>
#include <algorithm>
#include <random>

#include "ils.h"
#include "vnd.h"

using namespace std;

void imprimirSolucao(const vector<Voo>& solucao) {
    for (const auto& voo : solucao) {
        cout << "Voo " << voo.id
             << ": pista = " << voo.pista_alocada
             << ", horário real = " << voo.horario_real
             << ", penalidade = " << voo.penalidade
             << "\n";
    }
    cout << "-----------------------\n";
}


void perturbar(Airport* airport, int tamanhoBloco = 2) {

    auto& pistas = airport->pistas;
    auto& voos = airport->voos;
    auto& tempo_espera = airport->tempo_espera;

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> pistaDist(0, airport->num_pistas - 1);

    int pistaA, pistaB;
    do {
        pistaA = pistaDist(gen);
        pistaB = pistaDist(gen);
    } while (pistaA == pistaB || pistas[pistaA].size() < tamanhoBloco || pistas[pistaB].size() < tamanhoBloco);

    uniform_int_distribution<> idxA(0, pistas[pistaA].size() - tamanhoBloco);
    uniform_int_distribution<> idxB(0, pistas[pistaB].size() - tamanhoBloco);

    int startA = idxA(gen);
    int startB = idxB(gen);

    // Troca os blocos de voos entre as pistas
    vector<int> blocoA(pistas[pistaA].begin() + startA, pistas[pistaA].begin() + startA + tamanhoBloco);
    vector<int> blocoB(pistas[pistaB].begin() + startB, pistas[pistaB].begin() + startB + tamanhoBloco);

    copy(blocoB.begin(), blocoB.end(), pistas[pistaA].begin() + startA);
    copy(blocoA.begin(), blocoA.end(), pistas[pistaB].begin() + startB);

    for (int id : blocoA)
        voos[id].pista_alocada = pistaB;
    for (int id : blocoB)
        voos[id].pista_alocada = pistaA;

    // Atualiza voos_anterior e horarios nas duas pistas
    auto atualizarPista = [&voos, &pistas, &tempo_espera](int pista) {
        int anterior = -1;
        int tempoAtual = 0;

        for (int id : pistas[pista]) {
            voos[id].voo_anterior = anterior;

            if (anterior == -1) {
                voos[id].horario_real = voos[id].horario_prev;
            } else {
                int espera = tempo_espera[voos[anterior].id][voos[id].id];
                voos[id].horario_real = max(voos[anterior].horario_real + voos[anterior].duracao + espera, voos[id].horario_prev);
            }

            anterior = id;
        }
    };

    atualizarPista(pistaA);
    atualizarPista(pistaB);

    // Atualiza custo total
    int att_custo_total = 0;
    for (const Voo& v : voos) {
        if (v.horario_real > v.horario_prev)
            att_custo_total += (v.horario_real - v.horario_prev) * v.multa;
        else
            att_custo_total += (v.horario_prev - v.horario_real) * v.penalidade;
    }

    cout << "[Perturbação] Troca de blocos entre pistas " << pistaA << " e " << pistaB << " concluída.\n";
}

vector<Voo> ILS(Airport* airport, int maxIter) {
    // 1. Solução inicial
    VND(airport);
    vector<Voo> melhorSolucao = airport->voos;
    int melhorCusto = airport->calcularCustoTotal();
    
    vector<Voo> solucaoAtual = melhorSolucao;
    int custoAtual = melhorCusto;

    // Configuração do ILS
    const double temperatura_inicial = melhorCusto * 0.1;
    double temperatura = temperatura_inicial;
    const double fator_resfriamento = 0.95;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    for (int iter = 0; iter < maxIter; iter++) {
        // 2. Perturbação
        Airport copia = *airport; // Cria cópia completa
        perturbar(&copia);
        
        // 3. Busca Local
        VND(&copia);
        if (copia.calcularCustoTotal() < airport->calcularCustoTotal()) {
            *airport = copia;  // Atualiza somente se melhorar
        }
        int custoLocal = copia.calcularCustoTotal();

        // 4. Critério de Aceitação (com tempera simulada)
        bool aceitar = false;
        if (custoLocal < custoAtual) {
            aceitar = true;
        } else {
            double probabilidade = exp((custoAtual - custoLocal) / temperatura);
            if (dis(gen) < probabilidade) {
                aceitar = true;
            }
        }

        if (aceitar) {
            *airport = copia; // Atualiza a solução atual
            solucaoAtual = copia.voos;
            custoAtual = custoLocal;

            if (custoLocal < melhorCusto) {
                melhorSolucao = copia.voos;
                melhorCusto = custoLocal;
                cout << "Iter " << iter << ": Novo melhor custo = " << melhorCusto << endl;
            }
        }

        // Resfriamento
        temperatura *= fator_resfriamento;
    }

    return melhorSolucao;
}