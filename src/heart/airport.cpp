#include "airport.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <ctime>

#include "../algorithms/guloso.h"

using namespace std;

//ler e mostrar dados iniciais
bool Airport::lerDados(const string& nome_arquivo) {

    ifstream MyReadFile(nome_arquivo);
    if (!MyReadFile) {
        cout << "Erro ao abrir o arquivo" << endl;
        return false;
    }

    MyReadFile >> num_voos >> num_pistas;
    voos.resize(num_voos);
    tempo_espera.resize(num_voos, vector<int>(num_voos));

    //r = tempos mínimos
    for (int i = 0; i < num_voos; i++) {
        MyReadFile >> voos[i].horario_prev;
        voos[i].id = i;
    }

    //c = duração
    for (int i = 0; i < num_voos; i++) {
        MyReadFile >> voos[i].duracao;
    }

    //p = penalidades
    for (int i = 0; i < num_voos; i++) {
        MyReadFile >> voos[i].penalidade;
    }

    //t = matriz de tempos de espera
    for (int i = 0; i < num_voos; i++) {
        for (int j = 0; j < num_voos; j++) {
            MyReadFile >> tempo_espera[i][j];
        }
    }

    bkp_voos = voos;
    bkp_pistas = bkp_pistas;

    MyReadFile.close();
    return true;
}

//funções aux
void Airport::calcularMultas() {
    for (auto& voo : voos) {

        int horario = voo.horario_real - voo.horario_prev;

        if (horario < 0) {

            cout << "Atraso negativo (invalido) par ao voo: " << voo.id << endl;
            cout << "horario real: " << voo.horario_real << " || horario previsto: " << voo.horario_prev << endl;
            cout << "Nenhuma multa aplicada." << endl << endl;
            
        } else {
            voo.multa = horario * voo.penalidade;
        }
    }
}

int Airport::calcularCustoTotal() {
    int total = 0;

    for (const auto& voo : voos) {
        total += voo.multa;
    }
    
    return total;
}

void Airport::calcularBKPMultas() {
    for (auto& voo : bkp_voos) {

        int horario = voo.horario_real - voo.horario_prev;

        if (horario < 0) {

            cout << "Atraso negativo (invalido) par ao voo: " << voo.id << endl;
            cout << "horario real: " << voo.horario_real << " || horario previsto: " << voo.horario_prev << endl;
            cout << "Nenhuma multa aplicada." << endl << endl;
            
        } else {
            voo.multa = horario * voo.penalidade;
        }
    }
}

int Airport::calcularBKPCustoTotal() {
    int total = 0;

    for (const auto& voo : bkp_voos) {
        total += voo.multa;
    }
    
    return total;
}

void Airport::salvarMelhorSolucao() {

    bkp_voos = voos; 
    bkp_pistas = pistas; 
    bkp_custo = calcularCustoTotal();

}

void Airport::restaurarMelhorSolucao() {

    voos = bkp_voos;
    pistas = bkp_pistas;
    custo_total = bkp_custo;

}


//mostrar e executar guloso
void Airport::mostrarSolucaoNoTerminal(int custo_total) {
    
    cout << "Custo total: " << custo_total << endl;
    cout << "Alocacao de voos por pista:" << endl;

    for (size_t p = 0; p < pistas.size(); ++p) {
        cout << "Pista " << p+1 << ": ";

        for (int id : pistas[p]) {
            cout << id << " ";
        }
        
        cout << endl;
    }
}

void Airport::mostrarSolucaoBKPNoTerminal(int custo) {
    
    cout << "Custo total: " << custo << endl;
    cout << "Alocacao de voos por pista:" << endl;

    for (size_t p = 0; p < bkp_pistas.size(); ++p) {
        cout << "Pista " << p+1 << ": ";

        for (int id : bkp_pistas[p]) {
            cout << id << " ";
        }
        
        cout << endl;
    }
}


bool Airport::executarAlocacao(const string& arquivo_entrada) {

    if (!lerDados(arquivo_entrada)) {
        cerr << "Falha ao carregar dados.\n";
        return false;
    }
    //clock_t start = clock();
    Guloso(this);
    //clock_t start = clock();
    //double elapsed = end - start;
    //cout << "Guloso run in " << elapsed / CLOCKS_PER_SEC; << "s\n";
    calcularBKPMultas();
    bkp_custo = calcularBKPCustoTotal();
    mostrarSolucaoBKPNoTerminal(bkp_custo);
    restaurarMelhorSolucao();
    return true;
}

// No arquivo airport.cpp, adicione a implementação:
Voo* Airport::encontrarVooPorId(int id) {
    for (auto& v : voos) {  // Note: não precisa de "airport.voos" porque está dentro da classe
        if (v.id == id) {
            return &v;
        }
    }
    return nullptr;  // Retorna nullptr se não encontrar
}

