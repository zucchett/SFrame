// $Id: CycleCreators.py 344 2016-04-03 19:52:15Z zucchett $

// Local include(s):
#include "../include/DMSearches.h"

// External include(s):
#include "../GoodRunsLists/include/TGoodRunsListReader.h"
#include "../include/kfactors.h"
#include <TMath.h>

bool SortByCSV(UZH::Jet j1, UZH::Jet j2) {return(j1.csv() > j2.csv());}

ClassImp( DMAnalysis );


DMAnalysis::DMAnalysis() : SCycleBase(),
    m_eventInfo( this ),
    m_electron( this ),
    m_muon( this ),
    m_tau( this ),
    m_jetAK4( this ),
    m_missingEt( this ),
    m_genParticle( this ),
    m_pileupReweightingTool( this ),
    m_bTaggingScaleTool( this ),
    m_ScaleFactorTool( this ),
    m_CorrectionTool( this ),
    m_VariableTool( this )
{

    m_logger << INFO << "Initialization" << SLogger::endmsg;
    SetLogName( GetName() );

    // Read configuration details from XML file
    DeclareProperty( "RecoTreeName",              m_recoTreeName = "physics" );
    // Declare trees
    DeclareProperty( "OutputTreeName",            m_outputTreeName = "DM");

    // Declare varibles in trees
    DeclareProperty( "IsData",                    m_isData                   = false );
    DeclareProperty( "IsSignal",                  m_isSignal                 = false );

    DeclareProperty( "ElectronName",              m_electronName             = "el" );
    DeclareProperty( "MuonName",                  m_muonName                 = "mu" );
    DeclareProperty( "TauName",                   m_tauName                  = "tau" );
    DeclareProperty( "JetAK4Name",                m_jetAK4Name               = "jetAK4" );
    DeclareProperty( "MissingEtName",             m_missingEtName            = "MET" );
    DeclareProperty( "GenParticleName",           m_genParticleName          = "genParticle" );


    DeclareProperty( "Elec1PtCut",                m_Elec1PtCut               =  40. );
    DeclareProperty( "Elec2PtCut",                m_Elec2PtCut               =  25. );
    DeclareProperty( "ElecPtCut",                 m_ElecPtCut                =  10. );
    DeclareProperty( "Elec1EtaCut",               m_Elec1EtaCut              =  2.1 );
    DeclareProperty( "Elec2EtaCut",               m_Elec2EtaCut              =  2.5 );
    DeclareProperty( "ElecEtaCut",                m_ElecEtaCut               =  2.5 );
    DeclareProperty( "Muon1PtCut",                m_Muon1PtCut               =  30. );
    DeclareProperty( "Muon2PtCut",                m_Muon2PtCut               =  10. );
    DeclareProperty( "MuonPtCut",                 m_MuonPtCut                =  10. );
    DeclareProperty( "Muon1EtaCut",               m_Muon1EtaCut              =  2.1 );
    DeclareProperty( "Muon2EtaCut",               m_Muon2EtaCut              =  2.4 );
    DeclareProperty( "MuonEtaCut",                m_MuonEtaCut               =  2.4 );
    DeclareProperty( "TauPtCut",                  m_TauPtCut                 =  18. );
    DeclareProperty( "TauEtaCut",                 m_TauEtaCut                =  2.3 );
    DeclareProperty( "AK4PtCut",                  m_AK4PtCut                 =  30. );
    DeclareProperty( "AK4EtaCut",                 m_AK4EtaCut                =  2.4 );
    DeclareProperty( "MEtPtCut",                  m_MEtPtCut                 =  250. );
    DeclareProperty( "RecPtCut",                  m_RecPtCut                 =  160. );
    DeclareProperty( "VPtCut",                    m_VPtCut                   = -1. );
    DeclareProperty( "nJetsCut",                  m_nJetsCut                 = 2 );

    // External objects name
    DeclareProperty( "JSONFileName",              m_JSONFileName             = "../GoodRunsLists/JSON/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt" ); //Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt

    // Prepend SFrame dir
    m_JSONFileName = std::string (std::getenv("SFRAME_DIR")) + m_JSONFileName;

}

// Destructor
DMAnalysis::~DMAnalysis() {

  m_logger << INFO << "Tschoe!" << SLogger::endmsg;

}


// Begin Job
void DMAnalysis::BeginCycle() throw( SError ) {

    m_logger << INFO << "Begin Job" << SLogger::endmsg;

    // Load the GRL:
    if (m_isData) {
        m_logger << INFO << "Loading GoodRunsList from file: " << m_JSONFileName << SLogger::endmsg;
        Root::TGoodRunsListReader reader( TString( m_JSONFileName.c_str() ) );
        if( !reader.Interpret() ) {
            m_logger << FATAL << "Couldn't read in the GRL!" << SLogger::endmsg;
            throw SError( ( "Couldn't read in file: " + m_JSONFileName ).c_str(), SError::SkipCycle );
        }
        m_grl = reader.GetMergedGoodRunsList();
        m_grl.Summary();
        m_grl.SetName( "MyGoodRunsList" );
        // Add it as a configuration object, so that the worker nodes will receive it:
        AddConfigObject( &m_grl );
    }

    m_triggerNames.clear();
    //m_triggerNames[""] = std::vector<std::string> ();
    //m_triggerNames["SingleMu"].push_back("HLT_Mu45_eta2p1_v");

    m_triggerNames["SingleIsoEle"].push_back("HLT_Ele27_eta2p1_WPTight_Gsf_v");
    m_triggerNames["SingleMu"].push_back("HLT_Mu50_v");
    m_triggerNames["SingleMu"].push_back("HLT_TkMu50_v");
    m_triggerNames["SingleIsoMu"].push_back("HLT_IsoMu27_v");
    m_triggerNames["SingleIsoMu"].push_back("HLT_IsoTkMu27_v");
    m_triggerNames["SingleIsoMu"].push_back("HLT_IsoMu24_v");
    m_triggerNames["SingleIsoMu"].push_back("HLT_IsoTkMu24_v");
    m_triggerNames["DoubleMu"].push_back("HLT_Mu27_TkMu8_v");
    m_triggerNames["DoubleIsoMu"].push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v");
    m_triggerNames["DoubleIsoMu"].push_back("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v");
    m_triggerNames["SingleEle"].push_back("HLT_Ele105_CaloIdVT_GsfTrkIdT_v");
    m_triggerNames["SingleEle"].push_back("HLT_Ele115_CaloIdVT_GsfTrkIdT_v");
    m_triggerNames["SingleIsoEle"].push_back("HLT_Ele32_WPTight_Gsf_v");
    m_triggerNames["SingleIsoEle"].push_back("HLT_Ele32_eta2p1_WPTight_Gsf_v");
    m_triggerNames["SingleIsoEle"].push_back("HLT_Ele27_WPLoose_Gsf_v");
    m_triggerNames["SingleIsoEle"].push_back("HLT_Ele27_WPTight_Gsf_v");
    m_triggerNames["DoubleIsoEle"].push_back("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");
    m_triggerNames["DoubleEle"].push_back("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v");
    m_triggerNames["METMHTNoMu"].push_back("HLT_PFMETNoMu90_PFMHTNoMu90_IDTight_v");
    m_triggerNames["METMHTNoMu"].push_back("HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v");
    m_triggerNames["METMHTNoMu"].push_back("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v");
    m_triggerNames["METMHTNoMu"].push_back("HLT_PFMETNoMu90_JetIdCleaned_PFMHTNoMu90_IDTight_v");
    m_triggerNames["METMHTNoMu"].push_back("HLT_PFMETNoMu120_JetIdCleaned_PFMHTNoMu120_IDTight_v");
    m_triggerNames["METMHTNoMu"].push_back("HLT_MonoCentralPFJet80_PFMETNoMu120_JetIdCleaned_PFMHTNoMu120_IDTight_v");
    m_triggerNames["METMHT"].push_back("HLT_PFMET110_PFMHT110_IDTight_v");
    m_triggerNames["METMHT"].push_back("HLT_PFMET120_PFMHT120_IDTight_v");
    //m_triggerNames["MET"].push_back("HLT_PFMET120_BTagCSV_p067_v");
    m_triggerNames["MET"].push_back("HLT_PFMET170_NoiseCleaned_v");
    m_triggerNames["MET"].push_back("HLT_PFMET170_HBHECleaned_v");
    m_triggerNames["MET"].push_back("HLT_PFMET170_HBHE_BeamHaloCleaned_v");

    return;
}


// End Job
void DMAnalysis::EndCycle() throw( SError ) {
    std::cout << "Events analyzed:\t" << nEvents <<std::endl;
    return;
}

