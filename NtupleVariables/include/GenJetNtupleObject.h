// Dear emacs, this is -*- c++ -*-
// $Id: GenJetNtupleObject.h 37457 2010-07-05 12:04:33Z mann $

// THIS FILE HAS BEEN GENERATED AUTOMATICALLY. DO NOT EDIT DIRECTLY, CHANGES WILL BE LOST UPON NEXT CODE GENERATION.
// Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn 


#ifndef SFRAME_NtupleVARIABLES_GenJetNtupleObject_H
#define SFRAME_NtupleVARIABLES_GenJetNtupleObject_H

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
  *  @short Class that can read the variables produced by GenJetNtupleObject
  *
  *         This class can be used to read the offline muon information from
  *         an ntuple produced by the SingleTopDPDMaker code.
  *
  * @author Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn   
  *
  */
  
  enum GenJetDetails {
    GenJetTruth = 1,
    GenJetAll = 1,

  };
  
  // forward declaration of NtupleObjectNames
  class NtupleObjectNames;
  class GenJetNtupleObject : public SInputVariables< SCycleBaseNTuple > {

    public:
    /// Constructor specifying the parent of the object
    GenJetNtupleObject( SCycleBaseNTuple* parent );

    /// remember if connect succeeded
    void setConnectSucceeded(const unsigned int index, const bool success);
    bool getConnectSucceeded(const unsigned int index) const {return m_connectsucceeded.at(index);}  

    /// Connect the variables to the input branches
    void ConnectVariables( const TString& treeName,
                           UInt_t detail_level = 0,
                           const TString& prefix = "GenJet_",
                           const TString& ntupleType = "NtupleMakerNtuple" ) throw( SError );

    void ConnectVariables( const TString& treeName,
                           const TString& prefix = "GenJet_",
                           const TString& ntupleType = "NtupleMakerNtuple" ) throw( SError );

    int getDetailLevel() const {return detailLevel;}   

   
    enum ConnectionIndex { 
     kgenJetAK8_softdropmass=1, 
     kgenJetAK8_pt=2, 
     kgenJetAK8_phi=3, 
     kgenJetAK8_eta=4, 
     kgenJetAK8_e=5, 
 
      kEnd 
    }; 


    // vectors of properties defined in Particle.h
    

    
    // vectors of object specific variables
    std::vector< int >  *genJetAK8_softdropmass;
    std::vector< int >  *genJetAK8_pt;
    std::vector< int >  *genJetAK8_phi;
    std::vector< int >  *genJetAK8_eta;
    std::vector< int >  *genJetAK8_e;


    std::vector<int> m_connectsucceeded;

    // save actual detail level and group
    Int_t detailLevel;
    std::string detailGroup;

  }; // class GenJetNtupleObject

} // namespace Ntuple

#endif // SFRAME_NtupleVARIABLES_GenJetNtupleObject_H
