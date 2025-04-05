#include <iostream>
#include <vector>
#include <algorithm>
#include "funcoes/dados.h"

using namespace std;

// Função de comparação para ordenar voos pelo tempo de liberação
bool compararPorLiberacao(const Voo &a, const Voo &b) {
    return a.horario < b.horario;
}

// Algoritmo guloso para alocar voos nas pistas
void alocarVoos(vector<Voo> &voos, int m) {
    // Ordena os voos pelo tempo de liberação (horario)

    //perguntar se pode usar
    sort(voos.begin(), voos.end(), compararPorLiberacao);

    // Vetor para armazenar o tempo disponível em cada pista
    vector<int> pistas(m, 0);

    // Alocar voos
    for (Voo &voo : voos) {
        // Encontrar a primeira pista disponível
        for (int i = 0; i < m; i++) {
            if (pistas[i] <= voo.horario) { // Se a pista estiver livre no tempo horario
                cout << "Voo " << voo.id << " alocado na pista " << i + 1
                     << " no tempo " << max(pistas[i], voo.horario) << endl;

                // Atualiza o tempo de disponibilidade da pista
                pistas[i] = max(pistas[i], voo.horario) + voo.duracao;
                break;
            }
        }
    }
}
