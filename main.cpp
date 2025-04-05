#include <iostream>
#include "funcoes/dados.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <arquivo>" << endl;
        return 1;
    }

    if (!lerDados(argv[1])) {
        return 1;
    }

    alocarVoos(voos, m);

    return 0;
}
