#include <iostream>
#include <fstream>
#include <ctime> 
#include "src/heart/airport.h"
#include "src/algorithms/vnd.h"
#include "src/algorithms/guloso.h"
#include "src/algorithms/ils.h"

using namespace std;

int main(int argc, char* argv[]) {

    Airport airport;  
    ofstream saida("data/output/resultados.txt");

    if (!saida) {
        cerr << "Erro ao criar arquivo de saída" << endl;
        return 1;
    }

    //guloso
    if (!airport.executarAlocacao(argv[1])){

        cerr << "Falha na alocação inicial" << endl;
    
    }

    saida << "Guloso:" << endl;
    saida << airport.bkp_custo << endl;
    for (const auto& pista : airport.pistas) {
        for (int id : pista) {
            saida << id+1 << " "; 
        }
        saida << endl;
    }

    //VND
    VND(&airport);
    saida << "\nVND:" << endl;
    saida << airport.bkp_custo << endl;
    for (const auto& pista : airport.pistas) {
        for (int id : pista) {
            saida << id+1 << " "; 
        }
        saida << endl;
    }

    //ILS
    int maxIter = 10;  // Número de iterações
    ILS(&airport, maxIter);
    saida << "\nILS com " << maxIter << " iterações:" << endl;
    saida << airport.bkp_custo << endl;
    for (const auto& pista : airport.pistas) {
        for (int id : pista) {
            saida << id+1 << " "; 
        }
        saida << endl;
    }
        
    saida.close();


    /*/ 1. Carrega dados e executa alocação inicial (Guloso)
    if (!airport.executarAlocacao(argv[1])) {

        cerr << "Falha na alocação inicial" << endl;
        return 1;
    
    }

    for (auto& v : airport.bkp_voos){

        cout << "\nVoo " << v.id << " || h_real: " << v.horario_real << " || h_prev: " << v.horario_prev
        << "|| duracao: " << v.duracao 
        << "|| multa: " << v.multa << "|| voo anterior: " << v.voo_anterior << endl;
    };

    clock_t start = clock();

    //2. Executa VND na solução inicial
    //VND(&airport);

    clock_t end = clock();
    double elapsed = end - start;
    cout << elapsed / CLOCKS_PER_SEC; << "s\n";

    for (size_t p = 0; p < airport.bkp_pistas.size(); p++) {
        cout << "\n=== Pista " << p << " ===" << endl;
        
        for (size_t pos = 0; pos < airport.bkp_pistas[p].size(); pos++) {
            int id_voo = airport.bkp_pistas[p][pos];
            
            // Encontra o voo correspondente no vetor de voos
            Voo* voo = nullptr;

            for (auto& v : airport.bkp_voos) {
                if (v.id == id_voo) {
                    voo = &v;
                    break;
                }
            }
            
            if (voo) {
                cout << "Voo " << voo->id 
                     << " | HR: " << voo->horario_real 
                     << " | HP: " << voo->horario_prev
                     << " | Dur: " << voo->duracao 
                     << " | Multa: " << voo->multa 
                     << " | Ant: " << voo->voo_anterior << endl;
            } else {
                cout << "Voo " << id_voo << " (não encontrado!)" << endl;
            }
        }
    };

    int maxIter = 1;  // Número de iterações
    ILS(&airport, maxIter);

    
    // 3. Mostra resultados
    cout << "\n=== RESULTADO FINAL ===" << endl;
    airport.mostrarSolucaoNoTerminal(airport.bkp_custo);
    airport.escreverSolucao("solucao_final.txt");*/

    return 0;
}