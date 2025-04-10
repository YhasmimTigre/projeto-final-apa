//Para receber as funções a serem usadas no VND

#ifdef GULOSO_H
#define GULOSO_H

int calcularPenalidade(pistas, t);

struct Pista {
    int id;
    int tempo_liberacao;
    vector<Voos> voos_alocados;
};

struct Problema {
    vector<Pista> pistas;
    int solucao;
}
#endif