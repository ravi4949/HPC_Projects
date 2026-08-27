#!/bin/bash
set -e

SERIAL_BIN=./serial_edge
PARALLEL_BIN=./parallel_edge

N=4096
THREAD_COUNTS=(1 2 4 8 16)

echo "n,time_seconds,checksum" > serial_edge_results.csv
$SERIAL_BIN $N >> serial_edge_results.csv
cat serial_edge_results.csv

echo "n,num_threads,time_seconds,checksum" > parallel_edge_results.csv
for T in "${THREAD_COUNTS[@]}"; do
    $PARALLEL_BIN $N $T >> parallel_edge_results.csv
done
cat parallel_edge_results.csv

echo "All experiments complete."
echo "Results saved to: serial_edge_results.csv, parallel_edge_results.csv"