#!/bin/bash

for i in $(seq 1 9); do
    # rm suma_32sgn
    gcc -x assembler-with-cpp -D TEST=$i -no-pie $1.s -o $1
    printf "__TEST%02d__%35s\n" $i "" | tr " " "-" ; ./$1
done