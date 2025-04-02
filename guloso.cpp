#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Estrutura para armazenar informações do voo
struct Voo {
    int id;       // Identificador do voo
    int r_i;      // Tempo de liberação
    int duracao;  // Tempo necessário na pista (ex: taxiamento/pouso/decolagem)
};

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

// Função principal
int main() {
    // Lista de voos (id, tempo de liberação, duração na pista)
    vector<Voo> voos = {
        {1, 10, 3},
        {2, 5, 2},
        {3, 8, 1},
        {4, 3, 2}
    };

    int numPistas = 2; // Número de pistas disponíveis

    alocarVoos(voos, numPistas);

    return 0;
}
