#ifndef CorrectionTool_h
#define CorrectionTool_h

#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TGraphAsymmErrors.h"
#include "TRandom3.h"
#include "TLorentzVector.h"
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <string>
// SFrame include(s):
#include "core/include/SError.h"
#include "plug-ins/include/SToolBase.h"

#include "../interface/EnergyScaleCorrection_class.h"




class CorrectionTool  : public SToolBase   {

    private:
        std::string m_name;
        
        TF1* MassGenBarrel;
        TF1* MassGenEndcap;
        TF1* MassRecoBarrel;
        TF1* MassRecoEndcap;
        TF1* MassResBarrel;
        TF1* MassResEndcap;


    public:

        CorrectionTool( SCycleBase* parent, const char* name = "CorrectionTool" );
        ~CorrectionTool(); 
        
        void BeginInputData( const SInputData& id ) throw( SError );
        
        double GetCorrectedEnergy(double, bool, int, bool, double, double, double);
        TLorentzVector GetCorrectedJet(TLorentzVector, int=0);
        TLorentzVector GetCorrectedJet(TLorentzVector, float, float,  float, float, int=0);
        double GetCorrectedMass(double, double, double, double, bool, int=0, int=0);
        
        EnergyScaleCorrection_class* eScaler;
        TRandom3* trandom;
        
        std::string m_CorrectionFileName;
        std::string m_JetResFileName;
        std::string m_MassGenFileName, m_MassGenBarrelHistName, m_MassGenEndcapHistName, m_MassRecoFileName, m_MassRecoBarrelHistName, m_MassRecoEndcapHistName, m_MassResFileName, m_MassResBarrelHistName, m_MassResEndcapHistName;
        
        std::vector<float*> JetRes;
        
};


#endif //  Correction_h


