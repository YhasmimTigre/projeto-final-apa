#include <iostream>
#include <ctime> 
#include "src/heart/airport.h"
#include "src/algorithms/vnd.h"
#include "src/algorithms/guloso.h"

using namespace std;

int main(int argc, char* argv[]) {

    Airport airport;

    if (!airport.executarAlocacao(argv[1])) {
        
        cerr << "Falha na alocação inicial" << endl;
        return 1;
    
    }

    VND(&airport);


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