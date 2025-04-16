#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include "../core/airport_loader.h"
#include "guloso.h"
#include "alocador.h"
#include <climits>

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

    //cout << "Voos na pista " << pista << ": " << voos_pista.size() << endl; 

    // Calcula custo da pista
    for (auto& voo_ptr: voos_pista){
        Voo& voo = *voo_ptr;
        int tempo_inicio;

        if (ultimo_voo != -1){
            tempo_inicio = max(ultimo_tempo + tempo_espera[ultimo_voo][voo.id], voo.horario_prev);
        } else {
            tempo_inicio = voo.horario_prev;
        }

        // Atualiza Voo
        voo.horario_real = tempo_inicio;
        voo.voo_anterior = ultimo_voo;
        int atraso = max(0, tempo_inicio - voo.horario_prev);
        voo.multa = atraso * voo.penalidade;
        custo_pista += voo.multa;

        if (voo.horario_real < 0 || voo.multa < 0) {
            cout << "Voo " << voo.id << ": horario_real=" << voo.horario_real
                 << ", multa=" << voo.multa << endl;
        }

        ultimo_tempo = tempo_inicio + voo.duracao;
        ultimo_voo = voo.id;
    }

    //cout << "Custo total da pista " << pista << ": " << custo_pista << endl;
    return custo_pista;

}

void validarCusto(int custo, string local) {
    if (custo < 0) {
        cout << "ERRO: Custo negativo detectado em " << local << ": " << custo << endl;
        
        // Exibir detalhes dos voos para debug
        for (const auto& voo : voos) {
            cout << "Voo " << voo.id 
                 << ", pista=" << voo.pista_alocada
                 << ", prev=" << voo.horario_prev
                 << ", real=" << voo.horario_real
                 << ", multa=" << voo.multa << endl;
        }
        
        // Recalcular o custo total e exibir
        int custoRecalculado = calcularCustoTotal();
        cout << "Custo total recalculado: " << custoRecalculado << endl;
    }
}

// MOVIMENTO 3: Inverter dois voos consecutivos na mesma pista
bool vizinhanca1() {//O(n^2)
    bool melhorou = false;
    vector<Voo> voos_backup = voos;
    int custo_backup = novo_custo; // salva o custo atual antes da inversão

    // Agrupa voos por pista mantendo a ordem atual
    vector<vector<int>> voos_por_pista(m);
    for (int i = 0; i < n; i++) {
        voos_por_pista[voos[i].pista_alocada].push_back(i);
    }

    for (int p = 0; p < m; p++) {
        for (size_t i = 0; i + 1 < voos_por_pista[p].size(); i++) {
            int id1 = voos_por_pista[p][i];
            int id2 = voos_por_pista[p][i+1];
            
            // Remove o custo da pista antes da inversão
            int custo_antes = recalcularPista(p);
            //novo_custo = calcularCustoTotal() - recalcularPista(p);
            
            // Inverte a ordem dos voos na mesma pista
            // Aqui precisamos modificar os índices na estrutura voos em vez de
            // simplesmente fazer swap, já que cada voo tem sua própria pista_alocada
            
            
            // Inverte a ordem dos voos
            swap(voos[id1].horario_real, voos[id2].horario_real);
            swap(voos[id1].voo_anterior, voos[id2].voo_anterior);
            
            // Recalcula a pista afetada
            int custo_depois = recalcularPista(p);
            novo_custo += custo_depois - custo_antes;

            if (novo_custo < 0) {
                cout << "WARNING: Cost became negative: " << novo_custo << endl;
                cout << "custo_antes: " << custo_antes << ", custo_depois: " << custo_depois << endl;
                // Use fallback to recalculate
                novo_custo = calcularCustoTotal();
            }

            cout << "Novo custo: " << novo_custo << endl;

            if (novo_custo < custo_minimo_global) {
                custo_minimo_global = novo_custo;
                melhorou = true;
                voos_backup = voos; // Atualiza o backup
                break; // Sai do loop de inversões
            } else {
                voos = voos_backup; // Restaura a solução
                novo_custo = custo_backup; // Restaura o custo anterior
            }
            validarCusto(novo_custo, "vizinhanca1 após movimento");
        }
        if (melhorou) break; // Sai do loop de pistas se encontrou melhoria
    }

    return melhorou;
}

