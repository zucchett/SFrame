// Dear emacs, this is -*- c++ -*-
// $Id: TauNtupleObject.h 37457 2010-07-05 12:04:33Z mann $

// THIS FILE HAS BEEN GENERATED AUTOMATICALLY. DO NOT EDIT DIRECTLY, CHANGES WILL BE LOST UPON NEXT CODE GENERATION.
// Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn 


#ifndef SFRAME_NtupleVARIABLES_TauNtupleObject_H
#define SFRAME_NtupleVARIABLES_TauNtupleObject_H

// Local include(s):
#include "NtupleObjectNames.h"

// STL include(s):
#include <vector>
#include <string>

// ROOT include(s):
#include <TString.h>

// SFrame include(s):
#include "core/include/SError.h"
#include "core/include/SCycleBaseNTuple.h"
#include "plug-ins/include/SInputVariables.h"

namespace Ntuple {

  /**
  *  @short Class that can read the variables produced by TauNtupleObject
  *
  *         This class can be used to read the offline muon information from
  *         an ntuple produced by the SingleTopDPDMaker code.
  *
  * @author Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn   
  *
  */
  
  enum TauDetails {
    TauBasic = 1,
    TauID = 2,
    TauAdvancedID = 4,
    TauAll = 7,

  };
  
  // forward declaration of NtupleObjectNames
  class NtupleObjectNames;
  class TauNtupleObject : public SInputVariables< SCycleBaseNTuple > {

    public:
    /// Constructor specifying the parent of the object
    TauNtupleObject( SCycleBaseNTuple* parent );

    /// remember if connect succeeded
    void setConnectSucceeded(const unsigned int index, const bool success);
    bool getConnectSucceeded(const unsigned int index) const {return m_connectsucceeded.at(index);}  

    /// Connect the variables to the input branches
    void ConnectVariables( const TString& treeName,
                           UInt_t detail_level = 0,
                           const TString& prefix = "Tau_",
                           const TString& ntupleType = "NtupleMakerNtuple" ) throw( SError );

    void ConnectVariables( const TString& treeName,
                           const TString& prefix = "Tau_",
                           const TString& ntupleType = "NtupleMakerNtuple" ) throw( SError );

    int getDetailLevel() const {return detailLevel;}   


    // particle vector size
    Int_t                   N;
   
    enum ConnectionIndex { 
     kdecayModeFindingNewDMs=5, 
     kdecayModeFinding=6, 
     kbyLooseCombinedIsolationDeltaBetaCorr3Hits=7, 
     kbyMediumCombinedIsolationDeltaBetaCorr3Hits=8, 
     kbyTightCombinedIsolationDeltaBetaCorr3Hits=9, 
     kbyCombinedIsolationDeltaBetaCorrRaw3Hits=10, 
     kchargedIsoPtSum=11, 
     kneutralIsoPtSum=12, 
     kpuCorrPtSum=13, 
     kchargedIsoPtSumdR03=14, 
     kfootprintCorrectiondR03=15, 
     kneutralIsoPtSumdR03=16, 
     kneutralIsoPtSumWeight=17, 
     kneutralIsoPtSumWeightdR03=18, 
     kphotonPtSumOutsideSignalConedR03=19, 
     kbyIsolationMVArun2v1DBdR03oldDMwLTraw=20, 
     kbyIsolationMVArun2v1DBnewDMwLTraw=21, 
     kbyIsolationMVArun2v1DBoldDMwLTraw=22, 
     kbyIsolationMVArun2v1PWdR03oldDMwLTraw=23, 
     kbyIsolationMVArun2v1PWnewDMwLTraw=24, 
     kbyIsolationMVArun2v1PWoldDMwLTraw=25, 
     kbyLooseIsolationMVArun2v1DBdR03oldDMwLT=26, 
     kbyLooseIsolationMVArun2v1DBnewDMwLT=27, 
     kbyLooseIsolationMVArun2v1DBoldDMwLT=28, 
     kbyLooseIsolationMVArun2v1PWdR03oldDMwLT=29, 
     kbyLooseIsolationMVArun2v1PWnewDMwLT=30, 
     kbyLooseIsolationMVArun2v1PWoldDMwLT=31, 
     kbyMediumIsolationMVArun2v1DBdR03oldDMwLT=32, 
     kbyMediumIsolationMVArun2v1DBnewDMwLT=33, 
     kbyMediumIsolationMVArun2v1DBoldDMwLT=34, 
     kbyMediumIsolationMVArun2v1PWdR03oldDMwLT=35, 
     kbyMediumIsolationMVArun2v1PWnewDMwLT=36, 
     kbyMediumIsolationMVArun2v1PWoldDMwLT=37, 
     kbyTightIsolationMVArun2v1DBdR03oldDMwLT=38, 
     kbyTightIsolationMVArun2v1DBnewDMwLT=39, 
     kbyTightIsolationMVArun2v1DBoldDMwLT=40, 
     kbyTightIsolationMVArun2v1PWdR03oldDMwLT=41, 
     kbyTightIsolationMVArun2v1PWnewDMwLT=42, 
     kbyTightIsolationMVArun2v1PWoldDMwLT=43, 
     kbyVLooseIsolationMVArun2v1DBdR03oldDMwLT=44, 
     kbyVLooseIsolationMVArun2v1DBnewDMwLT=45, 
     kbyVLooseIsolationMVArun2v1DBoldDMwLT=46, 
     kbyVLooseIsolationMVArun2v1PWdR03oldDMwLT=47, 
     kbyVLooseIsolationMVArun2v1PWnewDMwLT=48, 
     kbyVLooseIsolationMVArun2v1PWoldDMwLT=49, 
     kbyVTightIsolationMVArun2v1DBdR03oldDMwLT=50, 
     kbyVTightIsolationMVArun2v1DBnewDMwLT=51, 
     kbyVTightIsolationMVArun2v1DBoldDMwLT=52, 
     kbyVTightIsolationMVArun2v1PWdR03oldDMwLT=53, 
     kbyVTightIsolationMVArun2v1PWnewDMwLT=54, 
     kbyVTightIsolationMVArun2v1PWoldDMwLT=55, 
     kbyVVTightIsolationMVArun2v1DBdR03oldDMwLT=56, 
     kbyVVTightIsolationMVArun2v1DBnewDMwLT=57, 
     kbyVVTightIsolationMVArun2v1DBoldDMwLT=58, 
     kbyVVTightIsolationMVArun2v1PWdR03oldDMwLT=59, 
     kbyVVTightIsolationMVArun2v1PWnewDMwLT=60, 
     kbyVVTightIsolationMVArun2v1PWoldDMwLT=61, 
     kagainstElectronMVA6raw=62, 
     kagainstElectronMVA6category=63, 
     kagainstElectronVLooseMVA6=64, 
     kagainstElectronLooseMVA6=65, 
     kagainstElectronMediumMVA6=66, 
     kagainstElectronTightMVA6=67, 
     kagainstElectronVTightMVA6=68, 
     kagainstMuonLoose3=69, 
     kagainstMuonTight3=70, 
     kbyPhotonPtSumOutsideSignalCone=71, 
     kfootprintCorrection=72, 
     kpdgId=1, 
     kcharge=2, 
     kd0=3, 
     kTauType=4, 
 
