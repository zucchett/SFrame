#!/bin/bash

name=$1
SR=$2

cd datacards_$name/

echo Merging datacards...
for card in *_AH0l2bSR.txt
do
    signal=$(echo $card | cut -d "_" -f 1-4)
    echo $signal

    if [ "$SR" = "1b" ]; then
	echo SR is 1b
	SRAHstring="AH0l0fSR="$signal"_AH0l0fSR.txt AH0l1fSR="$signal"_AH0l1fSR.txt"
	SRSLstring="SL1e0fSR="$signal"_SL1e0fSR.txt SL1e1fSR="$signal"_SL1e1fSR.txt SL1m0fSR="$signal"_SL1m0fSR.txt SL1m1fSR="$signal"_SL1m1fSR.txt"
    elif [ "$SR" = "2b" ]; then
	echo SR is 2b
	SRAHstring="AH0l2bSR="$signal"_AH0l2bSR.txt"
	SRSLstring="SL1m2bSR="$signal"_SL1m2bSR.txt SL1e2bSR="$signal"_SL1e2bSR.txt"
    elif [ "$SR" = "1b2b" ]; then
	echo SR is 1b2b
	SRAHstring="AH0l0fSR="$signal"_AH0l0fSR.txt AH0l1fSR="$signal"_AH0l1fSR.txt AH0l2bSR="$signal"_AH0l2bSR.txt"
	SRSLstring="SL1e0fSR="$signal"_SL1e0fSR.txt SL1e1fSR="$signal"_SL1e1fSR.txt SL1m0fSR="$signal"_SL1m0fSR.txt SL1m1fSR="$signal"_SL1m1fSR.txt SL1m2bSR="$signal"_SL1m2bSR.txt SL1e2bSR="$signal"_SL1e2bSR.txt"
    fi

    echo Merging datacards for $card and $signal
    
    combineCards.py $SRAHstring AH1eWR="$signal"_AH1eWR.txt  AH1mWR="$signal"_AH1mWR.txt  AH2eZR="$signal"_AH2eZR.txt  AH2mZR="$signal"_AH2mZR.txt  AH1eTR="$signal"_AH1eTR.txt  AH1mTR="$signal"_AH1mTR.txt > "$signal$SR"_AH.txt

    combineCards.py $SRSLstring SL1eWR="$signal"_SL1eWR.txt  SL1mWR="$signal"_SL1mWR.txt  SL1e1mTR="$signal"_SL1e1mTR.txt SL2eTR="$signal"_SL2eTR.txt SL2mTR="$signal"_SL2mTR.txt > "$signal$SR"_SL.txt
    
    combineCards.py $SRAHstring AH1eWR="$signal"_AH1eWR.txt  AH1mWR="$signal"_AH1mWR.txt  AH2eZR="$signal"_AH2eZR.txt  AH2mZR="$signal"_AH2mZR.txt  AH1eTR="$signal"_AH1eTR.txt  AH1mTR="$signal"_AH1mTR.txt $SRSLstring SL1eWR="$signal"_SL1eWR.txt  SL1mWR="$signal"_SL1mWR.txt  SL1e1mTR="$signal"_SL1e1mTR.txt SL2eTR="$signal"_SL2eTR.txt SL2mTR="$signal"_SL2mTR.txt  > "$signal$SR"_ALL.txt

done

cd ..

echo -e "\e[00;32mAll clear\e[00m"

