#!/bin/bash  
clear
gcc p-matrix.c -o pmatrix -fopenmp
gcc s-matrix.c -o smatrix -fopenmp
n=8
thr=8
# continue until $n equals 5
while [ $n -le 2048 ]
do
	# echo "parallel $n - $thr" >> output.time.txt
	# ts=$(date +%s%N) ;./pmatrix $n $thr; tt=$((($(date +%s%N) - $ts)/1000000)); echo "$tt" >> output.time.txt
    # echo "secuential $n" >> output.time.txt
    tts=$(date +%s%N) ; ./smatrix $n; ttt=$((($(date +%s%N) - $tts)/1000000)); echo "$ttt"  >> output.time.txt
	n=$(( n*2 ))	 # increments $n
done
echo "end of script"  
