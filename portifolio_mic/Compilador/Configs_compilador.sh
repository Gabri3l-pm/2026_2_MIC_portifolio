#!/usr/bin/env bash

# Define o arquivo .hex (usa 'main.hex' por padrão se nenhum for passado)
HEX_FILE="${1:-main.hex}"

if [ ! -f "$HEX_FILE" ]; then
    echo "❌ Erro: O arquivo '$HEX_FILE' não foi encontrado."
    exit 1
fi

# Detecta automaticamente a primeira porta USB conectada (/dev/ttyACM* ou /dev/ttyUSB*)
PORT=$(ls /dev/ttyACM* /dev/ttyUSB* 2>/dev/null | head -n 1)

if [ -z "$PORT" ]; then
    echo "❌ Erro: Nenhuma placa/conversor USB detectado em /dev/ttyACM* ou /dev/ttyUSB*."
    exit 1
fi

echo "🚀 Placa detectada na porta: $PORT"
echo "📦 Gravando $HEX_FILE no ATmega328p..."

# Comando avrdude (altere '-c arduino' se usar outro gravador como usbasp)
sudo avrdude -c arduino -p ATMEGA328P -P "$PORT" -b 115200 -U flash:w:"$HEX_FILE":i
