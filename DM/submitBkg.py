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
#for d in ["WJetsToLNu_Pt-100To250", "WJetsToLNu_Pt-250To400", "WJetsToLNu_Pt-400To600", "WJetsToLNu_Pt-400To650", "WJetsToLNu_Pt-600ToInf"]:
for d in ["DYJetsToLL", "DYJetsToLL_HT-100to200", "DYJetsToLL_HT-200to400", "DYJetsToLL_HT-400to600", "DYJetsToLL_HT-600to800", "DYJetsToLL_HT-800to1200", "DYJetsToLL_HT-1200to2500", "DYJetsToLL_HT-2500toInf", "WJetsToLNu", "WJetsToLNu_HT-70To100", "WJetsToLNu_HT-100To200", "WJetsToLNu_HT-200To400", "WJetsToLNu_HT-400To600", "WJetsToLNu_HT-600To800", "WJetsToLNu_HT-800To1200", "WJetsToLNu_HT-1200To2500", "WJetsToLNu_HT-2500ToInf", "ZJetsToNuNu_HT-100To200", "ZJetsToNuNu_HT-200To400", "ZJetsToNuNu_HT-400To600", "ZJetsToNuNu_HT-600To800", "ZJetsToNuNu_HT-800To1200", "ZJetsToNuNu_HT-1200To2500", "ZJetsToNuNu_HT-2500ToInf", "WWTo4Q", "WWTo2L2Nu", "WWTo1L1Nu2Q", "WZTo1L1Nu2Q", "WZTo2L2Q", "WZTo2Q2Nu", "ZZTo2L2Q", "ZZTo2L2Nu", "ZZTo2Q2Nu", "ZZTo4Q", "ZZTo4L", "ZHToLLBB", "ZHToNuNuBB", "ZHToQQBB", "WHToLNuBB", "GluGluHToBB"]:
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
