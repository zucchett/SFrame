#! /usr/bin/env python

#(abs(lepton1_eta)<1.4442 || abs(lepton1_eta)>1.566) && 
#0.9535

selection = {
    
    # Hadronic category
    #SRs - 2b
    "AH0l2bSR" : "isZtoNN && MET_pt>250. && nJets>=3 && nBTagJets>=2 && MinDPhi12>1 && mTb>180 && Jet1_pt/HT<0.5",

    #SRs - 1b
    "AH0l0fSR" : "isZtoNN && MET_pt>250. && nJets>=3 && nBTagJets==1 && nForwardJets==0 && MinDPhi12>1 && mTb>180",
    "AH0l1fSR" : "isZtoNN && MET_pt>250. && nJets>=3 && nBTagJets==1 && nForwardJets>=1 && MinDPhi12>1 && mTb>180",

    #W CR
    "AH1eWR" : "isWtoEN && nElectrons==1 && nMuons==0 && nTaus==0 && MET_pt>250. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && mT<160. && nJets>=3 && nBTagJets==0",
    "AH1mWR" : "isWtoMN && nElectrons==0 && nMuons==1 && nTaus==0 && MET_pt>250. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && mT<160. && nJets>=3 && nBTagJets==0",
    
    #Znunu CR
    "AH2eZR" : "isZtoEE && FakeMET_pt>250. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && (mZ>60. && mZ<120.) && nJets>=3 && nBTagJets==0 && Lepton2_pt>30. && abs(Lepton2_eta)<2.1 && Lepton2_id==4 && nMuons==0 && nElectrons==2",
    "AH2mZR" : "isZtoMM && FakeMET_pt>250. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && (mZ>60. && mZ<120.) && nJets>=3 && nBTagJets==0 && Lepton2_pt>30. && Lepton2_id==4 && Lepton2_pfIso<0.15 && nMuons==2 && nElectrons==0",
    
    #tt(1l) CR
    "AH1eTR" : "isWtoEN && MET_pt>250. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && mT<160. && nJets>=3 && nBTagJets>=1 && nElectrons==1 && nMuons==0 && MinDPhi>1.",
    "AH1mTR" : "isWtoMN && MET_pt>250. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && mT<160. && nJets>=3 && nBTagJets>=1  && nElectrons==0 && nMuons==1 && MinDPhi>1.",
    

    # Semileptonic category
    #SR - 2b
    "SL1m2bSR" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets>=2 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180",
    "SL1e2bSR" : "isWtoEN && nElectrons==1 && nMuons==0 && MET_pt>160. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && nJets>=2 && nBTagJets>=2 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180",

    #SR
    "SL1m0fSR" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets==0 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180",
    "SL1m1fSR" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets>=1 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180",

    "SL1e0fSR" : "isWtoEN && nElectrons==1 && nMuons==0 && MET_pt>160. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets==0 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180",
    "SL1e1fSR" : "isWtoEN && nElectrons==1 && nMuons==0 && MET_pt>160. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets>=1 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180",
    
    #W CRs
    "SL1mWR"  : "isWtoMN && nElectrons==0 && nMuons==1 && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4  && nJets>=2 && nBTagJets==0 && MET_pt>160. && mT>160",
    "SL1eWR"  : "isWtoEN && nElectrons==1 && nMuons==0 && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4  && nJets>=2 && nBTagJets==0 && MET_pt>160. && mT>160",

    #tt(2l) CR
    "SL2mTR" : "isZtoMM && nElectrons==0 && nMuons==2 && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && Lepton2_pt>30. && Lepton2_pfIso<0.15 && Lepton2_id==4 && nJets>=2 && nBTagJets>=1 && MET_pt>160.",
    "SL2eTR" : "isZtoEE && nElectrons==2 && nMuons==0 && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && Lepton2_pt>30. && abs(Lepton2_eta)<2.1 && Lepton2_id==4 && nJets>=2 && nBTagJets>=1 && MET_pt>160.",
    "SL1e1mTR" : "isTtoEM && nElectrons==1 && nMuons==1 && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && Lepton2_pt>30. && Lepton2_pfIso<0.15 && Lepton2_id==4 && nJets>=2 && nBTagJets>=1 && MET_pt>160.", 
    
}

