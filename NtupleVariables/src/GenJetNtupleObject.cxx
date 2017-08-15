
// THIS FILE HAS BEEN GENERATED AUTOMATICALLY. DO NOT EDIT DIRECTLY, CHANGES WILL BE LOST UPON NEXT CODE GENERATION.
// Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn 

// Local include(s):
#include "../include/GenJetNtupleObject.h"

namespace Ntuple {

  GenJetNtupleObject::GenJetNtupleObject( SCycleBaseNTuple* parent )
    : SInputVariables< SCycleBaseNTuple >( parent ) {
      m_connectsucceeded.resize(kEnd);
  }

  void GenJetNtupleObject::setConnectSucceeded(const unsigned int index, const bool success) {
    if (m_connectsucceeded.size() < index+1)  m_connectsucceeded.resize(index+1);
    m_connectsucceeded.at(index) = success;
  }

  void GenJetNtupleObject::ConnectVariables( const TString& treeName,
                                         const TString& prefix,
                                         const TString& ntupleType ) throw( SError ) {
   
     GenJetNtupleObject::ConnectVariables( treeName, Ntuple::GenJetAll, prefix, ntupleType);
                                         
  }                                         

  void GenJetNtupleObject::ConnectVariables( const TString& treeName,
                                         UInt_t detail_level,
                                         const TString& prefix,
                                         const TString& ntupleType ) throw( SError ) {
                                         
    // get instance of NtupleObjectNames
    NtupleObjectNames m_objectNames(ntupleType);
    
    
    // connect variables that are defined in Particle.cxx
        
    

    // connect object specific variables
if(  ((detail_level & Ntuple::GenJetTruth) == Ntuple::GenJetTruth)  ) {
     setConnectSucceeded(1, ConnectVariable( treeName, prefix + m_objectNames.getName("genJetAK8_softdropmass"), genJetAK8_softdropmass)); 
    setConnectSucceeded(2, ConnectVariable( treeName, prefix + m_objectNames.getName("genJetAK8_pt"), genJetAK8_pt)); 
    setConnectSucceeded(3, ConnectVariable( treeName, prefix + m_objectNames.getName("genJetAK8_phi"), genJetAK8_phi)); 
    setConnectSucceeded(4, ConnectVariable( treeName, prefix + m_objectNames.getName("genJetAK8_eta"), genJetAK8_eta)); 
    setConnectSucceeded(5, ConnectVariable( treeName, prefix + m_objectNames.getName("genJetAK8_e"), genJetAK8_e)); 
}


        
    // save actual detail_level
    detailLevel = detail_level;

    return;

  }

} // namespace Ntuple
