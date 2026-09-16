#!/bin/bash

PROGRAM="./main"
THREADS=(1 2 4 8 16 32 64 128 256 512 1024)
RUNS=5
OUTPUT="results.txt"

> "$OUTPUT"

echo "Threads Median_ms" >> "$OUTPUT"

for threads in "${THREADS[@]}"; do
    echo "===== $threads threads ====="

    times=()

    for ((run=1; run<=RUNS; run++)); do
        output=$("$PROGRAM" "$threads")

        time=$(echo "$output" | grep "Total - Init:" | awk '{print $4}')

        times+=("$time")

        echo "Run $run: $time ms"
    done

    median=$(printf "%s\n" "${times[@]}" | sort -n | sed -n '3p')

    echo "Median: $median ms"
    echo

    echo "$threads $median" >> "$OUTPUT"
done

echo "Done!"
echo "Results saved to $OUTPUT"