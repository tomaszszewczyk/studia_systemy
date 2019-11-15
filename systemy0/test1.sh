#!/bin/bash

gcc -c -o lib.o lib.c
ar rcs lib.a lib.o
gcc -o static insert.c lib.a

echo static
time ./static 50000

gcc -shared -o liblib.so lib.o
gcc -o dynamic insert.c -L`pwd` -llib

export LD_LIBRARY_PATH=`pwd`
echo dynamic
time ./dynamic 50000

gcc -o runtime insert_dyn.c -ldl

echo runtime
time ./runtime 50000
