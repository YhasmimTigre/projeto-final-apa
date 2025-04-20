#include <iostream>
#include <vector>
#include <algorithm>
#include "../heart/airport.h"
using namespace std;

// aux local
static bool testarMelhoria(Airport* airport, int custo_atual, const string& mensagem) {

    int novo_custo = airport->calcularCustoTotal();
    cout << mensagem << novo_custo;
    
    if (novo_custo < custo_atual) {
    
        cout << " \033[1;32m(MELHOR!)\033[0m\n";
        airport->salvarMelhorSolucao();
        return true;
    
    } else {
    
        cout << " \033[1;31m(piorou)\033[0m\n";
        airport->restaurarMelhorSolucao();
        return false;
    
    }
}

// MOVIMENTO 1: Inverter dois voos consecutivos na mesma pista
bool vizinhanca1(Airport* airport) {

    cout << "\n--- VIZINHANCA 1 INICIADA ---\n";
    int custo_atual = airport->calcularCustoTotal();
    cout << "Custo inicial: " << custo_atual << "\n\n";

    bool melhorou = false;

    for (int p = 0; p < airport->num_pistas; p++) {

        for (size_t i = 0; i < airport->pistas[p].size() - 1; i++) {

            //vamos lidar com o backup direto na classe

            int id1 = airport->pistas[p][i];
            int id2 = airport->pistas[p][i+1];
            cout << "  Testando P" << p << ": V" << id1 << " <--> V" << id2;

            // Tenta mover voo i para posição i+2 (intra pista)
            if (airport->inverterVoosConsecutivos(p, i)) {

                if (testarMelhoria(airport, custo_atual, " | Custo novo: ")) {
                    melhorou = true;
                    break;  // sai do loop se houve melhoria
                }

            } else {
                    cout << " | \033[1;33m(invalido)\033[0m\n";
            }            
        }
        if (melhorou) break;  // sai do loop se houve melhoria
    }

    cout << "\n--- FIM VIZINHANCA 1 ---\n";
    cout << "Custo final: " << airport->calcularCustoTotal() << "\n";
    return false;  // nenhuma melhoria encontrada
}

// MOVIMENTO 2: Mover um voo para outra pista
bool vizinhanca2(Airport* airport) {
    cout << "\n--- VIZINHANCA 2 INICIADA ---\n";
    cout << "Custo inicial: " << airport->custo_total << "\n\n";

    for (int p = 0; p < airport->num_pistas; p++) {
        for (size_t i = 0; i < airport->pistas[p].size() - 2; i++) {

            int id1 = airport->pistas[p][i];
            int id2 = airport->pistas[p][i+2];
            cout << "  Testando P" << p << ": V" << id1 << " → V" << id2;

            // Tenta mover voo i para posição i+2 (intra pista)
            if (airport->insertIntraPista(p, i, i+2)) {

                if (testarMelhoria(airport, airport->custo_total, " | Custo novo: ")) {
                    return true;
                }
                
            } else {
                cout << " | \033[1;33m(invalido)\033[0m\n";
            }
        }
    }
    cout << "\n--- FIM VIZINHANCA 2 ---\n";
    cout << "Custo final: " << airport->custo_total << "\n";
    return false;  // nenhuma melhoria
}

// MOVIMENTO 3: opt2 intra pista com limite de distância
bool vizinhanca3(Airport* airport) {

    const int max_gap = 5; 
    int custo_atual = airport->calcularCustoTotal();

    cout << "\n--- VIZINHANCA 3 (2-Opt com limite) INICIADA ---\n";
    cout << "Custo inicial: " << custo_atual << "\n\n";

    for (int p = 0; p < airport->num_pistas; p++) {

        int tam = airport->pistas[p].size();

        for (int i = 0; i < tam - 1; i++) {
            for (int j = i + 2; j < min(i + max_gap, tam); j++) {
                cout << "  Testando P" << p << ": invertendo [" << i << ", " << j << "]";

                // Aplicar inversão
                if (airport->opt2IntraPista(p, i, j, max_gap)) {
                    if (testarMelhoria(airport, custo_atual, " | Custo novo: ")) {
                        return true;
                    }
                } else {
                    cout << " | \033[1;33m(invalido)\033[0m\n";
                }
            }
        }
    }

    cout << "\n--- FIM VIZINHANCA 3 ---\n";
    cout << "Custo final: " << airport->custo_total << "\n";
    return false;
}

void VND(Airport* airport) {

    cout << "VND Solução inicial:\n";
    for (int p = 0; p < airport->num_pistas; p++) {
        cout << "Pista " << p << ": ";
        for (int id : airport->pistas[p]) cout << id+1 << " ";
        cout << endl;
    }

    using Vizinhanca = bool(*)(Airport*);
    vector<Vizinhanca> vizinhancas = {vizinhanca1, vizinhanca2, vizinhanca3};
    int k = 0;

    cout << "\n--- VND INICIADO ---\n";
    airport->custo_total = airport->calcularCustoTotal();  // Atualiza custo inicial
    airport->custo_melhor = airport->custo_total;          // Inicializa melhor custo
    cout << "Custo inicial: " << airport->custo_total << "\n\n";

    while (k < vizinhancas.size()) {
        cout << "Tentando vizinhanca " << k + 1 << "...\n";

        bool melhorou = vizinhancas[k](airport);

        if (melhorou) {
            cout << "Melhoria encontrada! Voltando para primeira vizinhanca.\n\n";
            k = 0;
            airport->custo_total = airport->calcularCustoTotal();  // Atualiza custo atual
        } else {
            k++;
        }
    }

    cout << "--- FIM DO VND ---\n";
    

    // Opcional: Mostra a melhor alocação
    cout << "\nMelhor alocacao encontrada:\n";
    cout << "Melhor custo final: " << airport->custo_melhor << endl;
    for (size_t p = 0; p < airport->melhor_pistas.size(); ++p) {
        cout << "Pista " << p << ": ";
        for (int id : airport->melhor_pistas[p]) {
            cout << id << " ";
        }
        cout << endl;
    }
}