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
bool vizinhanca1() {
    cout << "\n--- VIZINHANCA 1 INICIADA ---\n";
    cout << "Custo inicial: " << custo_total << "\n\n";

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
                    cout << " \033[1;32m(MELHOR!)\033[0m\n";
                    return true;  // <<< FIRST IMPROVEMENT: retorna assim que melhora
                } else {
                    // Desfaz
                    pistas = pistas_bkp;
                    voos = voos_bkp;
                    cout << " \033[1;31m(piorou)\033[0m\n";
                }
            } else {
                cout << " | \033[1;33m(invalido)\033[0m\n";
            }
        }
    }

    cout << "\n--- FIM VIZINHANCA 1 ---\n";
    cout << "Custo final: " << custo_total << "\n";
    return false;  // nenhuma melhoria encontrada
}

// MOVIMENTO 2: Mover um voo para outra pista
bool vizinhanca2() {
    cout << "\n--- VIZINHANCA 2 INICIADA ---\n";
    cout << "Custo inicial: " << custo_total << "\n\n";

    for (int p = 0; p < m; p++) {
        for (size_t i = 0; i < pistas[p].size() - 2; i++) {
            // Backup
            auto pistas_bkp = pistas;
            auto voos_bkp = voos;
            int id1 = pistas[p][i];
            int id2 = pistas[p][i+2];

            cout << "  Testando P" << p << ": V" << id1 << " → V" << id2;

            // Tenta mover voo i para posição i+2 (intra pista)
            if (insertIntraPista(p, i, i+2)) {
                int novo_custo = calcularCustoTotal();
                cout << " | Custo novo: " << novo_custo;

                if (novo_custo < custo_total) {
                    custo_total = novo_custo;
                    cout << " \033[1;32m(MELHOR!)\033[0m\n";
                    return true;  // <<< FIRST IMPROVEMENT
                } else {
                    pistas = pistas_bkp;
                    voos = voos_bkp;
                    cout << " \033[1;31m(piorou)\033[0m\n";
                }
            } else {
                cout << " | \033[1;33m(invalido)\033[0m\n";
            }
        }
    }

    cout << "\n--- FIM VIZINHANCA 2 ---\n";
    cout << "Custo final: " << custo_total << "\n";
    return false;  // nenhuma melhoria
}



// MOVIMENTO 3: 
bool vizinhanca3() {
    int max_gap = 5;

    cout << "\n--- VIZINHANÇA 3 (2-Opt com limite) INICIADA ---\n";
    cout << "Custo inicial: " << custo_total << "\n\n";

    for (int p = 0; p < m; p++) {
        int tam = pistas[p].size();

        for (int i = 0; i < tam - 1; i++) {
            for (int j = i + 2; j < min(i + max_gap, tam); j++) {
                cout << "  Testando P" << p << ": invertendo [" << i << ", " << j << "]";

                auto pistas_bkp = pistas;
                auto voos_bkp = voos;
                int custo_antigo = custo_total;

                // Aplicar inversão
                opt2IntraPista(p, i, j);
                int novo_custo = calcularCustoTotal();

                if (novo_custo < custo_antigo) {
                    custo_total = novo_custo;
                    cout << " \033[1;32m(MELHOR!)\033[0m\n";
                    return true;  // <<< FIRST IMPROVEMENT
                } else {
                    pistas = pistas_bkp;
                    voos = voos_bkp;
                    cout << " \033[1;31m(não melhorou)\033[0m\n";
                }
            }
        }
    }

    cout << "\n--- FIM VIZINHANÇA 3 ---\n";
    cout << "Custo final: " << custo_total << "\n";
    return false;
}


void VND() {
    vector<bool(*)()> vizinhancas = {vizinhanca1, vizinhanca2, vizinhanca3};
    int k = 0;

    cout << "\n--- VND INICIADO ---\n";
    cout << "Custo inicial: " << custo_total << "\n\n";

    while (k < vizinhancas.size()) {
        cout << "Tentando vizinhança " << k + 1 << "...\n";

        bool melhorou = vizinhancas[k]();

        if (melhorou) {
            cout << "Melhoria encontrada! Voltando para vizinhança 1.\n\n";
            k = 0;  
        } else {
            k++;   
        }
    }

    cout << "--- FIM DO VND ---\n";
    cout << "Melhor custo final: " << custo_total << endl;
}
