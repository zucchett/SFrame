
// THIS FILE HAS BEEN GENERATED AUTOMATICALLY. DO NOT EDIT DIRECTLY, CHANGES WILL BE LOST UPON NEXT CODE GENERATION.
// Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn 

#ifndef __UZHTOP_GenJet_H__
#define __UZHTOP_GenJet_H__

#include <cmath>
#include "Particle.h"
#include <vector>
#include "GenJetNtupleObject.h"



namespace Ntuple {
  class GenJetNtupleObject;
}

namespace UZH {
  
  /**
   *  @short Class that maps GenJetNtupleObjects to GenJet Particle class
   *
   *         This class can be used to map the offline GenJet information from
   *         GenJetNtupleObjects to UZH::GenJet class. All particles inherit 
   *         from UZH::Particle.
   *
   * @author Code produced by Id: CodeIt.py 494 2010-07-30 13:41:32Z svn 
   *
   */

  class GenJet : public Basic 

  {
  public:

    /// default c'tor
    GenJet();
    /// default d'tor
    ~GenJet();
    
    /// c'tor with index
    GenJet( const Ntuple::GenJetNtupleObject* ana, const Int_t idx );


    
    // variable definitions



    int* m_genJetAK8_softdropmass;
    int* m_genJetAK8_pt;
    int* m_genJetAK8_phi;
    int* m_genJetAK8_eta;
    int* m_genJetAK8_e;







    // check level given here must be consistent with ...NtupleObject.cxx, otherwise you'll get a segfault
    int genJetAK8_softdropmass() const { /*if(!m_ana->getConnectSucceeded(Ntuple::GenJetNtupleObject::kgenJetAK8_softdropmass)) std::cout<<"genJetAK8_softdropmass not connected!"<<std::endl;*/ return *(m_genJetAK8_softdropmass); } 
    int genJetAK8_pt() const { /*if(!m_ana->getConnectSucceeded(Ntuple::GenJetNtupleObject::kgenJetAK8_pt)) std::cout<<"genJetAK8_pt not connected!"<<std::endl;*/ return *(m_genJetAK8_pt); } 
    int genJetAK8_phi() const { /*if(!m_ana->getConnectSucceeded(Ntuple::GenJetNtupleObject::kgenJetAK8_phi)) std::cout<<"genJetAK8_phi not connected!"<<std::endl;*/ return *(m_genJetAK8_phi); } 
    int genJetAK8_eta() const { /*if(!m_ana->getConnectSucceeded(Ntuple::GenJetNtupleObject::kgenJetAK8_eta)) std::cout<<"genJetAK8_eta not connected!"<<std::endl;*/ return *(m_genJetAK8_eta); } 
    int genJetAK8_e() const { /*if(!m_ana->getConnectSucceeded(Ntuple::GenJetNtupleObject::kgenJetAK8_e)) std::cout<<"genJetAK8_e not connected!"<<std::endl;*/ return *(m_genJetAK8_e); } 
    
    void genJetAK8_softdropmass( const int& val){ *(m_genJetAK8_softdropmass)=val; } 
    void genJetAK8_pt( const int& val){ *(m_genJetAK8_pt)=val; } 
    void genJetAK8_phi( const int& val){ *(m_genJetAK8_phi)=val; } 
    void genJetAK8_eta( const int& val){ *(m_genJetAK8_eta)=val; } 
    void genJetAK8_e( const int& val){ *(m_genJetAK8_e)=val; } 
    

  private:
    const Ntuple::GenJetNtupleObject* m_ana;
  }; // class GenJet

  typedef std::vector< GenJet > GenJetVec;
  typedef std::vector< GenJet >::iterator GenJetVecIt;
  typedef std::vector< GenJet >::const_iterator GenJetVecConstIt;





} // end of namespace UZH

/// output stream operator overloaded for GenJet objects
std::ostream& operator<<( std::ostream& out,
                          const UZH::GenJet& rhs );


#endif //__UZH_GenJet_H__
