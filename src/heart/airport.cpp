#include "airport.h"
#include <fstream>
#include <iostream>
#include <algorithm>
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

    MyReadFile.close();
    return true;
}

void Airport::escreverSolucao(const string& nome_arquivo) {

    // Cria arquivo de saída
    ofstream saida(nome_arquivo);
    if (!saida) {
        cerr << "Erro ao criar arquivo de saída" << endl;
        return;
    }

    calcularMultas(); // Atualiza multas antes de calcular o custo total 
    saida << calcularCustoTotal() << endl;

    // Agrupa voos por pista
    vector<vector<int>> voos_por_pista(num_pistas);

    for (const auto& voo : voos) {
        if (voo.pista_alocada >= 0 && voo.pista_alocada < num_pistas) {
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

//funções aux
void Airport::calcularMultas() {
    for (auto& voo : voos) {

        int horario = voo.horario_real - voo.horario_prev;

        if (horario < 0) {

            cout << "Atraso negativo (invalido) par ao voo: " << voo.id+1 << endl;
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

void Airport::salvarMelhorSolucao() {
    melhor_voos = voos; 
    melhor_pistas = pistas; 
    custo_melhor = calcularCustoTotal();
}

void Airport::restaurarMelhorSolucao() {
    if (!melhor_voos.empty()) {
        voos = melhor_voos;
        pistas = melhor_pistas;
        custo_total = custo_melhor;
    }
}


//mostrar e executar guloso
void Airport::mostrarSolucaoNoTerminal(int custo_total) {
    
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

bool Airport::executarAlocacao(const string& arquivo_entrada) {

    if (!lerDados(arquivo_entrada)) {
        cerr << "Falha ao carregar dados.\n";
        return false;
    }
     
    Guloso(this);
    calcularMultas();
    custo_total = calcularCustoTotal();
    mostrarSolucaoNoTerminal(custo_total);
    return true;
}


//VND
//vizinhancas
bool Airport::inverterVoosConsecutivos(int pista, int posicao_voo) { // Mov. Swap

    // Verificação inicial
    if (pista < 0 || pista >= num_pistas || posicao_voo < 0 || posicao_voo >= static_cast<int>(pistas[pista].size()) - 1) {
        cerr << "Invalido: pista=" << pista << ", pos=" << posicao_voo << endl;
        return false;
    }

    int id_atual = pistas[pista][posicao_voo];
    int id_proximo = pistas[pista][posicao_voo + 1];

    // Debug: mostra voos antes da inversão
    cout << "\n=== ANTES DA INVERSAO ===" << endl;
    cout << "Pista " << pista << " - Posicoes " << posicao_voo << " e " << posicao_voo+1 << endl;
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
        cerr << "Erro: Voos nao encontrados (" << id_atual << " ou " << id_proximo << ")" << endl;
        return false;
    }

    // Calcula novo horário
    int novo_horario_proximo = voo_atual->horario_real + voo_atual->duracao + tempo_espera[id_atual][id_proximo];
    cout << "Novo horario para Voo " << id_atual << ": " << novo_horario_proximo << endl;

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
    cout << "=== APOS INVERSAO ===" << endl;
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


bool Airport::insertIntraPista(int pista, int origem, int destino){

    //verificação de movimentos invalidos ou irrelevantes
    if (pista < 0 || pista >= num_pistas ||
        origem < 0 || destino < 0 ||
        origem >= static_cast<int>(pistas[pista].size()) ||
        destino > static_cast<int>(pistas[pista].size()) ||
        origem == destino || abs(origem - destino) == 1) {

        cerr << "Invalido: pista = " << pista << ", origem = " << origem << ", destino = " << destino << endl;
        return false;
    }

    const int id_voo = pistas[pista][origem];
    Voo& voo = voos[id_voo];

    if (destino > origem) destino--;
    pistas[pista].erase(pistas[pista].begin() + origem); // Remove o voo da posição origem
    pistas[pista].insert(pistas[pista].begin() + destino, id_voo); // Insere na posição destino

    // Atualiza os voos e horarios
    int horario_anterior = (destino > 0) ? voos[pistas[pista][destino-1]].horario_real : 0;
    for (int k = (destino > 0) ? destino - 1 : 0; k < pistas[pista].size(); ++k) {
        Voo& v = voos[pistas[pista][k]];
        v.horario_real = max(horario_anterior + ((k > 0) ? 
                            (voos[pistas[pista][k-1]].duracao + tempo_espera[pistas[pista][k-1]][v.id]) : 0),
                            v.horario_prev);
        horario_anterior = v.horario_real;
    }

    // Debug: mostra voos após inserção
    cout << "\n=== APOS INSERCAO ===" << endl;
    cout << "Voo " << id_voo << ": HR=" << voos[id_voo].horario_real 
         << ", Ant=" << voos[id_voo].voo_anterior << endl;
    cout << "Ordem na pista: ";

    for (int id : pistas[pista]) cout << id << " ";
    cout << endl << endl;
    calcularMultas();
    return true;
}

bool Airport::opt2IntraPista(int pista, int i, int j, int max_gap) {

    // Verificação completa
    if (pista < 0 || pista >= num_pistas || i < 0 || j <= i ||
        j >= static_cast<int>(pistas[pista].size()) || (j - i) > max_gap) {
        cerr << "Invalido: pista=" << pista << ", i=" << i << ", j=" << j << ", max_gap=" << max_gap << endl;
        return false;
    }

    // Debug: mostra voos antes da operação OPT-2
    cout << "\n=== ANTES DA OPERACAO OPT-2 ===" << endl;
    cout << "Pista " << pista << " - Segmento de " << i << " ate " << j << endl;
    cout << "Ordem na pista: ";
    for (int id : pistas[pista]) cout << id << " ";
    cout << endl;
    
    // Mostrar os voos no segmento
    for (int k = i; k <= j; k++) {
        int id_voo = pistas[pista][k];
        cout << "Voo " << id_voo << ": HR=" << voos[id_voo].horario_real 
             << ", Dur=" << voos[id_voo].duracao 
             << ", Ant=" << voos[id_voo].voo_anterior
             << ", Mul=" << voos[id_voo].multa << endl;
    }

    // Cria cópia para avaliação
    vector<int> nova_pista = pistas[pista];
    std::reverse(nova_pista.begin() + i, nova_pista.begin() + j + 1);

    pistas[pista] = nova_pista;
    
    // Atualiza apenas os horários afetados
    int horario = (i > 0) ? voos[pistas[pista][i-1]].horario_real + 
                           voos[pistas[pista][i-1]].duracao + 
                           tempo_espera[pistas[pista][i-1]][pistas[pista][i]] : 0;
                           
    for (int k = i; k <= j; ++k) {
        int id_voo = pistas[pista][k];
        Voo& v = voos[id_voo];
        
        if (k > i) {
            int id_anterior = pistas[pista][k-1];
            horario = max(
                v.horario_prev,
                voos[id_anterior].horario_real + 
                voos[id_anterior].duracao + 
                tempo_espera[id_anterior][id_voo]
            );
        } else if (i > 0) {
            int id_anterior = pistas[pista][i-1];
            horario = max(
                v.horario_prev,
                voos[id_anterior].horario_real + 
                voos[id_anterior].duracao + 
                tempo_espera[id_anterior][id_voo]
            );
        } else {
            horario = v.horario_prev;
        }
        
        v.horario_real = horario;
        v.voo_anterior = (k > 0) ? pistas[pista][k-1] : -1;
    }

    calcularMultas();
    
    // Debug: mostra voos após a operação OPT-2
    cout << "=== APOS OPERACAO OPT-2 ===" << endl;
    cout << "Ordem na pista: ";
    for (int id : pistas[pista]) cout << id << " ";
    cout << endl;
    
    // Mostrar os voos no segmento após a alteração
    for (int k = i; k <= j; k++) {
        int id_voo = pistas[pista][k];
        cout << "Voo " << id_voo << ": HR=" << voos[id_voo].horario_real 
             << ", Dur=" << voos[id_voo].duracao 
             << ", Ant=" << voos[id_voo].voo_anterior
             << ", Mul=" << voos[id_voo].multa << endl;
    }
    cout << endl;
    
    return true;
}