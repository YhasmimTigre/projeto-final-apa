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
    // 1. Geração da solução inicial
    VND(airport);            // melhora a solução inicial
    vector<Voo> solucaoAtual = airport->voos; // solução inicial
    int custoAtual = airport->calcularCustoTotal();

    vector<Voo> melhorSolucao = solucaoAtual;
    int melhorCustoFinal = custoAtual;

    for (int iter = 0; iter < maxIter; iter++) {
        // 2. Cria uma cópia perturbada da solução atual

        vector<Voo> solucaoPerturbada = solucaoAtual;

        cout << "Antes da perturbação (iter " << iter + 1 << "):\n";
        imprimirSolucao(solucaoAtual);

        perturbar(airport);

        cout << "Depois da perturbação:\n";
        imprimirSolucao(solucaoPerturbada);

        // 3. Atribui ao vetor global para que o VND opere sobre ele
        airport->voos = solucaoPerturbada;
        VND(airport);  // refina a solução perturbada
        vector<Voo> solucaoLocal = airport->voos;
        int custoLocal = airport->calcularCustoTotal();

        cout << "Após VND na solução perturbada:\n";
        imprimirSolucao(airport->voos);  // `voos` é seu vetor global atualizado
        cout << "Custo local após VND: " << custoLocal << "\n";


        // 4. Aceita se for melhor
        if (custoLocal < custoAtual) {
            cout << "Solução melhor encontrada na iteração " << iter + 1 << " com custo " << custoLocal << "\n";
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