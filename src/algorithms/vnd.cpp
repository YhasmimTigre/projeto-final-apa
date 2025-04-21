#include <iostream>
#include <vector>
#include <algorithm>
#include "../heart/airport.h"
using namespace std;

// aux local
static bool testarMelhoria(Airport* airport, const string& mensagem) {

    int novo_custo = airport->calcularCustoTotal();
    
    if (novo_custo < airport->bkp_custo) {
    
        airport->salvarMelhorSolucao();
        return true;
    
    } else {

        airport->restaurarMelhorSolucao();
        return false;
    
    }
}

// MOVIMENTO 1: Inverter dois voos consecutivos na mesma pista
bool vizinhanca1(Airport* airport) {

    bool melhorou = false;

    for (int p = 0; p < airport->num_pistas; p++) {

        for (size_t i = 0; i < airport->pistas[p].size() - 1; i++) {

            // Tenta mover voo i para posição i+1 (intra pista)
            if (airport->inverterVoosConsecutivos(p, i)) {                

                if (testarMelhoria(airport, " | Custo novo: ")) {
                    melhorou = true;
                    break; 
                }      
            }     
        }
        if (melhorou) break; 
    }

    return false;  
}

// MOVIMENTO 2: Mover um voo para duas casas a frente
bool vizinhanca2(Airport* airport) {
 
    for (int p = 0; p < airport->num_pistas; p++) {
        for (size_t i = 0; i < airport->pistas[p].size() - 2; i++) {

            // Tenta mover voo i para posição i+2 (intra pista)
            if (airport->insertIntraPista(p, i, i+2)) {

                if (testarMelhoria(airport, " | Custo novo: ")) {
                    return true;
                }
            }
        }
    }
    return false;  // nenhuma melhoria
}

// MOVIMENTO 3: opt2 intra pista com limite de distância
bool vizinhanca3(Airport* airport) {

    const int max_gap = 5; 

    for (int p = 0; p < airport->num_pistas; p++) {

        int tam = airport->pistas[p].size();

        for (int i = 0; i < tam - 1; i++) {
            for (int j = i + 2; j < min(i + max_gap, tam); j++) {

                // Aplicar inversão
                if (airport->opt2IntraPista(p, i, j, max_gap)) {
                    if (testarMelhoria(airport, " | Custo novo: ")) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void VND(Airport* airport) {
    
    using Vizinhanca = bool(*)(Airport*);
    vector<Vizinhanca> vizinhancas = {vizinhanca1, vizinhanca2, vizinhanca3};
    size_t k = 0;

    while (k < vizinhancas.size()) {

        bool melhorou = vizinhancas[k](airport);

        if (melhorou) {
            k = 0;
            airport->bkp_custo = airport->calcularBKPCustoTotal();  // Atualiza melhor custo

        } else {
            k++;
        }
    }
}