#!/bin/bash
  
rm *.csv
echo "Array Size (inKB),Thread Count,Random,Sequential,RMW,CPU TIME(ns)" >> performance_output.csv

make
./cacheperf -t:1 -r -size:8192
./cacheperf -t:1 -r -size:8192 -RMW 
