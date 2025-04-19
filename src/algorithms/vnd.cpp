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

//variaveis de controle para os novos custos encontrados
int novo_custo;
bool melhorou = false;
//int custo_backup = novo_custo; 
vector<Voo> voos_backup = voos;
//double melhor_custo = custo_total;

// MOVIMENTO 1: Inverter dois voos consecutivos na mesma pista
void vizinhanca1() {
    bool melhorou;
    int iter = 0;
    
    cout << "\n--- VIZINHANCA 1 INICIADA ---\n";
    cout << "Custo inicial: " << custo_total << "\n\n";

    do {
        melhorou = false;
        iter++;
        
        cout << "ITERACAO " << iter << ":\n";
        
        for (int p = 0; p < m; p++) {
            for (size_t i = 0; i < pistas[p].size() - 1; i++) {
                // Backup
                auto pistas_bkp = pistas;
                auto voos_bkp = voos;
                int custo_antigo = custo_total;
                int id1 = pistas[p][i];
                int id2 = pistas[p][i+1];

                cout << "  Testando P" << p << ": V" << id1 << " ↔ V" << id2;
                
                if (inverterVoosConsecutivos(p, i)) {
                    int novo_custo = calcularCustoTotal();
                    cout << " | Custo novo: " << novo_custo;
                    
                    if (novo_custo < custo_total) {
                        custo_total = novo_custo;
                        melhorou = true;
                        cout << " \033[1;32m(MELHOR!)\033[0m";
                    } else {
                        // Desfaz
                        pistas = pistas_bkp;
                        voos = voos_bkp;
                        cout << " \033[1;31m(piorou)\033[0m";
                    }
                } else {
                    cout << " | \033[1;33m(invalido)\033[0m";
                }
                
                cout << endl;
            }
        }
        
        cout << "Melhor custo atual: " << custo_total << "\n\n";
        
    } while (melhorou && iter < 50);

    cout << "--- FIM VIZINHANCA 1 ---\n";
    cout << "Melhor custo final: " << custo_total << endl;
    cout << "Total iteracoes: " << iter << endl;
}
// MOVIMENTO 2: Mover um voo para outra pista
void vizinhanca2() {
    bool melhorou;
    int iter = 0;

    cout << "\n--- VIZINHANCA 2 INICIADA ---\n";
    cout << "Custo inicial: " << custo_total << "\n\n";

    do {
        melhorou = false;
        iter++;

        cout << "ITERACAO " << iter << ":\n";

        for (int p = 0; p < m; p++) {
            for (size_t i = 0; i < pistas[p].size() - 1; i++) {
                // Backup
                auto pistas_bkp = pistas;
                auto voos_bkp = voos;
                int custo_antigo = custo_total;
                int id1 = pistas[p][i];
                int id2 = pistas[p][i+1];

                cout << "  Testando P" << p << ": V" << id1 << " ↔ V" << id2;

                // Aqui substituímos o uso de inverterVoosConsecutivos por insertIntraPista
                if (insertIntraPista(p, i, i+1)) {  // Tentando mover o voo i para a posição i+1
                    int novo_custo = calcularCustoTotal();
                    cout << " | Custo novo: " << novo_custo;

                    if (novo_custo < custo_total) {
                        custo_total = novo_custo;
                        melhorou = true;
                        cout << " \033[1;32m(MELHOR!)\033[0m";
                    } else {
                        // Desfaz a mudança se o custo não melhorou
                        pistas = pistas_bkp;
                        voos = voos_bkp;
                        cout << " \033[1;31m(piorou)\033[0m";
                    }
                } else {
                    cout << " | \033[1;33m(invalido)\033[0m";
                }

                cout << endl;
            }
        }

        cout << "Melhor custo atual: " << custo_total << "\n\n";

    } while (melhorou && iter < 50);

    cout << "--- FIM VIZINHANCA 2 ---\n";
    cout << "Melhor custo final: " << custo_total << endl;
    cout << "Total iteracoes: " << iter << endl;
}



// MOVIMENTO 3: Trocar dois voos entre pistas diferentes
bool vizinhanca3() { 
    


 
    return melhorou;
}




// VND principal com critério de parada melhorado
/*void VND() {
    // Inicializa com o custo da solução gulosa
    custo_total = calcularCustoTotal();
    novo_custo = custo_total; // Adicionar esta linha

    cout << "Custo inicial: " << custo_total << endl;

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
                     << ", novo custo: " << custo_total << endl;
                break; // Volta para a primeira vizinhança
            }
        }

        iteracoes++;
        validarCusto(custo_total, "VND iteracao " + to_string(iteracoes));
        if (iteracoes >= MAX_ITERACOES) {
            cout << "Limite de iteracoes atingido (" << MAX_ITERACOES << ")" << endl;
            break;
        }

    } while (melhoria_global);

    // Atualiza o novo_custo global com o melhor encontrado
    novo_custo = custo_total;

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
}*/

