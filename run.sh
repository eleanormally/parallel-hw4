#!/bin/bash -x

#SBATCH --partition=el8-rpi
#SBATCH --gres=gpu:4
#SBATCH --time=15

cd barn/code/hw4
gcc main.c -pthread -o compute
echo 1 | xargs time ./compute 
seq 7 | xargs -I {} sh -c 'echo $((2**{}))' | xargs -I {} time ./compute {}
echo 1 | xargs -I {} time ./compute {} --strong
seq 7 | xargs -I {} sh -c 'echo $((2**{}))' | xargs -I {} time ./compute {} --strong
