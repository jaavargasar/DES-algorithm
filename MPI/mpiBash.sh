#!/bin/bash
sudo rm -R output.time.txt
clear
mpic++ -o mam cMpi.cpp
thr=2
n=1
result=0
# continue until $n equals 5
while [ $thr -le 32 ]
do
    n=1
    result=0
    echo "parallel threads: $thr" >> output.time.txt
    while [ $n -le 5 ]
    do
        # echo "parallel threads: $thr, N: $n" >> output.time.txt
	    ts=$(date +%s%N) ;mpirun -np $thr mam; tt=$((($(date +%s%N) - $ts)/1000000)); 
        result=$(( result+$tt ))
        # echo "$tt" >> output.time.txt
        n=$(( n+1 ))
    done
    # echo "$result" >> output.time.txt
    echo "scale=2; $result / 5" | bc >> output.time.txt
    thr=$(( thr*2 ))
done
echo "end of script"  
