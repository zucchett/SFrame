
// THIS FILE HAS BEEN GENERATED AUTOMATICALLY. DO NOT EDIT DIRECTLY, CHANGES WILL BE LOST UPON NEXT CODE GENERATION.
// Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn 

#include "../include/PuppiJet.h"

using namespace std;
using namespace UZH;

PuppiJet::PuppiJet() {
  
}


PuppiJet::PuppiJet( const Ntuple::PuppiJetNtupleObject* ana, const Int_t idx ) 
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
if(  ((ana->detailLevel & Ntuple::PuppiJetAnalysis) == Ntuple::PuppiJetAnalysis)  ) {
     if (ana->m_connectsucceeded[6]) m_jetAK8_puppi_tau1 = &((*ana->jetAK8_puppi_tau1)[idx]); else m_jetAK8_puppi_tau1 = 0; 
    if (ana->m_connectsucceeded[7]) m_jetAK8_puppi_tau2 = &((*ana->jetAK8_puppi_tau2)[idx]); else m_jetAK8_puppi_tau2 = 0; 
    if (ana->m_connectsucceeded[8]) m_jetAK8_puppi_tau3 = &((*ana->jetAK8_puppi_tau3)[idx]); else m_jetAK8_puppi_tau3 = 0; 
    if (ana->m_connectsucceeded[9]) m_jetAK8_puppi_softdrop_mass = &((*ana->jetAK8_puppi_softdrop_mass)[idx]); else m_jetAK8_puppi_softdrop_mass = 0; 
    if (ana->m_connectsucceeded[10]) m_jetAK8_puppi_softdrop_massCorr = &((*ana->jetAK8_puppi_softdrop_massCorr)[idx]); else m_jetAK8_puppi_softdrop_massCorr = 0; 
    if (ana->m_connectsucceeded[11]) m_jetAK8_puppi_softdrop_jec = &((*ana->jetAK8_puppi_softdrop_jec)[idx]); else m_jetAK8_puppi_softdrop_jec = 0; 
} // end of detail level Analysis

if(  ((ana->detailLevel & Ntuple::PuppiJetBasic) == Ntuple::PuppiJetBasic)  ) {
     if (ana->m_connectsucceeded[1]) m_jetAK8_puppi_pt = &((*ana->jetAK8_puppi_pt)[idx]); else m_jetAK8_puppi_pt = 0; 
    if (ana->m_connectsucceeded[2]) m_jetAK8_puppi_eta = &((*ana->jetAK8_puppi_eta)[idx]); else m_jetAK8_puppi_eta = 0; 
    if (ana->m_connectsucceeded[3]) m_jetAK8_puppi_mass = &((*ana->jetAK8_puppi_mass)[idx]); else m_jetAK8_puppi_mass = 0; 
    if (ana->m_connectsucceeded[4]) m_jetAK8_puppi_phi = &((*ana->jetAK8_puppi_phi)[idx]); else m_jetAK8_puppi_phi = 0; 
    if (ana->m_connectsucceeded[5]) m_jetAK8_puppi_e = &((*ana->jetAK8_puppi_e)[idx]); else m_jetAK8_puppi_e = 0; 
} // end of detail level Basic

if(  ((ana->detailLevel & Ntuple::PuppiJetSoftdropSubjets) == Ntuple::PuppiJetSoftdropSubjets)  ) {
     if (ana->m_connectsucceeded[12]) m_jetAK8_subjet_puppi_softdrop_N = &((*ana->jetAK8_subjet_puppi_softdrop_N)[idx]); else m_jetAK8_subjet_puppi_softdrop_N = 0; 
    if (ana->m_connectsucceeded[13]) m_jetAK8_subjet_puppi_softdrop_pt = &((*ana->jetAK8_subjet_puppi_softdrop_pt)[idx]); else m_jetAK8_subjet_puppi_softdrop_pt = 0; 
    if (ana->m_connectsucceeded[14]) m_jetAK8_subjet_puppi_softdrop_eta = &((*ana->jetAK8_subjet_puppi_softdrop_eta)[idx]); else m_jetAK8_subjet_puppi_softdrop_eta = 0; 
    if (ana->m_connectsucceeded[15]) m_jetAK8_subjet_puppi_softdrop_phi = &((*ana->jetAK8_subjet_puppi_softdrop_phi)[idx]); else m_jetAK8_subjet_puppi_softdrop_phi = 0; 
    if (ana->m_connectsucceeded[16]) m_jetAK8_subjet_puppi_softdrop_e = &((*ana->jetAK8_subjet_puppi_softdrop_e)[idx]); else m_jetAK8_subjet_puppi_softdrop_e = 0; 
    if (ana->m_connectsucceeded[17]) m_jetAK8_subjet_puppi_softdrop_m = &((*ana->jetAK8_subjet_puppi_softdrop_m)[idx]); else m_jetAK8_subjet_puppi_softdrop_m = 0; 
    if (ana->m_connectsucceeded[18]) m_jetAK8_subjet_puppi_softdrop_csv = &((*ana->jetAK8_subjet_puppi_softdrop_csv)[idx]); else m_jetAK8_subjet_puppi_softdrop_csv = 0; 
    if (ana->m_connectsucceeded[19]) m_jetAK8_subjet_puppi_softdrop_charge = &((*ana->jetAK8_subjet_puppi_softdrop_charge)[idx]); else m_jetAK8_subjet_puppi_softdrop_charge = 0; 
}







}


