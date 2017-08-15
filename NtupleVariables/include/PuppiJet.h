
// THIS FILE HAS BEEN GENERATED AUTOMATICALLY. DO NOT EDIT DIRECTLY, CHANGES WILL BE LOST UPON NEXT CODE GENERATION.
// Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn 

#ifndef __UZHTOP_PuppiJet_H__
#define __UZHTOP_PuppiJet_H__

#include <cmath>
#include "Particle.h"
#include <vector>
#include "PuppiJetNtupleObject.h"



namespace Ntuple {
  class PuppiJetNtupleObject;
}

namespace UZH {
  
  /**
   *  @short Class that maps PuppiJetNtupleObjects to PuppiJet Particle class
   *
   *         This class can be used to map the offline PuppiJet information from
   *         PuppiJetNtupleObjects to UZH::PuppiJet class. All particles inherit 
   *         from UZH::Particle.
   *
   * @author Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn 
   *
   */

  class PuppiJet : public Basic 

  {
  public:

    /// default c'tor
    PuppiJet();
    /// default d'tor
    ~PuppiJet();
    
    /// c'tor with index
    PuppiJet( const Ntuple::PuppiJetNtupleObject* ana, const Int_t idx );


    
    // variable definitions



    floatingnumber* m_jetAK8_puppi_tau1;
    floatingnumber* m_jetAK8_puppi_tau2;
    floatingnumber* m_jetAK8_puppi_tau3;
    floatingnumber* m_jetAK8_puppi_softdrop_mass;
    floatingnumber* m_jetAK8_puppi_softdrop_massCorr;
    floatingnumber* m_jetAK8_puppi_softdrop_jec;
    floatingnumber* m_jetAK8_puppi_pt;
    floatingnumber* m_jetAK8_puppi_eta;
    floatingnumber* m_jetAK8_puppi_mass;
    floatingnumber* m_jetAK8_puppi_phi;
    floatingnumber* m_jetAK8_puppi_e;
    int* m_jetAK8_subjet_puppi_softdrop_N;
    std::vector<floatingnumber>* m_jetAK8_subjet_puppi_softdrop_pt;
    std::vector<floatingnumber>* m_jetAK8_subjet_puppi_softdrop_eta;
    std::vector<floatingnumber>* m_jetAK8_subjet_puppi_softdrop_phi;
    std::vector<floatingnumber>* m_jetAK8_subjet_puppi_softdrop_e;
    std::vector<floatingnumber>* m_jetAK8_subjet_puppi_softdrop_m;
    std::vector<floatingnumber>* m_jetAK8_subjet_puppi_softdrop_csv;
    std::vector<int>* m_jetAK8_subjet_puppi_softdrop_charge;







