// Dear emacs, this is -*- c++ -*-
// $Id: CycleCreators.py 344 2012-12-13 13:10:53Z krasznaa $
#ifndef DMAnalysis_H
#define DMAnalysis_H

// SFrame include(s):
#include "core/include/SCycleBase.h"

// ROOT include(s):
#include <TBits.h>
#include "TMatrixD.h"
// External include(s):
#include "../NtupleVariables/include/JetNtupleObject.h"
#include "../NtupleVariables/include/Jet.h"
#include "../NtupleVariables/include/PuppiJetNtupleObject.h"
#include "../NtupleVariables/include/PuppiJet.h"
#include "../NtupleVariables/include/EventInfoNtupleObject.h"
#include "../NtupleVariables/include/ElectronNtupleObject.h"
#include "../NtupleVariables/include/Electron.h"
#include "../NtupleVariables/include/MuonNtupleObject.h"
#include "../NtupleVariables/include/Muon.h"
#include "../NtupleVariables/include/TauNtupleObject.h"
#include "../NtupleVariables/include/Tau.h"
#include "../NtupleVariables/include/MissingEtNtupleObject.h"
#include "../NtupleVariables/include/MissingEt.h"
#include "../NtupleVariables/include/GenJetNtupleObject.h"
#include "../NtupleVariables/include/GenParticleNtupleObject.h"
#include "../NtupleVariables/include/GenParticle.h"
#include "../GoodRunsLists/include/TGoodRunsList.h"
#include "../PileupReweightingTool/include/PileupReweightingTool.h"
#include "../BTaggingTools/include/BTaggingScaleTool.h"
//#include "../SVFit/include/NSVfitStandaloneAlgorithm.h"
//#include "../SVfitStandalone/interface/SVfitStandaloneAlgorithm.h"
#include "../ScaleFactorTool/interface/ScaleFactorTool.h"
#include "../CorrectionTool/interface/CorrectionTool.h"
#include "../VariableTool/interface/VariableTool.h"

class TH1D;
class TH2D;
class TRandom3;
class TBits;
namespace UZH {
  class Jet;
  class PuppiJet;
  class Electron;
  class Muon;
  class Tau;
  class MissingEt;
  class GenParticle;
}

/**
 *   @short Put short description of class here
 *
 *          Put a longer description over here...
 *
 *  @author zucchett
 * @version $Revision: 344 $
 */
class DMAnalysis : public SCycleBase {

    public:

        /// Default constructor
        DMAnalysis();
        /// Default destructor
        ~DMAnalysis();

        /// Function called at the beginning of the cycle
        virtual void BeginCycle() throw( SError );
        /// Function called at the end of the cycle
        virtual void EndCycle() throw( SError );

        /// Function called at the beginning of a new input data
        virtual void BeginInputData( const SInputData& ) throw( SError );
        /// Function called after finishing to process an input data
        virtual void EndInputData  ( const SInputData& ) throw( SError );

        /// Function called after opening each new input file
        virtual void BeginInputFile( const SInputData& ) throw( SError );

        /// Function called for every event
        virtual void ExecuteEvent( const SInputData&, Double_t ) throw( SError );

        /// Function to clear/reset all output branches
        virtual void clearBranches();

        /// Function to check good lumi section
        virtual bool isGoodEvent(int runNumber, int lumiSection);

        /// Function to check for trigger pass
        virtual bool passTrigger(std::string);

        /// Function to return a map of trigger pass
        virtual std::map<std::string, bool> getTriggerMap();

        /// Function to check for MET filters pass
        virtual bool passMETFilters(bool);


    private:
        // Put all your private variables here

        // Input variable objects:
        Ntuple::EventInfoNtupleObject   m_eventInfo; ///< event info container
        Ntuple::ElectronNtupleObject    m_electron;            ///< electron container
        Ntuple::MuonNtupleObject        m_muon;            ///< muon container
        Ntuple::TauNtupleObject         m_tau;            ///< tau container
        Ntuple::JetNtupleObject         m_jetAK4;            ///< jet container
        Ntuple::MissingEtNtupleObject   m_missingEt;            ///< missing E_T container
        Ntuple::GenParticleNtupleObject m_genParticle;            ///< gen particle container

        // SFrame Objects
        Root::TGoodRunsList m_grl;
        PileupReweightingTool m_pileupReweightingTool;
        BTaggingScaleTool m_bTaggingScaleTool;
        ScaleFactorTool m_ScaleFactorTool;
        CorrectionTool m_CorrectionTool;
        VariableTool m_VariableTool;

        // --- XML settings ---
        // naming
        std::string m_recoTreeName;       ///< name of tree with reconstructed objects in ntuple
        std::string m_outputTreeName;    ///< name of output tree

