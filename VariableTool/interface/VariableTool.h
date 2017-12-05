#ifndef VariableTool_h
#define VariableTool_h

#include "TROOT.h"
#include "TFile.h"
#include "TH2.h"
#include "TF1.h"
#include "TLorentzVector.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TVectorD.h"
#include "TMatrixD.h"
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <string>
// SFrame include(s):
#include "core/include/SError.h"
#include "plug-ins/include/SToolBase.h"

#include "MT2Utility.h"
#include "mt2w_bisect.h"
#include "mt2bl_bisect.h"
#include "Mt2Com_bisect.h"


class VariableTool  : public SToolBase   {

    private:
        std::string m_name;

        Mt2Com_bisect* Mt2cal;

    public:

        VariableTool( SCycleBase* parent, const char* name = "VariableTool" );
        ~VariableTool(); 
        
        void BeginInputData( const SInputData& id ) throw( SError );
        
        void EventShape(std::vector<TLorentzVector>*, float&, float&);
        std::vector<float> ReturnEventShape(TLorentzVector&, TLorentzVector&, TLorentzVector&, TLorentzVector&, bool);
        float ReturnCentrality(TLorentzVector&, TLorentzVector&, TLorentzVector&, TLorentzVector&);
        float ReturnCosThetaStar(TLorentzVector&, TLorentzVector&);
        float ReturnCosTheta1(TLorentzVector&, TLorentzVector&, TLorentzVector&, TLorentzVector&, TLorentzVector&);
        float ReturnCosTheta2(TLorentzVector&, TLorentzVector&, TLorentzVector&, TLorentzVector&, TLorentzVector&);
        float ReturnPhi(TLorentzVector&, TLorentzVector&, TLorentzVector&, TLorentzVector&, TLorentzVector&);
        float ReturnPhi1(TLorentzVector&, TLorentzVector&, TLorentzVector&);
        float ReturnCosThetaJ(TLorentzVector&, TLorentzVector&);
        float RecoverNeutrinoPz(TLorentzVector lep, TLorentzVector met);
        float ReturnMT2W(std::vector<TLorentzVector>&, std::vector<TLorentzVector>&, TLorentzVector&, TVector2&);
};


#endif //  Variable_h


