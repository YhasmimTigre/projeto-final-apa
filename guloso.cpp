#include <iostream>
#include <vector>
#include <algorithm>
#include "funcoes/dados.h"

using namespace std;

// Função de comparação para ordenar voos pelo tempo de liberação
bool compararPorLiberacao(const Voo &a, const Voo &b) {
    return a.horario < b.horario;
}

void atualizarVooePista(vector<Voo> &voos, int i, int tempo) { // i = indice do voo
    tempo = tempo_espera[i-1][i];


    voo[i].horario_real = voo[i-1].horario_real + voo[i-1].duracao + tempo;
    voo[i].multa = voo[i].penalidade * (voo[i].horario_real - voo[i].horario_prev);


    // Atualiza o horario de disponibilidade da pista
    pistas[i] = max(pistas[i], voo[i].horario_real) + voo[i].duracao + tempo;
}


// Algoritmo guloso para alocar voos nas pistas
void alocarVoos(vector<Voo> &voos, int m) {
    // Ordena os voos pelo tempo de liberação (horario)

    //perguntar se pode usar
    sort(voos.begin(), voos.end(), compararPorLiberacao);

    // Vetor para armazenar o horario disponível em cada pista
    vector<int> pistas(m, 0);

    // Alocar voos
    for (Voo &voo : voos) {
        // Encontrar a primeira pista disponível
        for (int i = 0; i < m; i++) {
            if (pistas[i] <= voo.horario_real) { // tem que ver a pista com a menor diferenca e se o valor do horario_real é maior que o horario disponivel
                cout << "Voo " << voo.id << " alocado na pista " << i + 1
                     << " no tempo " << max(pistas[i], voo.horario_real) << endl;

                
                
                break;
            }
        }
    }
}
