path2xml="$SFRAME_DIR/../BatchSubmission/xmls_ReMiniAOD"
path2tmp="$SFRAME_DIR/../AZh/AnalysisTemp"
outDir="$SFRAME_DIR/../AZh/AnalysisOutput"
jobName="AZhAnalysisJob"
cycleName="AZhAnalysis"
outputLevel="WARNING"
nEventsMax=-1
nProcesses=1
nFiles=5
hCPU="03:30:00"
hVMEM="1500M"
postFix = ""


dataSets = []
listDir = "../BatchSubmission/lists_ReMiniAOD"
for d in ["GluGluToAToZhToLLBB_MXXX"]:
    for m in ["225", "250", "275", "300", "325", "350", "400", "500", "600", "700", "750", "800", "900", "1000"]:
        s = d.replace("XXX", m)
        dataSets.append([s, open(listDir+"/"+s+".txt").read().splitlines()])

userItems = [
    ["IsData", "false"],
    ["IsSignal", "true"],
]


jobOptionsFile2=open("AnalysisOptions.py", 'r')
command2=""
for i in [o for o in jobOptionsFile2.readlines()]:
  if ("#E" + "nd") in i : break
  command2+=i
jobOptionsFile2.close()
exec command2
userItems += AddUserItems

#userItems += [ ["XMassLowerCut", 0.], ["XTMassLowerCut", 0.], ["XJJMassLowerCut", 0.], ]


inputTrees=["ntuplizer/tree"]
outputTrees=["tree"]
