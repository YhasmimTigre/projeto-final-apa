#include <iostream>
#include <vector>
#include <algorithm>

#include "funcoes/dados.h"

dados.LerDados

// Função de comparação para ordenar voos pelo tempo de liberação
bool compararPorLiberacao(const Voo &a, const Voo &b) {
    return a.r_i < b.r_i;
}

// Algoritmo guloso para alocar voos nas pistas
void alocarVoos(vector<Voo> &voos, int numPistas) {
    // Ordena os voos pelo tempo de liberação (r_i)
    sort(voos.begin(), voos.end(), compararPorLiberacao);

    // Vetor para armazenar o tempo disponível em cada pista
    vector<int> pistas(numPistas, 0);

    // Alocar voos
    for (Voo &voo : voos) {
        // Encontrar a primeira pista disponível
        for (int i = 0; i < numPistas; i++) {
            if (pistas[i] <= voo.r_i) { // Se a pista estiver livre no tempo r_i
                cout << "Voo " << voo.id << " alocado na pista " << i + 1 
                     << " no tempo " << max(pistas[i], voo.r_i) << endl;

                // Atualiza o tempo de disponibilidade da pista
                pistas[i] = max(pistas[i], voo.r_i) + voo.duracao;
                break;
            }
        }
    }
}
