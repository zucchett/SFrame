#! /usr/bin/env python

#(abs(lepton1_eta)<1.4442 || abs(lepton1_eta)>1.566) && 
#0.9535

selection = {

    ##Dilepton category
    # "DL2eSR" : "isZtoEE && nElectrons==2 && nMuons==0 && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && Lepton2_pt>30. && abs(Lepton2_eta)<2.1 && Lepton2_id==4 && (nJets==1 || nJets==2) && nBTagJets==1 && (abs(Lepton1_eta)<1.4442 || abs(Lepton1_eta)>1.566) && (abs(Lepton2_eta)<1.4442 || abs(Lepton2_eta)>1.566) && MET_pt>160 ",
    # "DL2mSR" : "isZtoMM && nElectrons==0 && nMuons==2 && Lepton1_pt>30. && Lepton1_id==4 && Lepton2_pt>30. && Lepton2_id==4 && Lepton1_pfIso<0.15 && Lepton2_pfIso<0.15 && (nJets==1 || nJets==2) && nBTagJets==1 && MET_pt>160",
    #"DL1e1mSR" : "isTtoEM && nElectrons==1 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 &&  Lepton2_pt>30. && Lepton2_pfIso<0.15 && Lepton2_id==4 && nJets>1 && nBTagJets>0 && (abs(Lepton1_eta)<1.4442 || abs(Lepton1_eta)>1.566)",  
    
    ## Hadronic category
    
    #pre-selection and N-1
    # "AH0lP_0F" : "isZtoNN && MET_pt>250. && nJets>=3 && nBTagJets==1 && nForwardJets==0",
    # "AH0lP_1F" : "isZtoNN && MET_pt>250. && nJets>=3 && nBTagJets==1 && nForwardJets>=1",
    # "AH0lPphi_0F" : "isZtoNN && MET_pt>250. && nJets>=3 && nBTagJets==1 && nForwardJets==0 && MinDPhi12>1",
    # "AH0lPphi_1F" : "isZtoNN && MET_pt>250. && nJets>=3 && nBTagJets==1 && nForwardJets>=1 && MinDPhi12>1",
    # "AH0lPdr_0F" : "isZtoNN && MET_pt>250. && nJets>=3 && nBTagJets==1 && nForwardJets==0 && MinDPhi12>1 && min_DeltaR<2",
    # "AH0lPdr_1F" : "isZtoNN && MET_pt>250. && nJets>=3 && nBTagJets==1 && nForwardJets>=1 && MinDPhi12>1 && min_DeltaR<2",
    # "AH0lPmtb_0F" : "isZtoNN && MET_pt>250. && nJets>=3 && nBTagJets==1 && nForwardJets==0 && MinDPhi12>1 && mTb>200",
    # "AH0lPmtb_1F" : "isZtoNN && MET_pt>250. && nJets>=3 && nBTagJets==1 && nForwardJets>=1 && MinDPhi12>1 && mTb>200",

    #SR
    "AH0lSR_0F" : "isZtoNN && MET_pt>250. && nJets>=3 && nBTagJets==1 && nForwardJets==0 && MinDPhi12>1",
    "AH0lSR_1F" : "isZtoNN && MET_pt>250. && nJets>=3 && nBTagJets==1 && nForwardJets>=1 && MinDPhi12>1",

    # #CRs
    # "AH1eWR" : "isWtoEN && nElectrons==1 && nMuons==0 && nTaus==0 && MET_pt>250. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && mT<160. && nJets>=3 && nBTagJets==0",
    # "AH1mWR" : "isWtoMN && nElectrons==0 && nMuons==1 && nTaus==0 && MET_pt>250. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_pfIso<0.15 && Lepton1_id==4 && mT<160. && nJets>=3 && nBTagJets==0",
    
    # "AH2eZR" : "isZtoEE && FakeMET_pt>250. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && (mZ>60. && mZ<120.) && nJets>=3 && nBTagJets==0 && Lepton2_pt>30. && abs(Lepton2_eta)<2.1 && Lepton2_id==4 && nMuons==0 && nElectrons==2",
    # "AH2mZR" : "isZtoMM && FakeMET_pt>250. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && (mZ>60. && mZ<120.) && nJets>=3 && nBTagJets==0 && Lepton2_pt>30. && Lepton2_id==4 && Lepton2_pfIso<0.15 && nMuons==2 && nElectrons==0",
    
    # "AH1eTR" : "isWtoEN && MET_pt>250. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && mT<160. && nJets>=3 && nBTagJets>=1 && nElectrons==1 && nMuons==0 && MinDPhi>1.",
    # "AH1mTR" : "isWtoMN && MET_pt>250. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && mT<160. && nJets>=3 && nBTagJets>=1  && nElectrons==0 && nMuons==1 && MinDPhi>1.",
    
    ## Semileptonic category
    
    #pre-selection and N-1
    # "SL1mP_0F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets==0",
    # "SL1mP_1F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets>=1",
    # "SL1mPmt_0F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets==0 && mT>160 ",
    # "SL1mPmt_1F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets>=1 && mT>160 ",
    # "SL1mPmt2_0F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets==0 && mT>160 && mT2>200",
    # "SL1mPmt2_1F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets>=1 && mT>160 && mT2>200",
    # "SL1mPphi_0F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets==0 && mT>160 && mT2>200 && MinDPhi12>1.2",
    # "SL1mPphi_1F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets>=1 && mT>160 && mT2>200 && MinDPhi12>1.2",
    # "SL1mPmtb_0F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets==0 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180",
    # "SL1mPmtb_1F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets>=1 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180",
    # "SL1mPht_0F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets==0 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180 && HT>200",
    # "SL1mPht_1F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets>=1 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180 && HT>200",
    # "SL1eP_0F" : "isWtoEN && nElectrons==1 && nMuons==0 && MET_pt>160. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets==0",
    # "SL1eP_1F" : "isWtoEN && nElectrons==1 && nMuons==0 && MET_pt>160. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets>=1",
    
    #SR
    "SL1mSR_0F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets==0 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180 && HT>200",
    "SL1mSR_1F" : "isWtoMN && nElectrons==0 && nMuons==1 && MET_pt>160. && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets>=1 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180",
    "SL1eSR_0F" : "isWtoEN && nElectrons==1 && nMuons==0 && MET_pt>160. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets==0 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180 && HT>200",
    "SL1eSR_1F" : "isWtoEN && nElectrons==1 && nMuons==0 && MET_pt>160. && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && nJets>=2 && nBTagJets==1 && nForwardJets>=1 && mT>160 && mT2>200 && MinDPhi12>1.2 && mTb>180",
    
    # #CRs
    # "SL1mWR"  : "isWtoMN && nElectrons==0 && nMuons==1 && Lepton1_pt>30. && Lepton1_pfIso<0.15 && Lepton1_id==4  && nJets>=2 && nBTagJets==0 && MET_pt>160. && mT>160",
    # "SL1eWR"  : "isWtoEN && nElectrons==1 && nMuons==0 && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4  && nJets>=2 && nBTagJets==0 && MET_pt>160. && mT>160",

    # "SL1e1mTR" : "isTtoEM && nElectrons==1 && nMuons==1 && Lepton1_pt>30. && abs(Lepton1_eta)<2.1 && Lepton1_id==4 && Lepton2_pt>30. && Lepton2_pfIso<0.15 && Lepton2_id==4 && nJets>=2 && nBTagJets>=1 && MET_pt>160.",
    
}