    // check level given here must be consistent with ...NtupleObject.cxx, otherwise you'll get a segfault
    floatingnumber jetAK8_puppi_tau1() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_puppi_tau1)) std::cout<<"jetAK8_puppi_tau1 not connected!"<<std::endl;*/ return *(m_jetAK8_puppi_tau1); } 
    floatingnumber jetAK8_puppi_tau2() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_puppi_tau2)) std::cout<<"jetAK8_puppi_tau2 not connected!"<<std::endl;*/ return *(m_jetAK8_puppi_tau2); } 
    floatingnumber jetAK8_puppi_tau3() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_puppi_tau3)) std::cout<<"jetAK8_puppi_tau3 not connected!"<<std::endl;*/ return *(m_jetAK8_puppi_tau3); } 
    floatingnumber jetAK8_puppi_softdrop_mass() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_puppi_softdrop_mass)) std::cout<<"jetAK8_puppi_softdrop_mass not connected!"<<std::endl;*/ return *(m_jetAK8_puppi_softdrop_mass); } 
    floatingnumber jetAK8_puppi_softdrop_massCorr() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_puppi_softdrop_massCorr)) std::cout<<"jetAK8_puppi_softdrop_massCorr not connected!"<<std::endl;*/ return *(m_jetAK8_puppi_softdrop_massCorr); } 
    floatingnumber jetAK8_puppi_softdrop_jec() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_puppi_softdrop_jec)) std::cout<<"jetAK8_puppi_softdrop_jec not connected!"<<std::endl;*/ return *(m_jetAK8_puppi_softdrop_jec); } 
    floatingnumber jetAK8_puppi_pt() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_puppi_pt)) std::cout<<"jetAK8_puppi_pt not connected!"<<std::endl;*/ return *(m_jetAK8_puppi_pt); } 
    floatingnumber jetAK8_puppi_eta() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_puppi_eta)) std::cout<<"jetAK8_puppi_eta not connected!"<<std::endl;*/ return *(m_jetAK8_puppi_eta); } 
    floatingnumber jetAK8_puppi_mass() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_puppi_mass)) std::cout<<"jetAK8_puppi_mass not connected!"<<std::endl;*/ return *(m_jetAK8_puppi_mass); } 
    floatingnumber jetAK8_puppi_phi() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_puppi_phi)) std::cout<<"jetAK8_puppi_phi not connected!"<<std::endl;*/ return *(m_jetAK8_puppi_phi); } 
    floatingnumber jetAK8_puppi_e() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_puppi_e)) std::cout<<"jetAK8_puppi_e not connected!"<<std::endl;*/ return *(m_jetAK8_puppi_e); } 
    int jetAK8_subjet_puppi_softdrop_N() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_subjet_puppi_softdrop_N)) std::cout<<"jetAK8_subjet_puppi_softdrop_N not connected!"<<std::endl;*/ return *(m_jetAK8_subjet_puppi_softdrop_N); } 
    std::vector<floatingnumber> jetAK8_subjet_puppi_softdrop_pt() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_subjet_puppi_softdrop_pt)) std::cout<<"jetAK8_subjet_puppi_softdrop_pt not connected!"<<std::endl;*/ return *(m_jetAK8_subjet_puppi_softdrop_pt); } 
    std::vector<floatingnumber> jetAK8_subjet_puppi_softdrop_eta() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_subjet_puppi_softdrop_eta)) std::cout<<"jetAK8_subjet_puppi_softdrop_eta not connected!"<<std::endl;*/ return *(m_jetAK8_subjet_puppi_softdrop_eta); } 
    std::vector<floatingnumber> jetAK8_subjet_puppi_softdrop_phi() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_subjet_puppi_softdrop_phi)) std::cout<<"jetAK8_subjet_puppi_softdrop_phi not connected!"<<std::endl;*/ return *(m_jetAK8_subjet_puppi_softdrop_phi); } 
    std::vector<floatingnumber> jetAK8_subjet_puppi_softdrop_e() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_subjet_puppi_softdrop_e)) std::cout<<"jetAK8_subjet_puppi_softdrop_e not connected!"<<std::endl;*/ return *(m_jetAK8_subjet_puppi_softdrop_e); } 
    std::vector<floatingnumber> jetAK8_subjet_puppi_softdrop_m() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_subjet_puppi_softdrop_m)) std::cout<<"jetAK8_subjet_puppi_softdrop_m not connected!"<<std::endl;*/ return *(m_jetAK8_subjet_puppi_softdrop_m); } 
    std::vector<floatingnumber> jetAK8_subjet_puppi_softdrop_csv() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_subjet_puppi_softdrop_csv)) std::cout<<"jetAK8_subjet_puppi_softdrop_csv not connected!"<<std::endl;*/ return *(m_jetAK8_subjet_puppi_softdrop_csv); } 
    std::vector<int> jetAK8_subjet_puppi_softdrop_charge() const { /*if(!m_ana->getConnectSucceeded(Ntuple::PuppiJetNtupleObject::kjetAK8_subjet_puppi_softdrop_charge)) std::cout<<"jetAK8_subjet_puppi_softdrop_charge not connected!"<<std::endl;*/ return *(m_jetAK8_subjet_puppi_softdrop_charge); } 
    
    void jetAK8_puppi_tau1( const floatingnumber& val){ *(m_jetAK8_puppi_tau1)=val; } 
    void jetAK8_puppi_tau2( const floatingnumber& val){ *(m_jetAK8_puppi_tau2)=val; } 
    void jetAK8_puppi_tau3( const floatingnumber& val){ *(m_jetAK8_puppi_tau3)=val; } 
    void jetAK8_puppi_softdrop_mass( const floatingnumber& val){ *(m_jetAK8_puppi_softdrop_mass)=val; } 
    void jetAK8_puppi_softdrop_massCorr( const floatingnumber& val){ *(m_jetAK8_puppi_softdrop_massCorr)=val; } 
    void jetAK8_puppi_softdrop_jec( const floatingnumber& val){ *(m_jetAK8_puppi_softdrop_jec)=val; } 
    void jetAK8_puppi_pt( const floatingnumber& val){ *(m_jetAK8_puppi_pt)=val; } 
    void jetAK8_puppi_eta( const floatingnumber& val){ *(m_jetAK8_puppi_eta)=val; } 
    void jetAK8_puppi_mass( const floatingnumber& val){ *(m_jetAK8_puppi_mass)=val; } 
    void jetAK8_puppi_phi( const floatingnumber& val){ *(m_jetAK8_puppi_phi)=val; } 
    void jetAK8_puppi_e( const floatingnumber& val){ *(m_jetAK8_puppi_e)=val; } 
    void jetAK8_subjet_puppi_softdrop_N( const int& val){ *(m_jetAK8_subjet_puppi_softdrop_N)=val; } 
    void jetAK8_subjet_puppi_softdrop_pt( const std::vector<floatingnumber>& val){ *(m_jetAK8_subjet_puppi_softdrop_pt)=val; } 
    void jetAK8_subjet_puppi_softdrop_eta( const std::vector<floatingnumber>& val){ *(m_jetAK8_subjet_puppi_softdrop_eta)=val; } 
    void jetAK8_subjet_puppi_softdrop_phi( const std::vector<floatingnumber>& val){ *(m_jetAK8_subjet_puppi_softdrop_phi)=val; } 
    void jetAK8_subjet_puppi_softdrop_e( const std::vector<floatingnumber>& val){ *(m_jetAK8_subjet_puppi_softdrop_e)=val; } 
    void jetAK8_subjet_puppi_softdrop_m( const std::vector<floatingnumber>& val){ *(m_jetAK8_subjet_puppi_softdrop_m)=val; } 
    void jetAK8_subjet_puppi_softdrop_csv( const std::vector<floatingnumber>& val){ *(m_jetAK8_subjet_puppi_softdrop_csv)=val; } 
    void jetAK8_subjet_puppi_softdrop_charge( const std::vector<int>& val){ *(m_jetAK8_subjet_puppi_softdrop_charge)=val; } 
    

  private:
    const Ntuple::PuppiJetNtupleObject* m_ana;
  }; // class PuppiJet

  typedef std::vector< PuppiJet > PuppiJetVec;
  typedef std::vector< PuppiJet >::iterator PuppiJetVecIt;
  typedef std::vector< PuppiJet >::const_iterator PuppiJetVecConstIt;





} // end of namespace UZH

/// output stream operator overloaded for PuppiJet objects
std::ostream& operator<<( std::ostream& out,
                          const UZH::PuppiJet& rhs );


#endif //__UZH_PuppiJet_H__
