#!/bin/bash

test_set="../test_sets/$1"

inputs="$test_set/inputs/"
outputs="$test_set/outputs/"
configs="$test_set/configs/"
plots="$test_set/plots/"

default="default.cfg"

algorithms=("annealing" "clonal_selection")

for algo in ${algorithms[*]}
do
    mkdir -p "$outputs$algo/"
    mkdir -p "$configs$algo/"
    mkdir -p "$plots$algo/"
done

printf "0.00005\n1\n10"> "$configs${algorithms[0]}/$default"
printf "10\n5000\n250"> "$configs${algorithms[1]}/$default"

mkdir -p "$inputs"

