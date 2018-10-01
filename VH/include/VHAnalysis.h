// Dear emacs, this is -*- c++ -*-
// $Id: CycleCreators.py 344 2012-12-13 13:10:53Z krasznaa $
#ifndef VHAnalysis_H
#define VHAnalysis_H

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
#include "../NtupleVariables/include/GenJet.h"
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
class VHAnalysis : public SCycleBase {
    
    public:

        /// Default constructor
        VHAnalysis();
        /// Default destructor
        ~VHAnalysis();

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

        /// Function to obtain PU weight for MC
        virtual double getPUWeight();
        
        /// Function to check if a muon pass the tracker high pt id
        //virtual bool isTrackerHighPt(UZH::Muon);

        
        /// Function to fill cut flow
//        virtual float recoverNeutrinoPz(TLorentzVector, TLorentzVector);
        
        // Summer16 Electron Cut Id
//        virtual bool electronId(UZH::Electron*, int);
        
        /// Functions to fill plots
        virtual void fill4VectPlots(TLorentzVector, std::string, std::string);
        virtual void fillJetPlots(UZH::Jet, double, double, std::string, std::string);
        virtual void fillMEtPlots(UZH::MissingEt, std::string);
        virtual void fillEventPlots(std::string);
//        virtual void fillAnalysisPlots(std::string, bool=false);
//        virtual void fillTopControlPlots(std::string);
//        virtual void fillElectronPlots(UZH::Electron&, UZH::Electron&, TLorentzVector&, TLorentzVector&);
//        virtual void fillMuonPlots(UZH::Muon&, UZH::Muon&, TLorentzVector&, TLorentzVector&);

    private:
        // Put all your private variables here

        // Input variable objects:
        Ntuple::EventInfoNtupleObject   m_eventInfo; ///< event info container
        Ntuple::ElectronNtupleObject    m_electron;            ///< electron container
        Ntuple::MuonNtupleObject        m_muon;            ///< muon container
        Ntuple::TauNtupleObject         m_tau;            ///< tau container
        Ntuple::JetNtupleObject         m_jetAK4;            ///< jet container
        Ntuple::JetNtupleObject         m_jetAK8;            ///< jet container
        Ntuple::PuppiJetNtupleObject    m_jetAK8Puppi;            ///< jet container
        Ntuple::GenJetNtupleObject      m_genjetAK8;            ///< gen jet container
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
        double m_Tau21LowerCut;
        double m_Tau21UpperCut;
        double m_JMassLowerCut;
        double m_JMassUpperCut;
        double m_VMassLowerCut;
        double m_VMassInterCut;
        double m_VMassUpperCut;
        double m_HMassLowerCut;
        double m_HMassUpperCut;
        double m_XMassLowerCut;
        double m_XTMassLowerCut;
        double m_XJJMassLowerCut;

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
        float StitchWeight;
        float ZewkWeight;
        float WewkWeight;
        float TopWeight;
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
        float BTagAK4Weight;
        float BTagAK4WeightUp;
        float BTagAK4WeightDown;
        float BTagAK8Weight;
        float BTagAK8WeightUp;
        float BTagAK8WeightDown;
        float BBTagWeight;
        float BBTagWeightUp;
        float BBTagWeightDown;
        bool isZtoNN;
        bool isWtoEN;
        bool isWtoMN;
        bool isZtoEE;
        bool isZtoMM;
        bool isVtoQQ;
        bool isTtoEM;
        bool isTveto;
        bool isBoostedTau;
        bool isBoosted4B;
        int nPV;
        int nElectrons;
        int nMuons;
        int nTaus;
        int nPhotons;
        int nJets;
        int nJetsNoFatJet;
        int nFatJets;
        int nBTagJets;
        int nBVetoJets;
        int LheNl;
        int LheNj;
        float ST;
        float HT;
        float HTx;
        float HTy;
        float MHT;
        float MHTNoMu;
        float METNoMu;
        float MinMETMHT;
        float MinMETNoMuMHTNoMu;
        float MaxMETNoMuMHTNoMu;
        float MaxJetBTag;
        float MaxJetNoFatJetBTag;
        float MinJetMetDPhi;
        float JetMetDPhi;
        float VHDEta;
        float Centrality;
        float CosThetaStar;
        float CosTheta1;
        float CosTheta2;
        float Phi;
        float Phi1;
        
        // --- Tree Variables ---
        TLorentzVector X;
        TLorentzVector V;
        TLorentzVector H;
        float H_mass;
        float H_tau21;
        float H_ddt;
        float H_csv1;
        float H_csv2;
        int H_flav1;
        int H_flav2;
        float H_dbt;
        int H_ntag;
        float H_ptB;
        float H_chf;
        float H_nhf;
        float H_phf;
        float H_muf;
        float H_emf;
        int H_chm;
        float V_mass;
        float V_tmass;
        float V_tau21;
        float V_ddt;
        float V_csv1;
        float V_csv2;
        float V_dbt;
        int V_ntag;
        float X_mass;
        float X_tmass;

        // Macro adding the functions for dictionary generation
        ClassDef( VHAnalysis, 0 );

}; // class VHAnalysis

#endif // VHAnalysis_H