PuppiJet::~PuppiJet() {

}

ostream& operator<<( ostream& out,
                     const PuppiJet& rhs ) {
  
   out << "PuppiJet -" << ( Basic) rhs; 



  ;
if(  ((rhs.getLvl() & Ntuple::PuppiJetAnalysis) == Ntuple::PuppiJetAnalysis)  ) {
   out << " jetAK8_puppi_tau1 " << rhs.jetAK8_puppi_tau1();
  out << " jetAK8_puppi_tau2 " << rhs.jetAK8_puppi_tau2();
  out << " jetAK8_puppi_tau3 " << rhs.jetAK8_puppi_tau3();
  out << " jetAK8_puppi_softdrop_mass " << rhs.jetAK8_puppi_softdrop_mass();
  out << " jetAK8_puppi_softdrop_massCorr " << rhs.jetAK8_puppi_softdrop_massCorr();
  out << " jetAK8_puppi_softdrop_jec " << rhs.jetAK8_puppi_softdrop_jec();
;
} // end of detail level Analysis

if(  ((rhs.getLvl() & Ntuple::PuppiJetBasic) == Ntuple::PuppiJetBasic)  ) {
   out << " jetAK8_puppi_pt " << rhs.jetAK8_puppi_pt();
  out << " jetAK8_puppi_eta " << rhs.jetAK8_puppi_eta();
  out << " jetAK8_puppi_mass " << rhs.jetAK8_puppi_mass();
  out << " jetAK8_puppi_phi " << rhs.jetAK8_puppi_phi();
  out << " jetAK8_puppi_e " << rhs.jetAK8_puppi_e();
;
} // end of detail level Basic

if(  ((rhs.getLvl() & Ntuple::PuppiJetSoftdropSubjets) == Ntuple::PuppiJetSoftdropSubjets)  ) {
   out << " jetAK8_subjet_puppi_softdrop_N " << rhs.jetAK8_subjet_puppi_softdrop_N();
  try{  out << " jetAK8_subjet_puppi_softdrop_pt " << rhs.jetAK8_subjet_puppi_softdrop_pt().at(0);} catch(...){std::cout<<"except: no element"<<std::endl;};
  try{  out << " jetAK8_subjet_puppi_softdrop_eta " << rhs.jetAK8_subjet_puppi_softdrop_eta().at(0);} catch(...){std::cout<<"except: no element"<<std::endl;};
  try{  out << " jetAK8_subjet_puppi_softdrop_phi " << rhs.jetAK8_subjet_puppi_softdrop_phi().at(0);} catch(...){std::cout<<"except: no element"<<std::endl;};
  try{  out << " jetAK8_subjet_puppi_softdrop_e " << rhs.jetAK8_subjet_puppi_softdrop_e().at(0);} catch(...){std::cout<<"except: no element"<<std::endl;};
  try{  out << " jetAK8_subjet_puppi_softdrop_m " << rhs.jetAK8_subjet_puppi_softdrop_m().at(0);} catch(...){std::cout<<"except: no element"<<std::endl;};
  try{  out << " jetAK8_subjet_puppi_softdrop_csv " << rhs.jetAK8_subjet_puppi_softdrop_csv().at(0);} catch(...){std::cout<<"except: no element"<<std::endl;};
  try{  out << " jetAK8_subjet_puppi_softdrop_charge " << rhs.jetAK8_subjet_puppi_softdrop_charge().at(0);} catch(...){std::cout<<"except: no element"<<std::endl;};
;
}


  return out;
}












