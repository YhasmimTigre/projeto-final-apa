#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Voo{
    int horario;
    int penalidade;
};

string myText;


int main(int argc, char* argv[]){

    if (argc < 2){
        cout << "Uso: " << argv[0] << " <arquivo>" << endl;
        return 1;
    }
    
    ifstream MyReadFile(argv[1]);

    if(!MyReadFile){
        cout << "Erro ao abrir o arquivo" << endl;
        return 1;
    }
    
    while(getline (MyReadFile, myText)){
        cout << myText << endl;
    }

    MyReadFile.close();

    return 0;

}

