
// THIS FILE HAS BEEN GENERATED AUTOMATICALLY. DO NOT EDIT DIRECTLY, CHANGES WILL BE LOST UPON NEXT CODE GENERATION.
// Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn 

// Local include(s):
#include "../include/PuppiJetNtupleObject.h"

namespace Ntuple {

  PuppiJetNtupleObject::PuppiJetNtupleObject( SCycleBaseNTuple* parent )
    : SInputVariables< SCycleBaseNTuple >( parent ) {
      m_connectsucceeded.resize(kEnd);
  }

  void PuppiJetNtupleObject::setConnectSucceeded(const unsigned int index, const bool success) {
    if (m_connectsucceeded.size() < index+1)  m_connectsucceeded.resize(index+1);
    m_connectsucceeded.at(index) = success;
  }

  void PuppiJetNtupleObject::ConnectVariables( const TString& treeName,
                                         const TString& prefix,
                                         const TString& ntupleType ) throw( SError ) {
   
     PuppiJetNtupleObject::ConnectVariables( treeName, Ntuple::PuppiJetAll, prefix, ntupleType);
                                         
  }                                         

  void PuppiJetNtupleObject::ConnectVariables( const TString& treeName,
                                         UInt_t detail_level,
                                         const TString& prefix,
                                         const TString& ntupleType ) throw( SError ) {
                                         
    // get instance of NtupleObjectNames
    NtupleObjectNames m_objectNames(ntupleType);
    
    
    // connect variables that are defined in Particle.cxx
        
    

    // connect object specific variables
if(  ((detail_level & Ntuple::PuppiJetAnalysis) == Ntuple::PuppiJetAnalysis)  ) {
     setConnectSucceeded(6, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_puppi_tau1"), jetAK8_puppi_tau1)); 
    setConnectSucceeded(7, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_puppi_tau2"), jetAK8_puppi_tau2)); 
    setConnectSucceeded(8, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_puppi_tau3"), jetAK8_puppi_tau3)); 
    setConnectSucceeded(9, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_puppi_softdrop_mass"), jetAK8_puppi_softdrop_mass)); 
    setConnectSucceeded(10, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_puppi_softdrop_massCorr"), jetAK8_puppi_softdrop_massCorr)); 
    setConnectSucceeded(11, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_puppi_softdrop_jec"), jetAK8_puppi_softdrop_jec)); 
} // end of detail level Analysis

if(  ((detail_level & Ntuple::PuppiJetBasic) == Ntuple::PuppiJetBasic)  ) {
     setConnectSucceeded(1, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_puppi_pt"), jetAK8_puppi_pt)); 
    setConnectSucceeded(2, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_puppi_eta"), jetAK8_puppi_eta)); 
    setConnectSucceeded(3, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_puppi_mass"), jetAK8_puppi_mass)); 
    setConnectSucceeded(4, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_puppi_phi"), jetAK8_puppi_phi)); 
    setConnectSucceeded(5, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_puppi_e"), jetAK8_puppi_e)); 
} // end of detail level Basic

if(  ((detail_level & Ntuple::PuppiJetSoftdropSubjets) == Ntuple::PuppiJetSoftdropSubjets)  ) {
     setConnectSucceeded(12, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_subjet_puppi_softdrop_N"), jetAK8_subjet_puppi_softdrop_N)); 
    setConnectSucceeded(13, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_subjet_puppi_softdrop_pt"), jetAK8_subjet_puppi_softdrop_pt)); 
    setConnectSucceeded(14, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_subjet_puppi_softdrop_eta"), jetAK8_subjet_puppi_softdrop_eta)); 
    setConnectSucceeded(15, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_subjet_puppi_softdrop_phi"), jetAK8_subjet_puppi_softdrop_phi)); 
    setConnectSucceeded(16, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_subjet_puppi_softdrop_e"), jetAK8_subjet_puppi_softdrop_e)); 
    setConnectSucceeded(17, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_subjet_puppi_softdrop_m"), jetAK8_subjet_puppi_softdrop_m)); 
    setConnectSucceeded(18, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_subjet_puppi_softdrop_csv"), jetAK8_subjet_puppi_softdrop_csv)); 
    setConnectSucceeded(19, ConnectVariable( treeName, prefix + m_objectNames.getName("jetAK8_subjet_puppi_softdrop_charge"), jetAK8_subjet_puppi_softdrop_charge)); 
}


        
    // save actual detail_level
    detailLevel = detail_level;

    return;

  }

} // namespace Ntuple
