// Dear emacs, this is -*- c++ -*-
// $Id: PuppiJetNtupleObject.h 37457 2010-07-05 12:04:33Z mann $

// THIS FILE HAS BEEN GENERATED AUTOMATICALLY. DO NOT EDIT DIRECTLY, CHANGES WILL BE LOST UPON NEXT CODE GENERATION.
// Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn 


#ifndef SFRAME_NtupleVARIABLES_PuppiJetNtupleObject_H
#define SFRAME_NtupleVARIABLES_PuppiJetNtupleObject_H

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
  *  @short Class that can read the variables produced by PuppiJetNtupleObject
  *
  *         This class can be used to read the offline muon information from
  *         an ntuple produced by the SingleTopDPDMaker code.
  *
  * @author Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn   
  *
  */
  
  enum PuppiJetDetails {
    PuppiJetSoftdropSubjets = 1,
    PuppiJetAnalysis = 2,
    PuppiJetBasic = 4,
    PuppiJetAll = 7,

  };
  
  // forward declaration of NtupleObjectNames
  class NtupleObjectNames;
  class PuppiJetNtupleObject : public SInputVariables< SCycleBaseNTuple > {

    public:
    /// Constructor specifying the parent of the object
    PuppiJetNtupleObject( SCycleBaseNTuple* parent );

    /// remember if connect succeeded
    void setConnectSucceeded(const unsigned int index, const bool success);
    bool getConnectSucceeded(const unsigned int index) const {return m_connectsucceeded.at(index);}  

    /// Connect the variables to the input branches
    void ConnectVariables( const TString& treeName,
                           UInt_t detail_level = 0,
                           const TString& prefix = "PuppiJet_",
                           const TString& ntupleType = "NtupleMakerNtuple" ) throw( SError );

    void ConnectVariables( const TString& treeName,
                           const TString& prefix = "PuppiJet_",
                           const TString& ntupleType = "NtupleMakerNtuple" ) throw( SError );

    int getDetailLevel() const {return detailLevel;}   

   
    enum ConnectionIndex { 
     kjetAK8_puppi_tau1=6, 
     kjetAK8_puppi_tau2=7, 
     kjetAK8_puppi_tau3=8, 
     kjetAK8_puppi_softdrop_mass=9, 
     kjetAK8_puppi_softdrop_massCorr=10, 
     kjetAK8_puppi_softdrop_jec=11, 
     kjetAK8_puppi_pt=1, 
     kjetAK8_puppi_eta=2, 
     kjetAK8_puppi_mass=3, 
     kjetAK8_puppi_phi=4, 
     kjetAK8_puppi_e=5, 
     kjetAK8_subjet_puppi_softdrop_N=12, 
     kjetAK8_subjet_puppi_softdrop_pt=13, 
     kjetAK8_subjet_puppi_softdrop_eta=14, 
     kjetAK8_subjet_puppi_softdrop_phi=15, 
     kjetAK8_subjet_puppi_softdrop_e=16, 
     kjetAK8_subjet_puppi_softdrop_m=17, 
     kjetAK8_subjet_puppi_softdrop_csv=18, 
     kjetAK8_subjet_puppi_softdrop_charge=19, 
 
      kEnd 
    }; 


    // vectors of properties defined in Particle.h
    

    
    // vectors of object specific variables
    std::vector< floatingnumber >  *jetAK8_puppi_tau1;
    std::vector< floatingnumber >  *jetAK8_puppi_tau2;
    std::vector< floatingnumber >  *jetAK8_puppi_tau3;
    std::vector< floatingnumber >  *jetAK8_puppi_softdrop_mass;
    std::vector< floatingnumber >  *jetAK8_puppi_softdrop_massCorr;
    std::vector< floatingnumber >  *jetAK8_puppi_softdrop_jec;
    std::vector< floatingnumber >  *jetAK8_puppi_pt;
    std::vector< floatingnumber >  *jetAK8_puppi_eta;
    std::vector< floatingnumber >  *jetAK8_puppi_mass;
    std::vector< floatingnumber >  *jetAK8_puppi_phi;
    std::vector< floatingnumber >  *jetAK8_puppi_e;
    std::vector< int >  *jetAK8_subjet_puppi_softdrop_N;
    std::vector< std::vector<floatingnumber> >  *jetAK8_subjet_puppi_softdrop_pt;
    std::vector< std::vector<floatingnumber> >  *jetAK8_subjet_puppi_softdrop_eta;
    std::vector< std::vector<floatingnumber> >  *jetAK8_subjet_puppi_softdrop_phi;
    std::vector< std::vector<floatingnumber> >  *jetAK8_subjet_puppi_softdrop_e;
    std::vector< std::vector<floatingnumber> >  *jetAK8_subjet_puppi_softdrop_m;
    std::vector< std::vector<floatingnumber> >  *jetAK8_subjet_puppi_softdrop_csv;
    std::vector< std::vector<int> >  *jetAK8_subjet_puppi_softdrop_charge;


    std::vector<int> m_connectsucceeded;

    // save actual detail level and group
    Int_t detailLevel;
    std::string detailGroup;

  }; // class PuppiJetNtupleObject

} // namespace Ntuple

#endif // SFRAME_NtupleVARIABLES_PuppiJetNtupleObject_H
