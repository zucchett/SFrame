# Create grid proxy with: voms-proxy-init --voms cms

# -u option to avoid output buffering
python -u submitSFrame.py -j submitData.py --nosandbox --useEnvironment --keepTemp &> logData.txt &
python -u submitSFrame.py -j submitQCD.py --nosandbox --useEnvironment --keepTemp &> logQCD.txt &
python -u submitSFrame.py -j submitTop.py --nosandbox --useEnvironment --keepTemp &> logTop.txt &
python -u submitSFrame.py -j submitBkg.py --nosandbox --useEnvironment --keepTemp &> logBkg.txt &
python -u submitSFrame.py -j submitSignals.py --nosandbox --useEnvironment --keepTemp &> logSignals.txt &


sframe_main config/DMScalar_top_tChan_MChi1_MPhi100_config.xml
cp output/DMAnalysis.DM.1.root AnalysisOutput/DMAnalysis.DMScalar_top_tChan_MChi1_MPhi100.root

sframe_main config/DMScalar_top_tChan_MChi1_MPhi300_config.xml
cp output/DMAnalysis.DM.1.root AnalysisOutput/DMAnalysis.DMScalar_top_tChan_MChi1_MPhi300.root

sframe_main config/DMScalar_top_tWChan_MChi1_MPhi100_config.xml
cp output/DMAnalysis.DM.1.root AnalysisOutput/DMAnalysis.DMScalar_top_tWChan_MChi1_MPhi100.root

sframe_main config/DMScalar_top_tWChan_MChi1_MPhi300_config.xml
cp output/DMAnalysis.DM.1.root AnalysisOutput/DMAnalysis.DMScalar_top_tWChan_MChi1_MPhi300.root

