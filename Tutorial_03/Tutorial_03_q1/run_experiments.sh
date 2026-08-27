#!/bin/bash
set -e

SERIAL_BIN=./serial_pi
PARALLEL_BIN=./parallel_pi

SAMPLE_SIZES=(10 100 10000 1000000 10000000 100000000 1000000000)
THREAD_COUNTS=(1 2 4 8 16)
FIXED_N_FOR_THREAD_SCALING=10000000

echo "num_points,pi_estimate,error,time_seconds" > serial_results.csv
for N in "${SAMPLE_SIZES[@]}"; do
    $SERIAL_BIN $N >> serial_results.csv
done
cat serial_results.csv

echo "num_points,num_threads,pi_estimate,error,time_seconds" > parallel_points_results.csv
for N in "${SAMPLE_SIZES[@]}"; do
    $PARALLEL_BIN $N 4 >> parallel_points_results.csv
done
cat parallel_points_results.csv

echo "num_points,num_threads,pi_estimate,error,time_seconds" > thread_scaling_results.csv
for T in "${THREAD_COUNTS[@]}"; do
    $PARALLEL_BIN $FIXED_N_FOR_THREAD_SCALING $T >> thread_scaling_results.csv
done
cat thread_scaling_results.csv

echo "All experiments complete."
echo "Results saved to: serial_results.csv, parallel_points_results.csv, thread_scaling_results.csv"