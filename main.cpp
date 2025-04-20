#include <iostream>
#include <ctime> 
#include "src/heart/airport.h"
#include "src/algorithms/vnd.h"
#include "src/algorithms/guloso.h"
#include "src/algorithms/ils.h"

using namespace std;

int main(int argc, char* argv[]) {

    Airport airport;

    if (!airport.executarAlocacao(argv[1])) {
        
        cerr << "Falha na alocação inicial" << endl;
        return 1;
    
    }
    
    // 1. Carrega dados e executa alocação inicial (Guloso)
    if (!airport.executarAlocacao(argv[1])) {
        cerr << "Falha na alocação inicial" << endl;
        return 1;
    }

    // 2. Executa VND na solução inicial
    VND(&airport);
    
    /*// 3. Executa ILS (com número padrão de iterações ou valor fornecido)
    int iteracoes_ILS = (argc > 2) ? stoi(argv[2]) : 10; // Valor padrão: 50
    vector<Voo> melhorSolucao = ILS(&airport, iteracoes_ILS);
    
    // 4. Atualiza o airport com a melhor solução encontrada
    airport.voos = melhorSolucao;
    airport.calcularMultas();
    
    // 5. Exibe e salva resultados
    cout << "\n==== MELHOR SOLUÇÃO ENCONTRADA ====\n";
    airport.mostrarSolucaoNoTerminal(airport.calcularCustoTotal());
    airport.escreverSolucao("solucao_ils.txt");*/

    return 0;


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