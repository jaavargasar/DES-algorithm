#!/bin/bash
g++ parDes.cpp -o parDes -fopenmp
echo "time"
tts=$(date +%s%N) ; ./parDes ; ttt=$((($(date +%s%N) - $tts)/1000000)); echo "$ttt"
echo "end of script"  
