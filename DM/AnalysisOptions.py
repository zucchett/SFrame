loadLibs=[
  #"libGenVector",
    "libNtupleVariables",
    "libGoodRunsLists",
    "libPileupReweightingTool",
    "libBTaggingTools",
    "libDMSearches",
#    "libSVFit",
    "libScaleFactorTool",
    "libCorrectionTool",
    "libVariableTool",
]


loadPacks=[
    "SFrameCore.par",
    "NtupleVariables.par",
    "GoodRunsLists.par",
    "PileupReweightingTool.par",
    "BTaggingTools.par",
    "DMSearches.par",
#    "SVFit.par",
    "ScaleFactorTool.par",
    "CorrectionTool.par",
    "VariableTool.par",
]

compilePacks=[
  "/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/SFrame/../NtupleVariables",
  "/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/SFrame/../GoodRunsLists",
  "/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/SFrame/../PileupReweightingTool",
  "/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/SFrame/../BTaggingTools",
#  "/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/SFrame/../SVFit",
  "/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/SFrame/../DMSearches",
  "/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/SFrame/../ScaleFactorTool",
  "/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/SFrame/../CorrectionTool",
  "/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/SFrame/../VariableTool",
]

AddUserItems = [
    #general settings
    ["RecoTreeName", "tree"],
    ["OutputTreeName", "tree"],
#    ["JSONFileName", "/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/SFrame/../GoodRunsLists/JSON/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt"],#12.7
#    ["JSONFileName", "/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/SFrame/../GoodRunsLists/JSON/Cert_271036-280385_13TeV_PromptReco_Collisions16_JSON_NoL1T_v2.txt"],#27.2
    ["JSONFileName", "/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/SFrame/../GoodRunsLists/JSON/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt"],#36.773
]


#End
