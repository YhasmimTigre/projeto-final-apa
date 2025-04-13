#include <algorithm>
#include <iostream>
#include <climits>
#include "../core/airport_loader.h"
#include "guloso.h"

using namespace std;

void Guloso() {

    sort(voos.begin(), voos.end(), [](const Voo& a, const Voo& b) {
        return a.horario_prev < b.horario_prev;
    });

    vector<int> disponibilidade_pistas(m, 0);
    vector<int> ultimo_voo_pista(m, -1);

    /*cout << "quantidade pistas: " << disponibilidade_pistas.size() << endl;*/

    for (auto& voo : voos) {
        int melhor_pista = -1;
        int menor_custo = INT_MAX;
        int melhor_tempo_inicio = 0;

        for (int p = 0; p < m; p++) {

            // Calcula o tempo de início considerando o tempo de espera
            int tempo_inicio = max(disponibilidade_pistas[p], voo.horario_prev);

            /*cout << "tempo inicio: " << tempo_inicio << endl;*/

            // Se houver voo anterior na pista, adiciona tempo de espera t_ij
            if (ultimo_voo_pista[p] != -1) {
                int voo_anterior = ultimo_voo_pista[p];
                tempo_inicio += tempo_espera[voo_anterior][voo.id];  // t_ij

                /*cout << "voo anterior: " << voo_anterior << endl;
                cout << "voo atual: " << voo.id << endl;
                cout << "tempo de espera: " << tempo_espera[voo_anterior][voo.id] << endl;
                cout << "vai iniciar em: " << tempo_inicio << endl << endl;*/
            }

            int atraso = tempo_inicio - voo.horario_prev;
            int custo = atraso * voo.penalidade;

            /* cout << "atraso: " << atraso;
            cout << " menor custo: " << menor_custo;
            cout << " custo: " << custo << endl; */

            if (custo < menor_custo) {
                menor_custo = custo;
                melhor_pista = p;
                melhor_tempo_inicio = tempo_inicio;
            }
        }

        // Aloca o voo
        voo.pista_alocada = melhor_pista;
        voo.horario_real = melhor_tempo_inicio;
        voo.voo_anterior = ultimo_voo_pista[melhor_pista];

        // Atualiza disponibilidade da pista
        disponibilidade_pistas[melhor_pista] = voo.horario_real + voo.duracao;
        ultimo_voo_pista[melhor_pista] = voo.id;

        /*cout << "id: " << voo.id << " horario_real: " << voo.horario_real << " horario_previsto: " << voo.horario_prev
             << " duracao: " << voo.duracao << " penalidade: " << voo.penalidade << " multa: " << voo.multa
             << " pista: " << voo.pista_alocada << " voo que veio antes: " << voo.voo_anterior << endl;*/
    }
}