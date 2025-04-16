#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include "../core/airport_loader.h"
#include "guloso.h"
#include "alocador.h"

using namespace std;

// Variável para controlar o custo mínimo encontrado
int custo_minimo_global;
int novo_custo;

int recalcularPista(int pista) { 
    int custo_pista = 0;
    int ultimo_tempo = 0;
    int ultimo_voo = -1;

    vector<Voo*> voos_pista;
    for (auto& voo: voos){
        if (voo.pista_alocada == pista) {
            voos_pista.push_back(&voo);
        }
    }    

    // Calcula custo da pista
    for (auto& voo_ptr: voos_pista){
        Voo& voo = *voo_ptr;
        int tempo_inicio;

        if (ultimo_voo != -1){
            tempo_inicio = ultimo_tempo + horario_prev;
        } else {
            tempo_inicio = voo.horario_prev
        }

        // Atualiza Voo
        voo.horario_real = tempo_inicio;
        voo.voo_anterior = ultimo_voo;
        voo.atraso = tempo_inicio - voo.horario_prev;
        voo.multa = voo.atraso * voo.penalidade;
        custo_pista += voo.multa;
    }
    return custo_pista;

}

// MOVIMENTO 1: Trocar dois voos entre pistas diferentes
bool vizinhanca1() {
    bool melhorou = false;
    vector<Voo> voos_backup = voos;
    int custo_backup = novo_custo; // salva o custo atual antes da troca

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (voos[i].pista_alocada == voos[j].pista_alocada) continue;

            int pista_original_i = voos[i].pista_alocada;
            int pista_original_j = voos[j].pista_alocada;
            
            // Remove os custos antes da troca
            novo_custo -= recalcularPista(pista_original_i);
            novo_custo -= recalcularPista(pista_original_j);
            // Faz a troca
            swap(voos[i].pista_alocada, voos[j].pista_alocada);

            // Recalcula toda a solução
            novo_custo += recalcularPista(pista_original_i);
            novo_custo += recalcularPista(pista_original_j);

            if (novo_custo < custo_minimo_global) {
                custo_minimo_global = novo_custo;
                melhorou = true;
                voos_backup = voos; // Atualiza o backup com a melhor solução
                break; // Sai do loop interno
            } else {
                voos = voos_backup; // Restaura a solução anterior
                novo_custo = custo_backup; // Restaura o custo anterior
            }
        }
        if (melhorou) break; // Sai do loop externo se encontrou melhoria
    }

    return melhorou;
}

// MOVIMENTO 2: Mover um voo para outra pista
bool vizinhanca2() {
    bool melhorou = false;
    vector<Voo> voos_backup = voos;

    for (int i = 0; i < n; i++) {
        int pista_original = voos[i].pista_alocada;

        for (int nova_pista = 0; nova_pista < m; nova_pista++) {
            if (nova_pista == pista_original) continue;

            voos[i].pista_alocada = nova_pista;
            recalcularApenasPistas();
            int novo_custo = calcularCustoTotal();

            if (novo_custo < custo_minimo_global) {
                custo_minimo_global = novo_custo;
                melhorou = true;
                voos_backup = voos; // Atualiza o backup
                break; // Sai do loop de pistas
            } else {
                voos = voos_backup; // Restaura a solução
            }
        }
        if (melhorou) break; // Sai do loop de voos se encontrou melhoria
    }

    return melhorou;
}

// MOVIMENTO 3: Inverter dois voos consecutivos na mesma pista
bool vizinhanca3() {
    bool melhorou = false;
    vector<Voo> voos_backup = voos;

    // Agrupa voos por pista mantendo a ordem atual
    vector<vector<int>> voos_por_pista(m);
    for (int i = 0; i < n; i++) {
        voos_por_pista[voos[i].pista_alocada].push_back(i);
    }

    for (int p = 0; p < m; p++) {
        for (size_t i = 0; i + 1 < voos_por_pista[p].size(); i++) {
            // Inverte a ordem na pista
            swap(voos[voos_por_pista[p][i]], voos[voos_por_pista[p][i+1]]);

            // Recalcula a solução
            recalcularApenasPistas();
            int novo_custo = calcularCustoTotal();

            if (novo_custo < custo_minimo_global) {
                custo_minimo_global = novo_custo;
                melhorou = true;
                voos_backup = voos; // Atualiza o backup
                break; // Sai do loop de inversões
            } else {
                voos = voos_backup; // Restaura a solução
            }
        }
        if (melhorou) break; // Sai do loop de pistas se encontrou melhoria
    }

    return melhorou;
}

// VND principal com critério de parada melhorado
void VND() {
    // Inicializa com o custo da solução gulosa
    custo_minimo_global = calcularCustoTotal();
    cout << "Custo inicial: " << custo_minimo_global << endl;

    vector<bool (*)()> vizinhancas = {vizinhanca1, vizinhanca2, vizinhanca3};
    bool melhoria_global = false;
    int iteracoes = 0;
    const int MAX_ITERACOES = 100; // Limite de segurança

    do {
        melhoria_global = false;

        for (size_t k = 0; k < vizinhancas.size(); k++) {
            bool melhoria_local = vizinhancas[k]();

            if (melhoria_local) {
                melhoria_global = true;
                cout << "Melhoria encontrada com vizinhanca " << k+1
                     << ", novo custo: " << custo_minimo_global << endl;
                break; // Volta para a primeira vizinhança
            }
        }

        iteracoes++;
        if (iteracoes >= MAX_ITERACOES) {
            cout << "Limite de iteracoes atingido (" << MAX_ITERACOES << ")" << endl;
            break;
        }

    } while (melhoria_global);

    // Atualiza o novo_custo global com o melhor encontrado
    novo_custo = custo_minimo_global;

    // Exibe solução final
    cout << "\n--- Solucao Final VND ---\n";
    cout << "Custo total: " << novo_custo << "\n";
    cout << "Iteracoes realizadas: " << iteracoes << "\n";

    vector<vector<int>> voos_por_pista(m);
    for (const auto& voo : voos) {
        voos_por_pista[voo.pista_alocada].push_back(voo.id + 1);
    }

    for (int p = 0; p < m; p++) {
        cout << "Pista " << p+1 << ": ";
        for (int id : voos_por_pista[p]) {
            cout << id << " ";
        }
        cout << "\n";
    }
}