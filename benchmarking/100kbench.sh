#!/usr/bin/env bash

OUTPUT_FILE="benchmark_results100k.txt"
BODY_COUNTS=(100000)
THREAD_COUNT=6
RUNS=5

> "$OUTPUT_FILE"

for bodies in "${BODY_COUNTS[@]}"; do
    for i in $(seq 1 $RUNS); do
        echo "--- CUDA : $bodies bodies (Run $i) ---" >> "$OUTPUT_FILE"
        ./cuda/build/cuda-impl "$bodies" | awk '/=======AVERAGES=======/{flag=1} flag' >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE"
    done
done
