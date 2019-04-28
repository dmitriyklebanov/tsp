#!/bin/bash

default="default.cfg"

test_set="../test_sets/$2"

inputs="$test_set/inputs/"
outputs="$test_set/outputs/"
configs="$test_set/configs/"
plots="$test_set/plots/"

algo="$3"
test_name="$4"

function launch {
    echo -e "\e[1m\e[32m$algo\e[31m has been started on the test \"$1\"\e[0m"
    if [ -f "$configs$algo/$1.cfg" ];
    then
        ../bin/launch "$algo" "$inputs/$1" "$outputs$algo/$1.a" "$configs$algo/$1.cfg"
    else
        ../bin/launch "$algo" "$inputs/$1" "$outputs$algo/$1.a" "$configs$algo/$default"
    fi
}

function plot {
    echo -e "\e[1m\e[32m$algo\e[31m results are plotting on the test \"$1\"\e[0m"
    python3.5 sys/plot.py "$algo" "$inputs$1" "$outputs$algo/$1.a" "$plots$algo/$1.png"
}

if [ "$test_name" == "0" ];
then
    for test_name in $(ls "$inputs/")
    do
        $1 $test_name
    done
else
    $1 $test_name
fi