//VND
//vizinhancas
bool Airport::inverterVoosConsecutivos(int pista, int posicao_voo) {
    // Verificação de validade
    if (pista < 0 || pista >= num_pistas || posicao_voo < 0 || posicao_voo >= static_cast<int>(pistas[pista].size()) - 1) {
        cerr << "Invalido: pista=" << pista << ", pos=" << posicao_voo << endl;
        return false;
    }

    // IDs dos voos antes da inversão
    int id_voo1 = pistas[pista][posicao_voo];
    int id_voo2 = pistas[pista][posicao_voo + 1];

    // Ponteiros para os voos
    Voo *voo1 = nullptr, *voo2 = nullptr;
    for (auto& v : voos) {
        if (v.id == id_voo1) voo1 = &v;
        if (v.id == id_voo2) voo2 = &v;
    }

    if (!voo1 || !voo2) {
        cerr << "Erro: Voos nao encontrados (" << id_voo1 << " ou " << id_voo2 << ")" << endl;
        return false;
    }

    /*/ DEBUG antes
    cout << "\n=== ANTES DA INVERSAO ===" << endl;

    cout << "Pista: " << pista << endl;
    for (auto& id : bkp_pistas[pista]){

        Voo* v = encontrarVooPorId(id);

        cout << "\nVoo " << v->id << " || h_real: " << v->horario_real << " || h_prev: " << v->horario_prev
        << "|| duracao: " << v->duracao 
        << "|| multa: " << v->multa << "|| voo anterior: " << v->voo_anterior << endl;
    }; */

    // Inverte as posições manualmente
    pistas[pista][posicao_voo] = id_voo2;
    pistas[pista][posicao_voo + 1] = id_voo1;

    // Atualiza voo_anterior e horario_real de voo2 (agora na frente)
    if (posicao_voo == 0) {
        voo2->voo_anterior = -1;
        voo2->horario_real = voo2->horario_prev;
    } else {
        int id_voo_ant = pistas[pista][posicao_voo - 1];
        Voo* voo_ant = encontrarVooPorId(id_voo_ant);
        voo2->voo_anterior = voo_ant->id;
        voo2->horario_real = max(voo2->horario_prev, voo_ant->horario_real + voo_ant->duracao + tempo_espera[voo_ant->id][voo2->id]);
    }

    // Atualiza voo_anterior e horario_real de voo1 (agora atrás)
    voo1->voo_anterior = voo2->id;
    cout << "voo2->id: " << voo2->id << endl;
    voo1->horario_real = max(voo1->horario_prev, voo2->horario_real + voo2->duracao + tempo_espera[voo1->id][voo2->id]);

    // Atualizando os voos após a posição do voo2
    for (size_t k = posicao_voo + 2; k < pistas[pista].size(); ++k) {
        int id_atual = pistas[pista][k];
        int id_anterior = pistas[pista][k - 1];

        Voo* voo_atual = encontrarVooPorId(id_atual);
        Voo* voo_ant = encontrarVooPorId(id_anterior);
        //Voo* voo_atual = &voos[id_atual];
        //Voo* voo_ant = &voos[id_anterior];

        voo_atual->voo_anterior = voo_ant->id;
        voo_atual->horario_real = max(voo_atual->horario_prev, voo_ant->horario_real + voo_ant->duracao + tempo_espera[voo_ant->id][voo_atual->id]);
    }
    calcularMultas();
    /*/ DEBUG depois
    cout << "=== APOS INVERSAO ===" << endl;
    
    cout << "Pista: " << pista << endl;
    for (auto& id : bkp_pistas[pista]){

        Voo* v = encontrarVooPorId(id);

        cout << "\nVoo " << v->id << " || h_real: " << v->horario_real << " || h_prev: " << v->horario_prev
        << "|| duracao: " << v->duracao 
        << "|| multa: " << v->multa << "|| voo anterior: " << v->voo_anterior << endl;
    };*/

    return true;
}



