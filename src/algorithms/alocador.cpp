#include <iostream>
#include "alocador.h"
#include "../core/airport_loader.h"
#include "guloso.h"  

using namespace std;

int custo_total = 0;

void mostrarSolucaoNoTerminal(int custo_total) {
    cout << "Custo total: " << custo_total << endl;
    cout << "Alocacao de voos por pista:" << endl;

    // Agrupa voos por pista
    vector<vector<int>> voos_por_pista(m);
    for (const auto& voo : voos) {
        if (voo.pista_alocada >= 0 && voo.pista_alocada < m) {
            voos_por_pista[voo.pista_alocada].push_back(voo.id + 1);
        }
    }

    // Imprime voos por pista
    for (size_t p = 0; p < voos_por_pista.size(); ++p) {
        cout << "Pista " << p+1 << ": ";
        for (int id : voos_por_pista[p]) {
            cout << id << " ";
        }
        cout << endl;
    }
}

bool executarAlocacao(const string& arquivo_entrada) {
    if (!lerDados(arquivo_entrada)) {
        cerr << "Falha ao carregar dados.\n";
        return false;
    }

    Guloso();
    calcularMultas();
    custo_total = calcularCustoTotal();
    mostrarSolucaoNoTerminal(custo_total);
    return true;
}
