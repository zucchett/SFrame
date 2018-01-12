#!/bin/bash

cd datacards/

echo Merging datacards...
for card in *_AH0l2bSR.txt
do
    signal=$(echo $card | cut -d "_" -f 1-3)
    echo Merging datacards for $card and $signal
    combineCards.py AH0l0fSR="$signal"_AH0l0fSR.txt AH0l1fSR="$signal"_AH0l1fSR.txt AH0l2bSR="$signal"_AH0l2bSR.txt  AH0lCR="$signal"_AH0lCR.txt  AH1eWR="$signal"_AH1eWR.txt  AH1mWR="$signal"_AH1mWR.txt  AH2eZR="$signal"_AH2eZR.txt  AH2mZR="$signal"_AH2mZR.txt  AH1eTR="$signal"_AH1eTR.txt  AH1mTR="$signal"_AH1mTR.txt > "$signal"_AH.txt
    combineCards.py SL1e0fSR="$signal"_SL1e0fSR.txt SL1e1fSR="$signal"_SL1e1fSR.txt SL1e2bSR="$signal"_SL1e2bSR.txt SL1m0fSR="$signal"_SL1m0fSR.txt SL1m1fSR="$signal"_SL1m1fSR.txt SL1m2bSR="$signal"_SL1m2bSR.txt SL1eWR="$signal"_SL1eWR.txt  SL1mWR="$signal"_SL1mWR.txt  SL1e1mTR="$signal"_SL1e1mTR.txt > "$signal"_SL.txt
    combineCards.py AH0l0fSR="$signal"_AH0l0fSR.txt AH0l1fSR="$signal"_AH0l1fSR.txt AH0l2bSR="$signal"_AH0l2bSR.txt AH0lCR="$signal"_AH0lCR.txt  AH1eWR="$signal"_AH1eWR.txt  AH1mWR="$signal"_AH1mWR.txt  AH2eZR="$signal"_AH2eZR.txt  AH2mZR="$signal"_AH2mZR.txt  AH1eTR="$signal"_AH1eTR.txt  AH1mTR="$signal"_AH1mTR.txt SL1e0fSR="$signal"_SL1e0fSR.txt SL1e1fSR="$signal"_SL1e1fSR.txt SL1e2bSR="$signal"_SL1e2bSR.txt SL1m0fSR="$signal"_SL1m0fSR.txt SL1m1fSR="$signal"_SL1m1fSR.txt SL1m2bSR="$signal"_SL1m2bSR.txt SL1eWR="$signal"_SL1eWR.txt  SL1mWR="$signal"_SL1mWR.txt  SL1e1mTR="$signal"_SL1e1mTR.txt > "$signal"_ALL.txt
done

cd ..

echo -e "\e[00;32mAll clear\e[00m"

