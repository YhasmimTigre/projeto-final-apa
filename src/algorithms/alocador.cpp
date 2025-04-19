#include <iostream>
#include "alocador.h"
#include "../core/airport_loader.h"
#include "guloso.h"  

using namespace std;

int custo_total = 0;

//vizinhancas

bool inverterVoosConsecutivos(int pista, int posicao_voo) { // Mov. Swap
    // Verificação inicial
    if (pista < 0 || pista >= m || posicao_voo < 0 || posicao_voo >= static_cast<int>(pistas[pista].size()) - 1) {
        cerr << "Inválido: pista=" << pista << ", pos=" << posicao_voo << endl;
        return false;
    }

    int id_atual = pistas[pista][posicao_voo];
    int id_proximo = pistas[pista][posicao_voo + 1];

    // Debug: mostra voos antes da inversão
    cout << "\n=== ANTES DA INVERSÃO ===" << endl;
    cout << "Pista " << pista << " - Posições " << posicao_voo << " e " << posicao_voo+1 << endl;
    cout << "Voo " << id_atual << ": HR=" << voos[id_atual].horario_real 
         << ", Dur=" << voos[id_atual].duracao 
         << ", Ant=" << voos[id_atual].voo_anterior << endl;
    cout << "Voo " << id_proximo << ": HR=" << voos[id_proximo].horario_real 
         << ", Dur=" << voos[id_proximo].duracao 
         << ", Ant=" << voos[id_proximo].voo_anterior << endl;
    cout << "Tempo espera entre " << id_atual << "→" << id_proximo << ": " 
         << tempo_espera[id_atual][id_proximo] << endl;

    // Localiza os voos
    Voo *voo_atual = nullptr, *voo_proximo = nullptr;
    for (auto& v : voos) {
        if (v.id == id_atual) voo_atual = &v;
        if (v.id == id_proximo) voo_proximo = &v;
    }

    if (!voo_atual || !voo_proximo) {
        cerr << "Erro: Voos não encontrados (" << id_atual << " ou " << id_proximo << ")" << endl;
        return false;
    }

    // Calcula novo horário
    int novo_horario_proximo = voo_atual->horario_real + voo_atual->duracao + tempo_espera[id_atual][id_proximo];
    cout << "Novo horário para Voo " << id_atual << ": " << novo_horario_proximo << endl;

    // Executa a inversão
    swap(pistas[pista][posicao_voo], pistas[pista][posicao_voo + 1]);
    
    voo_proximo->horario_real = voo_atual->horario_real;
    voo_atual->horario_real = novo_horario_proximo;
    
    voo_atual->voo_anterior = voo_proximo->id;
    if (posicao_voo > 0) {
        voo_proximo->voo_anterior = pistas[pista][posicao_voo - 1];
    } else {
        voo_proximo->voo_anterior = -1;
    }

    // Debug: mostra voos após inversão
    cout << "=== APÓS INVERSÃO ===" << endl;
    cout << "Voo " << id_atual << ": HR=" << voo_atual->horario_real 
         << ", Ant=" << voo_atual->voo_anterior << endl;
    cout << "Voo " << id_proximo << ": HR=" << voo_proximo->horario_real 
         << ", Ant=" << voo_proximo->voo_anterior << endl;
    cout << "Ordem na pista: ";
    for (int id : pistas[pista]) cout << id << " ";
    cout << endl << endl;

    calcularMultas();
    return true;
}

bool insertIntraPista(int pista, int i, int j){
    if (pista < 0 || pista >= m || i < 0 || j < 0 || i >= static_cast<int>(pistas[pista].size()) || j >= static_cast<int>(pistas[pista].size())) {
        cerr << "Inválido: pista=" << pista << ", i=" << i << ", j=" << j << endl;
        return false;
    }
    if (i == j) return false; // Não faz sentido mover para a mesma posição
    //if (i > j) j--; // Garante que i < j

    int id_voo = pistas[pista][i];

    pistas[pista].erase(pistas[pista].begin() + i); // Remove o voo da posição i
    pistas[pista].insert(pistas[pista].begin() + j, id_voo); // Insere na posição j
    // Atualiza o voo
    voos[id_voo].pista_alocada = pista;
    voos[id_voo].horario_real = 0; // Resetando o horário real
    for (int k = 0; k < pistas[pista].size(); ++k) {
        int id = pistas[pista][k];
        if (k == 0) {
            voos[id].horario_real = max(voos[id].horario_prev, 0);
        } else {
            int tempo_min = voos[pistas[pista][k-1]].horario_real + voos[pistas[pista][k-1]].duracao + tempo_espera[pistas[pista][k-1]][id];
            voos[id].horario_real = max(tempo_min, voos[id].horario_prev);
        }
    }
    // Atualiza o voo anterior
    if (i > 0) {
        voos[id_voo].voo_anterior = pistas[pista][i - 1];
    } else {
        voos[id_voo].voo_anterior = -1;
    }
    if (j < static_cast<int>(pistas[pista].size()) - 1) {
        voos[id_voo].voo_anterior = pistas[pista][j + 1];
    } else {
        voos[id_voo].voo_anterior = -1;
    }
    // Debug: mostra voos após inserção
    cout << "=== APÓS INSERÇÃO ===" << endl;
    cout << "Voo " << id_voo << ": HR=" << voos[id_voo].horario_real 
         << ", Ant=" << voos[id_voo].voo_anterior << endl;
    cout << "Ordem na pista: ";
    for (int id : pistas[pista]) cout << id << " ";
    cout << endl << endl;
    calcularMultas();
    return true;


}






//mostrar e executar guloso
void mostrarSolucaoNoTerminal(int custo_total) {
    cout << "Custo total: " << custo_total << endl;
    cout << "Alocacao de voos por pista:" << endl;

    for (size_t p = 0; p < pistas.size(); ++p) {
        cout << "Pista " << p+1 << ": ";
        for (int id : pistas[p]) {
            cout << id+1 << " ";
        }
        cout << endl;
    }
}

bool executarAlocacao(const string& arquivo_entrada) {
    if (!lerDados(arquivo_entrada)) {
        cerr << "Falha ao carregar dados.\n";
        return false;
    }

    Guloso();
    calcularMultas();
    custo_total = calcularCustoTotal();
    mostrarSolucaoNoTerminal(custo_total);
    return true;
}
