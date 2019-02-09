#!/bin/bash
nvcc cudDes.cu -o cudDes
./cudDes
echo "time"
tts=$(date +%s%N) ; ./cudDes ; ttt=$((($(date +%s%N) - $tts)/1000000)); echo "$ttt"
echo "end of script"  
