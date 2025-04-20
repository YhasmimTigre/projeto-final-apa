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
    
    /*// 3. ILS
    int iteracoes_ILS = 2;
    cout << "\n=== EXECUTANDO ILS COM " << iteracoes_ILS << " ITERAÇÕES ===" << endl;
    vector<Voo> melhor_solucao = ILS(&airport, iteracoes_ILS);

    // 4. Resultados finais
    cout << "\n=== RESULTADOS FINAIS ===" << endl;
    airport.voos = melhor_solucao;  // Atualiza com a melhor solução encontrada
    airport.calcularMultas();
    int custo_final = airport.calcularCustoTotal();
    
    cout << "Melhor custo encontrado: " << custo_final << endl;
    airport.mostrarSolucaoNoTerminal(custo_final);*/
    
    // 5. Salva eno arquivo saida
    //airport.escreverSolucao("solucao_final.txt");


    //PARTE DO CLOCK
    /*
    //executa o guloso e calcula tempo médio de execução
    clock_t start = clock();
    if (!executarAlocacao(argv[1])) {
        return 1;
    }

    clock_t end = clock();
    double elapseG = end - start;

    //executa o VND e calcula tempo médio de execução
    clock_t start = clock();
    VND();
    clock_t end = clock();
    double elapseVND = end - start;*/

    return 0;
}