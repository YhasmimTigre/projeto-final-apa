#include <iostream>
#include <ctime> 
#include "src/heart/airport.h"
#include "src/algorithms/vnd.h"
#include "src/algorithms/guloso.h"
#include "src/algorithms/ils.h"

using namespace std;

int main(int argc, char* argv[]) {

    Airport airport;
    
    // 1. Carrega dados e executa alocação inicial (Guloso)
    if (!airport.executarAlocacao(argv[1])) {

        cerr << "Falha na alocação inicial" << endl;
        return 1;
    
    }

    //2. Executa VND na solução inicial
    VND(&airport);

    /*int maxIter = 1;  // Número de iterações
    //ILS(&airport, maxIter);

    // 3. Mostra resultados
    cout << "\n=== RESULTADO FINAL ===" << endl;
    airport.mostrarMelhorSolucao(airport.custo_melhor);
    airport.escreverSolucao("solucao_final.txt");*/

    return 0;
}