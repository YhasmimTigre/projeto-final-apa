#include <algorithm>
#include <iostream>
#include <climits>
#include "../heart/airport.h"
#include "guloso.h"

using namespace std;

void Guloso(Airport* airport) {

    cout << "Entrou no Guloso\n" << endl;

    //dados necessarios
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

    /*cout << "quantidade pistas: " << disponibilidade_pistas.size() << endl;*/

    for (auto& voo : voos) {
        int melhor_pista = -1;
        int menor_custo = INT_MAX;
        int melhor_tempo_inicio = 0;

        /*cout << "Voo id: " << voo.id << endl;*/

        for (int p = 0; p < m; p++) {

            int tempo_inicio;
            
            /*cout<< "Visitando pista: " << p << endl;*/

            // Se houver voo anterior na pista, adiciona tempo de espera t_ij
            if (!pistas[p].empty()) {
                int voo_anterior = pistas[p].back();
                tempo_inicio = max(disponibilidade_pistas[p]+ tempo_espera[voo_anterior][voo.id], voo.horario_prev);

                /*cout << "voo anterior: " << voo_anterior << endl;
                cout << "voo atual: " << voo.id << endl;
                cout << "tempo de espera: " << tempo_espera[voo_anterior][voo.id] << endl;
                cout << "vai iniciar em: " << tempo_inicio << endl;
                cout << "horario previsto era: " << voo.horario_prev << endl;*/
            } else {

                tempo_inicio = max(disponibilidade_pistas[p], voo.horario_prev);

                /*cout << "tempo inicio: " << tempo_inicio << endl;*/
            }

            int atraso = tempo_inicio - voo.horario_prev;
            int custo = atraso * voo.penalidade;

            /*cout << "atraso: " << atraso;
            cout << " custo: " << custo << endl;*/ 

            if (custo < menor_custo) {
                menor_custo = custo;
                melhor_pista = p;
                melhor_tempo_inicio = tempo_inicio;
            }
            /*cout << " menor custo: " << menor_custo << endl << endl;*/
        }     

        // Aloca o voo
        voo.pista_alocada = melhor_pista;
        voo.horario_real = melhor_tempo_inicio;
        voo.voo_anterior = pistas[melhor_pista].empty() ? -1 : pistas[melhor_pista].back();

        // Atualiza disponibilidade da pista
        disponibilidade_pistas[melhor_pista] = voo.horario_real + voo.duracao;
        pistas[melhor_pista].push_back(voo.id);

        /*cout << "id: " << voo.id << " horario_real: " << voo.horario_real << " horario_previsto: " << voo.horario_prev
             << " duracao: " << voo.duracao << " penalidade: " << voo.penalidade << " multa: " << voo.multa
             << " pista: " << voo.pista_alocada << " voo que veio antes: " << voo.voo_anterior << endl;*/
    }



}