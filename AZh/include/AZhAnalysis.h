// Dear emacs, this is -*- c++ -*-
// $Id: CycleCreators.py 344 2012-12-13 13:10:53Z krasznaa $
#ifndef AZhAnalysis_H
#define AZhAnalysis_H

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
class AZhAnalysis : public SCycleBase {
    
    public:

        /// Default constructor
        AZhAnalysis();
        /// Default destructor
        ~AZhAnalysis();

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
        double m_ElecEtaCut;
        double m_Muon1PtCut;
        double m_Muon2PtCut;
        double m_MuonPtCut;
        double m_MuonEtaCut;
        double m_TauPtCut;
        double m_TauEtaCut;
        double m_AK4PtCut;
        double m_AK4EtaCut;
        double m_AK8PtCut;
        double m_AK8EtaCut;
        double m_MEtPtCut;
        double m_VPtCut;
        double m_MinJetMETDPhi;
        double m_VMassLowerCut;
        double m_VMassUpperCut;
        double m_HMassLowerCut;
        double m_HMassUpperCut;
        double m_XMassLowerCut;
        double m_XTMassLowerCut;

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
        float TopWeight;
        float QCDWeightUp;
        float QCDWeightDown;
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
        bool isZtoEE;
        bool isZtoMM;
        bool isTveto;
        int nPV;
        int nElectrons;
        int nMuons;
        int nTaus;
        int nPhotons;
        int nJets;
        int nBJets;
        int nBQuarks;
        int nBTagJets;
        int nBVetoJets;
        int LheNl;
        int LheNj;
        int LheNb;
        float LheHT;
        float LheV_pt;
        float ST;
        float HT;
        float HTx;
        float HTy;
        float MHT;
        float MET_pt;
        float MET_phi;
        float fakeMET_pt;
        float fakeMET_phi;
        float MHTNoMu;
        float METNoMu;
        float METNoEle;
        float MinMETMHT;
        float MinMETNoMuMHTNoMu;
        float MinJetMetDPhi;
        float JetMetDPhi;
        float Centrality;
        float CosThetaStar;
        float CosTheta1;
        float CosTheta2;
        float Phi;
        float Phi1;
        
        // --- Tree Variables ---
        TLorentzVector Lepton1;
        TLorentzVector Lepton2;
        TLorentzVector kLepton1;
        TLorentzVector kLepton2;
        TLorentzVector Jet1;
        TLorentzVector Jet2;
        TLorentzVector Jet3;
        TLorentzVector kJet1;
        TLorentzVector kJet2;
        TLorentzVector kJet3;
        TLorentzVector MEt;
        TLorentzVector kMEt;
        TLorentzVector Z;
        TLorentzVector H;
        TLorentzVector A;
        TLorentzVector kZ;
        TLorentzVector kH;
        TLorentzVector kA;
        float Lepton1_pt;
        float Lepton2_pt;
        float Lepton1_pfIso;
        float Lepton2_pfIso;
        float Jet1_pt;
        float Jet2_pt;
        float Jet3_pt;
        float kJet1_pt;
        float kJet2_pt;
        float kJet3_pt;
        float Jet1_csv;
        float Jet2_csv;
        float Jet3_csv;
        float W_pt;
        float W_tmass;
        float Z_pt;
        float Z_mass;
        float H_pt;
        float H_mass;
        float A_pt;
        float A_mass;
        float A_tmass;
        float kH_pt;
        float kH_mass;
        float kA_pt;
        float kA_mass;
        float kA_tmass;

        // Macro adding the functions for dictionary generation
        ClassDef( AZhAnalysis, 0 );

}; // class AZhAnalysis

#endif // AZhAnalysis_H

