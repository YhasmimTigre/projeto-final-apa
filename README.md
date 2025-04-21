# # Projeto Final - Algoritmos para Problemas de Alocação

**Análise e Projeto de Algoritmos (APA) - 2024.2**

Este repositório contém a implementação de algoritmos para o problema de alocação de voos em pistas de aeroporto, otimizando o custo total associado a atrasos.

## Algoritmos Implementados

- **Algoritmo Guloso**: Construção inicial de solução por penalidades decrescentes
- **VND (Variable Neighborhood Descent)**: Busca local usando múltiplas estruturas de vizinhança
- **ILS (Iterated Local Search)**: Meta-heurística com perturbações para escapar de ótimos locais

## Como Executar

### Usando o script automatizado

Execute o script que automatiza compilação e execução para todas as instâncias:

```bash
./script.sh
```

Esse script:
- Compila o código utilizando o Makefile
- Processa todas as instâncias no diretório input
- Gera resultados em output
- Mostra o tempo de execução para cada algoritmo

### Compilação e execução manual

Alternativamente, você pode compilar e executar manualmente:

```bash
# Compilação com g++
g++ main.cpp src/heart/airport.cpp src/algorithms/guloso.cpp src/algorithms/vnd.cpp src/algorithms/ils.cpp -Iinclude -o programa

# Executar para uma instância específica
./programa data/input/<instancia>.txt
```

Ao rodar no Codespace, você pode rodar os seguintes comandos:

```bash
g++ -std=c++17 -o alocador main.cpp \
    src/algorithms/*.cpp \
    src/heart/*.cpp \
    -I./src/algorithms \
    -I./src/heart
```

```bash
./alocador /workspaces/projeto-final-apa/data/input/<instancia>.txt
```

## Formato dos Arquivos de Saída

Os arquivos gerados em output contêm:
- Custo total da solução para cada algoritmo
- Alocação de voos nas pistas
- Tempo de execução em milissegundos para cada algoritmo

## Requisitos

- C++11 ou superior
- g++ ou compatível
- Permissão de execução para script.sh (`chmod +x script.sh`)






---

**Autores:** Adriel Ferreira, Kamily Assis, Yhasmim Tigre

