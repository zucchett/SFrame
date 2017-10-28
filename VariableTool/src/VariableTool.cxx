#include <cstdlib>
#include <limits>

#include <TFile.h>

#include "MT2Utility.cc"
#include "mt2w_bisect.cc"
#include "mt2bl_bisect.cc"
#include "Mt2Com_bisect.cc"

#include "../interface/VariableTool.h"

VariableTool::VariableTool(SCycleBase* parent, const char* name ): SToolBase( parent ), m_name( name ) {
    SetLogName( name );

    Mt2cal = new Mt2Com_bisect();
}


void VariableTool::BeginInputData( const SInputData& ) throw( SError ) {

}



VariableTool::~VariableTool() {
    delete Mt2cal;
}

// --------------------------------------------------
// --------------------   OTHERS  -------------------
// --------------------------------------------------


// Event Shape Variables, following http://home.fnal.gov/~mrenna/lutp0613man2/node233.html
void VariableTool::EventShape(std::vector<TLorentzVector>* Jets, float& sphericity, float& aplanarity) {
  TLorentzVector Ptot;
  for(std::vector<TLorentzVector>::const_iterator ijet=Jets->begin(); ijet!=Jets->end(); ++ijet) {
    Ptot(0)+=ijet->Px();
    Ptot(1)+=ijet->Py();
    Ptot(2)+=ijet->Pz();
    Ptot(3)+=ijet->E();
  }

  TVector3 beta=Ptot.BoostVector();

  TMatrixD PTensor(3,3);
  double p2=0.0;
  for(std::vector<TLorentzVector>::const_iterator ijet=Jets->begin(); ijet!=Jets->end(); ++ijet) {
    TLorentzVector* jet = new TLorentzVector(*ijet);
    jet->Boost(-beta);
    //p2+=jet->P()*jet->P();
    p2+=jet->Px()*jet->Px()+jet->Py()*jet->Py()+jet->Pz()*jet->Pz();
    PTensor(0,0)+=jet->Px()*jet->Px();
    PTensor(0,1)+=jet->Px()*jet->Py();
    PTensor(0,2)+=jet->Px()*jet->Pz();
    PTensor(1,0)+=jet->Py()*jet->Px();
    PTensor(1,1)+=jet->Py()*jet->Py();
    PTensor(1,2)+=jet->Py()*jet->Pz();
    PTensor(2,0)+=jet->Pz()*jet->Px();
    PTensor(2,1)+=jet->Pz()*jet->Py();
    PTensor(2,2)+=jet->Pz()*jet->Pz();
    delete jet;
  }
  PTensor(0,0)/=p2;
  PTensor(0,1)/=p2;
  PTensor(0,2)/=p2;
  PTensor(1,0)/=p2;
  PTensor(1,1)/=p2;
  PTensor(1,2)/=p2;
  PTensor(2,0)/=p2;
  PTensor(2,1)/=p2;
  PTensor(2,2)/=p2;

  TVectorD EigenVal(3);
  TMatrixD EigenVec(3,3);

  EigenVec=PTensor.EigenVectors(EigenVal);

  //std::cout << "eigenvalues " << EigenVal[0] << " " << EigenVal[1] << " " << EigenVal[2] << std::endl;

  sphericity=3./2.*(EigenVal[1]+EigenVal[2]);
  aplanarity=3./2.*EigenVal[2];
}