void DMAnalysis::BeginInputData( const SInputData& id ) throw( SError ) {

    m_logger << INFO << "IsData:\t" <<                m_isData               << SLogger::endmsg;
    m_logger << INFO << "IsSignal:\t" <<              m_isSignal             << SLogger::endmsg;

    m_logger << INFO << "ElectronName:\t" <<          m_electronName         << SLogger::endmsg;
    m_logger << INFO << "MuonName:\t" <<              m_muonName             << SLogger::endmsg;
    m_logger << INFO << "TauName:\t" <<               m_tauName              << SLogger::endmsg;
    m_logger << INFO << "JetAK4Name:\t" <<            m_jetAK4Name           << SLogger::endmsg;
    m_logger << INFO << "MissingEtName:\t" <<         m_missingEtName        << SLogger::endmsg;
    m_logger << INFO << "GenParticleName:\t" <<       m_genParticleName      << SLogger::endmsg;

    m_logger << INFO << "Elec1PtCut:\t" <<            m_Elec1PtCut           << SLogger::endmsg;
    m_logger << INFO << "Elec2PtCut:\t" <<            m_Elec2PtCut           << SLogger::endmsg;
    m_logger << INFO << "ElecPtCut:\t" <<             m_ElecPtCut            << SLogger::endmsg;
    m_logger << INFO << "Elec1EtaCut:\t" <<            m_ElecEtaCut           << SLogger::endmsg;
    m_logger << INFO << "Elec2EtaCut:\t" <<            m_ElecEtaCut           << SLogger::endmsg;
    m_logger << INFO << "ElecEtaCut:\t" <<            m_ElecEtaCut           << SLogger::endmsg;
    m_logger << INFO << "Muon1PtCut:\t" <<            m_Muon1PtCut           << SLogger::endmsg;
    m_logger << INFO << "Muon2PtCut:\t" <<            m_Muon2PtCut           << SLogger::endmsg;
    m_logger << INFO << "MuonPtCut:\t" <<             m_MuonPtCut            << SLogger::endmsg;
    m_logger << INFO << "MuonEtaCut:\t" <<            m_MuonEtaCut           << SLogger::endmsg;
    m_logger << INFO << "Muon1EtaCut:\t" <<            m_MuonEtaCut           << SLogger::endmsg;
    m_logger << INFO << "Muon2EtaCut:\t" <<            m_MuonEtaCut           << SLogger::endmsg;
    m_logger << INFO << "TauPtCut:\t" <<              m_TauPtCut             << SLogger::endmsg;
    m_logger << INFO << "TauEtaCut:\t" <<             m_TauEtaCut            << SLogger::endmsg;
    m_logger << INFO << "AK4PtCut:\t" <<              m_AK4PtCut             << SLogger::endmsg;
    m_logger << INFO << "AK4EtaCut:\t" <<             m_AK4EtaCut            << SLogger::endmsg;
    m_logger << INFO << "MEtPtCut:\t" <<              m_MEtPtCut             << SLogger::endmsg;
    m_logger << INFO << "RecPtCut:\t" <<              m_RecPtCut             << SLogger::endmsg;
    m_logger << INFO << "VPtCut:\t" <<                m_VPtCut               << SLogger::endmsg;
    m_logger << INFO << "nJetsCut:\t" <<              m_nJetsCut             << SLogger::endmsg;


    // External objects name
    m_logger << INFO << "JSONFileName:\t" <<          m_JSONFileName         << SLogger::endmsg;

    if(m_isData) {
        TObject* grl;
        if( ! ( grl = GetConfigObject( "MyGoodRunsList" ) ) ) {
          m_logger << FATAL << "Can't access the GRL!" << SLogger::endmsg;
          throw SError( "Can't access the GRL!", SError::SkipCycle );
        }
        m_grl = *( dynamic_cast< Root::TGoodRunsList* >( grl ) );
    }
    else {
        m_pileupReweightingTool.BeginInputData( id );
    }

    m_bTaggingScaleTool.BeginInputData( id );
    m_ScaleFactorTool.BeginInputData( id );
    m_CorrectionTool.BeginInputData( id );
    m_VariableTool.BeginInputData( id );

    if(isMC) {
        m_bTaggingScaleTool.bookHistograms();
    }


    //
    // output branches
    //
    DeclareVariable( isMC,                "isMC",  m_outputTreeName.c_str());
    DeclareVariable( EventNumber,         "eventNumber",  m_outputTreeName.c_str());
    DeclareVariable( LumiNumber,          "lumiNumber",  m_outputTreeName.c_str());
    DeclareVariable( RunNumber,           "runNumber",  m_outputTreeName.c_str());

    DeclareVariable( EventWeight,         "eventWeight",  m_outputTreeName.c_str());
    DeclareVariable( WewkWeight,          "WewkWeight",  m_outputTreeName.c_str());
    DeclareVariable( ZewkWeight,          "ZewkWeight",  m_outputTreeName.c_str());
    DeclareVariable( WqcdWeight,          "WqcdWeight",  m_outputTreeName.c_str());
    DeclareVariable( ZqcdWeight,          "ZqcdWeight",  m_outputTreeName.c_str());
    DeclareVariable( TopWeight,           "TopWeight",  m_outputTreeName.c_str());
    DeclareVariable( QCDRenWeightUp,      "QCDRenWeightUp",  m_outputTreeName.c_str());
    DeclareVariable( QCDRenWeightDown,    "QCDRenWeightDown",  m_outputTreeName.c_str());
    DeclareVariable( QCDFacWeightUp,      "QCDFacWeightUp",  m_outputTreeName.c_str());
    DeclareVariable( QCDFacWeightDown,    "QCDFacWeightDown",  m_outputTreeName.c_str());
    DeclareVariable( PUWeight,            "puWeight",  m_outputTreeName.c_str());
    DeclareVariable( PUWeightUp,          "puWeightUp",  m_outputTreeName.c_str());
    DeclareVariable( PUWeightDown,        "puWeightDown",  m_outputTreeName.c_str());
    DeclareVariable( TriggerWeight,       "triggerWeight",  m_outputTreeName.c_str());
    DeclareVariable( TriggerWeightUp,     "triggerWeightUp",  m_outputTreeName.c_str());
    DeclareVariable( TriggerWeightDown,   "triggerWeightDown",  m_outputTreeName.c_str());
    DeclareVariable( LeptonWeight,        "leptonWeight",  m_outputTreeName.c_str());
    DeclareVariable( LeptonWeightUp,      "leptonWeightUp",  m_outputTreeName.c_str());
    DeclareVariable( LeptonWeightDown,    "leptonWeightDown",  m_outputTreeName.c_str());
    DeclareVariable( BTagWeight,          "bTagWeight",  m_outputTreeName.c_str());
    DeclareVariable( BTagWeightUp,        "bTagWeightUp",  m_outputTreeName.c_str());
    DeclareVariable( BTagWeightDown,      "bTagWeightDown",  m_outputTreeName.c_str());

    DeclareVariable( isZtoNN,             "isZtoNN",  m_outputTreeName.c_str());
    DeclareVariable( isWtoEN,             "isWtoEN",  m_outputTreeName.c_str());
    DeclareVariable( isWtoMN,             "isWtoMN",  m_outputTreeName.c_str());
    DeclareVariable( isTtoEM,             "isTtoEM",  m_outputTreeName.c_str());
    DeclareVariable( isZtoEE,             "isZtoEE",  m_outputTreeName.c_str());
    DeclareVariable( isZtoMM,             "isZtoMM",  m_outputTreeName.c_str());

    DeclareVariable( nPV,                 "nPV",  m_outputTreeName.c_str());
    DeclareVariable( nElectrons,          "nElectrons",  m_outputTreeName.c_str());
    DeclareVariable( nMuons,              "nMuons",  m_outputTreeName.c_str());
    DeclareVariable( nTaus,               "nTaus",  m_outputTreeName.c_str());
    DeclareVariable( nJets,               "nJets",  m_outputTreeName.c_str());
    DeclareVariable( nForwardJets,        "nForwardJets",  m_outputTreeName.c_str());
    DeclareVariable( nBJets,              "nBJets",  m_outputTreeName.c_str());
    DeclareVariable( nBQuarks,            "nBQuarks",  m_outputTreeName.c_str());
    DeclareVariable( nBTagJets,           "nBTagJets",  m_outputTreeName.c_str());

    DeclareVariable( LheNl,               "LheNl",  m_outputTreeName.c_str());
    DeclareVariable( LheNj,               "LheNj",  m_outputTreeName.c_str());
    DeclareVariable( LheHT,               "LheHT",  m_outputTreeName.c_str());
    DeclareVariable( LheV_pt,             "LheV_pt",  m_outputTreeName.c_str());

    DeclareVariable( MET_pt,              "MET_pt",  m_outputTreeName.c_str());
    DeclareVariable( MET_phi,             "MET_phi",  m_outputTreeName.c_str());
    DeclareVariable( MET_sign,            "MET_sign",  m_outputTreeName.c_str());
    DeclareVariable( fakeMET_pt,          "FakeMET_pt",  m_outputTreeName.c_str());
    DeclareVariable( ST,                  "ST",  m_outputTreeName.c_str());
    DeclareVariable( HT,                  "HT",  m_outputTreeName.c_str());
    DeclareVariable( MinJetMetDPhi,       "MinDPhi",  m_outputTreeName.c_str());
    DeclareVariable( MinLepMetDPhi,       "MinLepMetDPhi",m_outputTreeName.c_str());
    DeclareVariable( MinBJetMetDPhi,      "MinBJetMetDPhi",m_outputTreeName.c_str());
    DeclareVariable( MinLepJetDPhi,       "MinLepJetDPhi",m_outputTreeName.c_str());
    DeclareVariable( MaxLepMetDPhi,       "MaxLepMetDPhi",m_outputTreeName.c_str());
    DeclareVariable( MaxJetMetDPhi,       "MaxJetMetDPhi",  m_outputTreeName.c_str());
    DeclareVariable( MaxBJetMetDPhi,      "MaxBJetMetDPhi",m_outputTreeName.c_str());
    DeclareVariable( MaxLepJetDPhi,       "MaxLepJetDPhi",m_outputTreeName.c_str());
    DeclareVariable( MinJetMetDPhi12,     "MinDPhi12",  m_outputTreeName.c_str());
    
    DeclareVariable( V_pt,                "V_pt",  m_outputTreeName.c_str());
    DeclareVariable( mZ,                  "mZ",  m_outputTreeName.c_str());
    DeclareVariable( mT,                  "mT",  m_outputTreeName.c_str());
    DeclareVariable( mT2,                 "mT2",  m_outputTreeName.c_str());
    
    DeclareVariable( mW,                  "mW",  m_outputTreeName.c_str());
    DeclareVariable( mTop,                "mTop",  m_outputTreeName.c_str());
    DeclareVariable( kTop,                "kTop",  m_outputTreeName.c_str());

    DeclareVariable( Lepton1_pt,          "Lepton1_pt",  m_outputTreeName.c_str());
    DeclareVariable( Lepton2_pt,          "Lepton2_pt",  m_outputTreeName.c_str());
    DeclareVariable( Lepton1_eta,         "Lepton1_eta",  m_outputTreeName.c_str());
    DeclareVariable( Lepton2_eta,         "Lepton2_eta",  m_outputTreeName.c_str());
    DeclareVariable( Lepton1_phi,         "Lepton1_phi",  m_outputTreeName.c_str());
    DeclareVariable( Lepton2_phi,         "Lepton2_phi",  m_outputTreeName.c_str());
    DeclareVariable( Lepton1_id,          "Lepton1_id",  m_outputTreeName.c_str());
    DeclareVariable( Lepton2_id,          "Lepton2_id",  m_outputTreeName.c_str());
    DeclareVariable( Lepton1_pfIso,       "Lepton1_pfIso",  m_outputTreeName.c_str());
    DeclareVariable( Lepton2_pfIso,       "Lepton2_pfIso",  m_outputTreeName.c_str());

    DeclareVariable( Jet1_pt,             "Jet1_pt",  m_outputTreeName.c_str());
    DeclareVariable( Jet2_pt,             "Jet2_pt",  m_outputTreeName.c_str());
    DeclareVariable( Jet3_pt,             "Jet3_pt",  m_outputTreeName.c_str());
    DeclareVariable( Jet4_pt,             "Jet4_pt",  m_outputTreeName.c_str());
    DeclareVariable( Jet5_pt,             "Jet5_pt",  m_outputTreeName.c_str());
    DeclareVariable( Jet6_pt,             "Jet6_pt",  m_outputTreeName.c_str());
    DeclareVariable( JetF_pt,             "JetF_pt",  m_outputTreeName.c_str());
    DeclareVariable( Jet1_E,              "Jet1_E",  m_outputTreeName.c_str());
    DeclareVariable( Jet2_E,              "Jet2_E",  m_outputTreeName.c_str());
    DeclareVariable( Jet3_E,              "Jet3_E",  m_outputTreeName.c_str());
    DeclareVariable( Jet4_E,              "Jet4_E",  m_outputTreeName.c_str());
    DeclareVariable( Jet5_E,              "Jet5_E",  m_outputTreeName.c_str());
    DeclareVariable( Jet6_E,              "Jet6_E",  m_outputTreeName.c_str());
    DeclareVariable( JetF_E,              "JetF_E",  m_outputTreeName.c_str());
    DeclareVariable( Jet1_eta,            "Jet1_eta",  m_outputTreeName.c_str());
    DeclareVariable( Jet2_eta,            "Jet2_eta",  m_outputTreeName.c_str());
    DeclareVariable( Jet3_eta,            "Jet3_eta",  m_outputTreeName.c_str());
    DeclareVariable( Jet4_eta,            "Jet4_eta",  m_outputTreeName.c_str());
    DeclareVariable( Jet5_eta,            "Jet5_eta",  m_outputTreeName.c_str());
    DeclareVariable( Jet6_eta,            "Jet6_eta",  m_outputTreeName.c_str());
    DeclareVariable( JetF_eta,            "JetF_eta",  m_outputTreeName.c_str());
    DeclareVariable( Jet1_phi,            "Jet1_phi",  m_outputTreeName.c_str());
    DeclareVariable( Jet2_phi,            "Jet2_phi",  m_outputTreeName.c_str());
    DeclareVariable( Jet3_phi,            "Jet3_phi",  m_outputTreeName.c_str());
    DeclareVariable( Jet4_phi,            "Jet4_phi",  m_outputTreeName.c_str());
    DeclareVariable( Jet5_phi,            "Jet5_phi",  m_outputTreeName.c_str());
    DeclareVariable( Jet6_phi,            "Jet6_phi",  m_outputTreeName.c_str());
    DeclareVariable( JetF_phi,            "JetF_phi",  m_outputTreeName.c_str());
    DeclareVariable( Jet1_csv,            "Jet1_csv",  m_outputTreeName.c_str());
    DeclareVariable( Jet2_csv,            "Jet2_csv",  m_outputTreeName.c_str());
    DeclareVariable( Jet3_csv,            "Jet3_csv",  m_outputTreeName.c_str());
    DeclareVariable( Jet4_csv,            "Jet4_csv",  m_outputTreeName.c_str());
    DeclareVariable( Jet5_csv,            "Jet5_csv",  m_outputTreeName.c_str());
    DeclareVariable( Jet6_csv,            "Jet6_csv",  m_outputTreeName.c_str());

    //DeclareVariable( Sphericity,          "Sphericity", m_outputTreeName.c_str());
    //DeclareVariable( Aplanarity,          "Aplanarity", m_outputTreeName.c_str());
    
    // DeclareVariable( CosThetaStar,        "CosThetaStar", m_outputTreeName.c_str());
    // DeclareVariable( CosTheta1,           "CosTheta1", m_outputTreeName.c_str());
    // DeclareVariable( CosTheta2,           "CosTheta2", m_outputTreeName.c_str());
    // DeclareVariable( Phi,                 "Phi", m_outputTreeName.c_str());
    // DeclareVariable( Phi1,                "Phi1", m_outputTreeName.c_str());
    // DeclareVariable( CosThetaJ,           "CosThetaJ", m_outputTreeName.c_str());



    //
    // Declare the output histograms:
    //
    // Histograms in main directory
    Book( TH1F( "Events", ";;Events", 1, 0, 1));
    // Cutflow
    std::vector<std::string> labelsZtoNN = {"All", "Trigger", "MET", "Lepton veto", "Tau veto", "Cleaning", "Jet Noise", "Jets #geq 2", "H mass", "1 b-tag", "2 b-tag"};
    std::vector<std::string> labelsZtoEE = {"All", "Trigger", "Ele reco", "Ele p_{T}", "Ele Id+Iso", "Z(ee) candidate", "Z p_{T}", "Jets #geq 2", "H mass", "1 b-tag", "2 b-tag"};
    std::vector<std::string> labelsZtoMM = {"All", "Trigger", "Muon reco", "Muon p_{T}", "Muon Id", "Muon Iso", "Z(#mu#mu) candidate", "Z p_{T}", "Jets #geq 2", "H mass", "1 b-tag", "2 b-tag"};

    Book( TH1F( "Events", ";;Events;log", labelsZtoNN.size(), 0, labelsZtoNN.size()), "0l" );
    Book( TH1F( "Events", ";;Events;log", labelsZtoEE.size(), 0, labelsZtoEE.size()), "2e" );
    Book( TH1F( "Events", ";;Events;log", labelsZtoMM.size(), 0, labelsZtoMM.size()), "2m" );

    for(unsigned int i = 0; i < labelsZtoNN.size(); i++) Hist("Events", "0l")->GetXaxis()->SetBinLabel(i+1, labelsZtoNN[i].c_str());
    for(unsigned int i = 0; i < labelsZtoEE.size(); i++) Hist("Events", "2e")->GetXaxis()->SetBinLabel(i+1, labelsZtoEE[i].c_str());
    for(unsigned int i = 0; i < labelsZtoMM.size(); i++) Hist("Events", "2m")->GetXaxis()->SetBinLabel(i+1, labelsZtoMM[i].c_str());

    // ---------- SEMILEPTONIC Z->nn CHANNEL ----------
    Book( TH1F( "EventWeight", ";Event Weight;Events", 100, 0., 2.), "0l" );
    Book( TH1F( "GenWeight", ";Gen Weight;Events", 100, 0., 2.), "0l" );
    Book( TH1F( "TopWeight", ";Top Weight;Events", 100, 0., 2.), "0l" );
    Book( TH1F( "PUWeight", ";Pileup Weight;Events", 100, 0., 10.), "0l" );
    Book( TH1F( "TriggerWeight", ";Trigger Weight;Events", 100, 0., 2.), "0l" );
    Book( TH1F( "LeptonWeight", ";Lepton Weight;Events", 100, 0., 2.), "0l" );
    Book( TH1F( "BTagWeight", ";BTag Weight;Events", 100, 0., 2.), "0l" );
    Book( TH1F( "Jet1_chf", ";jet charged hadron fraction;Events", 25, 0, 1), "0l" );
    Book( TH1F( "Jet1_nhf", ";jet neutral hadron fraction;Events", 25, 0, 1), "0l" );
    Book( TH1F( "Jet1_phf", ";jet photon fraction;Events", 25, 0, 1), "0l" );
    Book( TH1F( "Jet1_muf", ";jet muon fraction;Events", 25, 0, 1), "0l" );
    Book( TH1F( "Jet1_emf", ";jet electron fraction;Events", 25, 0, 1), "0l" );
    Book( TH1F( "Jet1_chm", ";jet charged hadron multiplicity;Events", 100, 0, 100), "0l" );
    Book( TH1F( "Jet2_chf", ";jet charged hadron fraction;Events", 25, 0, 1), "0l" );
    Book( TH1F( "Jet2_nhf", ";jet neutral hadron fraction;Events", 25, 0, 1), "0l" );
    Book( TH1F( "Jet2_phf", ";jet photon fraction;Events", 25, 0, 1), "0l" );
    Book( TH1F( "Jet2_muf", ";jet muon fraction;Events", 25, 0, 1), "0l" );
    Book( TH1F( "Jet2_emf", ";jet electron fraction;Events", 25, 0, 1), "0l" );
    Book( TH1F( "Jet2_chm", ";jet charged hadron multiplicity;Events", 100, 0, 100), "0l" );
    Book( TH1F( "Jet1MetDPhi", ";#Delta #varphi (jet1-#slash{E}_{T});Events", 28, 0, 3.15), "0l" );
    Book( TH1F( "Jet2MetDPhi", ";#Delta #varphi (jet1-#slash{E}_{T});Events", 28, 0, 3.15), "0l" );
    Book( TH1F( "MinJetMetDPhi", ";min #Delta #varphi (jet-#slash{E}_{T});Events;log", 28, 0, 3.15), "0l" );
    Book( TH1F( "nJets", ";number of jets;Events;log", 10, -0.5, 9.5), "0l" );
    Book( TH1F( "HT", ";HT (GeV);Events;log", 80, 0, 2000), "0l" );
    Book( TH1F( "MinBJetMetDPhi", ";min #Delta #varphi (bjets-#slash{E}_{T});Events;log", 28, 0, 3.15), "0l");
    Book( TH1F( "MaxJetMetDPhi", ";max #Delta #varphi (jet-#slash{E}_{T});Events;log", 28, 0, 3.15), "0l" );
    Book( TH1F( "MaxBJetMetDPhi", ";max #Delta #varphi (bjets-#slash{E}_{T});Events;log", 28, 0, 3.15), "0l");
    Book( TH1F( "METFilters", "Events;", 15, -0.5, 14.5), "0l" );
    
    
    // ---------- SEMILEPTONIC W->en CHANNEL ----------
    Book( TH1F( "EventWeight", ";Event Weight;Events", 100, 0., 2.), "1e" );
    Book( TH1F( "GenWeight", ";Gen Weight;Events", 100, 0., 2.), "1e" );
    Book( TH1F( "TopWeight", ";Top Weight;Events", 100, 0., 2.), "1e" );
    Book( TH1F( "PUWeight", ";Pileup Weight;Events", 100, 0., 10.), "1e" );
    Book( TH1F( "TriggerWeight", ";Trigger Weight;Events", 100, 0., 2.), "1e" );
    Book( TH1F( "LeptonWeight", ";Lepton Weight;Events", 100, 0., 2.), "1e" );
    Book( TH1F( "BTagWeight", ";BTag Weight;Events", 100, 0., 2.), "1e" );
    Book( TH1F( "nJets", ";number of jets;Events;log", 10, -0.5, 9.5), "1e" );
    Book( TH1F( "HT", ";HT (GeV);Events;log", 80, 0, 2000), "1e" );
    Book( TH1F( "Electron1_Id", ";electron 1 Id;Events;log", 5, -0.5, 4.5), "1e" );
    Book( TH1F( "Electron1_pfIso", ";electron 1 PFIso;Events;log", 25, 0, 5.), "1e" );
    Book( TH1F( "W_pt", ";p_{T}^{W} (GeV);Events;log", 80, 0, 2000), "1e" );
    Book( TH1F( "W_tmass", ";m_{T}^{W} (GeV);Events", 50, 0, 250), "1e" );
    Book( TH1F( "MinLepMetDPhi", ";min #Delta #varphi (lepton-#slash{E}_{T});Events;log", 28, 0, 3.15), "1e");
    Book( TH1F( "MinLepJetDPhi", ";min #Delta #varphi (lepton-jet);Events;log", 28, 0, 3.15), "1e");
    Book( TH1F( "MaxLepMetDPhi", ";max #Delta #varphi (lepton-#slash{E}_{T});Events;log", 28, 0, 3.15), "1e");
    Book( TH1F( "MaxLepJetDPhi", ";max #Delta #varphi (lepton-jet);Events;log", 28, 0, 3.15), "1e");

    // ---------- SEMILEPTONIC W->mn CHANNEL ----------
    Book( TH1F( "EventWeight", ";Event Weight;Events", 100, 0., 2.), "1m" );
    Book( TH1F( "GenWeight", ";Gen Weight;Events", 100, 0., 2.), "1m" );
    Book( TH1F( "TopWeight", ";Top Weight;Events", 100, 0., 2.), "1m" );
    Book( TH1F( "PUWeight", ";Pileup Weight;Events", 100, 0., 10.), "1m" );
    Book( TH1F( "TriggerWeight", ";Trigger Weight;Events", 100, 0., 2.), "1m" );
    Book( TH1F( "LeptonWeight", ";Lepton Weight;Events", 100, 0., 2.), "1m" );
    Book( TH1F( "BTagWeight", ";BTag Weight;Events", 100, 0., 2.), "1m" );
    Book( TH1F( "nJets", ";number of jets;Events;log", 10, -0.5, 9.5), "1m" );
    Book( TH1F( "HT", ";HT (GeV);Events;log", 80, 0, 2000), "1m" );
    Book( TH1F( "Muon1_Id", ";muon 1 Id;Events;log", 5, -0.5, 4.5), "1m" );
    Book( TH1F( "Muon1_pfIso", ";muon 1 PFIso;Events;log", 50, 0, 5.), "1m" );
    Book( TH1F( "W_pt", ";p_{T}^{W} (GeV);Events;log", 80, 0, 2000), "1m" );
    Book( TH1F( "W_tmass", ";m_{T}^{W} (GeV);Events", 50, 0, 250), "1m" );


    // ---------- SEMILEPTONIC Z->ee CHANNEL ----------
    Book( TH1F( "EventWeight", ";Event Weight;Events", 100, 0., 2.), "2e" );
    Book( TH1F( "GenWeight", ";Gen Weight;Events", 100, 0., 2.), "2e" );
    Book( TH1F( "TopWeight", ";Top Weight;Events", 100, 0., 2.), "2e" );
    Book( TH1F( "PUWeight", ";Pileup Weight;Events", 100, 0., 10.), "2e" );
    Book( TH1F( "TriggerWeight", ";Trigger Weight;Events", 100, 0., 2.), "2e" );
    Book( TH1F( "LeptonWeight", ";Lepton Weight;Events", 100, 0., 2.), "2e" );
    Book( TH1F( "BTagWeight", ";BTag Weight;Events", 100, 0., 2.), "2e" );
    Book( TH1F( "nJets", ";number of jets;Events;log", 10, -0.5, 9.5), "2e" );
    Book( TH1F( "HT", ";HT (GeV);Events;log", 80, 0, 2000), "2e" );
    Book( TH1F( "Electron1_Id", ";electron 1 Id;Events;log", 5, -0.5, 4.5), "2e" );
    Book( TH1F( "Electron2_Id", ";electron 2 Id;Events;log", 5, -0.5, 4.5), "2e" );
    Book( TH1F( "Electron1_pfIso", ";electron 1 PFIso;Events;log", 25, 0, 5.), "2e" );
    Book( TH1F( "Electron2_pfIso", ";electron 2 PFIso;Events;log", 25, 0, 5.), "2e" );
    Book( TH1F( "Z_pt", ";p_{T}^{Z} (GeV);Events;log", 100, 0, 2000), "2e" );
    Book( TH1F( "Z_mass", ";Z mass (GeV);Events", 40, 70, 110), "2e" );
    Book( TH1F( "Z_massRaw", ";Z mass [uncorrected] (GeV);Events", 40, 70, 110), "2e" );
    Book( TH1F( "Z_massBB", ";Z mass [barrel-barrel] (GeV);Events", 40, 70, 110), "2e" );
    Book( TH1F( "Z_massBE", ";Z mass [barrel-endcaps] (GeV);Events", 40, 70, 110), "2e" );
    Book( TH1F( "Z_massEB", ";Z mass [endcaps-barrel] (GeV);Events", 40, 70, 110), "2e" );
    Book( TH1F( "Z_massEE", ";Z mass [endcaps-endcaps] (GeV);Events", 40, 70, 110), "2e" );


    // ---------- SEMILEPTONIC Z->mm CHANNEL ----------
    Book( TH1F( "EventWeight", ";Event Weight;Events", 100, 0., 2.), "2m" );
    Book( TH1F( "GenWeight", ";Gen Weight;Events", 100, 0., 2.), "2m" );
    Book( TH1F( "TopWeight", ";Top Weight;Events", 100, 0., 2.), "2m" );
    Book( TH1F( "PUWeight", ";Pileup Weight;Events", 100, 0., 10.), "2m" );
    Book( TH1F( "TriggerWeight", ";Trigger Weight;Events", 100, 0., 2.), "2m" );
    Book( TH1F( "LeptonWeight", ";Lepton Weight;Events", 100, 0., 2.), "2m" );
    Book( TH1F( "BTagWeight", ";BTag Weight;Events", 100, 0., 2.), "2m" );
    Book( TH1F( "nJets", ";number of jets;Events;log", 10, -0.5, 9.5), "2m" );
    Book( TH1F( "HT", ";HT (GeV);Events;log", 80, 0, 2000), "2m" );
    Book( TH1F( "Muon1_Id", ";muon 1 Id;Events;log", 5, -0.5, 4.5), "2m" );
    Book( TH1F( "Muon2_Id", ";muon 2 Id;Events;log", 5, -0.5, 4.5), "2m" );
    Book( TH1F( "Muon1_pfIso", ";muon 1 PFIso;Events;log", 50, 0, 5.), "2m" );
    Book( TH1F( "Muon2_pfIso", ";muon 2 PFIso;Events;log", 50, 0, 5.), "2m" );
    Book( TH1F( "Z_pt", ";p_{T}^{Z} (GeV);Events;log", 100, 0, 2000), "2m" );
    Book( TH1F( "Z_mass", ";Z mass (GeV);Events", 40, 70, 110), "2m" );
    Book( TH1F( "Z_massRaw", ";Z mass [uncorrected] (GeV);Events", 40, 70, 110), "2m" );
    Book( TH1F( "Z_massBB", ";Z mass [barrel-barrel] (GeV);Events", 40, 70, 110), "2m" );
    Book( TH1F( "Z_massBE", ";Z mass [barrel-endcaps] (GeV);Events", 40, 70, 110), "2m" );
    Book( TH1F( "Z_massEB", ";Z mass [endcaps-barrel] (GeV);Events", 40, 70, 110), "2m" );
    Book( TH1F( "Z_massEE", ";Z mass [endcaps-endcaps] (GeV);Events", 40, 70, 110), "2m" );

    // ---------- DILEPTONIC T->em CHANNEL ----------
    Book( TH1F( "EventWeight", ";Event Weight;Events", 100, 0., 2.), "1e1m" );
    Book( TH1F( "GenWeight", ";Gen Weight;Events", 100, 0., 2.), "1e1m" );
    Book( TH1F( "TopWeight", ";Top Weight;Events", 100, 0., 2.), "1e1m" );
    Book( TH1F( "PUWeight", ";Pileup Weight;Events", 100, 0., 10.), "1e1m" );
    Book( TH1F( "TriggerWeight", ";Trigger Weight;Events", 100, 0., 2.), "1e1m" );
    Book( TH1F( "LeptonWeight", ";Lepton Weight;Events", 100, 0., 2.), "1e1m" );
    Book( TH1F( "BTagWeight", ";BTag Weight;Events", 100, 0., 2.), "1e1m" );
    Book( TH1F( "nJets", ";number of jets;Events;log", 10, -0.5, 9.5), "1e1m" );
    Book( TH1F( "HT", ";HT (GeV);Events;log", 80, 0, 2000), "1e1m" );
    Book( TH1F( "Electron1_Id", ";electron 1 Id;Events;log", 5, -0.5, 4.5), "1e1m" );
    Book( TH1F( "Muon1_Id", ";muon 1 Id;Events;log", 5, -0.5, 4.5), "1e1m" );
    Book( TH1F( "Electron1_pfIso", ";electron 1 PFIso;Events;log", 50, 0, 5.), "1e1m" );
    Book( TH1F( "Muon1_pfIso", ";muon 1 PFIso;Events;log", 50, 0, 5.), "1e1m" );

    // Gen
    Book( TH1F( "LheV_pt", ";V p_{T} (GeV);Events;log", 400, 0, 4000), "Gen" );
    Book( TH1F( "GenV_pt", ";V p_{T} (GeV);Events;log", 400, 0, 4000), "Gen" );
    Book( TH1F( "LheV_mass", ";V mass (GeV);Events;log", 50, 0, 150), "Gen" );
    Book( TH1F( "GenV_mass", ";V mass (GeV);Events;log", 50, 0, 150), "Gen" );
    Book( TH1F( "Lhe_HT", ";HT (GeV);Events;log", 400, 0, 4000), "Gen" );
    Book( TH1F( "Gen_HT", ";HT (GeV);Events;log", 400, 0, 4000), "Gen" );
    Book( TH1F( "LheNl", ";number of gen leptons;Events;log", 5, -0.5, 4.5), "Gen" );
    Book( TH1F( "LheNj", ";number of gen partons;Events;log", 5, -0.5, 4.5), "Gen" );
    Book( TH1F( "GenX_mass", ";X mass (GeV);Events;log", 1000, 100, 1100), "Gen" );

    // Trigger
    Book( TH1F( "HLT_PFMET170_vs_SingleElectron_NUM", ";MET (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET170_vs_SingleElectron_DEN", ";MET (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET120_PFMHT120_vs_SingleElectron_NUM", ";MET (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET120_PFMHT120_vs_SingleElectron_DEN", ";MET (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMETNoMu120_PFMHTNoMu120_vs_SingleElectron_NUM", ";min(METNoMu, MHTNoMu) (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMETNoMu120_PFMHTNoMu120_vs_SingleElectron_DEN", ";min(METNoMu, MHTNoMu) (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET_OR_vs_SingleElectron_NUM", ";min(METNoMu, MHTNoMu) (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET_OR_vs_SingleElectron_DEN", ";min(METNoMu, MHTNoMu) (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET170_vs_SingleMuon_NUM", ";MET (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET170_vs_SingleMuon_DEN", ";MET (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET120_PFMHT120_vs_SingleMuon_NUM", ";MET (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET120_PFMHT120_vs_SingleMuon_DEN", ";MET (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMETNoMu120_PFMHTNoMu120_vs_SingleMuon_NUM", ";min(METNoMu, MHTNoMu) (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMETNoMu120_PFMHTNoMu120_vs_SingleMuon_DEN", ";min(METNoMu, MHTNoMu) (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET_OR_vs_SingleMuon_NUM", ";min(METNoMu, MHTNoMu) (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET_OR_vs_SingleMuon_DEN", ";min(METNoMu, MHTNoMu) (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET_OR_vs_JetHT_NUM", ";min(METNoMu, MHTNoMu) (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET_OR_vs_JetHT_DEN", ";min(METNoMu, MHTNoMu) (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET_OR_NUM", ";min(METNoMu, MHTNoMu) (GeV);Efficiency", 100, 100, 600), "Trigger" );
    Book( TH1F( "HLT_PFMET_OR_DEN", ";min(MET, MHT) (GeV);Efficiency", 100, 100, 600), "Trigger" );
    
    // Event shape variables
    Book( TH1F( "Sphericity", "S", 100, 0., 1.), "Trigger" );
    Book( TH1F( "Aplanarity", "A", 100, 0., 1./2.), "Trigger" );
    
    // // Angular variables
    // Book( TH1F( "CosThetaStar", "cos #vartheta *", 100, -1., 1.), "Trigger" );
    // Book( TH1F( "CosTheta1", "cos #vartheta_1", 100, -1., 1.), "Trigger" );
    // Book( TH1F( "CosTheta2", "cos #vartheta_2", 100, -1., 1.), "Trigger" );
    // Book( TH1F( "Phi", "#varphi", 100, 0., 3.15), "Trigger" );
    // Book( TH1F( "Phi1", "#varphi_1", 100, 0., 3.15), "Trigger" );
    // Book( TH1F( "CosThetaJ", "cos #vartheta_J", 100, -1., 1.), "Trigger" );

    return;
}

