#!/bin/bash
# Basic range in for loop
for value in `seq 1 2 31`;
do
    for value2 in `seq 1 2 31`
    do
    ./code lena256.pgm $value $value2
    ./decode codage.mlc $value $value2
    done
done