// Event Shape Variables, following http://home.fnal.gov/~mrenna/lutp0613man2/node233.html
std::vector<float> VariableTool::ReturnEventShape(TLorentzVector& L1, TLorentzVector& L2, TLorentzVector& B1, TLorentzVector& B2, bool ferox) {
  std::vector<TLorentzVector> P4;
  P4.push_back(L1);
  P4.push_back(L2);
  P4.push_back(B1);
  P4.push_back(B2);
  
  TLorentzVector P4tot;
  for(unsigned i=0; i<P4.size(); i++) P4tot+=P4.at(i);
  for(unsigned i=0; i<P4.size(); i++) P4tot.Boost(-P4tot.BoostVector());
  
  float den(0.);
  TMatrixF STensor(3, 3);
  for(unsigned i=0; i<P4.size(); i++) {
    if(ferox) den+=P4.at(i).M();
    else den+=P4.at(i).M2();
    STensor(0, 0)+=P4.at(i).Px()*P4.at(i).Px();
    STensor(0, 1)+=P4.at(i).Px()*P4.at(i).Py();
    STensor(0, 2)+=P4.at(i).Px()*P4.at(i).Pz();
    STensor(1, 0)+=P4.at(i).Py()*P4.at(i).Px();
    STensor(1, 1)+=P4.at(i).Py()*P4.at(i).Py();
    STensor(1, 2)+=P4.at(i).Py()*P4.at(i).Pz();
    STensor(2, 0)+=P4.at(i).Pz()*P4.at(i).Px();
    STensor(2, 1)+=P4.at(i).Pz()*P4.at(i).Py();
    STensor(2, 2)+=P4.at(i).Pz()*P4.at(i).Pz();
    if(ferox) for(int m=0; m<3; m++) for(int n=0; n<3; n++) STensor(m, n)/=P4.at(i).M();
  }
  for(int m=0; m<3; m++) for(int n=0; n<3; n++) STensor(m, n)/=den;

  TVectorF EigenVal(3);
  TMatrixF EigenVec(3,3);
  EigenVec=STensor.EigenVectors(EigenVal);
  float Norm(EigenVal[0]+EigenVal[1]+EigenVal[2]);
  std::vector<float> EV(3);
  for(int i=0; i<3; i++) EV[i]=EigenVal[i]/Norm;
  std::sort(EV.begin(), EV.end());
  return EV;// Warning, Eigenvalues ordered decreasingly istead of increasingly as in paper http://arxiv.org/abs/1010.3698
}

float VariableTool::ReturnCentrality(TLorentzVector& L1, TLorentzVector& L2, TLorentzVector& B1, TLorentzVector& B2) {
  TLorentzVector tL1(L1);
  TLorentzVector tL2(L2);
  TLorentzVector tB1(B1);
  TLorentzVector tB2(B2);
  TLorentzVector tA(tL1+tL2+tB1+tB2);
  // Boost objects to the A rest frame
  tL1.Boost( -tA.BoostVector() );
  tL2.Boost( -tA.BoostVector() );
  tB1.Boost( -tA.BoostVector() );
  tB2.Boost( -tA.BoostVector() );
  // Calc Centrality
  float value = (tL1.Pt()+tL2.Pt()+tB1.Pt()+tB2.Pt())/(tL1.Energy()+tL2.Energy()+tB1.Energy()+tB2.Energy());
  if(value!=value || isinf(value) || value<0.) return 0.;
  return value;
}

// --------------------   RADIATION  -------------------

//// Pull angle http://arxiv.org/abs/1001.5027v3
//void VariableTool::ReturnThetaPull(const pat::Jet* Jet1, const pat::Jet* Jet2, float& s_pull1, float& s_pull2, float& b_pull1, float& b_pull2) {
//  // Jet direction only with charged candidates
//  reco::Candidate::LorentzVector Axis1;
//  for(unsigned int i=0; i<Jet1->getPFConstituents().size(); i++) {
//    if(Jet1->getPFConstituent(i)->charge()!=0) {
//      Axis1+=Jet1->getPFConstituent(i)->p4();
//    }
//  }
//  reco::Candidate::LorentzVector Axis2;
//  for(unsigned int i=0; i<Jet2->getPFConstituents().size(); i++) {
//    if(Jet2->getPFConstituent(i)->charge()!=0) {
//      Axis2+=Jet2->getPFConstituent(i)->p4();
//    }
//  }
//  // t Vector, lies int the y-Phi plane, it is not a 4-vector: see paper
//  TVector2 t1(0, 0), t2(0, 0);
//  for(unsigned int i=0; i<Jet1->getPFConstituents().size(); i++) {
//    if(Jet1->getPFConstituent(i)->charge()!=0) {
//      TVector2 r(Jet1->getPFConstituent(i)->rapidity()-Axis1.Rapidity(), deltaPhi(Jet1->getPFConstituent(i)->phi(), Axis1.Phi()));
//      t1+=( Jet1->getPFConstituent(i)->pt() * r.Mod() / Axis1.pt() ) * r;
//    }
//  }
//  for(unsigned int i=0; i<Jet2->getPFConstituents().size(); i++) {
//    if(Jet2->getPFConstituent(i)->charge()!=0) {
//      TVector2 r(Jet2->getPFConstituent(i)->rapidity()-Axis2.Rapidity(), deltaPhi(Jet2->getPFConstituent(i)->phi(), Axis2.Phi()));
//      t2+=( Jet2->getPFConstituent(i)->pt() * r.Mod() / Axis2.pt() ) * r;
//    }
//  }
//  // Signal
//  // Axis12: Jet1->Jet2
//  TVector2 Axis12( Axis2.Rapidity()-Axis1.Rapidity(), deltaPhi(Axis2.Phi(), Axis1.Phi()) );
//  if(t1.Mod()>0.) s_pull1 = t1.DeltaPhi( Axis12 );
//  if(t2.Mod()>0.) s_pull2 = t2.DeltaPhi(-1*Axis12);
//  
//  // Background
//  // Beams
//  TVector2 Beam1( Axis1.Rapidity()>Axis2.Rapidity() ? +1. : -1 , 0.);
//  TVector2 Beam2( -1*Beam1 );
//  if(t1.Mod()>0.) b_pull1 = t1.DeltaPhi( Beam1 );
//  if(t2.Mod()>0.) b_pull2 = t2.DeltaPhi( Beam2 );
//}

