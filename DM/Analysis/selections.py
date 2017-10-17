#! /usr/bin/env python

#(abs(lepton1_eta)<1.4442 || abs(lepton1_eta)>1.566) && 
#0.9535

selection = {
    # Hadronic category
    "AH0lSR" : "isZtoNN && MET_pt>250. && MinDPhi>1. && nJets>=3 && nBTagJets>=1",
    "AH0lCR" : "isZtoNN && MET_pt>250. && MinDPhi>1. && nJets>=3 && nBTagJets==0",
    "AH1eWR" : "isWtoEN && nElectrons==1 && nMuons==0 && nTaus==0 && MET_pt>250. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && mT<160. && nJets>=3 && nBTagJets==0 && (abs(Lepton1_eta)<1.4442 || abs(Lepton1_eta)>1.566)",
    "AH1mWR" : "isWtoMN && nElectrons==0 && nMuons==1 && nTaus==0 && MET_pt>250. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_pfIso<0.15 && Lepton1_id==4 && mT<160. && nJets>=3 && nBTagJets==0",
    "AH2eZR" : "isZtoEE && FakeMET_pt>250. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && (mZ>60. && mZ<120.) && nJets>=3 && nBTagJets==0 && Lepton2_pt>30. && abs(Lepton2_eta)<2.1 && Lepton2_id==4 && nMuons==0 && nElectrons==2  && (abs(Lepton1_eta)<1.4442 || abs(Lepton1_eta)>1.566) && (abs(Lepton2_eta)<1.4442 || abs(Lepton2_eta)>1.566)",
    "AH2mZR" : "isZtoMM && FakeMET_pt>250. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_pfIso<0.15 && Lepton1_id==4 && (mZ>60. && mZ<120.) && nJets>=3 && nBTagJets==0 && Lepton2_pt>30. && abs(Lepton2_eta)<2.1 && Lepton2_id==4 && Lepton2_pfIso<0.15 && nMuons==2 && nElectrons==0",
    "AH1eTR" : "isWtoEN && MET_pt>250. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && mT<160. && nJets>=3 && nBTagJets>=1 && nElectrons==1 && nMuons==0 && MinDPhi>1. && (abs(Lepton1_eta)<1.4442 || abs(Lepton1_eta)>1.566)",
    "AH1mTR" : "isWtoMN && MET_pt>250. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && mT<160. && nJets>=3 && nBTagJets>=1  && nElectrons==0 && nMuons==1 && MinDPhi>1.",
    
    # Semileptonic category
    "SL1eSR" : "isWtoEN && nElectrons==1 && nMuons==0 && nTaus==0 && MET_pt>160. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && nJets>=2 && nBTagJets>=1 && MinDPhi12>1.2 && mT>160. && mT2>200. && (abs(Lepton1_eta)<1.4442 || abs(Lepton1_eta)>1.566)",
    "SL1mSR" : "isWtoMN && nElectrons==0 && nMuons==1 && nTaus==0 && MET_pt>160. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets>=1 && MinDPhi12>1.2 && mT>160. && mT2>200.",
    #"SL1lSR" : "(isWtoEN || isWtoMN) && nJets>=2 && nBTagJets>=1 && mT>160. && mT2>200.", # don't use this
    "SL1eWR" : "isWtoEN && nElectrons==1 && nMuons==0 && nTaus==0 && MET_pt>160. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && nJets>=2 && nBTagJets==0 && mT>160. && (abs(Lepton1_eta)<1.4442 || abs(Lepton1_eta)>1.566)",
    "SL1mWR" : "isWtoMN && nElectrons==0 && nMuons==1 && nTaus==0 && MET_pt>160. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==0 && mT>160.",
    "SL1e1mTR" : "isTtoEM && nElectrons==1 && nMuons==1 && nTaus==0 && MET_pt>160. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && Lepton2_pt>30. && abs(Lepton2_eta)<2.1 && Lepton2_pfIso<0.15 && Lepton2_id==4 && nJets>=2 && nBTagJets>=1 && (abs(Lepton1_eta)<1.4442 || abs(Lepton1_eta)>1.566)",
    # Z control region, not sure it really matters

    # ttDM
#    "0lSR" : "isZtoNN && nJets>=4 && nBTagJets>=2 && MinDPhi>1.0 && MET_pt>200.",
#    "1eSR" : "isWtoEN && nJets>=3 && nBTagJets>=1 && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && mT>160. && mT2>200. && MET_pt>160.",
#    "1mSR" : "isWtoMN && nJets>=3 && nBTagJets>=1 && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && mT>160. && mT2>200. && MET_pt>160.",
#    "1lSR" : "isWtoEN && nJets>=3 && nBTagJets>=1 && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && mT>160. && mT2>200. && MET_pt>160.",
}

