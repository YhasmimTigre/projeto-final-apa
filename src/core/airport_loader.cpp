//
// Created by kamil on 05/04/2025.
//

#include "airport_loader.h"
#include <fstream>
#include <iostream>

using namespace std;

vector<Voo> voos;
vector<vector<int>> tempo_espera;
int n, m =0;

bool lerDados(const string& nome_arquivo) {
    ifstream MyReadFile(nome_arquivo);
    if (!MyReadFile) {
        cout << "Erro ao abrir o arquivo" << endl;
        return false;
    }

    MyReadFile >> n >> m;
    voos.resize(n);
    tempo_espera.resize(n, vector<int>(n));

    //r = tempos mínimos
    for (int i = 0; i < n; i++) {
        MyReadFile >> voos[i].horario_prev;
        voos[i].id = i;
    }

    //c = duração
    for (int i = 0; i < n; i++) {
        MyReadFile >> voos[i].duracao;
    }

    //p = penalidades
    for (int i = 0; i < n; i++) {
        MyReadFile >> voos[i].penalidade;
    }

    //t = matriz de tempos de espera
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            MyReadFile >> tempo_espera[i][j];
        }
    }

    MyReadFile.close();
    return true;
}

void exibirDados() {
    cout << "Numero de voos: " << n << ", numero de pistas: " << m << endl;

    cout << "\nVoos:\n";
    for (const auto& voo : voos) {
        cout << "Voo " << voo.id + 1
             << ", horario: " << voo.horario_prev
             << ", duracao: " << voo.duracao
             << ", penalidade: " << voo.penalidade << endl;
    }

    cout << "\nMatriz de tempos obrigatorios de espera:\n";
    for (const auto& linha : tempo_espera) {
        for (int t : linha) {
            cout << t << " ";
        }
        cout << endl;
    }
}