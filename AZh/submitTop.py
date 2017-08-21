path2xml="$SFRAME_DIR/../BatchSubmission/xmls_ReMiniAOD"
path2tmp="$SFRAME_DIR/../AZh/AnalysisTemp"
outDir="$SFRAME_DIR/../AZh/AnalysisOutput"
jobName="AZhAnalysisJob"
cycleName="AZhAnalysis"
outputLevel="WARNING"
nEventsMax=-1
nProcesses=1
nFiles=20
hCPU="06:00:00"
hVMEM="1500M"
postFix = ""


dataSets = []
listDir = "../BatchSubmission/lists_ReMiniAOD"
for d in ["TT", "TTToSemiLeptonic", "TTTo2L2Nu", "ST_s-channel", "ST_t-channel_antitop", "ST_t-channel_top", "ST_tW_antitop", "ST_tW_top", "TTWJetsToLNu", "TTZToLLNuNu"]:
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
