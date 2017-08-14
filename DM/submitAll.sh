# Create grid proxy with: voms-proxy-init --voms cms

# -u option to avoid output buffering
python -u submitSFrame.py -j submitData.py --nosandbox --useEnvironment --keepTemp &> logData.txt &
python -u submitSFrame.py -j submitQCD.py --nosandbox --useEnvironment --keepTemp &> logQCD.txt &
python -u submitSFrame.py -j submitTop.py --nosandbox --useEnvironment --keepTemp &> logTop.txt &
python -u submitSFrame.py -j submitBkg.py --nosandbox --useEnvironment --keepTemp &> logBkg.txt &
python -u submitSFrame.py -j submitSignals.py --nosandbox --useEnvironment --keepTemp &> logSignals.txt &

