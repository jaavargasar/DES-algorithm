#!/bin/bash
sudo rm -R output.time.txt
clear
nvcc cctest.cu -o cctest
thr=2
n=1
result=0
# 2^13
while [ $thr -le 8192 ] 
do
    n=1
    result=0
    echo "parallel threads: $thr" >> output.time.txt
    while [ $n -le 5 ]
    do
        # echo "parallel threads: $thr, N: $n" >> output.time.txt
	    ts=$(date +%s%N) ;./cctest $thr; tt=$((($(date +%s%N) - $ts)/1000000)); 
        result=$(( result+$tt ))
        # echo "$tt" >> output.time.txt
        n=$(( n+1 ))
    done
    # echo "$result" >> output.time.txt
    echo "scale=2; $result / 5" | bc >> output.time.txt
    thr=$(( thr*2 ))
done
echo "end of script"  
