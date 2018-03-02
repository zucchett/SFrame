#!/bin/bash

name=$1

echo $name

python limit.py -s tttDM -c SL -m SC -j 1b -N $name -b
python limit.py -s tttDM -c SL -m SC -j 2b -N $name -b
python limit.py -s tttDM -c SL -m SC -j 1b2b -N $name -b

python limit.py -s tttDM -c AH -m SC -j 1b -N $name -b
python limit.py -s tttDM -c AH -m SC -j 2b -N $name -b
python limit.py -s tttDM -c AH -m SC -j 1b2b -N $name -b

python limit.py -s tttDM -c ALL -m SC -j 1b -N $name -b
python limit.py -s tttDM -c ALL -m SC -j 2b -N $name -b
python limit.py -s tttDM -c ALL -m SC -j 1b2b -N $name -b

python limit.py -s tttDM -c SL -m PS -j 1b -N $name -b
python limit.py -s tttDM -c SL -m PS -j 2b -N $name -b
python limit.py -s tttDM -c SL -m PS -j 1b2b -N $name -b

python limit.py -s tttDM -c AH -m PS -j 1b -N $name -b
python limit.py -s tttDM -c AH -m PS -j 2b -N $name -b
python limit.py -s tttDM -c AH -m PS -j 1b2b -N $name -b

python limit.py -s tttDM -c ALL -m PS -j 1b -N $name -b
python limit.py -s tttDM -c ALL -m PS -j 2b -N $name -b
python limit.py -s tttDM -c ALL -m PS -j 1b2b -N $name -b