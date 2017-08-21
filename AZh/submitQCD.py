path2xml="$SFRAME_DIR/../BatchSubmission/xmls_ReMiniAOD"
path2tmp="$SFRAME_DIR/../AZh/AnalysisTemp"
outDir="$SFRAME_DIR/../AZh/AnalysisOutput"
jobName="AZhAnalysisJob"
cycleName="AZhAnalysis"
outputLevel="WARNING"
nEventsMax=-1
nProcesses=1
nFiles=10
hCPU="06:00:00"
hVMEM="1500M"
postFix = ""


dataSets = []
listDir = "../BatchSubmission/lists_ReMiniAOD"
for d in ["QCD_HT100to200", "QCD_HT200to300", "QCD_HT300to500", "QCD_HT500to700", "QCD_HT700to1000", "QCD_HT1000to1500", "QCD_HT1500to2000", "QCD_HT2000toInf"]:
    dataSets.append([d, open(listDir+"/"+d+".txt").read().splitlines()])


userItems = [
    ["IsData", "false"],
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
