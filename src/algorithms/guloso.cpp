#include <algorithm>
#include <iostream>
#include <climits>
#include "../heart/airport.h"
#include "guloso.h"

using namespace std;

void Guloso(Airport* airport) {

    auto& voos = airport->bkp_voos;
    auto& pistas = airport->bkp_pistas;
    auto& tempo_espera = airport->tempo_espera;
    int m = airport->num_pistas;

    sort(voos.begin(), voos.end(), [](const Voo& a, const Voo& b) {
        return a.horario_prev < b.horario_prev;
    });

    vector<int> disponibilidade_pistas(m, 0);
    pistas.clear();
    pistas.resize(m);

    for (auto& voo : voos) {
        int melhor_pista = -1;
        int menor_custo = INT_MAX;
        int melhor_tempo_inicio = 0;

        for (int p = 0; p < m; p++) {

            int tempo_inicio;

            if (!pistas[p].empty()) {
                int voo_anterior = pistas[p].back();
                tempo_inicio = max(disponibilidade_pistas[p]+ tempo_espera[voo_anterior][voo.id], voo.horario_prev);

            } else {

                tempo_inicio = max(disponibilidade_pistas[p], voo.horario_prev);

            }

            int atraso = tempo_inicio - voo.horario_prev;
            int custo = atraso * voo.penalidade;

            if (custo < menor_custo) {
                menor_custo = custo;
                melhor_pista = p;
                melhor_tempo_inicio = tempo_inicio;
            }

        }     

        voo.pista_alocada = melhor_pista;
        voo.horario_real = melhor_tempo_inicio;
        voo.voo_anterior = pistas[melhor_pista].empty() ? -1 : pistas[melhor_pista].back();

        disponibilidade_pistas[melhor_pista] = voo.horario_real + voo.duracao;
        pistas[melhor_pista].push_back(voo.id);

    }
}