// MOVIMENTO 2: Mover um voo para outra pista
bool vizinhanca2() {//O((n^2)*m)
    bool melhorou = false;
    vector<Voo> voos_backup = voos;
    int custo_backup = novo_custo; // salva o custo atual antes da troca

    for (int i = 0; i < n; i++) {
        int pista_original = voos[i].pista_alocada;

        for (int nova_pista = 0; nova_pista < m; nova_pista++) {
            if (nova_pista == pista_original) continue;

            // Remove os custos antes da movimentação
            int custo_antes = recalcularPista(pista_original) + recalcularPista(nova_pista);

            // Faz a movimentação
            voos[i].pista_alocada = nova_pista;

            // Recalcula os custos após a movimentação
            int custo_depois = recalcularPista(pista_original) + recalcularPista(nova_pista);
            novo_custo += custo_depois - custo_antes;

            if (novo_custo < 0) {
                cout << "WARNING: Cost became negative: " << novo_custo << endl;
                cout << "custo_antes: " << custo_antes << ", custo_depois: " << custo_depois << endl;
                // Use fallback to recalculate
                novo_custo = calcularCustoTotal();
            }

            if (novo_custo < custo_minimo_global) {
                custo_minimo_global = novo_custo;
                melhorou = true;
                voos_backup = voos; // Atualiza o backup com a melhor solução
                break; // Sai do loop de pistas
            } else {
                voos = voos_backup; // Restaura a solução anterior
                novo_custo = custo_backup; // Restaura o custo anterior
            }
            validarCusto(novo_custo, "vizinhanca2 após movimento");
        }
        if (melhorou) break; // Sai do loop de voos se encontrou melhoria
    }

    return melhorou;
}


// MOVIMENTO 1: Trocar dois voos entre pistas diferentes
bool vizinhanca3() { //O(n^3)
    bool melhorou = false;
    vector<Voo> voos_backup = voos;
    int custo_backup = novo_custo; // salva o custo atual antes da troca

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (voos[i].pista_alocada == voos[j].pista_alocada) continue;

            int pista_original_i = voos[i].pista_alocada;
            int pista_original_j = voos[j].pista_alocada;
            
            // Remove os custos antes da troca
            int custo_antes = recalcularPista(pista_original_i) + recalcularPista(pista_original_j);

            // Faz a troca
            swap(voos[i].pista_alocada, voos[j].pista_alocada);

            // Recalcula toda a solução
            int custo_depois = recalcularPista(pista_original_i) + recalcularPista(pista_original_j);
            novo_custo += custo_depois - custo_antes;

            if (novo_custo < 0) {
                cout << "WARNING: Cost became negative: " << novo_custo << endl;
                cout << "custo_antes: " << custo_antes << ", custo_depois: " << custo_depois << endl;
                // Use fallback to recalculate
                novo_custo = calcularCustoTotal();
            }

            if (novo_custo < custo_minimo_global) {
                custo_minimo_global = novo_custo;
                melhorou = true;
                break; // Sai do loop interno
            } else {
                voos = voos_backup; // Restaura a solução anterior
                novo_custo = custo_backup; // Restaura o custo anterior
            }
            validarCusto(novo_custo, "vizinhanca3 após movimento");
        }
        if (melhorou) break; // Sai do loop externo se encontrou melhoria
    }

    return melhorou;
}



