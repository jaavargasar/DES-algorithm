#!/bin/bash
nvcc cctest.cu -o cctest
echo "time"
tts=$(date +%s%N) ; ./cctest ; ttt=$((($(date +%s%N) - $tts)/1000000)); echo "$ttt"
echo "end of script"  
