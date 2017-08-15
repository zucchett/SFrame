
// THIS FILE HAS BEEN GENERATED AUTOMATICALLY. DO NOT EDIT DIRECTLY, CHANGES WILL BE LOST UPON NEXT CODE GENERATION.
// Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn 

#include "../include/GenJet.h"

using namespace std;
using namespace UZH;

GenJet::GenJet() {
  
}


GenJet::GenJet( const Ntuple::GenJetNtupleObject* ana, const Int_t idx ) 
: Basic( idx )



{
  m_ana=ana;
  // copy variables defined in Particle.h

  m_lvl    = ana->detailLevel;


  // copy rest of variables
  /*${ {AllNoBools:    printf("acc#name#\n"); if (ana->m_connectsucceeded[#index#]) 
         {printf("?\n"); m_#name# = &((*ana->#name#)[idx]);}
    else {printf("fak\n"); m_#name# = new #type#(); *m_#name# = #default#; } }}
  */
if(  ((ana->detailLevel & Ntuple::GenJetTruth) == Ntuple::GenJetTruth)  ) {
     if (ana->m_connectsucceeded[1]) m_genJetAK8_softdropmass = &((*ana->genJetAK8_softdropmass)[idx]); else m_genJetAK8_softdropmass = 0; 
    if (ana->m_connectsucceeded[2]) m_genJetAK8_pt = &((*ana->genJetAK8_pt)[idx]); else m_genJetAK8_pt = 0; 
    if (ana->m_connectsucceeded[3]) m_genJetAK8_phi = &((*ana->genJetAK8_phi)[idx]); else m_genJetAK8_phi = 0; 
    if (ana->m_connectsucceeded[4]) m_genJetAK8_eta = &((*ana->genJetAK8_eta)[idx]); else m_genJetAK8_eta = 0; 
    if (ana->m_connectsucceeded[5]) m_genJetAK8_e = &((*ana->genJetAK8_e)[idx]); else m_genJetAK8_e = 0; 
}







}


GenJet::~GenJet() {

}

ostream& operator<<( ostream& out,
                     const GenJet& rhs ) {
  
   out << "GenJet -" << ( Basic) rhs; 



  ;
if(  ((rhs.getLvl() & Ntuple::GenJetTruth) == Ntuple::GenJetTruth)  ) {
   out << " genJetAK8_softdropmass " << rhs.genJetAK8_softdropmass();
  out << " genJetAK8_pt " << rhs.genJetAK8_pt();
  out << " genJetAK8_phi " << rhs.genJetAK8_phi();
  out << " genJetAK8_eta " << rhs.genJetAK8_eta();
  out << " genJetAK8_e " << rhs.genJetAK8_e();
;
}


  return out;
}












