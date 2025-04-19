#ifndef ALOCADOR_H
#define ALOCADOR_H

#include <string>
using namespace std;

bool executarAlocacao(const string& arquivo_entrada);
bool inverterVoosConsecutivos(int pista, int posicao_voo);
bool insertIntraPista(int id_voo, int pista, int posicao_voo);
bool opt2IntraPista(int p, int i, int j, int max_gap = 5);
extern int custo_total;
void mostrarSolucaoNoTerminal(int custo_total);
#endif //ALOCADOR_H
