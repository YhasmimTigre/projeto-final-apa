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

void calcularMultas() {
    for (auto& voo : voos) {
        int atraso = max(0, voo.horario_real - voo.horario_prev);
        voo.multa = atraso * voo.penalidade;
    }
}

int calcularCustoTotal() {
    int total = 0;
    for (const auto& voo : voos) {
        total += voo.multa;
    }
    return total;
}

void escreverSolucao(const string& nome_arquivo, int custo_total) {
    ofstream saida(nome_arquivo);
    if (!saida) {
        cerr << "Erro ao criar arquivo de saída" << endl;
        return;
    }

    saida << custo_total << endl;

    // Agrupa voos por pista
    vector<vector<int>> voos_por_pista(m);
    for (const auto& voo : voos) {
        if (voo.pista_alocada >= 0 && voo.pista_alocada < m) {
            voos_por_pista[voo.pista_alocada].push_back(voo.id + 1); // +1 para IDs começarem em 1
        }
    }

    // Escreve voos por pista
    for (const auto& pista : voos_por_pista) {
        for (int id : pista) {
            saida << id << " ";
        }
        saida << endl;
    }

    saida.close();
}

//o GAP representa a diferença percentual entre a solução heurística (guloso) e a ótima (VND)
double calcularGAP(int valorHeuristica, int valorOtimo) {
    double gap = ( (static_cast<double>(valor) - valorOtimo) / valorOtimo ) * 100.0;
    return gap;Heuristica
}



