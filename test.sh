#!/bin/sh

echo '\ngcc:'
gcc -march=native -Ofast -flto test.c -static && taskset -c 0 ./a.out
echo 'clang:'
clang -march=native -Ofast -flto test.c -static && taskset -c 0 ./a.out
