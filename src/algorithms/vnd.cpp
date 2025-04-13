#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include "../core/airport_loader.h"
#include "guloso.h"
#include "alocador.h"

using namespace std;

// MOVIMENTO 1: Trocar dois voos entre pistas diferentes
bool vizinhanca1() {
    vector<Voo> backup = voos; // Salva a solução atual
    // Percorre pares de voos
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            // Se os voos estão em pistas diferentes, tenta trocá-los
            if (voos[i].pista_alocada != voos[j].pista_alocada) {
                swap(voos[i], voos[j]);
                int novo_custo = calcularCustoTotal();
                if (novo_custo < custo_total) {
                    return true; // Movimento aceito
                }
                // Se não melhorar, restaura a solução
                voos = backup;
            }
        }
    }
    return false; // Nenhuma melhoria encontrada nesta vizinhança
}

// MOVIMENTO 2: Mover um voo de sua pista atual para outra pista
bool vizinhanca2() {
    vector<Voo> backup = voos; // Salva a solução atual
    for (int i = 0; i < n; i++) {
        int pista_atual = voos[i].pista_alocada;
        // Tenta mover o voo para cada outra pista
        for (int nova_pista = 0; nova_pista < m; nova_pista++) {
            if (nova_pista == pista_atual)
                continue;
            voos[i].pista_alocada = nova_pista;
            // Em uma implementação completa, aqui poderíamos ajustar também o campo 'voo_anterior' se necessário.
            int novo_custo = calcularCustoTotal();
            if (novo_custo < custo_total) {
                return true;
            }
            voos = backup;
        }
    }
    return false;
}

// MOVIMENTO 3: Inverter a ordem de dois voos que estão na mesma pista
bool vizinhanca3() {
    vector<Voo> backup = voos; // Salva a solução atual
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            // Verifica se ambos estão alocados na mesma pista
            if (voos[i].pista_alocada == voos[j].pista_alocada) {
                // Inverte a posição dos dois voos (troca todos os campos)
                swap(voos[i], voos[j]);
                int novo_custo = calcularCustoTotal();
                if (novo_custo < custo_total)
                    return true;
                voos = backup;
            }
        }
    }
    return false;
}

// VND principal
void VND() {
    int k = 1;
    const int k_max = 3;

    while (k <= k_max) {
        bool melhorou = false;
        if (k == 1) melhorou = vizinhanca1();
        else if (k == 2) melhorou = vizinhanca2();
        else if (k == 3) melhorou = vizinhanca3();

        if (melhorou){
            k = 1;
            custo_total = calcularCustoTotal();
            cout << "Nova melhoria encontrada, custo = " << custo_total << endl;
        }
        else{++k;}

    }

    // Exibe a solução final
    cout << "\n--- Solucao Final ---\n";
    cout << "Custo total (soma das multas): " << custo_total << "\n";
    for (int pista = 0; pista < m; pista++) {
        cout << "Pista " << pista + 1 << ": ";
        // Exibe os voos alocados nesta pista (baseado na atribuição atual)
        for (int i = 0; i < n; i++) {
            if (voos[i].pista_alocada == pista) {
                cout << "V" << voos[i].id + 1 << " ";
            }
        }
        cout << "\n";
    }
}