        // object names
        std::string m_jetAK4Name;            ///< name of AK4 jet collection in tree with reconstructed objects
        std::string m_jetAK8Name;       ///< name of AK8 jet collection in tree with reconstructed objects
        std::string m_genjetAK8Name;       ///< name of AK8 gen jet collection in tree with reconstructed objects
        std::string m_electronName;       ///< name of electron collection in tree with reconstructed objects
        std::string m_muonName;       ///< name of muon collection in tree with reconstructed objects
        std::string m_tauName;       ///< name of tau collection in tree with reconstructed objects
        std::string m_missingEtName;       ///< name of missing E_T collection in tree with reconstructed objects
        std::string m_genParticleName;       ///< name of gen particle collection in tree with reconstructed objects

        // flags
        bool  m_isData;
        bool  m_isSignal;
        // analysis cuts, need to be double
        double m_Elec1PtCut;
        double m_Elec2PtCut;
        double m_ElecPtCut;
        double m_Elec1EtaCut;
        double m_Elec2EtaCut;
        double m_ElecEtaCut;
        double m_Muon1PtCut;
        double m_Muon2PtCut;
        double m_MuonPtCut;
        double m_Muon1EtaCut;
        double m_Muon2EtaCut;
        double m_MuonEtaCut;
        double m_TauPtCut;
        double m_TauEtaCut;
        double m_AK4PtCut;
        double m_AK4EtaCut;
        double m_AK8PtCut;
        double m_AK8EtaCut;
        double m_MEtPtCut;
        double m_VPtCut;
        int m_nJetsCut;

        // file names
        std::string m_JSONFileName;

        // other variables needed
        std::map<std::string, std::vector<std::string> > m_triggerNames;


        // --- Analysis Variables ---
        bool isMC;
        unsigned long long nEvents;
        unsigned long long EventNumber;
        unsigned long long LumiNumber;
        unsigned long long RunNumber;
        float EventWeight;
        float GenWeight;
        float WewkWeight;
        float ZewkWeight;
        float WqcdWeight;
        float ZqcdWeight;  
        float TopWeight;
        float QCDRenWeightUp;
        float QCDRenWeightDown;
        float QCDFacWeightUp;
        float QCDFacWeightDown;
        float PUWeight;
        float PUWeightUp;
        float PUWeightDown;
        float TriggerWeight;
        float TriggerWeightUp;
        float TriggerWeightDown;
        float LeptonWeight;
        float LeptonWeightUp;
        float LeptonWeightDown;
        float BTagWeight;
        float BTagWeightUp;
        float BTagWeightDown;
        bool isZtoNN;
        bool isWtoEN;
        bool isWtoMN;
        bool isTtoEM;
        bool isZtoEE;
        bool isZtoMM;
        bool isTveto;
        int nPV;
        int nElectrons;
        int nMuons;
        int nTaus;
        int nPhotons;
        int nJets;
        int nForwardJets;
        int nBJets;
        int nBQuarks;
        int nBTagJets;
        int nBVetoJets;
        int LheNl;
        int LheNj;
        int LheNb;
        float LheHT;
        float LheV_pt;
        float MET_pt;
        float MET_phi;
        float MET_sign;
        float fakeMET_pt;
        float ST;
        float HT;
        float HTx;
        float HTy;
        float MHT;
        float MHTNoMu;
        float METNoMu;
        float MinMETMHT;
        float MinMETNoMuMHTNoMu;
        float MinJetMetDPhi;
        float MinLepMetDPhi;
        float MinBJetMetDPhi;
        float MinLepJetDPhi;
        float MaxLepMetDPhi;
        float MaxJetMetDPhi;
        float MaxBJetMetDPhi;
        float MaxLepJetDPhi;
        float MinJetMetDPhi12;
        float mZ;
        float mT;
        float mT2;
        float V_pt;

        // --- Tree Variables ---
        TLorentzVector Lepton1;
        TLorentzVector Lepton2;
        TLorentzVector Jet1;
        TLorentzVector Jet2;
        TLorentzVector Jet3;
        TLorentzVector Jet4;
        TLorentzVector MEt;
        TLorentzVector V;
        float Lepton1_pt;
        float Lepton2_pt;
        float Lepton1_eta;
        float Lepton2_eta;
        float Lepton1_phi;
        float Lepton2_phi;
        float Lepton1_pfIso;
        float Lepton2_pfIso;
        int Lepton1_id;
        int Lepton2_id;
        float Jet1_pt;
        float Jet2_pt;
        float Jet3_pt;
        float Jet4_pt;
        float JetF_pt;
        float Jet1_eta;
        float Jet2_eta;
        float Jet3_eta;
        float Jet4_eta;
        float JetF_eta;
        float Jet1_phi;
        float Jet2_phi;
        float Jet3_phi;
        float Jet4_phi;
        float JetF_phi;
        float Jet1_csv;
        float Jet2_csv;
        float Jet3_csv;
        float Jet4_csv;
  
        // ESV
        float Sphericity;
        float Aplanarity;

        // // Angular correlations
        // float CosThetaStar;
        // float CosTheta1;
        // float CosTheta2;
        // float Phi;
        // float Phi1;
        // float CosThetaJ;
        // // Macro adding the functions for dictionary generation
        ClassDef( DMAnalysis, 0 );

}; // class DMAnalysis

#endif // DMAnalysis_H
