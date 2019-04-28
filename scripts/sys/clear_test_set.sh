#!/bin/bash

test_set="../test_sets/$1"

input="$test_set/inputs/"
output="$test_set/outputs/"
config="$test_set/configs/"
plot="$test_set/plots/"

rm -rf $input*
rm -rf $output*
rm -rf $config*
rm -rf $plot*

bash ./create_test_set.sh $1
