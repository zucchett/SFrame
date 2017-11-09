path2xml="$SFRAME_DIR/../BatchSubmission/xmls_ReMiniAOD"
path2tmp="$SFRAME_DIR/../DM/AnalysisTemp"
outDir="$SFRAME_DIR/../DM/AnalysisOutput"
jobName="DMAnalysisJob"
cycleName="DMAnalysis"
outputLevel="WARNING"
nEventsMax=-1
nProcesses=1
nFiles=38
hCPU="06:00:00"
hVMEM="1500M"
postFix = ""


dataSets = []
listDir = "../BatchSubmission/lists_ReMiniAOD"
for d in ["TTbarDMJets_pseudoscalar_Mchi-10_Mphi-100", "TTbarDMJets_pseudoscalar_Mchi-10_Mphi-10", "TTbarDMJets_pseudoscalar_Mchi-10_Mphi-15","TTbarDMJets_pseudoscalar_Mchi-10_Mphi-50","TTbarDMJets_pseudoscalar_Mchi-1_Mphi-10000","TTbarDMJets_pseudoscalar_Mchi-1_Mphi-1000","TTbarDMJets_pseudoscalar_Mchi-1_Mphi-100","TTbarDMJets_pseudoscalar_Mchi-1_Mphi-10","TTbarDMJets_pseudoscalar_Mchi-1_Mphi-200","TTbarDMJets_pseudoscalar_Mchi-1_Mphi-20","TTbarDMJets_pseudoscalar_Mchi-1_Mphi-300","TTbarDMJets_pseudoscalar_Mchi-1_Mphi-500","TTbarDMJets_pseudoscalar_Mchi-1_Mphi-50","TTbarDMJets_pseudoscalar_Mchi-50_Mphi-10","TTbarDMJets_pseudoscalar_Mchi-50_Mphi-200","TTbarDMJets_pseudoscalar_Mchi-50_Mphi-20","TTbarDMJets_pseudoscalar_Mchi-50_Mphi-300","TTbarDMJets_pseudoscalar_Mchi-50_Mphi-50","TTbarDMJets_pseudoscalar_Mchi-50_Mphi-95","TTbarDMJets_scalar_Mchi-10_Mphi-100","TTbarDMJets_scalar_Mchi-10_Mphi-10","TTbarDMJets_scalar_Mchi-10_Mphi-15","TTbarDMJets_scalar_Mchi-10_Mphi-50","TTbarDMJets_scalar_Mchi-1_Mphi-10000","TTbarDMJets_scalar_Mchi-1_Mphi-1000","TTbarDMJets_scalar_Mchi-1_Mphi-100","TTbarDMJets_scalar_Mchi-1_Mphi-10","TTbarDMJets_scalar_Mchi-1_Mphi-200","TTbarDMJets_scalar_Mchi-1_Mphi-20","TTbarDMJets_scalar_Mchi-1_Mphi-300","TTbarDMJets_scalar_Mchi-1_Mphi-500","TTbarDMJets_scalar_Mchi-1_Mphi-50","TTbarDMJets_scalar_Mchi-50_Mphi-10","TTbarDMJets_scalar_Mchi-50_Mphi-200","TTbarDMJets_scalar_Mchi-50_Mphi-20","TTbarDMJets_scalar_Mchi-50_Mphi-300","TTbarDMJets_scalar_Mchi-50_Mphi-50","TTbarDMJets_scalar_Mchi-50_Mphi-95"]:
    dataSets.append([d, open(listDir+"/"+d+".txt").read().splitlines()])


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


inputTrees=["ntuplizer/tree"]
outputTrees=["tree"]
