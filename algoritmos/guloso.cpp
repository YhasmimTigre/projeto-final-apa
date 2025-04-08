#include <iostream>
#include <vector>
#include <algorithm>
#include "funcoes/dados.h"

using namespace std;

// Função de comparação para ordenar voos pelo tempo de liberação
bool compararPorLiberacao(const Voo &a, const Voo &b) {
    return a.horario < b.horario;
}

void atualizarVooEPista(vector<Voo> &voos, vector<int> &pistas, 
    const vector<vector<int>> &tempo_espera, 
    int indice_voo, int pista_id) {// i = indice do voo

    if(indice_voo <= 0 || indice_voo >= voos.size()) return;    

    int tempo = tempo_espera[voos[indice_voo-1].id][voos[indice_voo].id];

    // Atualiza horário real do voo
    voos[indice_voo].horario_real = 
    max(voos[indice_voo-1].horario_real + voos[indice_voo-1].duracao + tempo,
        voos[indice_voo].horario_prev);

    // Calcula multa por atraso

    voos[indice_voo].multa = 
        voos[indice_voo].penalidade * 
        max(0, voos[indice_voo].horario_real - voos[indice_voo].horario_prev);


    // Atualiza disponibilidade da pista
    pistas[pista_id] = voos[indice_voo].horario_real + voos[indice_voo].duracao;
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
