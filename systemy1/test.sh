#!/bin/bash

COUNT=1000

for mode in sys lib
do
  for i in 4 512 4096 8192
  do
    echo $mode $i
    time $1 generate $mode file $COUNT $i > /dev/null
    time $1 copy $mode file file2 $COUNT $i > /dev/null
    time $1 sort $mode file $COUNT $i > /dev/null

    echo ""
    echo ""
  done
done
