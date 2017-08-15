#ifndef ScaleFactorTool_h
#define ScaleFactorTool_h

#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TAxis.h"
#include "TGraphAsymmErrors.h"
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <string>
// SFrame include(s):
#include "core/include/SError.h"
#include "plug-ins/include/SToolBase.h"




class ScaleFactorTool  : public SToolBase   {

    private:
        std::string m_name;
        
        TH2D* TrigSingleEle;
        TH2D* TrigSingleEleNum;
        TH2D* TrigSingleEleDen;
        TH2D* TrigSingleIsoEle;
        TH2D* TrigSingleIsoEleNum;
        TH2D* TrigSingleIsoEleDen;
        
        TH2D* EleIdTightWP;
        TH2D* EleIdTightWPNum;
        TH2D* EleIdTightWPDen;
        TH2D* EleIdMediumWP;
        TH2D* EleIdMediumWPNum;
        TH2D* EleIdMediumWPDen;
        TH2D* EleIdLooseWP;
        TH2D* EleIdLooseWPNum;
        TH2D* EleIdLooseWPDen;
        TH2D* EleIdVetoWP;
        TH2D* EleIdVetoWPNum;
        TH2D* EleIdVetoWPDen;
        TH2D* EleReco;
        TH2D* EleRecoNum;
        TH2D* EleRecoDen;
        
        
        TH2F* TrigSingleMuon;
        TH2F* TrigSingleMuonNum;
        TH2F* TrigSingleMuonDen;
        TH2F* TrigSingleIsoMuon;
        TH2F* TrigSingleIsoMuonNum;
        TH2F* TrigSingleIsoMuonDen;
        
        TH2F* MuonHighptId;
        TH2F* MuonHighptIdNum;
        TH2F* MuonHighptIdDen;
        TH2F* MuonTightId;
        TH2F* MuonTightIdNum;
        TH2F* MuonTightIdDen;
        TH2F* MuonMediumId;
        TH2F* MuonMediumIdNum;
        TH2F* MuonMediumIdDen;
        TH2F* MuonLooseId;
        TH2F* MuonLooseIdNum;
        TH2F* MuonLooseIdDen;
        TH2F* MuonTrkId;
        TH2F* MuonTrkIdNum;
        TH2F* MuonTrkIdDen;
        
        TH2F* MuonLoosePFIso;
        TH2F* MuonLoosePFIsoNum;
        TH2F* MuonLoosePFIsoDen;
        TH2F* MuonTightPFIso;
        TH2F* MuonTightPFIsoNum;
        TH2F* MuonTightPFIsoDen;
        TH2F* MuonLooseTrkIso;
        TH2F* MuonLooseTrkIsoNum;
        TH2F* MuonLooseTrkIsoDen;
        TH2F* MuonTightTrkIso;
        TH2F* MuonTightTrkIsoNum;
        TH2F* MuonTightTrkIsoDen;
        
        
        
        TGraphAsymmErrors* MuonTrk;
        
        TGraphAsymmErrors* TrigMET;
        
        TF1* W;
        TF1* Z;


    public:

        ScaleFactorTool( SCycleBase* parent, const char* name = "ScaleFactorTool" );
        ~ScaleFactorTool(); 
        
        void BeginInputData( const SInputData& id ) throw( SError );
        
        double GetTrigSingleEle(double, double, int=0);
        double GetTrigSingleEleEffData(double, double, int=0);
        double GetTrigSingleEleEffMC(double, double, int=0);
        double GetTrigSingleIsoEle(double, double, int=0);
        double GetTrigSingleIsoEleEffData(double, double, int=0);
        double GetTrigSingleIsoEleEffMC(double, double, int=0);
        double GetEleIdTightWP(double, double, int=0);
        double GetEleIdMediumWP(double, double, int=0);
        double GetEleIdLooseWP(double, double, int=0);
        double GetEleIdVetoWP(double, double, int=0);
        double GetEleReco(double, double, int=0);
        double GetTrigSingleMuon(double, double, int=0);
        double GetTrigSingleMuonEffData(double, double, int=0);
        double GetTrigSingleMuonEffMC(double, double, int=0);
        double GetTrigSingleIsoMuon(double, double, int=0);
        double GetTrigSingleIsoMuonEffData(double, double, int=0);
        double GetTrigSingleIsoMuonEffMC(double, double, int=0);
        double GetMuonHighptId(double, double, int=0);
        double GetMuonTrkId(double, double, int=0);
        double GetMuonTightId(double, double, int=0);
        double GetMuonMediumId(double, double, int=0);
        double GetMuonLooseId(double, double, int=0);
        double GetMuonLoosePFIso(double, double, int=0);
        double GetMuonTightPFIso(double, double, int=0);
        double GetMuonLooseTrkIso(double, double, int=0);
        double GetMuonTightTrkIso(double, double, int=0);
        double GetMuonTrk(int, int=0);
        double GetTrigMET(double, int=0);
        double GetEwkW(double);
        double GetEwkZ(double);
        
        double GetTrigEle(double, double, int=0);
        double GetTrigEle(double, double, double, double, int=0);
        double GetTrigMuonMET(double, double, double, int=0);
        double GetTrigMuonMET(double, double, double, double, double, int=0);
        
        std::string m_TrigSingleEleFileName, m_TrigSingleEleHistNumName, m_TrigSingleEleHistDenName, m_TrigSingleIsoEleFileName, m_TrigSingleIsoEleHistNumName, m_TrigSingleIsoEleHistDenName;
        std::string m_EleIdTightWPFileName, m_EleIdMediumWPFileName, m_EleIdLooseWPFileName, m_EleIdVetoWPFileName, m_EleRecoFileName;
        std::string m_EleIdTightWPHistNumName, m_EleIdTightWPHistDenName, m_EleIdMediumWPHistNumName, m_EleIdMediumWPHistDenName, m_EleIdLooseWPHistNumName, m_EleIdLooseWPHistDenName, m_EleIdVetoWPHistNumName, m_EleIdVetoWPHistDenName, m_EleRecoHistNumName, m_EleRecoHistDenName;
        std::string m_TrigSingleMuonFileName, m_TrigSingleMuonHistNumName, m_TrigSingleMuonHistDenName, m_TrigSingleIsoMuonHistNumName, m_TrigSingleIsoMuonHistDenName;
        std::string m_MuonIdFileName, m_MuonHighptIdHistNumName, m_MuonHighptIdHistDenName, m_MuonTrkIdFileName, m_MuonTrkIdHistNumName, m_MuonTrkIdHistDenName, m_MuonTightIdHistNumName, m_MuonTightIdHistDenName, m_MuonMediumIdHistNumName, m_MuonMediumIdHistDenName, m_MuonLooseIdHistNumName, m_MuonLooseIdHistDenName;
        std::string m_MuonIsoFileName, m_MuonLoosePFIsoHistNumName, m_MuonLoosePFIsoHistDenName, m_MuonTightPFIsoHistNumName, m_MuonTightPFIsoHistDenName, m_MuonLooseTrkIsoHistNumName, m_MuonLooseTrkIsoHistDenName, m_MuonTightTrkIsoHistNumName, m_MuonTightTrkIsoHistDenName;
        std::string m_MuonTrkFileName, m_MuonTrkHistName;
        std::string m_TrigMETFileName, m_TrigMETHistNumName, m_TrigMETHistDenName;
        std::string m_EWKFileName, m_WHistName, m_ZHistName;
        
};


#endif //  ScaleFactor_h


