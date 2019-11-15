#!/bin/bash

for f in heap merge quick
do
  for o in O0 O1 O2 O3 Os
  do
    gcc -$o -o $f $f.c
    echo ""
    echo $f $o
    time ./$f 1000000
  done
done