// MOVIMENTO 4: Realocar posição de um voo dentro da mesma pista
bool vizinhanca4() {//O(m*n^3)
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
                
                // Calcula o custo antes da realocação
                int custo_antes = recalcularPista(p);
                
                // Salva a ordem atual para poder restaurar
                vector<int> ordem_original = voos_por_pista[p];
                
                // Remove o voo da posição atual e insere na nova posição
                voos_por_pista[p].erase(voos_por_pista[p].begin() + pos_atual);
                voos_por_pista[p].insert(voos_por_pista[p].begin() + nova_pos, voo_id);
                
                // Reordena os voos mantendo a pista
                vector<int> horarios_reais(n, 0);
                vector<int> voos_anteriores(n, -1);
                
                // Preserva os valores originais
                for (int i = 0; i < n; i++) {
                    horarios_reais[i] = voos[i].horario_real;
                    voos_anteriores[i] = voos[i].voo_anterior;
                }
                
                // Recalcular horários reais e voo anterior para todos os voos na pista
                int ultimo_tempo = 0;
                int ultimo_voo_id = -1;
                
                // Importante: aplicar a nova ordem no vetor de voos
                for (size_t pos = 0; pos < voos_por_pista[p].size(); pos++) {
                    int voo_id = voos_por_pista[p][pos];
                    Voo& voo = voos[voo_id];
                    
                    int tempo_inicio;
                    if (ultimo_voo_id != -1) {
                        tempo_inicio = max(ultimo_tempo + tempo_espera[ultimo_voo_id][voo_id], voo.horario_prev);
                    } else {
                        tempo_inicio = voo.horario_prev;
                    }
                    
                    voo.horario_real = tempo_inicio;
                    voo.voo_anterior = ultimo_voo_id;
                    
                    ultimo_tempo = tempo_inicio + voo.duracao;
                    ultimo_voo_id = voo_id;
                }
                
                // Depois disso, recalcular o custo da pista
                int custo_depois = recalcularPista(p);
                
                // Atualiza o custo total
                novo_custo += custo_depois - custo_antes;
                
                if (novo_custo < 0) {
                    cout << "WARNING: Cost became negative: " << novo_custo << endl;
                    cout << "custo_antes: " << custo_antes << ", custo_depois: " << custo_depois << endl;
                    // Use fallback to recalculate
                    novo_custo = calcularCustoTotal();
                }

                if (novo_custo < custo_minimo_global) {
                    custo_minimo_global = novo_custo;
                    melhorou = true;
                    voos_backup = voos;
                    break;
                } else {
                    // Restaura a ordem original
                    voos_por_pista[p] = ordem_original;
                    voos = voos_backup;
                    novo_custo = custo_backup;
                }
                validarCusto(novo_custo, "vizinhanca4 após movimento");
            }
            if (melhorou) break;
        }
        if (melhorou) break;
    }
    
    return melhorou;
}

// MOVIMENTO 5: Trocar dois pares de voos consecutivos entre pistas diferentes
bool vizinhanca5() {//O((m^2)*(n^3))
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
                    
                    // Calcula o custo antes da troca
                    int custo_antes = calcularCustoTotal();

                    // Troca os pares de voos entre as pistas
                    voos[voo1_p1].pista_alocada = p2;
                    voos[voo2_p1].pista_alocada = p2;
                    voos[voo1_p2].pista_alocada = p1;
                    voos[voo2_p2].pista_alocada = p1;

                    // Recalcular todos os custos
                    recalcularPista(p1);
                    recalcularPista(p2);
                    int custo_depois = calcularCustoTotal();

                    // Atualiza o custo total
                    novo_custo = custo_depois;
                    
                    if (novo_custo < 0) {
                        cout << "WARNING: Cost became negative: " << novo_custo << endl;
                        cout << "custo_antes: " << custo_antes << ", custo_depois: " << custo_depois << endl;
                        // Use fallback to recalculate
                        novo_custo = calcularCustoTotal();
                    }

                    if (novo_custo < custo_minimo_global) {
                        custo_minimo_global = novo_custo;
                        melhorou = true;
                        voos_backup = voos;
                        break;
                    } else {
                        voos = voos_backup;
                        novo_custo = custo_backup;
                    }
                    validarCusto(novo_custo, "vizinhanca5 após movimento");
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
    novo_custo = custo_minimo_global; // Adicionar esta linha

    cout << "Custo inicial: " << custo_minimo_global << endl;

    vector<bool (*)()> vizinhancas = {vizinhanca1, vizinhanca2, vizinhanca3, vizinhanca4, vizinhanca5};
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
        validarCusto(custo_minimo_global, "VND iteração " + to_string(iteracoes));
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