      kEnd 
    }; 


    // vectors of properties defined in Particle.h
    std::vector< floatingnumber >  *e;
    std::vector< floatingnumber >  *pt;
    std::vector< floatingnumber >  *eta;
    std::vector< floatingnumber >  *phi;
    std::vector< floatingnumber >  *m;
    

    
    // vectors of object specific variables
    std::vector< floatingnumber >  *decayModeFindingNewDMs;
    std::vector< floatingnumber >  *decayModeFinding;
    std::vector< floatingnumber >  *byLooseCombinedIsolationDeltaBetaCorr3Hits;
    std::vector< floatingnumber >  *byMediumCombinedIsolationDeltaBetaCorr3Hits;
    std::vector< floatingnumber >  *byTightCombinedIsolationDeltaBetaCorr3Hits;
    std::vector< floatingnumber >  *byCombinedIsolationDeltaBetaCorrRaw3Hits;
    std::vector< floatingnumber >  *chargedIsoPtSum;
    std::vector< floatingnumber >  *neutralIsoPtSum;
    std::vector< floatingnumber >  *puCorrPtSum;
    std::vector< floatingnumber >  *chargedIsoPtSumdR03;
    std::vector< floatingnumber >  *footprintCorrectiondR03;
    std::vector< floatingnumber >  *neutralIsoPtSumdR03;
    std::vector< floatingnumber >  *neutralIsoPtSumWeight;
    std::vector< floatingnumber >  *neutralIsoPtSumWeightdR03;
    std::vector< floatingnumber >  *photonPtSumOutsideSignalConedR03;
    std::vector< floatingnumber >  *byIsolationMVArun2v1DBdR03oldDMwLTraw;
    std::vector< floatingnumber >  *byIsolationMVArun2v1DBnewDMwLTraw;
    std::vector< floatingnumber >  *byIsolationMVArun2v1DBoldDMwLTraw;
    std::vector< floatingnumber >  *byIsolationMVArun2v1PWdR03oldDMwLTraw;
    std::vector< floatingnumber >  *byIsolationMVArun2v1PWnewDMwLTraw;
    std::vector< floatingnumber >  *byIsolationMVArun2v1PWoldDMwLTraw;
    std::vector< floatingnumber >  *byLooseIsolationMVArun2v1DBdR03oldDMwLT;
    std::vector< floatingnumber >  *byLooseIsolationMVArun2v1DBnewDMwLT;
    std::vector< floatingnumber >  *byLooseIsolationMVArun2v1DBoldDMwLT;
    std::vector< floatingnumber >  *byLooseIsolationMVArun2v1PWdR03oldDMwLT;
    std::vector< floatingnumber >  *byLooseIsolationMVArun2v1PWnewDMwLT;
    std::vector< floatingnumber >  *byLooseIsolationMVArun2v1PWoldDMwLT;
    std::vector< floatingnumber >  *byMediumIsolationMVArun2v1DBdR03oldDMwLT;
    std::vector< floatingnumber >  *byMediumIsolationMVArun2v1DBnewDMwLT;
    std::vector< floatingnumber >  *byMediumIsolationMVArun2v1DBoldDMwLT;
    std::vector< floatingnumber >  *byMediumIsolationMVArun2v1PWdR03oldDMwLT;
    std::vector< floatingnumber >  *byMediumIsolationMVArun2v1PWnewDMwLT;
    std::vector< floatingnumber >  *byMediumIsolationMVArun2v1PWoldDMwLT;
    std::vector< floatingnumber >  *byTightIsolationMVArun2v1DBdR03oldDMwLT;
    std::vector< floatingnumber >  *byTightIsolationMVArun2v1DBnewDMwLT;
    std::vector< floatingnumber >  *byTightIsolationMVArun2v1DBoldDMwLT;
    std::vector< floatingnumber >  *byTightIsolationMVArun2v1PWdR03oldDMwLT;
    std::vector< floatingnumber >  *byTightIsolationMVArun2v1PWnewDMwLT;
    std::vector< floatingnumber >  *byTightIsolationMVArun2v1PWoldDMwLT;
    std::vector< floatingnumber >  *byVLooseIsolationMVArun2v1DBdR03oldDMwLT;
    std::vector< floatingnumber >  *byVLooseIsolationMVArun2v1DBnewDMwLT;
    std::vector< floatingnumber >  *byVLooseIsolationMVArun2v1DBoldDMwLT;
    std::vector< floatingnumber >  *byVLooseIsolationMVArun2v1PWdR03oldDMwLT;
    std::vector< floatingnumber >  *byVLooseIsolationMVArun2v1PWnewDMwLT;
    std::vector< floatingnumber >  *byVLooseIsolationMVArun2v1PWoldDMwLT;
    std::vector< floatingnumber >  *byVTightIsolationMVArun2v1DBdR03oldDMwLT;
    std::vector< floatingnumber >  *byVTightIsolationMVArun2v1DBnewDMwLT;
    std::vector< floatingnumber >  *byVTightIsolationMVArun2v1DBoldDMwLT;
    std::vector< floatingnumber >  *byVTightIsolationMVArun2v1PWdR03oldDMwLT;
    std::vector< floatingnumber >  *byVTightIsolationMVArun2v1PWnewDMwLT;
    std::vector< floatingnumber >  *byVTightIsolationMVArun2v1PWoldDMwLT;
    std::vector< floatingnumber >  *byVVTightIsolationMVArun2v1DBdR03oldDMwLT;
    std::vector< floatingnumber >  *byVVTightIsolationMVArun2v1DBnewDMwLT;
    std::vector< floatingnumber >  *byVVTightIsolationMVArun2v1DBoldDMwLT;
    std::vector< floatingnumber >  *byVVTightIsolationMVArun2v1PWdR03oldDMwLT;
    std::vector< floatingnumber >  *byVVTightIsolationMVArun2v1PWnewDMwLT;
    std::vector< floatingnumber >  *byVVTightIsolationMVArun2v1PWoldDMwLT;
    std::vector< floatingnumber >  *againstElectronMVA6raw;
    std::vector< floatingnumber >  *againstElectronMVA6category;
    std::vector< floatingnumber >  *againstElectronVLooseMVA6;
    std::vector< floatingnumber >  *againstElectronLooseMVA6;
    std::vector< floatingnumber >  *againstElectronMediumMVA6;
    std::vector< floatingnumber >  *againstElectronTightMVA6;
    std::vector< floatingnumber >  *againstElectronVTightMVA6;
    std::vector< floatingnumber >  *againstMuonLoose3;
    std::vector< floatingnumber >  *againstMuonTight3;
    std::vector< floatingnumber >  *byPhotonPtSumOutsideSignalCone;
    std::vector< floatingnumber >  *footprintCorrection;
    std::vector< int >  *pdgId;
    std::vector< floatingnumber >  *charge;
    std::vector< floatingnumber >  *d0;
    std::vector< int >  *TauType;


    std::vector<int> m_connectsucceeded;

    // save actual detail level and group
    Int_t detailLevel;
    std::string detailGroup;

  }; // class TauNtupleObject

} // namespace Ntuple

#endif // SFRAME_NtupleVARIABLES_TauNtupleObject_H