bool Airport::insertIntraPista(int pista, int origem, int destino){

    /*/ DEBUG antes
    cout << "\n=== ANTES DA INVERSAO ===" << endl;
     
    cout << "Pista: " << pista << endl;
    for (auto& id : bkp_pistas[pista]){

        Voo* v = encontrarVooPorId(id);

        cout << "\nVoo " << v->id << " || h_real: " << v->horario_real << " || h_prev: " << v->horario_prev
        << "|| duracao: " << v->duracao 
        << "|| multa: " << v->multa << "|| voo anterior: " << v->voo_anterior << endl;
    };*/

    //verificação de movimentos invalidos ou irrelevantes
    if (pista < 0 || pista >= num_pistas ||
        origem < 0 || destino < 0 ||
        origem >= static_cast<int>(pistas[pista].size()) ||
        destino > static_cast<int>(pistas[pista].size()) ||
        origem == destino || abs(origem - destino) == 1) {

        cerr << "Invalido: pista = " << pista << ", origem = " << origem << ", destino = " << destino << endl;
        return false;
    }

    //coloca voo origem no destino
    int id_voo_origem = pistas[pista][origem];
    //int id_voo_destino = pistas[pista][destino];

    for (int i = origem; i < destino; i++){
        pistas[pista][i] = pistas[pista][i+1];
    }

    pistas[pista][destino] = id_voo_origem;


    for (size_t k = 0; k < pistas[pista].size(); ++k) {

        Voo* voo_atual = encontrarVooPorId(pistas[pista][k]);
        Voo* voo_anterior = encontrarVooPorId(pistas[pista][k-1]);

        if (k == 0){
            voo_atual->voo_anterior = -1;
            voo_atual->horario_real = voo_atual->horario_prev;
        } else {

            voo_atual->voo_anterior = voo_anterior->id;
            voo_atual->horario_real = max(voo_atual->horario_prev, 
                voo_anterior->horario_real + voo_anterior->duracao + tempo_espera[voo_anterior->id][voo_atual->id]);
              
        }
    }

    calcularMultas();

    /*/ Debug: mostra voos após inserção
    cout << "=== APOS INVERSAO ===" << endl;
    
    cout << "Pista: " << pista << endl;
    for (auto& id : bkp_pistas[pista]){

        Voo* v = encontrarVooPorId(id);

        cout << "\nVoo " << v->id << " || h_real: " << v->horario_real << " || h_prev: " << v->horario_prev
        << "|| duracao: " << v->duracao 
        << "|| multa: " << v->multa << "|| voo anterior: " << v->voo_anterior << endl;
    };*/

    for (int id : bkp_pistas[pista]) cout << id << " ";
    cout << endl << endl;
    
    return true;
}

bool Airport::opt2IntraPista(int pista, int i, int j, int max_gap) {

    // Verificação completa
    if (pista < 0 || pista >= num_pistas || i < 0 || j <= i ||
        j >= static_cast<int>(pistas[pista].size()) || (j - i) > max_gap) {
        cerr << "Invalido: pista=" << pista << ", i=" << i << ", j=" << j << ", max_gap=" << max_gap << endl;
        return false;
    }

    /*/ Debug: mostra voos antes da operação OPT-2
    cout << "\n=== ANTES DA OPERACAO OPT-2 ===" << endl;
    cout << "Pista: " << pista << endl;
    for (auto& id : bkp_pistas[pista]){

        Voo* v = encontrarVooPorId(id);

        cout << "\nVoo " << v->id << " || h_real: " << v->horario_real << " || h_prev: " << v->horario_prev
        << "|| duracao: " << v->duracao 
        << "|| multa: " << v->multa << "|| voo anterior: " << v->voo_anterior << endl;
    };*/

    int aux = i;
    int aux_j = j;

    while (aux < aux_j){

        int aux_pista = pistas[pista][aux]; 
        pistas[pista][aux] = pistas[pista][aux_j];
        pistas[pista][aux_j] = aux_pista;

        aux++;
        aux_j--;

    }

    //atualiza os babados
    for (size_t k = i; k < pistas[pista].size(); ++k) {

        Voo* voo_atual = encontrarVooPorId(pistas[pista][k]);
        Voo* voo_anterior = encontrarVooPorId(pistas[pista][k-1]);

        if (k == 0){

            voo_atual->voo_anterior = -1;
            voo_atual->horario_real = voo_atual->horario_prev;

        } else {

            voo_atual->voo_anterior = voo_anterior->id;
            voo_atual->horario_real = max(voo_atual->horario_prev, 
                voo_anterior->horario_real + voo_anterior->duracao + tempo_espera[voo_anterior->id][voo_atual->id]);
              
        }
    }

    calcularMultas();
    
    /*/ Debug: mostra voos após a operação OPT-2
    cout << "\n=== APOS INVERSAO ===" << endl;
    
    cout << "Pista: " << pista << endl;
    for (auto& id : bkp_pistas[pista]){

        Voo* v = encontrarVooPorId(id);

        cout << "\nVoo " << v->id << " || h_real: " << v->horario_real << " || h_prev: " << v->horario_prev
        << "|| duracao: " << v->duracao 
        << "|| multa: " << v->multa << "|| voo anterior: " << v->voo_anterior << endl;
    };*/
    
    return true;
}