#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono> // Adicionar esta biblioteca para medir o tempo
#include "src/heart/airport.h"
#include "src/algorithms/vnd.h"
#include "src/algorithms/guloso.h"
#include "src/algorithms/ils.h"

using namespace std;
using namespace std::chrono; // Para usar o namespace de cronometragem

int main(int argc, char* argv[]) {

    Airport airport;  
    ofstream saida("data/output/resultados.txt");

    if (!saida) {
        cerr << "Erro ao criar arquivo de saída" << endl;
        return 1;
    }

    // Tempo total de execução
    auto inicio_total = high_resolution_clock::now();

    //guloso
    auto inicio_guloso = high_resolution_clock::now();
    
    if (!airport.executarAlocacao(argv[1])){
        cerr << "Falha na alocação inicial" << endl;
    }
    
    auto fim_guloso = high_resolution_clock::now();
    auto duracao_guloso = duration_cast<milliseconds>(fim_guloso - inicio_guloso);

    saida << "Guloso:" << endl;
    saida << "Tempo de execução: " << duracao_guloso.count() << " ms" << endl;
    saida << "Custo: " << airport.bkp_custo << endl;
    for (const auto& pista : airport.pistas) {
        for (int id : pista) {
            saida << id+1 << " "; 
        }
        saida << endl;
    }

    //VND
    auto inicio_vnd = high_resolution_clock::now();
    
    VND(&airport);
    
    auto fim_vnd = high_resolution_clock::now();
    auto duracao_vnd = duration_cast<milliseconds>(fim_vnd - inicio_vnd);

    saida << "\nVND:" << endl;
    saida << "Tempo de execução: " << duracao_vnd.count() << " ms" << endl;
    saida << "Custo: " << airport.bkp_custo << endl;
    for (const auto& pista : airport.pistas) {
        for (int id : pista) {
            saida << id+1 << " "; 
        }
        saida << endl;
    }

    //ILS
    auto inicio_ils = high_resolution_clock::now();
    
    int maxIter = 10;  // Número de iterações
    ILS(&airport, maxIter);
    
    auto fim_ils = high_resolution_clock::now();
    auto duracao_ils = duration_cast<milliseconds>(fim_ils - inicio_ils);

    saida << "\nILS com " << maxIter << " iterações:" << endl;
    saida << "Tempo de execução: " << duracao_ils.count() << " ms" << endl;
    saida << "Custo: " << airport.bkp_custo << endl;
    for (const auto& pista : airport.pistas) {
        for (int id : pista) {
            saida << id+1 << " "; 
        }
        saida << endl;
    }
    
    // Tempo total
    auto fim_total = high_resolution_clock::now();
    auto duracao_total = duration_cast<milliseconds>(fim_total - inicio_total);
    
    saida << "\nTempo total de execução: " << duracao_total.count() << " ms" << endl;
    
    saida.close();
    return 0;
}