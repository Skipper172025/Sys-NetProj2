#!/bin/bash

N=1000000
RUNS=10

echo "N,Threads,Mode,Run,Seconds" > benchmark.csv

for mode in locked nolock
do
    for t in {1..8}
    do
        for run in $(seq 1 "$RUNS")
        do
            if [ "$mode" = "locked" ]; then
    result=$(./mt-collatz "$N" "$t" 2>&1 >/dev/null)
else
    result=$(./mt-collatz "$N" "$t" -nolock 2>&1 >/dev/null)
fi

seconds=$(echo "$result" | cut -d',' -f3)

echo "$N,$t,$mode,$run,$seconds" >> benchmark.csv
        done
    done
done



