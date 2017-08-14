path2xml="$SFRAME_DIR/../BatchSubmission/xmls_ReMiniAOD"
path2tmp="$SFRAME_DIR/../DM/AnalysisTemp"
outDir="$SFRAME_DIR/../DM/AnalysisOutput"
jobName="DMAnalysisJob"
cycleName="DMAnalysis"
outputLevel="WARNING"
nEventsMax=-1
nProcesses=1
nFiles=20
hCPU="06:00:00"
hVMEM="1500M"
postFix = ""

dataSets = []
listDir = "../BatchSubmission/lists_ReMiniAOD"


for d in ["SingleMuon_Run2016B", "SingleMuon_Run2016C", "SingleMuon_Run2016D", "SingleMuon_Run2016E", "SingleMuon_Run2016F", "SingleMuon_Run2016G", "SingleMuon_Run2016H"]:
    dataSets.append([d, open(listDir+"/"+d+".txt").read().splitlines()])

for d in ["SingleElectron_Run2016B", "SingleElectron_Run2016C", "SingleElectron_Run2016D", "SingleElectron_Run2016E", "SingleElectron_Run2016F", "SingleElectron_Run2016G", "SingleElectron_Run2016H"]:
    dataSets.append([d, open(listDir+"/"+d+".txt").read().splitlines()])

#for d in ["MET_Run2016B", "MET_Run2016C", "MET_Run2016D", "MET_Run2016E", "MET_Run2016F", "MET_Run2016G", "MET_Run2016H"]:
#    dataSets.append([d, open(listDir+"/"+d+".txt").read().splitlines()])

#for d in ["JetHT_Run2016B", "JetHT_Run2016C", "JetHT_Run2016D", "JetHT_Run2016E", "JetHT_Run2016F", "JetHT_Run2016G", "JetHT_Run2016H"]:
#    dataSets.append([d, open(listDir+"/"+d+".txt").read().splitlines()])


userItems = [
    ["IsData", "true"],
    ["IsSignal", "false"],
]


jobOptionsFile2=open("AnalysisOptions.py", 'r')
command2=""
for i in [o for o in jobOptionsFile2.readlines()]:
  if ("#E" + "nd") in i : break
  command2+=i
jobOptionsFile2.close()
exec command2
userItems += AddUserItems


inputTrees=["ntuplizer/tree"]
outputTrees=["tree"]