void DMAnalysis::EndInputData( const SInputData& ) throw( SError ) {

   return;

}

void DMAnalysis::BeginInputFile( const SInputData& ) throw( SError ) {
    m_logger << INFO << "Connecting input variables" << SLogger::endmsg;

    if(m_isData) m_eventInfo.ConnectVariables( m_recoTreeName.c_str(), Ntuple::EventInfoTrigger|Ntuple::EventInfoMETFilters|Ntuple::EventInfoMuonFilters, "" );
    else m_eventInfo.ConnectVariables( m_recoTreeName.c_str(), Ntuple::EventInfoBasic|Ntuple::EventInfoTrigger|Ntuple::EventInfoMETFilters|Ntuple::EventInfoTruth|Ntuple::EventInfoPDF, "" );

    if(m_isData) m_electron.ConnectVariables( m_recoTreeName.c_str(), Ntuple::ElectronBasic|Ntuple::ElectronID|Ntuple::ElectronAdvancedID|Ntuple::ElectronIsolation|Ntuple::ElectronSuperCluster, (m_electronName + "_").c_str() ); // |Ntuple::ElectronShowerID
    else m_electron.ConnectVariables( m_recoTreeName.c_str(), Ntuple::ElectronBasic|Ntuple::ElectronID|Ntuple::ElectronAdvancedID|Ntuple::ElectronIsolation|Ntuple::ElectronSuperCluster, (m_electronName + "_").c_str() );
    m_muon.ConnectVariables( m_recoTreeName.c_str(), Ntuple::MuonBasic|Ntuple::MuonID|Ntuple::MuonIsolation|Ntuple::MuonTrack, (m_muonName + "_").c_str() );
    m_tau.ConnectVariables( m_recoTreeName.c_str(), Ntuple::TauBasic|Ntuple::TauID|Ntuple::TauAdvancedID, (m_tauName + "_").c_str() );

    if(m_isData) m_jetAK4.ConnectVariables( m_recoTreeName.c_str(), Ntuple::JetBasic|Ntuple::JetAnalysis, (m_jetAK4Name + "_").c_str() );
    else m_jetAK4.ConnectVariables( m_recoTreeName.c_str(), Ntuple::JetBasic|Ntuple::JetAnalysis|Ntuple::JetTruth, (m_jetAK4Name + "_").c_str() );

//    if(m_isData) m_jetAK8.ConnectVariables( m_recoTreeName.c_str(), Ntuple::JetBasic|Ntuple::JetAnalysis|Ntuple::JetSubstructure|Ntuple::JetSoftdropSubjets, (m_jetAK8Name + "_").c_str() );
//    else {
//        m_jetAK8.ConnectVariables( m_recoTreeName.c_str(), Ntuple::JetBasic|Ntuple::JetAnalysis|Ntuple::JetSubstructure|Ntuple::JetTruth|Ntuple::JetSoftdropSubjets|Ntuple::JetSoftdropSubjetsTruth|Ntuple::JetJER, (m_jetAK8Name + "_").c_str() );
//        m_genjetAK8.ConnectVariables( m_recoTreeName.c_str(), Ntuple::GenJetTruth, "" );
//    }
//
//    m_jetAK8Puppi.ConnectVariables( m_recoTreeName.c_str(), Ntuple::PuppiJetBasic|Ntuple::PuppiJetAnalysis|Ntuple::PuppiJetSoftdropSubjets, "" );

    m_missingEt.ConnectVariables( m_recoTreeName.c_str(), Ntuple::MissingEtBasic|Ntuple::MissingEtAnalysis , (m_missingEtName + "_").c_str() );
    if(!m_isData) m_genParticle.ConnectVariables( m_recoTreeName.c_str(), Ntuple::GenParticleBasic, (m_genParticleName + "_").c_str() );

    m_logger << INFO << "Connecting input variables completed" << SLogger::endmsg;
    return;
}



