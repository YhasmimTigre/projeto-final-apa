#include <iostream>
#include <ctime> 
#include "src/algorithms/alocador.h"
#include "src/algorithms/vnd.h"

using namespace std;

int main(int argc, char* argv[]) {

    if (!executarAlocacao(argv[1])) {
        return 1;
    }

    vizinhanca3();
    //VND();


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