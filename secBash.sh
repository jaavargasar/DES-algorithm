#!/bin/bash
g++ des1.cpp -o des1
echo "time"
tts=$(date +%s%N) ; ./des1 ; ttt=$((($(date +%s%N) - $tts)/1000000)); echo "$ttt"
echo "end of script"  
