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
    int custo_backup = novo_custo;

    for (int i = 0; i < n; i++) {
        int pista_original = voos[i].pista_alocada;
        
        for (int nova_pista = 0; nova_pista < m; nova_pista++) {
            if (nova_pista == pista_original) continue;

            // Remove os custos antes da troca
            novo_custo -= recalcularPista(pista_original_i);
            novo_custo -= recalcularPista(pista_original_j);

            voos[i].pista_alocada = nova_pista;
            
            novo_custo += recalcularPista(pista_original_i);
            novo_custo += recalcularPista(pista_original_j);

            if (novo_custo < custo_minimo_global) {
                custo_minimo_global = novo_custo;
                melhorou = true;
                voos_backup = voos; // Atualiza o backup
                break; // Sai do loop de pistas
            } else {
                voos = voos_backup; // Restaura a solução
                novo_custo = custo_backup; // Restaura o custo anterior
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
    int custo_backup = novo_custo;

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

// MOVIMENTO 4: Realocar posição de um voo dentro da mesma pista
bool vizinhanca4() {
    bool melhorou = false;
    vector<Voo> voos_backup = voos;
    int custo_backup = novo_custo;

    // Agrupa voos por pista mantendo a ordem atual
    vector<vector<int>> voos_por_pista(m);
    for (int i = 0; i < n; i++) {
        voos_por_pista[voos[i].pista_alocada].push_back(i);
    }

    for (int p = 0; p < m; p++) {
        if (voos_por_pista[p].size() < 3) continue; // Precisa de pelo menos 3 voos para realocar

        for (size_t pos_atual = 0; pos_atual < voos_por_pista[p].size(); pos_atual++) {
            int voo_id = voos_por_pista[p][pos_atual];
            
            // Tenta reposicionar para todas as outras posições na mesma pista
            for (size_t nova_pos = 0; nova_pos < voos_por_pista[p].size(); nova_pos++) {
                if (nova_pos == pos_atual) continue;
                
                novo_custo -= recalcularPista(p);
                
                // Remove o voo da posição atual
                voos_por_pista[p].erase(voos_por_pista[p].begin() + pos_atual);
                
                // Insere na nova posição
                voos_por_pista[p].insert(voos_por_pista[p].begin() + nova_pos, voo_id);
                
                // Reordena os voos na estrutura principal de acordo com a nova ordem
                for (int i = 0; i < voos.size(); i++) {
                    if (voos[i].pista_alocada == p) {
                        voos[i].pista_alocada = -1; // Marca temporariamente
                    }
                }
                
                for (size_t i = 0; i < voos_por_pista[p].size(); i++) {
                    voos[voos_por_pista[p][i]].pista_alocada = p;
                }
                
                novo_custo += recalcularPista(p);
                
                if (novo_custo < custo_minimo_global) {
                    custo_minimo_global = novo_custo;
                    melhorou = true;
                    voos_backup = voos;
                    break;
                } else {
                    voos = voos_backup;
                    novo_custo = custo_backup;
                    voos_por_pista[p].clear();
                    for (int i = 0; i < n; i++) {
                        if (voos[i].pista_alocada == p) {
                            voos_por_pista[p].push_back(i);
                        }
                    }
                }
            }
            if (melhorou) break;
        }
        if (melhorou) break;
    }
    
    return melhorou;
}

// MOVIMENTO 5: Trocar dois pares de voos consecutivos entre pistas diferentes
bool vizinhanca5() {
    bool melhorou = false;
    vector<Voo> voos_backup = voos;
    int custo_backup = novo_custo;

    // Agrupa voos por pista mantendo a ordem atual
    vector<vector<int>> voos_por_pista(m);
    for (int i = 0; i < n; i++) {
        voos_por_pista[voos[i].pista_alocada].push_back(i);
    }

    for (int p1 = 0; p1 < m; p1++) {
        if (voos_por_pista[p1].size() < 2) continue; // Precisa de pelo menos 2 voos
        
        for (int p2 = p1 + 1; p2 < m; p2++) {
            if (voos_por_pista[p2].size() < 2) continue; // Precisa de pelo menos 2 voos
            
            for (size_t i = 0; i < voos_por_pista[p1].size() - 1; i++) {
                int voo1_p1 = voos_por_pista[p1][i];
                int voo2_p1 = voos_por_pista[p1][i + 1];
                
                for (size_t j = 0; j < voos_por_pista[p2].size() - 1; j++) {
                    int voo1_p2 = voos_por_pista[p2][j];
                    int voo2_p2 = voos_por_pista[p2][j + 1];
                    
                    // Remove os custos das pistas afetadas
                    novo_custo -= recalcularPista(p1);
                    novo_custo -= recalcularPista(p2);
                    
                    // Troca os pares de voos entre as pistas
                    voos[voo1_p1].pista_alocada = p2;
                    voos[voo2_p1].pista_alocada = p2;
                    voos[voo1_p2].pista_alocada = p1;
                    voos[voo2_p2].pista_alocada = p1;
                    
                    // Recalcula os custos
                    novo_custo += recalcularPista(p1);
                    novo_custo += recalcularPista(p2);
                    
                    if (novo_custo < custo_minimo_global) {
                        custo_minimo_global = novo_custo;
                        melhorou = true;
                        voos_backup = voos;
                        break;
                    } else {
                        voos = voos_backup;
                        novo_custo = custo_backup;
                    }
                }
                if (melhorou) break;
            }
            if (melhorou) break;
        }
        if (melhorou) break;
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