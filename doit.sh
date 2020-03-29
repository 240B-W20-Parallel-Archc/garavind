#!/bin/bash
  
rm *.csv
echo "ArraySize(inKB),Thread_Count,Random,Sequential,RMW,CPU_TIME" >> performance_output.csv

make
./cacheperf -t:1 -r -size:512
./cacheperf -t:1 -r -size:512 -RMW 
