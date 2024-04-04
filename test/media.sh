#!/bin/bash

# Verificar si se proporcionó un archivo como argumento
if [ $# -ne 1 ]; then
    echo "Uso: $0 <archivo>"
    exit 1
fi

# Calcular la media de las notas usando awk
awk 'NR > 2 {sum += $3; n++} END {if (n > 0) printf "La media de descubierto es: %.2f\n", sum / n; else echo "No se encontraron notas válidas en el archivo."}' "$1"
