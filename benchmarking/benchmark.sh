#!/usr/bin/env bash

OUTPUT_FILE="benchmark_results.txt"
BODY_COUNTS=(10 100 1000 10000 100000)
THREAD_COUNT=6
RUNS=5

> "$OUTPUT_FILE"

for bodies in "${BODY_COUNTS[@]}"; do
    for i in $(seq 1 $RUNS); do
        echo "--- SERIAL : $bodies bodies (Run $i) ---" >> "$OUTPUT_FILE"
        ./serial/serial-impl "$bodies" | awk '/=======AVERAGES=======/{flag=1} flag' >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE"

        echo "--- PTHREADS : $bodies bodies, $THREAD_COUNT threads (Run $i) ---" >> "$OUTPUT_FILE"
        ./pthreads/thread-impl "$bodies" "$THREAD_COUNT" | awk '/=======AVERAGES=======/{flag=1} flag' >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE"

        echo "--- CUDA : $bodies bodies (Run $i) ---" >> "$OUTPUT_FILE"
        ./cuda/build/cuda-impl "$bodies" | awk '/=======AVERAGES=======/{flag=1} flag' >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE"
    done
done
