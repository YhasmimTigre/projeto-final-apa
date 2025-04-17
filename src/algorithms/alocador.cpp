#include <iostream>
#include "alocador.h"
#include "../core/airport_loader.h"
#include "guloso.h"  

using namespace std;

int custo_total = 0;

void mostrarSolucaoNoTerminal(int custo_total) {
    cout << "Custo total: " << custo_total << endl;
    cout << "Alocacao de voos por pista:" << endl;


    // Imprime voos por pista
    for (size_t p = 0; p < pistas.size(); ++p) {
        cout << "Pista " << p+1 << ": ";
        for (int id : pistas[p]) {
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