void DMAnalysis::ExecuteEvent( const SInputData&, Double_t ) throw( SError ) {

    clearBranches();
    std::string sample(this->GetHistInputFile()->GetName());

    m_logger << INFO << "ExecuteEvent\tevent: " << m_eventInfo.eventNumber << "\tlumi: " << m_eventInfo.lumiBlock << "\trun: " << m_eventInfo.runNumber << "\tin sample: " << sample << SLogger::endmsg;

    // --- Preliminary operations ---
    isMC = !m_isData;
    EventNumber = m_eventInfo.eventNumber;
    LumiNumber = m_eventInfo.lumiBlock;
    RunNumber = m_eventInfo.runNumber;
    nPV = m_eventInfo.PV_N;

    // Gen Weight
    if(isMC) {
        GenWeight = (m_eventInfo.genWeight < 0) ? -1 : 1;
        //PUWeight = getPUWeight();
        for(unsigned int v = 0; v < (m_eventInfo.actualIntPerXing)->size(); ++v) {
            if ( (*m_eventInfo.bunchCrossing)[v] == 0 ) {
                int npu( (*m_eventInfo.actualIntPerXing)[v] );
                PUWeight = m_pileupReweightingTool.getPileUpweight( npu );
                PUWeightUp = m_pileupReweightingTool.getPileUpweight( npu, +1 );
                PUWeightDown = m_pileupReweightingTool.getPileUpweight( npu, -1 );
                m_logger << VERBOSE << "Weight: " << PUWeight << " for true: " << (*m_eventInfo.actualIntPerXing)[v] << SLogger::endmsg;
                break;
            }
        }
        EventWeight *= GenWeight;
        EventWeight *= PUWeight;
    }
    // Check JSON
    else {
        if(!isGoodEvent(m_eventInfo.runNumber, m_eventInfo.lumiBlock)) {m_logger << INFO << " - Event not in JSON"  << SLogger::endmsg; throw SError( SError::SkipEvent ); }
    }
    if(!passMETFilters(!isMC)) {m_logger << INFO << " - Event fails MET filters"  << SLogger::endmsg; throw SError( SError::SkipEvent ); }

    std::map<std::string, bool> triggerMap;
    triggerMap = getTriggerMap();

    int nElectronsReco(0), nElectronsPt(0), nMuonsReco(0), nMuonsPt(0), nMuonsId(0);

    // --- Electrons ---
    std::vector<UZH::Electron> ElecVect;
    std::vector<TLorentzVector> CalibratedElecVect;
    for(int i = 0; i < m_electron.N; ++i) {
        UZH::Electron el(&m_electron, i);
        if(el.pt() < m_ElecPtCut || fabs(el.eta()) > m_ElecEtaCut || (fabs(el.eta()) > 1.4442 && fabs(el.eta())< 1.566) ) continue;
  nElectronsReco++;
        //if(i==0 && el.pt() < m_Elec1PtCut) break;
        //if(i==1 && el.pt() < m_Elec2PtCut) break;
        nElectronsPt++;
        if(i==0) for(int e=0; e<=(el.isVetoElectron()+el.isLooseElectron()+el.isMediumElectron()+el.isTightElectron()); e++) Hist("Electron1_Id", "2e")->Fill(e, EventWeight);
        if(i==1) for(int e=0; e<=(el.isVetoElectron()+el.isLooseElectron()+el.isMediumElectron()+el.isTightElectron()); e++) Hist("Electron2_Id", "2e")->Fill(e, EventWeight);
        if(i==0) Hist("Electron1_pfIso", "2e")->Fill(el.pfRhoCorrRelIso03(), EventWeight);
        if(i==1) Hist("Electron2_pfIso", "2e")->Fill(el.pfRhoCorrRelIso03(), EventWeight);
        //if(i==0 && !(el.isTightElectron())) break;
        //if(i==1 && !(el.isVetoElectron())) break;
 
        if(!(el.isVetoElectron())) continue;
        ST += el.pt();
        ElecVect.push_back(el);
        TLorentzVector el_tlv;
        el_tlv.SetPtEtaPhiE(el.pt(), el.eta(), el.phi(), el.e());
  
        //double energy = m_CorrectionTool.GetCorrectedEnergy(el.e(), isMC, m_eventInfo.runNumber, (fabs(el.eta()) < 1.4442), el.full5x5_r9(), el.superCluster_eta(), el.et());
        //m_logger << INFO << " - Electron " << i+1 << " has correction factor " << energy/el.e() << SLogger::endmsg;
        //el_tlv *= el.e()/energy;
        CalibratedElecVect.push_back(el_tlv);
    }
    nElectrons = ElecVect.size();

    // --- Muons ---
    std::vector<UZH::Muon> MuonVect;
    for(int i = 0; i < m_muon.N; ++i) {
        UZH::Muon mu(&m_muon, i);
        if(mu.pt() < m_MuonPtCut || fabs(mu.eta()) > m_MuonEtaCut) continue;
        nMuonsReco++;
        //if(i==0 && mu.pt() < m_Muon1PtCut) break;
        //if(i==1 && mu.pt() < m_Muon2PtCut) break;
        nMuonsPt++;
        if(i==0) for(int m=0; m<=(mu.isPFMuon()+mu.isLooseMuon()+mu.isMediumMuon()+mu.isTightMuon()); m++) Hist("Muon1_Id", "2m")->Fill(m, EventWeight);
        if(i==1) for(int m=0; m<=(mu.isPFMuon()+mu.isLooseMuon()+mu.isMediumMuon()+mu.isTightMuon()); m++) Hist("Muon2_Id", "2m")->Fill(m, EventWeight);
        //if(i==0 && !(mu.isTightMuon()==1)) break;
        //if(i==1 && !(mu.isLooseMuon()==1)) break;
        if(!(mu.isLooseMuon()==1)) continue;
        nMuonsId++;
        if(i==0) Hist("Muon1_pfIso", "2m")->Fill(mu.pfDeltaCorrRelIso(), EventWeight);
        if(i==1) Hist("Muon2_pfIso", "2m")->Fill(mu.pfDeltaCorrRelIso(), EventWeight);
        //if(i==0 && !(mu.pfDeltaCorrRelIso()<0.15)) break;
        //if(i==1 && !(mu.pfDeltaCorrRelIso()<0.25)) break;
        if(!(mu.pfDeltaCorrRelIso()<0.25)) continue;
        MuonVect.push_back(mu);
        ST += mu.pt();
    }
    nMuons = MuonVect.size();

    // --- Taus ---
    std::vector<UZH::Tau> TauVect;
    for(int i = 0; i < m_tau.N; ++i) {
        UZH::Tau tau(&m_tau, i);
        if( !(tau.pt() > m_TauPtCut && fabs(tau.eta()) < m_TauEtaCut && tau.byVTightIsolationMVArun2v1PWnewDMwLT()) ) continue;
        bool cleanTau(true);
        for(unsigned int e = 0; e < ElecVect.size(); e++) if(ElecVect[e].tlv().DeltaR(tau.tlv()) < 0.4) cleanTau = false;
        for(unsigned int m = 0; m < MuonVect.size(); m++) if(MuonVect[m].tlv().DeltaR(tau.tlv()) < 0.4) cleanTau = false;
        if(!cleanTau) continue;
        nTaus++;
    }

    // --- AK4 Jets ---
    std::vector<UZH::Jet> JetsVect;
    std::vector<UZH::Jet> JetsVectFor;
    std::vector<TLorentzVector> JetsVectRes;
    std::vector<TLorentzVector> JetsVectScaleUp;
    std::vector<TLorentzVector> JetsVectScaleDown;
    std::vector<TLorentzVector> JetsVectResUp;
    std::vector<TLorentzVector> JetsVectResDown;
    for(int i = 0; i < m_jetAK4.N; ++i) {
        UZH::Jet jet(&m_jetAK4, i);
        if(jet.pt() < m_AK4PtCut || fabs(jet.eta()) > m_AK4EtaCut || !jet.IDLoose()) continue;
        bool cleanJet(true);
        for(unsigned int e = 0; e < ElecVect.size(); e++) if(ElecVect[e].tlv().DeltaR(jet.tlv()) < 0.4) cleanJet = false;
        for(unsigned int m = 0; m < MuonVect.size(); m++) if(MuonVect[m].tlv().DeltaR(jet.tlv()) < 0.4) cleanJet = false;
        if(!cleanJet) continue;
        JetsVect.push_back(jet);
        // Systematics
//        TLorentzVector jetScaleUp(jet.tlv()), jetScaleDown(jet.tlv()), jetResUp(jet.tlv()), jetResDown(jet.tlv()), jetRes(jet.tlv());
//        if(m_isSignal) {
//            jetScaleUp *= jet.jecUp() / jet.jec();
//            jetScaleDown *= jet.jecDown() / jet.jec();
//            jetResUp = m_CorrectionTool.GetCorrectedJet(jet.tlv(), float(jet.jer_sigma_pt()), float(jet.jer_sf()), float(jet.jer_sf_up()), float(jet.jer_sf_down()), +1);
//            jetResDown = m_CorrectionTool.GetCorrectedJet(jet.tlv(), float(jet.jer_sigma_pt()), float(jet.jer_sf()), float(jet.jer_sf_up()), float(jet.jer_sf_down()), -1);
//            jetRes = m_CorrectionTool.GetCorrectedJet(jet.tlv(), float(jet.jer_sigma_pt()), float(jet.jer_sf()), float(jet.jer_sf_up()), float(jet.jer_sf_down()), 0);
//        }
//        JetsVectRes.push_back( jetRes );
//        JetsVectScaleUp.push_back( jetScaleUp );
//        JetsVectScaleDown.push_back( jetScaleDown );
//        JetsVectResUp.push_back( jetResUp );
//        JetsVectResDown.push_back( jetResDown );
        // Variables
        ST += jet.pt();
        HT += jet.pt();
        HTx += jet.tlv().Px();
        HTy += jet.tlv().Py();
        if(abs(jet.hadronFlavour())==5) nBJets++;
    }
    // Forward jets
    for(int i = 0; i < m_jetAK4.N; ++i) {
        UZH::Jet jet(&m_jetAK4, i);
        if(jet.pt() < m_AK4PtCut || fabs(jet.eta()) < m_AK4EtaCut || fabs(jet.eta()) > 4.0 || !jet.IDLoose()) continue;
        JetsVectFor.push_back(jet);
        nForwardJets++;
    }
    nJets = JetsVect.size();
    MHT = sqrt(HTx*HTx + HTy*HTy);
    MHTNoMu = MHT;

    // --- MET ---
    UZH::MissingEt MET( &m_missingEt, 0 );
    MET_pt = MET.et();
    MET_phi = MET.phi();
    MET_sign = MET_pt;
    TVector2 MET_tv2;
    MET_tv2.SetMagPhi(MET.et(), MET.phi());
    TLorentzVector MET_tlv;
    MET_tlv.SetPtEtaPhiE(MET.et(), 0., MET.phi(), MET.et());
    METNoMu = MuonVect.size() == 0 ? MET.et() : sqrt(pow(MET_tlv.Px() + MuonVect[0].tlv().Px(), 2) + pow(MET_tlv.Py() + MuonVect[0].tlv().Py(), 2));
    MinMETMHT = std::min(float(MET.et()), MHT);
    MinMETNoMuMHTNoMu = std::min(METNoMu, MHTNoMu);


    // min/max deltaphi (MET, jet)
    for(int i = 0; i < nJets; i++) {
      if(fabs(MET_tlv.DeltaPhi(JetsVect[i].tlv())) < MinJetMetDPhi) MinJetMetDPhi = fabs(MET_tlv.DeltaPhi(JetsVect[i].tlv()));
      if(fabs(MET_tlv.DeltaPhi(JetsVect[i].tlv())) > MaxJetMetDPhi) MaxJetMetDPhi = fabs(MET_tlv.DeltaPhi(JetsVect[i].tlv()));
    }
    for(int i = 0; i < std::min(2, nJets); i++) if(fabs(MET_tlv.DeltaPhi(JetsVect[i].tlv())) < MinJetMetDPhi12) MinJetMetDPhi12 = fabs(MET_tlv.DeltaPhi(JetsVect[i].tlv()));

    if(nJets == 0) MinJetMetDPhi = MinJetMetDPhi12 = MaxJetMetDPhi = -1.;

    // --- GEN ---
    TLorentzVector GenH;
    TLorentzVector GenV;
    TLorentzVector GenX;
    TLorentzVector LepP;
    TLorentzVector LepM;
    TLorentzVector BqaP;
    TLorentzVector BqaM;
    int GenVpdgId(0), GenLepPpdgId(0), GenLepMpdgId(0), HjetIndex(-1), VjetIndex(-1);
    float GenVPt(-1.), GenHT(0.), GenTopPt(-1.), GenAntiTopPt(-1.), TopPtWeight(1.), AntiTopPtWeight(1.);
    // Z and W electroweak corrections
    if(isMC && (m_isSignal || sample.find("DYJetsToLL") != std::string::npos || sample.find("ZJetsToNuNu") != std::string::npos || sample.find("WJetsToLNu") != std::string::npos)) {
        for(int i = 0; i < m_genParticle.N; ++i) {
            UZH::GenParticle gen(&m_genParticle, i);
            if(gen.status()==23) GenHT += gen.pt();
            if(GenH.Pt()<=0. && (gen.pdgId() == 36)) GenX = gen.tlv();
            if(GenH.Pt()<=0. && (gen.pdgId() == 25)) GenH = gen.tlv();
            if(GenV.Pt()<=0. && (gen.pdgId() == 23 || gen.pdgId() == 24) && gen.nDau()>0. && fabs(gen.dau()[0])!=gen.pdgId()) {GenV = gen.tlv(); GenVpdgId = gen.pdgId();}
            if(LepP.Pt()<=gen.pt() && ((gen.pdgId() >= +11 && gen.pdgId() <= +16 && gen.status()==1) || (gen.pdgId() == +15 && gen.status()==2)) ) {LepP = gen.tlv(); GenLepPpdgId = gen.pdgId();}
            if(LepM.Pt()<=gen.pt() && ((gen.pdgId() >= -16 && gen.pdgId() <= -11 && gen.status()==1) || (gen.pdgId() == -15 && gen.status()==2)) ) {LepM = gen.tlv(); GenLepMpdgId = gen.pdgId();}
            if(BqaP.Pt()<=gen.pt() && gen.pdgId() <= +5 && gen.status()==23) {BqaP = gen.tlv();}
            if(BqaM.Pt()<=gen.pt() && gen.pdgId() <= -5 && gen.status()==23) {BqaM = gen.tlv();}
            if(abs(gen.pdgId()) == 5 && gen.status()==23 && fabs(gen.eta()) < 2.4) nBQuarks++;
        }
        if(GenV.Pt()>0.) GenVPt = GenV.Pt();
        else if(LepP.Pt()>0. && LepM.Pt()>0.) {GenVPt = (LepP+LepM).Pt(); GenVpdgId = (GenLepPpdgId == -GenLepMpdgId) ? 23 : 24;}

        // if(GenVPt > 0. && (sample.find("DYJetsToLL") != std::string::npos || sample.find("ZJetsToNuNu") != std::string::npos)) ZewkWeight *= m_ScaleFactorTool.GetEwkZ(GenVPt);
        // if(GenVPt > 0. && (sample.find("WJetsToLNu") != std::string::npos))  WewkWeight *= m_ScaleFactorTool.GetEwkW(GenVPt);
        // EventWeight *= WewkWeight * ZewkWeight;

  if(GenVPt > 0. && (sample.find("DYJetsToLL") != std::string::npos || sample.find("ZJetsToNuNu") != std::string::npos)){
          ZewkWeight *= getEWKZ(GenVPt);
          ZqcdWeight *= getQCDZ(GenVPt);
        }
        if(GenVPt > 0. && (sample.find("WJetsToLNu") != std::string::npos)){
          WewkWeight *= getEWKW(GenVPt);
          WqcdWeight *= getQCDW(GenVPt);
        }
        EventWeight *= WewkWeight * WqcdWeight * ZewkWeight * ZqcdWeight;
    }

    // TTbar pT reweighting
    if(isMC && (sample.find("TT") != std::string::npos)) {
        //if(m_eventInfo.lheNj < 6) TopWeight *= 0.5;
        for(int i = 0; i < m_genParticle.N; ++i) {
            UZH::GenParticle gen(&m_genParticle, i);
            if(gen.pdgId() == +6) GenTopPt = gen.pt();
            if(gen.pdgId() == -6) GenAntiTopPt = gen.pt();
            //if(gen.status()==23 || gen.status()==33) std::cout << gen.pdgId() << "   " << gen.status() << std::endl;
        }
        if(GenTopPt>0.) TopPtWeight = exp(0.0615 - 0.0005*GenTopPt);
        if(GenAntiTopPt>0.) AntiTopPtWeight = exp(0.0615 - 0.0005*GenAntiTopPt);
        TopWeight *= sqrt(TopPtWeight * AntiTopPtWeight);
        //GenWeight *= TopWeight;
        //EventWeight *= TopWeight;
    }
    // QCD scales
    //QCDWeightUp = std::min(std::min(m_eventInfo.genFacWeightUp, m_eventInfo.genRenWeightUp), m_eventInfo.genFacRenWeightUp);
    //QCDWeightDown = std::max(std::max(m_eventInfo.genFacWeightDown, m_eventInfo.genRenWeightDown), m_eventInfo.genFacRenWeightDown);
    
    QCDRenWeightUp = std::max(0., std::min(2., double(m_eventInfo.genRenWeightUp)));
    QCDRenWeightDown = std::max(0., std::min(2., double(m_eventInfo.genRenWeightDown)));
    QCDFacWeightUp = std::max(0., std::min(2., double(m_eventInfo.genFacWeightUp)));
    QCDFacWeightDown = std::max(0., std::min(2., double(m_eventInfo.genFacWeightDown)));

    // Gen histograms
    Hist("LheV_pt", "Gen")->Fill(m_eventInfo.lheV_pt, GenWeight);
    Hist("GenV_pt", "Gen")->Fill(GenVPt, GenWeight);
    Hist("LheV_mass", "Gen")->Fill(m_eventInfo.lheV_mass, GenWeight);
    Hist("GenV_mass", "Gen")->Fill(GenV.M(), GenWeight);
    Hist("Lhe_HT", "Gen")->Fill(m_eventInfo.lheHT, GenWeight);
    Hist("Gen_HT", "Gen")->Fill(GenHT, GenWeight);
    Hist("LheNl", "Gen")->Fill(m_eventInfo.lheNl, GenWeight);
    Hist("LheNj", "Gen")->Fill(m_eventInfo.lheNj, GenWeight);
    Hist("GenX_mass", "Gen")->Fill(GenX.M(), GenWeight);

    LheV_pt = m_eventInfo.lheV_pt;
    LheHT = m_eventInfo.lheHT;
    LheNj = m_eventInfo.lheNj;
    LheNl = m_eventInfo.lheNl;

//    Hist("PDF_accept", "Sys")->Fill(0., EventWeight);
//    Hist("PDF_accept", "Sys")->Fill(+1., EventWeight*m_eventInfo.pdf_rms);
//    Hist("PDF_accept", "Sys")->Fill(-1., EventWeight*(2.-m_eventInfo.pdf_rms));

    // --- TRIGGER ---
    if(!isMC) {
        if(ElecVect.size() + MuonVect.size() > 0) {
            // MET Trigger with Single Electron
            if(triggerMap["SingleIsoEle"] && ElecVect.size() == 1 && ElecVect[0].pt() > 30. && ElecVect[0].isTightElectron()) {
                Hist("HLT_PFMET170_vs_SingleElectron_DEN", "Trigger")->Fill(MET.et(), 1.);
                Hist("HLT_PFMET120_PFMHT120_vs_SingleElectron_DEN", "Trigger")->Fill(MinMETMHT, 1.);
                Hist("HLT_PFMETNoMu120_PFMHTNoMu120_vs_SingleElectron_DEN", "Trigger")->Fill(MinMETNoMuMHTNoMu, 1.);
                Hist("HLT_PFMET_OR_vs_SingleElectron_DEN", "Trigger")->Fill(std::min(MinMETMHT, MinMETNoMuMHTNoMu), 1.);
                Hist("HLT_PFMET_OR_DEN", "Trigger")->Fill(std::min(MinMETMHT, MinMETNoMuMHTNoMu), 1.);
                if(triggerMap["MET"]) Hist("HLT_PFMET170_vs_SingleElectron_NUM", "Trigger")->Fill(MET.et(), 1.);
                if(triggerMap["METMHT"]) Hist("HLT_PFMET120_PFMHT120_vs_SingleElectron_NUM", "Trigger")->Fill(MinMETMHT, 1.);
                if(triggerMap["METMHTNoMu"]) Hist("HLT_PFMETNoMu120_PFMHTNoMu120_vs_SingleElectron_NUM", "Trigger")->Fill(MinMETNoMuMHTNoMu, 1.);
                if(triggerMap["MET"] || triggerMap["METMHT"] || triggerMap["METMHTNoMu"]) {
                    Hist("HLT_PFMET_OR_vs_SingleElectron_NUM", "Trigger")->Fill(std::min(MinMETMHT, MinMETNoMuMHTNoMu), 1.);
                    Hist("HLT_PFMET_OR_NUM", "Trigger")->Fill(std::min(MinMETMHT, MinMETNoMuMHTNoMu), 1.);
                }
            }
            // MET Trigger with Single Muon
            if(triggerMap["SingleIsoMu"] && MuonVect.size() == 1 && MuonVect[0].pt() > 30. && fabs(MuonVect[0].eta()) < 2.1 && MuonVect[0].isTightMuon()==1) {
                Hist("HLT_PFMET170_vs_SingleMuon_DEN", "Trigger")->Fill(MET.et(), 1.);
                Hist("HLT_PFMET120_PFMHT120_vs_SingleMuon_DEN", "Trigger")->Fill(MinMETMHT, 1.);
                Hist("HLT_PFMETNoMu120_PFMHTNoMu120_vs_SingleMuon_DEN", "Trigger")->Fill(MinMETNoMuMHTNoMu, 1.);
                Hist("HLT_PFMET_OR_vs_SingleMuon_DEN", "Trigger")->Fill(MinMETMHT, 1.);
                Hist("HLT_PFMET_OR_DEN", "Trigger")->Fill(std::min(MinMETMHT, MinMETNoMuMHTNoMu), 1.);
                if(triggerMap["MET"]) Hist("HLT_PFMET170_vs_SingleMuon_NUM", "Trigger")->Fill(MET.et(), 1.);
                if(triggerMap["METMHT"]) Hist("HLT_PFMET120_PFMHT120_vs_SingleMuon_NUM", "Trigger")->Fill(MinMETMHT, 1.);
                if(triggerMap["METMHTNoMu"]) Hist("HLT_PFMETNoMu120_PFMHTNoMu120_vs_SingleMuon_NUM", "Trigger")->Fill(MinMETNoMuMHTNoMu, 1.);
                if(triggerMap["MET"] || triggerMap["METMHT"] || triggerMap["METMHTNoMu"]) {
                    Hist("HLT_PFMET_OR_vs_SingleMuon_NUM", "Trigger")->Fill(MinMETMHT, 1.);
                    Hist("HLT_PFMET_OR_NUM", "Trigger")->Fill(std::min(MinMETMHT, MinMETNoMuMHTNoMu), 1.);
                }
            }
//            // MET Trigger with JetHT
//            if(triggerMap["JET"] && JetsVect.size() >= 1 && HT > 1200.) {
//                Hist("HLT_PFMET_OR_vs_JetHT_DEN", "Trigger")->Fill(MinMETMHT, 1.);
//                if(triggerMap["MET"] || triggerMap["METMHT"] || triggerMap["METMHTNoMu"]) {
//                    Hist("HLT_PFMET_OR_vs_JetHT_NUM", "Trigger")->Fill(MinMETMHT, 1.);
//                }
//            }
        }

    }

    Hist("Events")->Fill(0., GenWeight);
    Hist("Events", "0l")->Fill(0., EventWeight);
    Hist("Events", "2e")->Fill(0., EventWeight);
    Hist("Events", "2m")->Fill(0., EventWeight);

    // Filter by Trigger
    // if(!isMC) {
    //if(!triggerMap["SingleMu"] && !triggerMap["SingleIsoMu"] && !triggerMap["SingleEle"] && !triggerMap["SingleIsoEle"] && !triggerMap["MET"] && !triggerMap["METMHT"] && !triggerMap["METMHTNoMu"]) {m_logger << INFO << " - No trigger"  << SLogger::endmsg; throw SError( SError::SkipEvent ); }
    // }


    Hist("Events", "0l")->Fill(1., EventWeight);
    Hist("Events", "2e")->Fill(1., EventWeight);
    Hist("Events", "2m")->Fill(1., EventWeight);

    if(m_electron.N >= 2) Hist("Events", "2e")->Fill(2., EventWeight);
    if(nElectronsPt >= 2) Hist("Events", "2e")->Fill(3., EventWeight);
    if(ElecVect.size()>=2) Hist("Events", "2e")->Fill(4., EventWeight);

    if(m_muon.N >= 2) Hist("Events", "2m")->Fill(2., EventWeight);
    if(nMuonsPt >= 2) Hist("Events", "2m")->Fill(3., EventWeight);
    if(nMuonsId >= 2) Hist("Events", "2m")->Fill(4., EventWeight);
    if(MuonVect.size()>=2) Hist("Events", "2m")->Fill(5., EventWeight);

    // -----------------------------------
    //           VECTOR BOSON
    // -----------------------------------

    // Categorization depending on the number of leptons

    // ---------- Z TO LEPTONS ----------
    // Cascade: Zmm -> Zee -> Wmn -> Wen -> Znn -> Zqq
    // --- Z -> mumu ---
    if( MuonVect.size()>=2 || (MuonVect.size()>=2 && ElecVect.size()>=2 && MuonVect.at(0).pt() > ElecVect.at(0).pt()) ) {
        m_logger << INFO << " + Try to reconstruct Z -> mm"  << SLogger::endmsg;
        // Identify leptons
        if(MuonVect[0].charge() == MuonVect[1].charge()) {m_logger << INFO << " - No OS muons"  << SLogger::endmsg;}
        else {
            if((MuonVect[0].tlv() + MuonVect[1].tlv()).M() < 20.) {m_logger << INFO << " - No Z candidate"  << SLogger::endmsg;}
            else {
                // Check trigger consistency
                if(!triggerMap["SingleIsoMu"]) { m_logger << INFO << " - Trigger inconsistency" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
                // SF
                if(isMC) {
                    TriggerWeight *= m_ScaleFactorTool.GetTrigSingleIsoMuon(MuonVect[0].pt(), MuonVect[0].eta());
                    LeptonWeight *= m_ScaleFactorTool.GetMuonTightId(MuonVect[0].pt(), MuonVect[0].eta());
                    LeptonWeight *= m_ScaleFactorTool.GetMuonTightId(MuonVect[1].pt(), MuonVect[1].eta());
                    LeptonWeight *= m_ScaleFactorTool.GetMuonTightPFIso(MuonVect[0].pt(), MuonVect[0].eta());
                    LeptonWeight *= m_ScaleFactorTool.GetMuonTightPFIso(MuonVect[1].pt(), MuonVect[1].eta());
                    LeptonWeight *= pow(m_ScaleFactorTool.GetMuonTrk(nPV), 2);
                    TriggerWeightUp *= m_ScaleFactorTool.GetTrigSingleIsoMuon(MuonVect[0].pt(), MuonVect[0].eta(), +1);
                    LeptonWeightUp *= m_ScaleFactorTool.GetMuonTightId(MuonVect[0].pt(), MuonVect[0].eta(), +1);
                    LeptonWeightUp *= m_ScaleFactorTool.GetMuonTightId(MuonVect[1].pt(), MuonVect[1].eta(), +1);
                    LeptonWeightUp *= m_ScaleFactorTool.GetMuonTightPFIso(MuonVect[0].pt(), MuonVect[0].eta(), +1);
                    LeptonWeightUp *= m_ScaleFactorTool.GetMuonTightPFIso(MuonVect[1].pt(), MuonVect[1].eta(), +1);
                    LeptonWeightUp *= pow(m_ScaleFactorTool.GetMuonTrk(nPV, +1), 2);
                    TriggerWeightDown *= m_ScaleFactorTool.GetTrigSingleIsoMuon(MuonVect[0].pt(), MuonVect[0].eta(), -1);
                    LeptonWeightDown *= m_ScaleFactorTool.GetMuonTightId(MuonVect[0].pt(), MuonVect[0].eta(), -1);
                    LeptonWeightDown *= m_ScaleFactorTool.GetMuonTightId(MuonVect[1].pt(), MuonVect[1].eta(), -1);
                    LeptonWeightDown *= m_ScaleFactorTool.GetMuonTightPFIso(MuonVect[0].pt(), MuonVect[0].eta(), -1);
                    LeptonWeightDown *= m_ScaleFactorTool.GetMuonTightPFIso(MuonVect[1].pt(), MuonVect[1].eta(), -1);
                    LeptonWeightDown *= pow(m_ScaleFactorTool.GetMuonTrk(nPV, -1), 2);
                    EventWeight *= TriggerWeight * LeptonWeight;
                }
                // Build candidate
                Lepton1 = MuonVect[0].tlv();
                Lepton2 = MuonVect[1].tlv();
                Lepton1_pt = MuonVect[0].pt();
                Lepton2_pt = MuonVect[1].pt();
                Lepton1_eta = MuonVect[0].eta();
                Lepton2_eta = MuonVect[1].eta();
                Lepton1_phi = MuonVect[0].phi();
                Lepton2_phi = MuonVect[1].phi();
                Lepton1_pfIso = MuonVect[0].pfDeltaCorrRelIso();
                Lepton2_pfIso = MuonVect[1].pfDeltaCorrRelIso();
                Lepton1_id = MuonVect[0].isPFMuon()+MuonVect[0].isLooseMuon()+MuonVect[0].isMediumMuon()+MuonVect[0].isTightMuon();
                Lepton2_id = MuonVect[1].isPFMuon()+MuonVect[1].isLooseMuon()+MuonVect[1].isMediumMuon()+MuonVect[1].isTightMuon();
                V = Lepton1 + Lepton2;
                // nEvents
                Hist("Events", "2m")->Fill(6., EventWeight);
                Hist("Z_pt", "2m")->Fill(V.Pt(), EventWeight);
                Hist("Z_mass", "2m")->Fill(V.M(), EventWeight);
                Hist("Z_massRaw", "2m")->Fill((MuonVect[0].tlv() + MuonVect[1].tlv()).M(), EventWeight);
                if(fabs(MuonVect[0].eta()) < 1.2 && fabs(MuonVect[1].eta()) < 1.2) Hist("Z_massBB", "2m")->Fill(V.M(), EventWeight);
                if(fabs(MuonVect[0].eta()) < 1.2 && fabs(MuonVect[1].eta()) > 1.2) Hist("Z_massBE", "2m")->Fill(V.M(), EventWeight);
                if(fabs(MuonVect[0].eta()) > 1.2 && fabs(MuonVect[1].eta()) < 1.2) Hist("Z_massEB", "2m")->Fill(V.M(), EventWeight);
                if(fabs(MuonVect[0].eta()) > 1.2 && fabs(MuonVect[1].eta()) > 1.2) Hist("Z_massEE", "2m")->Fill(V.M(), EventWeight);
                if(V.Pt() < m_VPtCut) {m_logger << INFO << " - No V boost" << SLogger::endmsg;}
                else {
                    Hist("Events", "2m")->Fill(7., EventWeight);
                    isZtoMM = true;
                    mZ = V.M();
                    V_pt = V.Pt();
                    fakeMET_pt = sqrt(pow(MET_tlv.Px() + MuonVect[0].tlv().Px() + MuonVect[1].tlv().Px(), 2) + pow(MET_tlv.Py() + MuonVect[0].tlv().Py() + MuonVect[1].tlv().Py(), 2));
                    m_logger << INFO << " + Z -> mm candidate reconstructed" << SLogger::endmsg;
                    if(fakeMET_pt < m_MEtPtCut) {m_logger << INFO << " - Z -> mm candidate recoil too small"  << SLogger::endmsg; throw SError( SError::SkipEvent ); }
                }
            }
        }
    }
    // --- Z -> ee ---
    if(!isZtoMM && ElecVect.size()>=2) {
        m_logger << INFO << " + Try to reconstruct Z -> ee"  << SLogger::endmsg;
        // Identify leptons
        if(ElecVect[0].charge() == ElecVect[1].charge()) {m_logger << INFO << " - No OS electrons"  << SLogger::endmsg;}
        else {
            if((ElecVect[0].tlv() + ElecVect[1].tlv()).M() < 20.) {m_logger << INFO << " - No Z candidate"  << SLogger::endmsg;}
            else {
                // Check trigger consistency
                if(!triggerMap["SingleIsoEle"] && !triggerMap["SingleEle"]) { m_logger << INFO << " - Trigger inconsistency" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
                // SF
                if(isMC) {
                    TriggerWeight *= m_ScaleFactorTool.GetTrigSingleIsoEle(ElecVect[0].pt(), ElecVect[0].eta());
                    LeptonWeight *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[0].pt(), ElecVect[0].eta());
                    LeptonWeight *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[1].pt(), ElecVect[1].eta());
                    LeptonWeight *= m_ScaleFactorTool.GetEleReco(ElecVect[0].pt(), ElecVect[0].eta());
                    LeptonWeight *= m_ScaleFactorTool.GetEleReco(ElecVect[1].pt(), ElecVect[1].eta());
                    TriggerWeightUp *= m_ScaleFactorTool.GetTrigSingleIsoEle(ElecVect[0].pt(), ElecVect[0].eta(), +1);
                    LeptonWeightUp *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[0].pt(), ElecVect[0].eta(), +1);
                    LeptonWeightUp *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[1].pt(), ElecVect[1].eta(), +1);
                    LeptonWeightUp *= m_ScaleFactorTool.GetEleReco(ElecVect[0].pt(), ElecVect[0].eta(), +1);
                    LeptonWeightUp *= m_ScaleFactorTool.GetEleReco(ElecVect[1].pt(), ElecVect[1].eta(), +1);
                    TriggerWeightDown *= m_ScaleFactorTool.GetTrigSingleIsoEle(ElecVect[0].pt(), ElecVect[0].eta(), -1);
                    LeptonWeightDown *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[0].pt(), ElecVect[0].eta(), -1);
                    LeptonWeightDown *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[1].pt(), ElecVect[1].eta(), -1);
                    LeptonWeightDown *= m_ScaleFactorTool.GetEleReco(ElecVect[0].pt(), ElecVect[0].eta(), -1);
                    LeptonWeightDown *= m_ScaleFactorTool.GetEleReco(ElecVect[1].pt(), ElecVect[1].eta(), -1);
                    EventWeight *= TriggerWeight * LeptonWeight;
                }
                // Build candidate
                Lepton1 = CalibratedElecVect[0];
                Lepton2 = CalibratedElecVect[1];
                Lepton1_pt = CalibratedElecVect[0].Pt();
                Lepton2_pt = CalibratedElecVect[1].Pt();
                Lepton1_eta = CalibratedElecVect[0].Eta();
                Lepton2_eta = CalibratedElecVect[1].Eta();
                Lepton1_phi = CalibratedElecVect[0].Phi();
                Lepton2_phi = CalibratedElecVect[1].Phi();
                Lepton1_pfIso = ElecVect[0].pfRhoCorrRelIso03();
                Lepton2_pfIso = ElecVect[1].pfRhoCorrRelIso03();
                Lepton1_id = ElecVect[0].isVetoElectron()+ElecVect[0].isLooseElectron()+ElecVect[0].isMediumElectron()+ElecVect[0].isTightElectron();
                Lepton2_id = ElecVect[1].isVetoElectron()+ElecVect[1].isLooseElectron()+ElecVect[1].isMediumElectron()+ElecVect[1].isTightElectron();
                V = Lepton1 + Lepton2;
                // nEvents
                Hist("Events", "2e")->Fill(5., EventWeight);
                Hist("Z_pt", "2e")->Fill(V.Pt(), EventWeight);
                Hist("Z_mass", "2e")->Fill(V.M(), EventWeight);
                Hist("Z_massRaw", "2e")->Fill((ElecVect[0].tlv() + ElecVect[1].tlv()).M(), EventWeight);
                if(fabs(ElecVect[0].eta()) < 1.4442 && fabs(ElecVect[1].eta()) < 1.4442) Hist("Z_massBB", "2e")->Fill(V.M(), EventWeight);
                if(fabs(ElecVect[0].eta()) < 1.4442 && fabs(ElecVect[1].eta()) > 1.4442) Hist("Z_massBE", "2e")->Fill(V.M(), EventWeight);
                if(fabs(ElecVect[0].eta()) > 1.4442 && fabs(ElecVect[1].eta()) < 1.4442) Hist("Z_massEB", "2e")->Fill(V.M(), EventWeight);
                if(fabs(ElecVect[0].eta()) > 1.4442 && fabs(ElecVect[1].eta()) > 1.4442) Hist("Z_massEE", "2e")->Fill(V.M(), EventWeight);
                if(V.Pt() < m_VPtCut) {m_logger << INFO << " - No V boost" << SLogger::endmsg;}
                else {
                    Hist("Events", "2e")->Fill(6., EventWeight);
                    isZtoEE = true;
                    mZ = V.M();
                    V_pt = V.Pt();
                    fakeMET_pt = sqrt(pow(MET_tlv.Px() + ElecVect[0].tlv().Px() + ElecVect[1].tlv().Px(), 2) + pow(MET_tlv.Py() + ElecVect[0].tlv().Py() + ElecVect[1].tlv().Py(), 2));
                    m_logger << INFO << " + Z -> ee candidate reconstructed" << SLogger::endmsg;
                    if(fakeMET_pt < m_MEtPtCut) {m_logger << INFO << " - Z -> ee candidate recoil too small"  << SLogger::endmsg; throw SError( SError::SkipEvent ); }
                }
            }
        }
    }
    // ---------- INTERMEZZO: dileptonic Top CR ----------
    if(!isZtoMM && !isZtoEE && MuonVect.size()==1 && ElecVect.size()==1 && ElecVect[0].charge() != MuonVect[0].charge()) {
        // Check trigger consistency
        if(!triggerMap["SingleIsoMu"]) { m_logger << INFO << " - Trigger inconsistency" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
        // SF
        if(isMC) {
            //TriggerWeight *= m_ScaleFactorTool.GetTrigSingleIsoEle(ElecVect[0].pt(), ElecVect[0].eta());
            LeptonWeight *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[0].pt(), ElecVect[0].eta());
            LeptonWeight *= m_ScaleFactorTool.GetEleReco(ElecVect[0].pt(), ElecVect[0].eta());
            //TriggerWeightUp *= m_ScaleFactorTool.GetTrigSingleIsoEle(ElecVect[0].pt(), ElecVect[0].eta(), +1);
            LeptonWeightUp *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[0].pt(), ElecVect[0].eta(), +1);
            LeptonWeightUp *= m_ScaleFactorTool.GetEleReco(ElecVect[0].pt(), ElecVect[0].eta(), +1);
            //TriggerWeightDown *= m_ScaleFactorTool.GetTrigSingleIsoEle(ElecVect[0].pt(), ElecVect[0].eta(), -1);
            LeptonWeightDown *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[0].pt(), ElecVect[0].eta(), -1);
            LeptonWeightDown *= m_ScaleFactorTool.GetEleReco(ElecVect[0].pt(), ElecVect[0].eta(), -1);

            TriggerWeight *= m_ScaleFactorTool.GetTrigSingleIsoMuon(MuonVect[0].pt(), MuonVect[0].eta());
            LeptonWeight *= m_ScaleFactorTool.GetMuonTightId(MuonVect[0].pt(), MuonVect[0].eta());
            LeptonWeight *= m_ScaleFactorTool.GetMuonTightPFIso(MuonVect[0].pt(), MuonVect[0].eta());
            LeptonWeight *= pow(m_ScaleFactorTool.GetMuonTrk(nPV), 2);
            TriggerWeightUp *= m_ScaleFactorTool.GetTrigSingleIsoMuon(MuonVect[0].pt(), MuonVect[0].eta(), +1);
            LeptonWeightUp *= m_ScaleFactorTool.GetMuonTightId(MuonVect[0].pt(), MuonVect[0].eta(), +1);
            LeptonWeightUp *= m_ScaleFactorTool.GetMuonTightPFIso(MuonVect[0].pt(), MuonVect[0].eta(), +1);
            LeptonWeightUp *= pow(m_ScaleFactorTool.GetMuonTrk(nPV, +1), 2);
            TriggerWeightDown *= m_ScaleFactorTool.GetTrigSingleIsoMuon(MuonVect[0].pt(), MuonVect[0].eta(), -1);
            LeptonWeightDown *= m_ScaleFactorTool.GetMuonTightId(MuonVect[0].pt(), MuonVect[0].eta(), -1);
            LeptonWeightDown *= m_ScaleFactorTool.GetMuonTightPFIso(MuonVect[0].pt(), MuonVect[0].eta(), -1);
            LeptonWeightDown *= pow(m_ScaleFactorTool.GetMuonTrk(nPV, -1), 2);
            EventWeight *= TriggerWeight * LeptonWeight;
        }
        isTtoEM = true;
        Lepton1 = ElecVect[0].tlv();
        Lepton2 = MuonVect[0].tlv();
        Lepton1_pt = ElecVect[0].pt();
        Lepton2_pt = MuonVect[0].pt();
        Lepton1_eta = ElecVect[0].eta();
        Lepton2_eta = MuonVect[0].eta();
        Lepton1_phi = ElecVect[0].phi();
        Lepton2_phi = MuonVect[0].phi();
        Lepton1_pfIso = ElecVect[0].pfRhoCorrRelIso03();
        Lepton2_pfIso = MuonVect[0].pfDeltaCorrRelIso();
        Lepton1_id = ElecVect[0].isVetoElectron()+ElecVect[0].isLooseElectron()+ElecVect[0].isMediumElectron()+ElecVect[0].isTightElectron();
        Lepton2_id = MuonVect[0].isPFMuon()+MuonVect[0].isLooseMuon()+MuonVect[0].isMediumMuon()+MuonVect[0].isTightMuon();
        mZ = (Lepton1 + Lepton2).M();
        V_pt = (Lepton1 + Lepton2).Pt();
        fakeMET_pt = sqrt(pow(MET_tlv.Px() + ElecVect[0].tlv().Px() + MuonVect[0].tlv().Px(), 2) + pow(MET_tlv.Py() + ElecVect[0].tlv().Py() + MuonVect[0].tlv().Py(), 2));
        m_logger << INFO << " + TT -> mnen candidate reconstructed" << SLogger::endmsg;
        if(MET.et() < m_RecPtCut) {m_logger << INFO << " - TT -> mnen candidate met too small"  << SLogger::endmsg; throw SError( SError::SkipEvent ); }
    }
    // ---------- W TO LEPTON and NEUTRINO ----------
    // --- W -> munu ---
    if(!isZtoMM && !isZtoEE && !isTtoEM && (MuonVect.size()==1 || (MuonVect.size()==1 && ElecVect.size()==1 && MuonVect.at(0).pt() > ElecVect.at(0).pt()) )) {
        m_logger << INFO << " + Try to reconstruct W -> mn" << SLogger::endmsg;
        //Hist("Events", "Muon")->Fill(2., EventWeight);
        // Check trigger consistency
        if(!triggerMap["SingleIsoMu"]) { m_logger << INFO << " - Trigger inconsistency" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
        // SF
        if(isMC) {
            TriggerWeight *= m_ScaleFactorTool.GetTrigSingleIsoMuon(MuonVect[0].pt(), MuonVect[0].eta());
            LeptonWeight *= m_ScaleFactorTool.GetMuonTightId(MuonVect[0].pt(), MuonVect[0].eta());
            LeptonWeight *= m_ScaleFactorTool.GetMuonTightPFIso(MuonVect[0].pt(), MuonVect[0].eta());
            LeptonWeight *= pow(m_ScaleFactorTool.GetMuonTrk(nPV), 2);
            TriggerWeightUp *= m_ScaleFactorTool.GetTrigSingleIsoMuon(MuonVect[0].pt(), MuonVect[0].eta(), +1);
            LeptonWeightUp *= m_ScaleFactorTool.GetMuonTightId(MuonVect[0].pt(), MuonVect[0].eta(), +1);
            LeptonWeightUp *= m_ScaleFactorTool.GetMuonTightPFIso(MuonVect[0].pt(), MuonVect[0].eta(), +1);
            LeptonWeightUp *= pow(m_ScaleFactorTool.GetMuonTrk(nPV, +1), 2);
            TriggerWeightDown *= m_ScaleFactorTool.GetTrigSingleIsoMuon(MuonVect[0].pt(), MuonVect[0].eta(), -1);
            LeptonWeightDown *= m_ScaleFactorTool.GetMuonTightId(MuonVect[0].pt(), MuonVect[0].eta(), -1);
            LeptonWeightDown *= m_ScaleFactorTool.GetMuonTightPFIso(MuonVect[0].pt(), MuonVect[0].eta(), -1);
            LeptonWeightDown *= pow(m_ScaleFactorTool.GetMuonTrk(nPV, -1), 2);
            EventWeight *= TriggerWeight * LeptonWeight;
        }
        isWtoMN = true;
        Lepton1 = MuonVect[0].tlv();
        Lepton1_pt = MuonVect[0].pt();
        Lepton1_eta = MuonVect[0].eta();
        Lepton1_phi = MuonVect[0].phi();
        Lepton1_pfIso = MuonVect[0].pfDeltaCorrRelIso();
        Lepton1_id = MuonVect[0].isPFMuon()+MuonVect[0].isLooseMuon()+MuonVect[0].isMediumMuon()+MuonVect[0].isTightMuon();

        mT = sqrt(2.*MET.et()*MuonVect[0].pt()*(1.-cos(MuonVect[0].tlv().DeltaPhi(MET_tlv))));
  
        fakeMET_pt = sqrt(pow(MET_tlv.Px() + MuonVect[0].tlv().Px(), 2) + pow(MET_tlv.Py() + MuonVect[0].tlv().Py(), 2));
        V_pt = fakeMET_pt;
        Hist("W_tmass", "1m")->Fill(mT, EventWeight);
        m_logger << INFO << " + W -> mn candidate reconstructed" << SLogger::endmsg;
        if(MET.et() < m_RecPtCut) {m_logger << INFO << " - W -> mn candidate met too small"  << SLogger::endmsg; throw SError( SError::SkipEvent ); }
    }
    // --- W -> enu ---
    if(!isZtoMM && !isZtoEE && !isWtoMN && !isTtoEM && ElecVect.size()>=1) {
        m_logger << INFO << " + Try to reconstruct W -> en" << SLogger::endmsg;
        //Hist("Events", "enqq")->Fill(2., EventWeight);
        // Check trigger consistency
        if(!triggerMap["SingleIsoEle"] && !triggerMap["SingleEle"]) { m_logger << INFO << " - Trigger inconsistency" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
        // SF
        if(isMC) {
            TriggerWeight *= m_ScaleFactorTool.GetTrigSingleIsoEle(ElecVect[0].pt(), ElecVect[0].eta());
            LeptonWeight *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[0].pt(), ElecVect[0].eta());
            LeptonWeight *= m_ScaleFactorTool.GetEleReco(ElecVect[0].pt(), ElecVect[0].eta());
            TriggerWeightUp *= m_ScaleFactorTool.GetTrigSingleIsoEle(ElecVect[0].pt(), ElecVect[0].eta(), +1);
            LeptonWeightUp *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[0].pt(), ElecVect[0].eta(), +1);
            LeptonWeightUp *= m_ScaleFactorTool.GetEleReco(ElecVect[0].pt(), ElecVect[0].eta(), +1);
            TriggerWeightDown *= m_ScaleFactorTool.GetTrigSingleIsoEle(ElecVect[0].pt(), ElecVect[0].eta(), -1);
            LeptonWeightDown *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[0].pt(), ElecVect[0].eta(), -1);
            LeptonWeightDown *= m_ScaleFactorTool.GetEleReco(ElecVect[0].pt(), ElecVect[0].eta(), -1);
            EventWeight *= TriggerWeight * LeptonWeight;
        }
        isWtoEN = true;
        Lepton1 = ElecVect[0].tlv();
        Lepton1_pt = ElecVect[0].pt();
        Lepton1_eta = ElecVect[0].eta();
        Lepton1_phi = ElecVect[0].phi();
        Lepton1_pfIso = ElecVect[0].pfRhoCorrRelIso03();
        Lepton1_id = ElecVect[0].isVetoElectron()+ElecVect[0].isLooseElectron()+ElecVect[0].isMediumElectron()+ElecVect[0].isTightElectron();
        mT = sqrt(2.*MET.et()*ElecVect[0].pt()*(1.-cos(ElecVect[0].tlv().DeltaPhi(MET_tlv))));

        fakeMET_pt = sqrt(pow(MET_tlv.Px() + ElecVect[0].tlv().Px(), 2) + pow(MET_tlv.Py() + ElecVect[0].tlv().Py(), 2));
        V_pt = fakeMET_pt;
        Hist("W_tmass", "1e")->Fill(mT, EventWeight);
        m_logger << INFO << " + W -> en candidate reconstructed" << SLogger::endmsg;
        if(MET.et() < m_RecPtCut) {m_logger << INFO << " - W -> en candidate met too small"  << SLogger::endmsg; throw SError( SError::SkipEvent ); }
    }
    // ----------- Z TO NEUTRINOS ---------------
    if(!isZtoMM && !isZtoEE && !isTtoEM && !isWtoEN && ! isWtoMN) {
        m_logger << INFO << " + Try to reconstruct Z -> nn" << SLogger::endmsg;
        if(MET.et() < m_MEtPtCut) {m_logger << INFO << " - Low MET" << SLogger::endmsg;}
        else {
            // Check trigger consistency
            if(!isMC && !triggerMap["MET"] && !triggerMap["METMHT"] && !triggerMap["METMHTNoMu"]) { m_logger << INFO << " - Trigger inconsistency" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
            // Lepton veto
            Hist("Events", "0l")->Fill(2., EventWeight);
            if(!(nElectrons==0 && nMuons==0)) m_logger << INFO << " - Iso Leptons" << SLogger::endmsg;
            else {
                // Tau veto
                Hist("Events", "0l")->Fill(3., EventWeight);
                if(!(nTaus==0)) m_logger << INFO << " - Iso Taus" << SLogger::endmsg;
                else {
                    // Cleaning
                    Hist("Events", "0l")->Fill(4., EventWeight);
                    Hist("MinJetMetDPhi", "0l")->Fill(MinJetMetDPhi, EventWeight);
                    Hist("MaxJetMetDPhi", "0l")->Fill(MaxJetMetDPhi, EventWeight);
                    if(!(MinJetMetDPhi >= 0.4)) m_logger << INFO << " - ZtoNN event failed noise cleaning" << SLogger::endmsg;
                    else {
                        Hist("Events", "0l")->Fill(5., EventWeight);
                        if(!(nJets > 0 && JetsVect[0].IDTight() && JetsVect[0].chf() > 0.1)) m_logger << INFO << " - ZtoNN event jet noise cleaning" << SLogger::endmsg;
                        else {
                            Hist("Events", "0l")->Fill(6., EventWeight);
                            // Dummy V candidate
                            if(MET.et()!=MET.et()) { m_logger << WARNING << " - MET is nan" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
                            V.SetPtEtaPhiE(MET.et(), 0., MET.phi(), MET.et());
                            fakeMET_pt = MET.et();
                            if(isMC) {
                                TriggerWeight *= m_ScaleFactorTool.GetTrigMET(MET.et());
                            }
                            isZtoNN=true;
                            if(nJets >= 1) {
                                Hist("Jet1_chf", "0l")->Fill(JetsVect[0].chf(), EventWeight);
                                Hist("Jet1_nhf", "0l")->Fill(JetsVect[0].nhf(), EventWeight);
                                Hist("Jet1_phf", "0l")->Fill(JetsVect[0].phf(), EventWeight);
                                Hist("Jet1_muf", "0l")->Fill(JetsVect[0].muf(), EventWeight);
                                Hist("Jet1_emf", "0l")->Fill(JetsVect[0].emf(), EventWeight);
                                Hist("Jet1_chm", "0l")->Fill(JetsVect[0].chm(), EventWeight);
                                Hist("Jet1MetDPhi", "0l")->Fill(fabs(MET_tlv.DeltaPhi(JetsVect[0].tlv())), EventWeight);
                            }
                            if(nJets >= 2) {
                                Hist("Jet2_chf", "0l")->Fill(JetsVect[1].chf(), EventWeight);
                                Hist("Jet2_nhf", "0l")->Fill(JetsVect[1].nhf(), EventWeight);
                                Hist("Jet2_phf", "0l")->Fill(JetsVect[1].phf(), EventWeight);
                                Hist("Jet2_muf", "0l")->Fill(JetsVect[1].muf(), EventWeight);
                                Hist("Jet2_emf", "0l")->Fill(JetsVect[1].emf(), EventWeight);
                                Hist("Jet2_chm", "0l")->Fill(JetsVect[1].chm(), EventWeight);
                                Hist("Jet2MetDPhi", "0l")->Fill(fabs(MET_tlv.DeltaPhi(JetsVect[1].tlv())), EventWeight);
                            }
                            m_logger << INFO << " + Z -> nn candidate reconstructed" << SLogger::endmsg;
                        }
                    }
                }
            }
        }
    }

    // ----------- Nothing else matters ---------------
    if(!isZtoNN && !isWtoEN && !isWtoMN && !isTtoEM && !isZtoEE && !isZtoMM) { m_logger << INFO << " - No V candidate" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
    m_logger << INFO << " + Trigger consistency verified" << SLogger::endmsg;


    std::string category("");
    if(isZtoNN) category = "0l";
    else if(isWtoEN) category = "1e";
    else if(isWtoMN) category = "1m";
    else if(isZtoEE) category = "2e";
    else if(isZtoMM) category = "2m";
    else if(isTtoEM) category = "1e1m";

    Hist("EventWeight", category.c_str())->Fill(EventWeight);
    Hist("GenWeight", category.c_str())->Fill(GenWeight);
    Hist("TopWeight", category.c_str())->Fill(TopWeight);
    Hist("PUWeight", category.c_str())->Fill(PUWeight);
    Hist("TriggerWeight", category.c_str())->Fill(TriggerWeight);
    Hist("LeptonWeight", category.c_str())->Fill(LeptonWeight);
    Hist("nJets", category.c_str())->Fill(nJets, EventWeight);
    Hist("HT", category.c_str())->Fill(HT, EventWeight);

    // Jet multiplicity selection
    if(nJets/*+nForwardJets*/ < m_nJetsCut) { m_logger << INFO << " - Number of jets < " << m_nJetsCut << SLogger::endmsg; throw SError( SError::SkipEvent ); }
    if(isZtoNN && nJets < m_nJetsCut+1) { m_logger << INFO << " - Number of jets < " << m_nJetsCut+1 << SLogger::endmsg; throw SError( SError::SkipEvent ); }

    std::vector<UZH::Jet> JetsVectSorted(JetsVect.begin(), JetsVect.end());
    std::sort(JetsVectSorted.begin(), JetsVectSorted.end(), SortByCSV);
    //std::vector<UZH::Jet> JetsVectSortedLoose(JetsVectSorted.begin()+1, JetsVectSorted.end()); // Only for Loose SF

    m_logger << INFO << " + Filling jets" << SLogger::endmsg;

    if(nJets >= 1) {
      //Jet1.SetPtEtaPhiE(JetsVect[0].pt(), JetsVect[0].eta(), JetsVect[0].phi(), JetsVect[0].e());
      Jet1_pt = JetsVect[0].pt(); 
      Jet1_E = JetsVect[0].e();
      Jet1_eta = JetsVect[0].eta();        
      Jet1_phi = JetsVect[0].phi();
      Jet1_csv = JetsVect[0].csv();
    }
    if(nJets >= 2) {
      //Jet2.SetPtEtaPhiE(JetsVect[1].pt(), JetsVect[1].eta(), JetsVect[1].phi(), JetsVect[1].e());
      Jet2_pt = JetsVect[1].pt();
      Jet2_E = JetsVect[1].e();
      Jet2_eta = JetsVect[1].eta();
      Jet2_phi = JetsVect[1].phi();
      Jet2_csv = JetsVect[1].csv();
    }
    if(nJets >= 3) {
      //Jet3.SetPtEtaPhiE(JetsVect[2].pt(), JetsVect[2].eta(), JetsVect[2].phi(), JetsVect[2].e());
      Jet3_pt = JetsVect[2].pt();
      Jet3_E = JetsVect[2].e();
      Jet3_eta = JetsVect[2].eta();
      Jet3_phi = JetsVect[2].phi();
      Jet3_csv = JetsVect[2].csv();
    }
    if(nJets >= 4) {
      //Jet4.SetPtEtaPhiE(JetsVect[3].pt(), JetsVect[3].eta(), JetsVect[3].phi(), JetsVect[3].e());
      Jet4_pt = JetsVect[3].pt();
      Jet4_E = JetsVect[3].e();
      Jet4_eta = JetsVect[3].eta();
      Jet4_phi = JetsVect[3].phi();
      Jet4_csv = JetsVect[3].csv();
    }
    if(nJets >= 5) {
      Jet5_pt = JetsVect[4].pt();
      Jet5_E = JetsVect[4].e();
      Jet5_eta = JetsVect[4].eta();
      Jet5_phi = JetsVect[4].phi();
      Jet5_csv = JetsVect[4].csv();
    }
    if(nJets >= 6) {
      Jet6_pt = JetsVect[5].pt();
      Jet6_E = JetsVect[5].e();
      Jet6_eta = JetsVect[5].eta();
      Jet6_phi = JetsVect[5].phi();
      Jet6_csv = JetsVect[5].csv();
    }
    if(nForwardJets >= 1) {
      //JetF.SetPtEtaPhiE(JetsVectFor[0].pt(), JetsVectFor[0].eta(), JetsVectFor[0].phi(), JetsVectFor[0].e());
      JetF_pt = JetsVectFor[0].pt();
      JetF_E = JetsVectFor[0].e();
      JetF_eta = JetsVectFor[0].eta();
      JetF_phi = JetsVectFor[0].phi();
      //JetF_csv = JetsVectFor[0].csv();
    }

    // For MT2W
    std::vector<TLorentzVector> bJets, lJets;
    for(unsigned int i=0; i<JetsVect.size(); i++) {
      if(m_bTaggingScaleTool.isTagged(JetsVect[i].csv())){
          bJets.push_back(JetsVect[i].tlv());
          nBTagJets++;
      }
      else lJets.push_back(JetsVect[i].tlv());
    }

    m_logger << INFO << " + Calculating MT2W" << SLogger::endmsg;
    if(isWtoEN || isWtoMN) {
      mT2 = m_VariableTool.ReturnMT2W(lJets, bJets, Lepton1, MET_tv2);
    }
    m_logger << INFO << " + MT2W = " << mT2 << SLogger::endmsg;
    
    // Top kinematic reconstruction
    if(isZtoNN) { // Only in all-hadonic for now
        int ij1, ij2;
        if(JetsVectSorted[1].pt() > JetsVectSorted[2].pt()) {ij1 = 1; ij2 = 2;}
        else {ij1 = 2; ij2 = 1;}
        mW = (JetsVectSorted[ij1].tlv() + JetsVectSorted[ij2].tlv()).M();
        mTop = (JetsVectSorted[0].tlv() + JetsVectSorted[ij1].tlv() + JetsVectSorted[ij2].tlv()).M();
        
        TLorentzVector kJet1, kJet2;
        float jec1(mW > 80.18 ? JetsVectSorted[ij1].jecDown() : JetsVectSorted[ij1].jecUp()), jec2(mW > 80.18 ? JetsVectSorted[ij2].jecDown() : JetsVectSorted[ij2].jecUp());
        float kF(80.18/mW), w1(2.*jec1/(jec1+jec2)), w2(2.*jec2/(jec1+jec2));
        kJet1 = JetsVectSorted[ij1].tlv() * kF * w1;
        kJet2 = JetsVectSorted[ij2].tlv() * kF * w2;
        mW = (kJet1 + kJet2).M();
        kTop = (kJet1 + kJet2 + JetsVectSorted[0].tlv()).M();
    }
    
    // b-tagging Scale Factors
    m_logger << INFO << " + Calculating b-tagging scale factors" << SLogger::endmsg;
    if(isMC) {
        float BTagAK4Weight(1.), BTagAK4WeightUp(1.), BTagAK4WeightDown(1.), BTagAK4WeightBUp(1.), BTagAK4WeightBDown(1.), BTagAK4WeightLUp(1.), BTagAK4WeightLDown(1.), BTagAK4WeightB1(1.), BTagAK4WeightB2(1.), BTagAK4WeightL1(1.), BTagAK4WeightL2(1.);
        BTagAK4Weight *= m_bTaggingScaleTool.getScaleFactor(JetsVect);
        BTagAK4WeightBUp *= m_bTaggingScaleTool.getScaleFactor(JetsVect, +1, 0);
        BTagAK4WeightBDown *= m_bTaggingScaleTool.getScaleFactor(JetsVect, -1, 0);
        BTagAK4WeightLUp *= m_bTaggingScaleTool.getScaleFactor(JetsVect, 0, +1);
        BTagAK4WeightLDown *= m_bTaggingScaleTool.getScaleFactor(JetsVect, 0, -1);
        // Tight
//        BTagAK4Weight *= m_bTaggingScaleTool.getScaleFactor_tag(JetsVectSorted[0]);
//        BTagAK4WeightBUp *= m_bTaggingScaleTool.getScaleFactor_tag(JetsVectSorted[0], +1, 0);
//        BTagAK4WeightBDown *= m_bTaggingScaleTool.getScaleFactor_tag(JetsVectSorted[0], -1, 0);
//        BTagAK4WeightLUp *= m_bTaggingScaleTool.getScaleFactor_tag(JetsVectSorted[0], 0, +1);
//        BTagAK4WeightLDown *= m_bTaggingScaleTool.getScaleFactor_tag(JetsVectSorted[0], 0, -1);
        // Loose
//        BTagAK4Weight *= m_bTaggingScaleTool.getScaleFactor_veto(JetsVectSortedLoose);
//        BTagAK4WeightBUp *= m_bTaggingScaleTool.getScaleFactor_veto(JetsVectSortedLoose, +1, 0);
//        BTagAK4WeightBDown *= m_bTaggingScaleTool.getScaleFactor_veto(JetsVectSortedLoose, -1, 0);
//        BTagAK4WeightLUp *= m_bTaggingScaleTool.getScaleFactor_veto(JetsVectSortedLoose, 0, +1);
//        BTagAK4WeightLDown *= m_bTaggingScaleTool.getScaleFactor_veto(JetsVectSortedLoose, 0, -1);
        // Merge B and L
        BTagAK4WeightUp = BTagAK4Weight + sqrt( pow(BTagAK4WeightBUp-BTagAK4Weight, 2) + pow(BTagAK4WeightLUp-BTagAK4Weight, 2) );
        BTagAK4WeightDown = BTagAK4Weight - sqrt( pow(BTagAK4Weight-BTagAK4WeightBDown, 2) + pow(BTagAK4Weight-BTagAK4WeightLDown, 2) );
        m_logger << INFO << " + BTag weights: " << BTagAK4Weight << ", B Up: "<<BTagAK4WeightBUp << ", B Down: " << BTagAK4WeightBDown << ", L Up"  << BTagAK4WeightLUp << ", L Down" << BTagAK4WeightLDown << " | Up: " << BTagAK4WeightUp << ", Down: " << BTagAK4WeightDown << SLogger::endmsg;
        m_logger << INFO << " + BTag weights: tag " << BTagAK4Weight << ", veto: " << BTagAK4Weight << SLogger::endmsg;
        BTagWeight = BTagAK4Weight;
        BTagWeightUp = BTagAK4WeightBUp;
        BTagWeightDown = BTagAK4WeightBDown;
        //EventWeight *= BTagWeight; // not yet

        // Fill histograms
        if(isMC) {
            m_bTaggingScaleTool.fillEfficiencies(JetsVect);
            m_bTaggingScaleTool.fillEfficiencies_veto(JetsVect);
            m_bTaggingScaleTool.fillEfficiencies_tag(JetsVect);
        }
    }


    // Min/Max DeltaPhi for lepton
    if(isWtoEN || isZtoEE){
        for(int i = 0; i < nElectrons; ++i){
            if(fabs(MET_tlv.DeltaPhi(ElecVect[i].tlv())) < MinLepMetDPhi) MinLepMetDPhi = fabs(MET_tlv.DeltaPhi(ElecVect[i].tlv()));
            if(fabs(MET_tlv.DeltaPhi(ElecVect[i].tlv())) > MaxLepMetDPhi) MaxLepMetDPhi = fabs(MET_tlv.DeltaPhi(ElecVect[i].tlv()));
        }
    }
    if(isWtoMN || isZtoMM){
        for(int i = 0; i < nMuons; ++i){
            if(fabs(MET_tlv.DeltaPhi(MuonVect[i].tlv())) < MinLepMetDPhi) MinLepMetDPhi = fabs(MET_tlv.DeltaPhi(MuonVect[i].tlv()));
            if(fabs(MET_tlv.DeltaPhi(MuonVect[i].tlv())) > MaxLepMetDPhi) MaxLepMetDPhi = fabs(MET_tlv.DeltaPhi(MuonVect[i].tlv()));
        }
    }
    if(isTtoEM){
        if(fabs(MET_tlv.DeltaPhi(MuonVect[0].tlv())) > MaxLepMetDPhi) MaxLepMetDPhi = fabs(MET_tlv.DeltaPhi(MuonVect[0].tlv()));
        if(fabs(MET_tlv.DeltaPhi(ElecVect[0].tlv())) > MaxLepMetDPhi) MaxLepMetDPhi = fabs(MET_tlv.DeltaPhi(ElecVect[0].tlv()));
    }

    // min/max deltaphi (lepton, jet)
    for(int i = 0; i < nJets; i++){
        if(isWtoEN || isZtoEE){
            for(int j = 0; j < nElectrons; j++){
                if(fabs((ElecVect[j].tlv()).DeltaPhi(JetsVect[i].tlv())) < MinLepJetDPhi) MinLepJetDPhi = fabs((ElecVect[j].tlv()).DeltaPhi(JetsVect[i].tlv()));
                if(fabs((ElecVect[j].tlv()).DeltaPhi(JetsVect[i].tlv())) > MaxLepJetDPhi) MaxLepJetDPhi = fabs((ElecVect[j].tlv()).DeltaPhi(JetsVect[i].tlv()));
            }
        }
        if(isWtoMN || isZtoMM){
            for(int j = 0; j < nMuons; j++){
                if(fabs((MuonVect[j].tlv()).DeltaPhi(JetsVect[i].tlv())) < MinLepJetDPhi) MinLepJetDPhi = fabs((MuonVect[j].tlv()).DeltaPhi(JetsVect[i].tlv()));
                if(fabs((MuonVect[j].tlv()).DeltaPhi(JetsVect[i].tlv())) > MaxLepJetDPhi) MaxLepJetDPhi = fabs((MuonVect[j].tlv()).DeltaPhi(JetsVect[i].tlv()));
            }
        }
        if(isTtoEM){
            if(fabs((MuonVect[0].tlv()).DeltaPhi(JetsVect[i].tlv())) < MinLepJetDPhi) MinLepJetDPhi = fabs((MuonVect[0].tlv()).DeltaPhi(JetsVect[i].tlv()));
            if(fabs((ElecVect[0].tlv()).DeltaPhi(JetsVect[i].tlv())) > MaxLepJetDPhi) MaxLepJetDPhi = fabs((ElecVect[0].tlv()).DeltaPhi(JetsVect[i].tlv()));
        }
    }

    // min/max deltaphi (b-jets, MET)
    if (nBTagJets > 0){
        for(int i = 0; i < nBTagJets ;++i){
            if(fabs(MET_tlv.DeltaPhi(bJets[i])) < MinBJetMetDPhi) MinBJetMetDPhi = fabs(MET_tlv.DeltaPhi(bJets[i]));
            if(fabs(MET_tlv.DeltaPhi(bJets[i])) > MaxBJetMetDPhi) MaxBJetMetDPhi = fabs(MET_tlv.DeltaPhi(bJets[i]));
        }
    }

    // // add event shape variable
    // std::vector<TLorentzVector> *Jets = new std::vector<TLorentzVector>;

    // for(int i = 0; i < nJets; i++) Jets->push_back(JetsVect[i].tlv());
    // for(int i = 0; i < nElectrons; i++) Jets->push_back(ElecVect[i].tlv());
    // for(int i = 0; i < nMuons; i++) Jets->push_back(MuonVect[i].tlv());
    //for(int i = 0; i < nBTagJets ; i++) Jets->push_back(bJets[i]);
    //Jets->push_back(MET_tlv);

    //m_VariableTool.EventShape(Jets, Sphericity, Aplanarity);


    // angular correlations

    m_logger << INFO << " + Tree filled" << SLogger::endmsg;

    return;
}







bool DMAnalysis::isGoodEvent(int runNumber, int lumiSection) {

  bool isGood = true;
  if (m_isData) {
    isGood = m_grl.HasRunLumiBlock( runNumber, lumiSection );
    if( !isGood ) {
      m_logger << WARNING << "Bad event! Run: " << runNumber <<  " - Lumi Section: " << lumiSection << SLogger::endmsg;
      // throw SError( SError::SkipEvent );
    }
    else m_logger << VERBOSE << "Good event! Run: " << runNumber <<  " - Lumi Section: " << lumiSection << SLogger::endmsg;
  }

  return isGood;
}


std::map<std::string, bool> DMAnalysis::getTriggerMap() {
    std::map<std::string, bool> map;
    for(std::map<std::string, std::vector<std::string> >::iterator it = m_triggerNames.begin(); it != m_triggerNames.end(); ++it) {
        bool pass(false);
        for(unsigned int i = 0; i < it->second.size(); i++) {
            // Iterate trigger map
            //if(!pass && (*m_eventInfo.trigDecision)[(it->second).at(i)]) pass = true;
            for(std::map<std::string,bool>::iterator tt = (m_eventInfo.trigDecision)->begin(); tt != (m_eventInfo.trigDecision)->end(); ++tt) {
                if((tt->first).find((it->second).at(i)) != std::string::npos && tt->second == true) {
                    m_logger << INFO << "Trigger pass: " << (tt->first) << SLogger::endmsg;
                    pass = true;
                    break;
                }
            }
            if(pass) break;
        }
        map[it->first] = pass;
    }
    return map;
}

bool DMAnalysis::passTrigger(std::string cat) {
  bool passTrigger = false;
  for(std::map<std::string,bool>::iterator it = (m_eventInfo.trigDecision)->begin(); it != (m_eventInfo.trigDecision)->end(); ++it) {
      for (unsigned int t = 0; t < m_triggerNames[cat].size(); ++t ){
          if ((it->first).find(m_triggerNames[cat][t]) != std::string::npos) {
              if (it->second == true) {
                  m_logger << INFO << "Trigger pass: " << (it->first) << SLogger::endmsg;
                  passTrigger = true;
                  return passTrigger;
              }
          }
      }
  }
  return passTrigger;
}


bool DMAnalysis::passMETFilters(bool data) {

    // using only what's recommended in https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2
  //Updating to Morion17 recommendations

  //print statements for debugging, will remove later again
  /*  cout<< "m_eventInfo.PV_filter "<<m_eventInfo.PV_filter<<endl;
  cout<<     "m_eventInfo.passFilter_CSCHalo "<<m_eventInfo.passFilter_CSCHalo<<endl;
  cout<<     "m_eventInfo.passFilter_HBHE "<<m_eventInfo.passFilter_HBHE<<endl;
  cout<<     "m_eventInfo.passFilter_HBHEIso "<<m_eventInfo.passFilter_HBHEIso<<endl;
  cout<<     "m_eventInfo.passFilter_ECALDeadCell "<<m_eventInfo.passFilter_ECALDeadCell<<endl;
  cout<<     "m_eventInfo.passFilter_EEBadSc "<<m_eventInfo.passFilter_EEBadSc<<endl;
  cout<<     "m_eventInfo.passFilter_globalTightHalo2016 "<<m_eventInfo.passFilter_globalTightHalo2016<<endl;
  cout<<     "m_eventInfo.passFilter_muonBadTrack "<<m_eventInfo.passFilter_muonBadTrack<<endl;
  cout<<     "m_eventInfo.passFilter_chargedHadronTrackResolution "<< m_eventInfo.passFilter_chargedHadronTrackResolution<<endl;
  cout<<     "m_eventInfo.flag_badMuons "<<m_eventInfo.flag_badMuons<<endl;
  cout<<     "m_eventInfo.flag_duplicateMuons "<<m_eventInfo.flag_duplicateMuons<<endl;
  cout<<     "m_eventInfo.flag_nobadMuons "<<m_eventInfo.flag_nobadMuons<<endl;*/
    if( !(m_eventInfo.PV_filter) ) return false;
    Hist( "METFilters", "0l" )->Fill(1);

    //CSC filter is deprecated
    //    if( !(m_eventInfo.passFilter_CSCHalo) ) return false;
    //Hist( "METFilters", "0l" )->Fill(2);

    if( !(m_eventInfo.passFilter_HBHE) ) return false;
    Hist( "METFilters", "0l" )->Fill(3);
    if( !(m_eventInfo.passFilter_HBHEIso) ) return false;
    Hist( "METFilters", "0l" )->Fill(4);
    if( !(m_eventInfo.passFilter_ECALDeadCell) ) return false;
    Hist( "METFilters", "0l" )->Fill(5);

    if(data) if( !(m_eventInfo.passFilter_EEBadSc) ) return false;
    Hist( "METFilters", "0l" )->Fill(6);

    if( !(m_eventInfo.passFilter_globalTightHalo2016) ) return false;
    Hist( "METFilters", "0l" )->Fill(7);

    //These need updating after ICHEP, will leave them for now
    if( !(m_eventInfo.passFilter_muonBadTrack) ) return false;
    Hist( "METFilters", "0l" )->Fill(8);
    if( !(m_eventInfo.passFilter_chargedHadronTrackResolution) ) return false;
    Hist( "METFilters", "0l" )->Fill(9);
    //  if( !(m_eventInfo.passFilter_METFilters) ) return false;
    //   Hist( "METFilters" )->Fill(10);
    if(data) {
      //Fixed now, they are 0 when event is good and 1 when the MET in the event was corrected
        if( (m_eventInfo.flag_badMuons) ) return false;
        Hist( "METFilters", "0l" )->Fill(10);
        if( (m_eventInfo.flag_duplicateMuons) ) return false;
        Hist( "METFilters", "0l" )->Fill(11);

  //still don't know what this is
  // if( (m_eventInfo.flag_nobadMuons) ) return false;
        //Hist( "METFilters", "0l" )->Fill(12);
    }

    return true;

}



void DMAnalysis::clearBranches() {
    EventWeight = GenWeight = ZewkWeight = WewkWeight = TopWeight = QCDRenWeightUp = QCDRenWeightDown = QCDFacWeightUp = QCDFacWeightDown = PUWeight = PUWeightUp = PUWeightDown = TriggerWeight = TriggerWeightUp = TriggerWeightDown = LeptonWeight = LeptonWeightUp = LeptonWeightDown = BTagWeight = BTagWeightUp = BTagWeightDown = 1.;
    isZtoNN = isWtoEN = isWtoMN = isTtoEM = isZtoEE = isZtoMM = isTveto = false;
    LheV_pt = LheHT = LheNl = LheNj = LheNb = 0;
    ZqcdWeight = WqcdWeight = 1.;
    nPV = nElectrons = nMuons = nTaus = nPhotons = nJets = nForwardJets = nBJets = nBQuarks = nBTagJets = nBVetoJets = 0;
    HT = HTx = HTy = MHT = MHTNoMu = METNoMu = MinMETMHT = MinMETNoMuMHTNoMu = ST = MET_pt = MET_phi = MET_sign = fakeMET_pt = 0.;
    V_pt = mZ = mT = mT2 = mW = mTop = kTop = 0.;
    Sphericity, Aplanarity = -10;
    MinLepJetDPhi = MinLepMetDPhi = MinJetMetDPhi = MinJetMetDPhi12 = MinBJetMetDPhi = 10.;
    MaxLepMetDPhi = MaxLepJetDPhi = MaxJetMetDPhi = MaxBJetMetDPhi = -1.;

    Lepton1 = Lepton2 = Jet1 = Jet2 = Jet3 = Jet4 = V = TLorentzVector();
    Lepton1_pt = Lepton2_pt = Lepton1_eta = Lepton2_eta = Lepton1_phi = Lepton2_phi = Lepton1_pfIso = Lepton2_pfIso = Lepton1_id = Lepton2_id = Jet1_pt = Jet2_pt = Jet3_pt = Jet4_pt = Jet5_pt = Jet6_pt =JetF_pt = Jet1_E = Jet2_E = Jet3_E = Jet4_E = Jet5_E = Jet6_E =JetF_E = Jet1_eta = Jet2_eta = Jet3_eta = Jet4_eta = Jet5_eta = Jet6_eta =JetF_eta = Jet1_phi = Jet2_phi = Jet3_phi = Jet4_phi = Jet5_phi = Jet6_phi =JetF_phi = Jet1_csv = Jet2_csv = Jet3_csv = Jet4_csv = Jet5_csv = Jet6_csv = -9.;
}
