#!/bin/bash
# filepath: /home/adriel/Documentos/CI/P6/APA/projeto-final-apa/script.sh

# Verificar se o programa foi compilado
if [ ! -f "./programa" ]; then
    echo "Compilando o programa..."
    make clean && make
fi

# Criar pasta de saída se não existir
mkdir -p data/output

# Guarda o tempo de início
start_time=$(date +%s)

# Iterar sobre todos os arquivos .txt na pasta data/input
for input_file in data/input/*.txt; do
    # Extrair apenas o nome do arquivo sem o caminho e a extensão
    filename=$(basename "$input_file" .txt)
    
    echo "Processando $filename..."
    
    # Criar nome personalizado para o arquivo de saída
    output_file="data/output/${filename}_resultado.txt"
    
    # Executar o programa com o arquivo de entrada
    ./programa "$input_file"
    
    # Verificar se a execução foi bem-sucedida
    if [ $? -eq 0 ]; then
        echo "✓ Processamento de $filename concluído com sucesso."
        
        # Verificar se o arquivo de resultados foi criado
        if [ -f "data/output/resultados.txt" ]; then
            # Mover o arquivo com nome específico para a instância
            mv "data/output/resultados.txt" "$output_file"
            echo "  Resultado salvo em $output_file"
        else
            echo "  Aviso: Arquivo de resultados não encontrado."
        fi
    else
        echo "✗ Erro ao processar $filename"
    fi
    
    echo "----------------------------------------"
done

# Calcular tempo total
end_time=$(date +%s)
execution_time=$((end_time - start_time))
minutes=$((execution_time / 60))
seconds=$((execution_time % 60))

echo "Todos os arquivos foram processados em $minutes minutos e $seconds segundos."