//float VariableTool::ReturnGirth(const pat::Jet* Jet) {
//  float girth(0.);
//  reco::Candidate::LorentzVector Axis;
//  for(unsigned int i=0; i<Jet->getPFConstituents().size(); i++) {
//    if(Jet->getPFConstituent(i)->charge()!=0) {
//      Axis+=Jet->getPFConstituent(i)->p4();
//    }
//  }
//  for(unsigned int i=0; i<Jet->getPFConstituents().size(); i++) {
//    if(Jet->getPFConstituent(i)->charge()!=0) {
//      TVector2 r(Jet->getPFConstituent(i)->rapidity()-Axis.Rapidity(), deltaPhi(Jet->getPFConstituent(i)->phi(), Axis.Phi()));
//      girth+=Jet->getPFConstituent(i)->pt() * r.Mod() / Axis.pt();
//    }
//  }
//  return girth;
//}


// --------------------   ANGULAR  -------------------

float VariableTool::ReturnCosThetaStar(TLorentzVector& theX, TLorentzVector& theV) {
  TLorentzVector pA(theX);
  TLorentzVector pZ(theV);
  // Boost the Z to the A rest frame
  pZ.Boost( -pA.BoostVector() );
  float value=pZ.CosTheta();
  if(value!=value || isinf(value)) return -2.;
  return value;
}

float VariableTool::ReturnCosTheta1(TLorentzVector& theV, TLorentzVector& theL1, TLorentzVector& theL2, TLorentzVector& theB1, TLorentzVector& theB2) {
  TLorentzVector pA(theV);
  TLorentzVector pL1(theL1);
  TLorentzVector pL2(theL2);
  TLorentzVector pB1(theB1);
  TLorentzVector pB2(theB2);
  // Boost objects to the A rest frame
  pL1.Boost( -pA.BoostVector() );
  pL2.Boost( -pA.BoostVector() );
  pB1.Boost( -pA.BoostVector() );
  pB2.Boost( -pA.BoostVector() );
  // Reconstruct H in A rest frame
  TLorentzVector pHr = pB1 + pB2;
  // cos theta = H dot L1 / (|H|*|L1|)
  float value=pHr.Vect().Dot( pL1.Vect() ) / ( pHr.Vect().Mag()*pL1.Vect().Mag() );
  if(value!=value || isinf(value)) return -2.;
  return value;
}

