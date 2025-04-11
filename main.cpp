#include <iostream>
#include "src/algorithms/alocador.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_entrada>\n";
        return 1;
    }

    if (!executarAlocacao(argv[1])) {
        return 1;
    }
    return 0;
}