#!/bin/bash

# How to run:
#  source combine.sh -m alpha
# To filter jobs:
#  source combine.sh -c AZh2l

option=""

if [[ "$1" != "-c" ]] || [[ "$2" == "" ]]
then
    echo Select a category:
    echo "  -c:          AH, SL, ALL"
    return
fi

higgsCombine() {
    card=$1
    cat=$2
    inputfile=datacards/$card.txt
    outputfile=combine/$card.txt
    massstring=$(cut -f 2 -d "_" <<< $card)
    mass=$(echo $massstring | tr -dc '0-9')
    echo Running $cat on $mass $inputfile...
    #echo $inputfile $outputfile $card
    
    combine -M AsymptoticLimits --datacard $inputfile -m $mass $option | grep -e Observed -e Expected | awk '{print $NF}' > $outputfile  # --rMin=0.001
    #combine -M Significance --datacard $inputfile --significance -m $mass $option | grep -e Significance -e value | awk '{print $NF}' | sed -e 's/)//g' >> $outputfile
    #combine -M FitDiagnostics --datacard $inputfile -m $mass $option | grep Best | sed -e 's/\// /g' | awk '{print $4,$5,$6}' | sed -e 's/ /\n/g' >> $outputfile
}


for card in datacards/*_$2.txt
do
    analysis=$(basename $(dirname $card))
    signal=$(basename $card .txt)
#    echo Running with category $2 on $signal
    higgsCombine $signal $2 &
done

## Clean
wait

rm higgsCombine*.root
rm roostats-*
rm mlfit*.root

echo -e "\e[00;32mAll clear\e[00m"



# To derive Impacts (https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsWG/SWGuideNonStandardCombineUses#Nuisance_parameter_impacts):
# text2workspace.py datacards/dijet/XVHah_M2000.txt
# produces .root file in the same directory of the datacard
# combine -M MultiDimFit -n _initialFit_Test --algo singles --redefineSignalPOIs r --robustFit 1 -m 2000 -d XVHah_M2000.root
# combineTool.py -M Impacts -d XVHah_M2000.root -m 2000 --robustFit 1 --doFits --parallel 10
# combineTool.py -M Impacts -d XVHah_M2000.root -m 2000 -o impacts.json
# plotImpacts.py -i impacts.json -o impacts


