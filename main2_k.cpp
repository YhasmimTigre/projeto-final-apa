#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Voo{
    int id_voo, horario, duracao, penalidade;
};

//a matriz
vector<vector<int>> tempos_espera;

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

    int n,m; //num voo e pistas
    arquivo >> n >> m;

    vector<Voo> voos(n);

    //tempos minimos r
    for (int i = 0; i < n; i++){
      arquivo >> voos[i].horario;
      voos[i].id = i;
    }

    //duração c
    for (int i = 0; i < n; i++){
      arquivo >> voos[i].duracao;
    }

    //penalidades p
    for (int i = 0; i < n; i++){
      arquivo >> voos[i].penalidade;
    }

    //matriz tempos de espera t
    tempo_espera.resize(n, vector<int>(n));
    for (int i = 0; i < n; i++){
      for  (j = 0; j < n; j++){
        arquivo >> tempos_espera[i][j];
      }
    }

    MyReadFile.close();

    //dados lidos
    cout << "Número de voos: " << n << ", número de pistas: " << m << endl;

    cout << "\n Voos:\n";
    for (const auto& voo : voos){
      cout << "voo " << voo.id
           << ", horario " << voo.horario
           << ", duração: " << voo.duracao
           << "penalidade: " << voo.penalidade << endl;
    }

    cout << "\n Matriz de tempos obgts de espera:\n";
    for (const auto& linha : tempo_espera){
      for (int t:linha){
        cout << t << " ";
    }
    cout << endl;
   }

    return 0;
}