float VariableTool::ReturnCosTheta2(TLorentzVector& theH, TLorentzVector& theL1, TLorentzVector& theL2, TLorentzVector& theB1, TLorentzVector& theB2) {
  TLorentzVector pA(theH);
  TLorentzVector pL1(theL1);
  TLorentzVector pL2(theL2);
  TLorentzVector pB1(theB1);
  TLorentzVector pB2(theB2);
  // Boost objects to the A rest frame
  pL1.Boost( -pA.BoostVector() );
  pL2.Boost( -pA.BoostVector() );
  pB1.Boost( -pA.BoostVector() );
  pB2.Boost( -pA.BoostVector() );
  // Reconstruct Z in A rest frame
  TLorentzVector pZr = pL1 + pL2;
  // cos theta = Z dot B1 / (|Z|*|B1|)
  float value=pZr.Vect().Dot( pB1.Vect() ) / ( pZr.Vect().Mag()*pB1.Vect().Mag() );
  if(value!=value || isinf(value)) return -2.;
  return value;
}

float VariableTool::ReturnPhi(TLorentzVector& theX, TLorentzVector& theL1, TLorentzVector& theL2, TLorentzVector& theB1, TLorentzVector& theB2) {
  TLorentzVector pA(theX);
  TLorentzVector pL1(theL1);
  TLorentzVector pL2(theL2);
  TLorentzVector pB1(theB1);
  TLorentzVector pB2(theB2);
  // Boost objects to the A rest frame
  pL1.Boost( -pA.BoostVector() );
  pL2.Boost( -pA.BoostVector() );
  pB1.Boost( -pA.BoostVector() );
  pB2.Boost( -pA.BoostVector() );
  // Build unit vectors orthogonal to the decay planes
  TVector3 Zplane=pL1.Vect().Cross( pL2.Vect() ); // L1 x L2
  TVector3 Hplane=pB1.Vect().Cross( pB2.Vect() ); // B1 x B2
  if(Zplane.Mag() == 0. || Hplane.Mag() == 0.) return -4.;
  Zplane.SetMag(1.);
  Hplane.SetMag(1.);
  // Sign of Phi
  TLorentzVector pZr = pL1 + pL2;
  float sgn = pZr.Vect().Dot( Zplane.Cross(Hplane) );
  sgn/=fabs(sgn);
  
  float value=sgn * acos( Zplane.Dot(Hplane) );
  if(value!=value || isinf(value)) return -5.;
  return value;
}


float VariableTool::ReturnPhi1(TLorentzVector& theX, TLorentzVector& theL1, TLorentzVector& theL2) {
  TLorentzVector pA(theX);
  TLorentzVector pL1(theL1);
  TLorentzVector pL2(theL2);
  TVector3 beamAxis(0., 0., 1.);
  // Boost objects to the A rest frame
  pL1.Boost( -pA.BoostVector() );
  pL2.Boost( -pA.BoostVector() );
  // Reconstruct Z in A rest frame
  TLorentzVector pZr = pL1 + pL2;
  // Build unit vectors orthogonal to the decay planes
  TVector3 Zplane=pL1.Vect().Cross( pL2.Vect() ); // L1 x L2
  TVector3 Bplane=beamAxis.Cross( pZr.Vect() ); // Beam x Z, beam/Z plane
  if(Zplane.Mag() == 0. || Bplane.Mag() == 0.) return -4.;
  Zplane.SetMag(1.);
  Bplane.SetMag(1.);
  // Sign of Phi1
  float sgn = pZr.Vect().Dot( Zplane.Cross(Bplane) );
  sgn/=fabs(sgn);
  
  float value=sgn * acos( Zplane.Dot(Bplane) );
  if(value!=value || isinf(value)) return -5.;
  return value;
}

// CMS-JME-13-006
float VariableTool::ReturnCosThetaJ(TLorentzVector& theV, TLorentzVector& theQ1) {
  TLorentzVector pV(theV);
  TLorentzVector pQ1(theQ1);

  pQ1.Boost( -pV.BoostVector() ); // Boost q1 along V had direction

  // cos theta = pQ1 dot pV / (|pQ1|*|pV|)
  float value=pQ1.Vect().Dot( pV.Vect() ) / ( pQ1.Vect().Mag()*pV.Vect().Mag() );
  if(value!=value || isinf(value)) return -2.;
  return value;
}


// --- MT2W ---

float VariableTool::ReturnMT2W(std::vector<TLorentzVector>& lJets, std::vector<TLorentzVector>& bJets, TLorentzVector& lep, TVector2& met) {
  float mt2w = Mt2cal->calculateMT2w(lJets, bJets, lep, met, "MT2w");
  return mt2w;
}
