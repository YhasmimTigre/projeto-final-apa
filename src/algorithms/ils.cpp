#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>

#include "ils.h"
#include "vnd.h"

using namespace std;

void imprimirInfoPista(Airport* airport, int pista, const string& mensagem = "INFORMAÇÃO DE PISTA") {
    cout << "\n=== " << mensagem << " ===" << endl;
    
    // Verificar se a pista é válida
    if (pista < 0 || pista >= airport->num_pistas) {
        cout << "Pista " << pista << " inválida!" << endl;
        return;
    }
    
    cout << "Pista: " << pista << endl;
    
    // Verificar se há voos na pista
    if (airport->pistas[pista].empty()) {
        cout << "Não há voos nesta pista." << endl;
        return;
    }
    
    // Imprimir informações de cada voo na pista
    for (auto& id : airport->pistas[pista]) {
        Voo* v = airport->encontrarVooPorId(id);
        
        if (v) {
            cout << "\nVoo " << v->id 
                 << " || h_real: " << v->horario_real 
                 << " || h_prev: " << v->horario_prev
                 << " || duracao: " << v->duracao
                 << " || multa: " << v->multa 
                 << " || voo anterior: " << v->voo_anterior << endl;
        } else {
            cout << "\nVoo " << id << " não encontrado!" << endl;
        }
    }
}

void imprimirSolucao(const Airport* airport) {
    for (const auto& voo : airport->voos) {
        cout << "Voo " << voo.id
             << ": pista = " << voo.pista_alocada
             << ", horário real = " << voo.horario_real
             << ", penalidade = " << voo.penalidade
             << "\n";
    }
    cout << "-----------------------\n";
}

void perturbar(Airport* airport, int tamanhoBloco = 2) {

    // DEBUG antes perturbar
    //cout << "\n=== ANTES TROCA DE BLOCOS ===" << endl;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> pistaDist(0, airport->num_pistas - 1);

    int pistaA, pistaB;
    do {
        pistaA = pistaDist(gen);
        pistaB = pistaDist(gen);
    } while (pistaA == pistaB || 
             airport->pistas[pistaA].size() < tamanhoBloco || 
             airport->pistas[pistaB].size() < tamanhoBloco);

    uniform_int_distribution<> idxA(0, airport->pistas[pistaA].size() - tamanhoBloco);
    uniform_int_distribution<> idxB(0, airport->pistas[pistaB].size() - tamanhoBloco);

    int startA = idxA(gen);
    int startB = idxB(gen);

    

    imprimirInfoPista(airport, pistaA, "ANTES DA TROCA DE BLOCOS");
    imprimirInfoPista(airport, pistaB, "ANTES DA TROCA DE BLOCOS");
    // Troca os blocos de voos entre as pistas
    vector<int> blocoA(airport->pistas[pistaA].begin() + startA, 
                      airport->pistas[pistaA].begin() + startA + tamanhoBloco);
    vector<int> blocoB(airport->pistas[pistaB].begin() + startB, 
                      airport->pistas[pistaB].begin() + startB + tamanhoBloco);

    // Realiza a troca
    for (int i = 0; i < tamanhoBloco; i++) {
        airport->pistas[pistaA][startA + i] = blocoB[i];
        airport->pistas[pistaB][startB + i] = blocoA[i];
        
        // Atualiza as pistas alocadas nos objetos Voo
        Voo* vooB = airport->encontrarVooPorId(blocoB[i]);
        Voo* vooA = airport->encontrarVooPorId(blocoA[i]);
        
        if (vooB) vooB->pista_alocada = pistaA;
        if (vooA) vooA->pista_alocada = pistaB;
    }
    

    // Atualiza horários e predecessores
    auto atualizarPista = [airport](int pista) {
        int anterior = -1; // ID do voo anterior
        
        for (size_t i = 0; i < airport->pistas[pista].size(); i++) {
            int id_atual = airport->pistas[pista][i];
            Voo* voo_atual = airport->encontrarVooPorId(id_atual);
            
            if (!voo_atual) {
                cerr << "Erro: Voo " << id_atual << " não encontrado!" << endl;
                continue;
            }
            
            // Atualiza voo anterior
            voo_atual->voo_anterior = anterior;
            
            // Calcula horário real
            if (anterior == -1) {
                // Primeiro voo da pista
                voo_atual->horario_real = voo_atual->horario_prev;
            } else {
                Voo* voo_anterior = airport->encontrarVooPorId(anterior);
                if (voo_anterior) {
                    int espera = airport->tempo_espera[anterior][id_atual];
                    voo_atual->horario_real = max(
                        voo_atual->horario_prev,
                        voo_anterior->horario_real + voo_anterior->duracao + espera
                    );
                }
            }
            
            anterior = id_atual;
        }
    };
    

    atualizarPista(pistaA);
    atualizarPista(pistaB);

    airport->calcularMultas();

    cout << "[Perturbação] Troca de blocos entre pistas " << pistaA << " e " << pistaB << " concluída.\n";

    // DEBUG depois
    /*cout << "=== APOS TROCA DE BLOCOS ===" << endl;
    
    cout << "Pista: " << airport->bkp_pistas << endl;
    for (auto& id : airport->bkp_pistas[pistaB]){

        Voo* v = airport->encontrarVooPorId(id);

        cout << "\nVoo " << v->id << " || h_real: " << v->horario_real << " || h_prev: " << v->horario_prev
        << "|| duracao: " << v->duracao 
        << "|| multa: " << v->multa << "|| voo anterior: " << v->voo_anterior << endl;
    };*/

    imprimirInfoPista(airport, pistaA, "DEPOIS DA TROCA DE BLOCOS");
    imprimirInfoPista(airport, pistaB, "DEPOIS DA TROCA DE BLOCOS");

}

void ILS(Airport* airport, int maxIter) {

    // Executa VND inicial
    VND(airport);
    airport->salvarMelhorSolucao();
    int melhorCusto = airport->bkp_custo;

    cout << "\n--- Iniciando ILS ---\n";
    cout << "Custo inicial: " << melhorCusto << "\n";

    for (int iter = 0; iter < maxIter; iter++) {
        cout << "\nIteração " << iter + 1 << "/" << maxIter << endl;

        // Aplica perturbação
        perturbar(airport);

        // Refina com VND
        VND(airport);

        // Avalia a solução
        int custoAtual = airport->custo_total;
        if (custoAtual < melhorCusto) {
            cout << "Nova melhor solução encontrada: " << custoAtual << " (anterior: " << melhorCusto << ")\n";
            *airport = *airport;  // airport a solução melhorada
            airport->salvarMelhorSolucao();
            melhorCusto = custoAtual;
        }
    }

    // Restaura a melhor solução encontrada
    airport->restaurarMelhorSolucao();
    cout << "\n--- Fim do ILS ---\n";
    cout << "Melhor custo final: " << melhorCusto << "\n";
    cout << "Melhor alocação:\n";
    imprimirSolucao(airport);
}