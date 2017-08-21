// $Id: CycleCreators.py 344 2016-04-03 19:52:15Z zucchett $

// Local include(s):
#include "../include/AZhAnalysis.h"

// External include(s):
#include "../GoodRunsLists/include/TGoodRunsListReader.h"

#include <TMath.h>

bool SortByCSV(UZH::Jet j1, UZH::Jet j2) {return(j1.csv() > j2.csv());}

ClassImp( AZhAnalysis );


AZhAnalysis::AZhAnalysis() : SCycleBase(),
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
    DeclareProperty( "OutputTreeName",            m_outputTreeName = "AZh");

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
    DeclareProperty( "ElecEtaCut",                m_ElecEtaCut               =  2.5 );
    DeclareProperty( "Muon1PtCut",                m_Muon1PtCut               =  30. );
    DeclareProperty( "Muon2PtCut",                m_Muon2PtCut               =  10. );
    DeclareProperty( "MuonPtCut",                 m_MuonPtCut                =  10. );
    DeclareProperty( "MuonEtaCut",                m_MuonEtaCut               =  2.4 );
    DeclareProperty( "TauPtCut",                  m_TauPtCut                 =  18. );
    DeclareProperty( "TauEtaCut",                 m_TauEtaCut                =  2.3 );
    DeclareProperty( "AK4PtCut",                  m_AK4PtCut                 =  30. );
    DeclareProperty( "AK4EtaCut",                 m_AK4EtaCut                =  2.4 );
    DeclareProperty( "MEtPtCut",                  m_MEtPtCut                 =  250. );
    DeclareProperty( "VPtCut",                    m_VPtCut                   = -1. );
    DeclareProperty( "VMassLowerCut",             m_VMassLowerCut            =  70. );
    DeclareProperty( "VMassUpperCut",             m_VMassUpperCut            = 105. );
    DeclareProperty( "HMassLowerCut",             m_HMassLowerCut            = 95. );
    DeclareProperty( "HMassUpperCut",             m_HMassUpperCut            = 145. );
    DeclareProperty( "XMassLowerCut",             m_XMassLowerCut            = -1. );
    DeclareProperty( "XTMassLowerCut",            m_XTMassLowerCut           = -1. );
    
    // External objects name
    DeclareProperty( "JSONFileName",              m_JSONFileName             = "../GoodRunsLists/JSON/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt" ); //Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt

    // Prepend SFrame dir
    m_JSONFileName = std::string (std::getenv("SFRAME_DIR")) + m_JSONFileName;
    
}

// Destructor
AZhAnalysis::~AZhAnalysis() {

  m_logger << INFO << "Tschoe!" << SLogger::endmsg;

}


// Begin Job
void AZhAnalysis::BeginCycle() throw( SError ) {

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
//    m_triggerNames[""] = std::vector<std::string> ();
    //m_triggerNames["SingleMu"].push_back("HLT_Mu45_eta2p1_v");
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
void AZhAnalysis::EndCycle() throw( SError ) {
    std::cout << "Events analyzed:\t" << nEvents <<std::endl;
    return;
}

void AZhAnalysis::BeginInputData( const SInputData& id ) throw( SError ) {

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
    m_logger << INFO << "ElecEtaCut:\t" <<            m_ElecEtaCut           << SLogger::endmsg;
    m_logger << INFO << "Muon1PtCut:\t" <<            m_Muon1PtCut           << SLogger::endmsg;
    m_logger << INFO << "Muon2PtCut:\t" <<            m_Muon2PtCut           << SLogger::endmsg;
    m_logger << INFO << "MuonPtCut:\t" <<             m_MuonPtCut            << SLogger::endmsg;
    m_logger << INFO << "MuonEtaCut:\t" <<            m_MuonEtaCut           << SLogger::endmsg;
    m_logger << INFO << "TauPtCut:\t" <<              m_TauPtCut             << SLogger::endmsg;
    m_logger << INFO << "TauEtaCut:\t" <<             m_TauEtaCut            << SLogger::endmsg;
    m_logger << INFO << "AK4PtCut:\t" <<              m_AK4PtCut             << SLogger::endmsg;
    m_logger << INFO << "AK4EtaCut:\t" <<             m_AK4EtaCut            << SLogger::endmsg;
    m_logger << INFO << "MEtPtCut:\t" <<              m_MEtPtCut             << SLogger::endmsg;
    m_logger << INFO << "VPtCut:\t" <<                m_VPtCut               << SLogger::endmsg;
    m_logger << INFO << "VMassLowerCut:\t" <<         m_VMassLowerCut        << SLogger::endmsg;
    m_logger << INFO << "VMassUpperCut:\t" <<         m_VMassUpperCut        << SLogger::endmsg;
    m_logger << INFO << "HMassLowerCut:\t" <<         m_HMassLowerCut        << SLogger::endmsg;
    m_logger << INFO << "HMassUpperCut:\t" <<         m_HMassUpperCut        << SLogger::endmsg;
    m_logger << INFO << "XMassLowerCut:\t" <<         m_XMassLowerCut        << SLogger::endmsg;
    m_logger << INFO << "XTMassLowerCut:\t" <<        m_XTMassLowerCut       << SLogger::endmsg;
    
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
    DeclareVariable( TopWeight,           "TopWeight",  m_outputTreeName.c_str());
    DeclareVariable( QCDWeightUp,         "QCDWeightUp",  m_outputTreeName.c_str());
    DeclareVariable( QCDWeightDown,       "QCDWeightDown",  m_outputTreeName.c_str());
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
    DeclareVariable( isZtoEE,             "isZtoEE",  m_outputTreeName.c_str());
    DeclareVariable( isZtoMM,             "isZtoMM",  m_outputTreeName.c_str());
    
    DeclareVariable( nPV,                 "nPV",  m_outputTreeName.c_str());
    DeclareVariable( nElectrons,          "nElectrons",  m_outputTreeName.c_str());
    DeclareVariable( nMuons,              "nMuons",  m_outputTreeName.c_str());
    DeclareVariable( nTaus,               "nTaus",  m_outputTreeName.c_str());
    DeclareVariable( nJets,               "nJets",  m_outputTreeName.c_str());
    DeclareVariable( nBJets,              "nBJets",  m_outputTreeName.c_str());
    DeclareVariable( nBQuarks,            "nBQuarks",  m_outputTreeName.c_str());
    DeclareVariable( nBTagJets,           "nBTagJets",  m_outputTreeName.c_str());
    
    DeclareVariable( LheNl,               "LheNl",  m_outputTreeName.c_str());
    DeclareVariable( LheNj,               "LheNj",  m_outputTreeName.c_str());
    DeclareVariable( LheNb,               "LheNb",  m_outputTreeName.c_str());
    DeclareVariable( LheHT,               "LheHT",  m_outputTreeName.c_str());
    DeclareVariable( LheV_pt,             "LheV_pt",  m_outputTreeName.c_str());
    
    DeclareVariable( ST,                  "ST",  m_outputTreeName.c_str());
    DeclareVariable( HT,                  "HT",  m_outputTreeName.c_str());
    DeclareVariable( MET_pt,              "MET_pt",  m_outputTreeName.c_str());
    DeclareVariable( MET_phi,             "MET_phi",  m_outputTreeName.c_str());
    DeclareVariable( MinJetMetDPhi,       "MinDPhi",  m_outputTreeName.c_str());
    DeclareVariable( CosThetaStar,        "CosThetaStar",  m_outputTreeName.c_str());
    DeclareVariable( CosTheta1,           "CosTheta1",  m_outputTreeName.c_str());
    DeclareVariable( CosTheta2,           "CosTheta2",  m_outputTreeName.c_str());
    DeclareVariable( Phi,                 "Phi",  m_outputTreeName.c_str());
    DeclareVariable( Phi1,                "Phi1",  m_outputTreeName.c_str());
    DeclareVariable( Centrality,          "Centrality",  m_outputTreeName.c_str());
    
    DeclareVariable( Lepton1_pt,          "Lepton1_pt",  m_outputTreeName.c_str());
    DeclareVariable( Lepton2_pt,          "Lepton2_pt",  m_outputTreeName.c_str());
    DeclareVariable( Lepton1_pfIso,       "Lepton1_pfIso",  m_outputTreeName.c_str());
    DeclareVariable( Lepton2_pfIso,       "Lepton2_pfIso",  m_outputTreeName.c_str());
    DeclareVariable( Jet1_pt,             "Jet1_pt",  m_outputTreeName.c_str());
    DeclareVariable( Jet2_pt,             "Jet2_pt",  m_outputTreeName.c_str());
    DeclareVariable( Jet3_pt,             "Jet3_pt",  m_outputTreeName.c_str());
    DeclareVariable( kJet1_pt,            "kJet1_pt",  m_outputTreeName.c_str());
    DeclareVariable( kJet2_pt,            "kJet2_pt",  m_outputTreeName.c_str());
    DeclareVariable( Jet1_csv,            "Jet1_csv",  m_outputTreeName.c_str());
    DeclareVariable( Jet2_csv,            "Jet2_csv",  m_outputTreeName.c_str());
    DeclareVariable( Jet3_csv,            "Jet3_csv",  m_outputTreeName.c_str());

    DeclareVariable( Z_pt,                "Z_pt",  m_outputTreeName.c_str());
    DeclareVariable( Z_mass,              "Z_mass",  m_outputTreeName.c_str());
    DeclareVariable( H_pt,                "H_pt",  m_outputTreeName.c_str());
    DeclareVariable( H_mass,              "H_mass",  m_outputTreeName.c_str());
    DeclareVariable( A_pt,                "A_pt",  m_outputTreeName.c_str());
    DeclareVariable( A_mass,              "A_mass",  m_outputTreeName.c_str());
    DeclareVariable( A_tmass,             "A_tmass",  m_outputTreeName.c_str());
    DeclareVariable( kH_pt,               "kH_pt",  m_outputTreeName.c_str());
    DeclareVariable( kH_mass,             "kH_mass",  m_outputTreeName.c_str());
    DeclareVariable( kA_pt,               "kA_pt",  m_outputTreeName.c_str());
    DeclareVariable( kA_mass,             "kA_mass",  m_outputTreeName.c_str());
    DeclareVariable( kA_tmass,            "kA_tmass",  m_outputTreeName.c_str());

    //
    // Declare the output histograms:
    //
    // Histograms in main directory
    Book( TH1F( "Events", ";;Events", 1, 0, 1));
    // Cutflow
    std::vector<std::string> labelsZtoNN = {"All", "Trigger", "MET", "Lepton veto", "Tau veto", "Cleaning", "X_{T} mass", "Jets #geq 2", "H mass", "1 b-tag", "2 b-tag", "3 b-tag"};
    std::vector<std::string> labelsZtoEE = {"All", "Trigger", "Ele reco", "Ele p_{T}", "Ele Id+Iso", "Z(ee) candidate", "Z p_{T}", "Jets #geq 2", "H mass", "1 b-tag", "2 b-tag", "3 b-tag"};
    std::vector<std::string> labelsZtoMM = {"All", "Trigger", "Muon reco", "Muon p_{T}", "Muon Id", "Muon Iso", "Z(#mu#mu) candidate", "Z p_{T}", "Jets #geq 2", "H mass", "1 b-tag", "2 b-tag", "3 b-tag"};
    
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
    Book( TH1F( "A_tmass", ";m_{X} (GeV);Events", 100, 100, 1100), "0l" );
    Book( TH1F( "A_tmassKinH", ";m_{X} (GeV);Events", 100, 100, 1100), "0l" );
    Book( TH1F( "METFilters", "Events;", 15, -0.5, 14.5), "0l" );
    
    
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
    Book( TH1F( "A_mass", ";m_{X} (GeV);Events", 100, 100, 1100), "2e" );
    Book( TH1F( "A_massKinZ", ";m_{X} (GeV);Events", 100, 100, 1100), "2e" );
    Book( TH1F( "A_massKinH", ";m_{X} (GeV);Events", 100, 100, 1100), "2e" );
    Book( TH1F( "A_massKinZH", ";m_{X} (GeV);Events", 100, 100, 1100), "2e" );
    
    
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
    Book( TH1F( "A_mass", ";m_{X} (GeV);Events", 100, 100, 1100), "2m" );
    Book( TH1F( "A_massKinZ", ";m_{X} (GeV);Events", 100, 100, 1100), "2m" );
    Book( TH1F( "A_massKinH", ";m_{X} (GeV);Events", 100, 100, 1100), "2m" );
    Book( TH1F( "A_massKinZH", ";m_{X} (GeV);Events", 100, 100, 1100), "2m" );
    
    // Gen
    Book( TH1F( "LheV_pt", ";V p_{T} (GeV);Events;log", 200, 0, 2000), "Gen" );
    Book( TH1F( "GenV_pt", ";V p_{T} (GeV);Events;log", 200, 0, 2000), "Gen" );
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
    
    return;
}

void AZhAnalysis::EndInputData( const SInputData& ) throw( SError ) {
   
   return;

}

void AZhAnalysis::BeginInputFile( const SInputData& ) throw( SError ) {
    m_logger << INFO << "Connecting input variables" << SLogger::endmsg;

    if(m_isData) m_eventInfo.ConnectVariables( m_recoTreeName.c_str(), Ntuple::EventInfoTrigger|Ntuple::EventInfoMETFilters/*|Ntuple::EventInfoMuonFilters*/, "" );
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



void AZhAnalysis::ExecuteEvent( const SInputData&, Double_t ) throw( SError ) {
    
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
        if(el.pt() < m_ElecPtCut || fabs(el.eta()) > m_ElecEtaCut) continue;
        nElectronsReco++;
        if(i==0 && el.pt() < m_Elec1PtCut) continue;
        if(i==1 && el.pt() < m_Elec2PtCut) continue;
        nElectronsPt++;
        if(i==0) for(int e=0; e<=(el.isVetoElectron()+el.isLooseElectron()+el.isMediumElectron()+el.isTightElectron()); e++) Hist("Electron1_Id", "2e")->Fill(e, EventWeight);
        if(i==1) for(int e=0; e<=(el.isVetoElectron()+el.isLooseElectron()+el.isMediumElectron()+el.isTightElectron()); e++) Hist("Electron2_Id", "2e")->Fill(e, EventWeight);
        if(i==0) Hist("Electron1_pfIso", "2e")->Fill(el.pfRhoCorrRelIso03(), EventWeight);
        if(i==1) Hist("Electron2_pfIso", "2e")->Fill(el.pfRhoCorrRelIso03(), EventWeight);
        if(i==0 && !(el.isTightElectron()==1)) continue;
        if(i>=1 && !(el.isLooseElectron()==1)) continue;
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
        if(i==0 && mu.pt() < m_Muon1PtCut) continue;
        if(i==1 && mu.pt() < m_Muon2PtCut) continue;
        nMuonsPt++;
        if(i==0) for(int m=0; m<=(mu.isPFMuon()+mu.isLooseMuon()+mu.isMediumMuon()+mu.isTightMuon()); m++) Hist("Muon1_Id", "2m")->Fill(m, EventWeight);
        if(i==1) for(int m=0; m<=(mu.isPFMuon()+mu.isLooseMuon()+mu.isMediumMuon()+mu.isTightMuon()); m++) Hist("Muon2_Id", "2m")->Fill(m, EventWeight);
        if(i==0 && !(mu.isTightMuon()==1)) continue;
        if(i>=1 && !(mu.isLooseMuon()==1)) continue;
        nMuonsId++;
        if(i==0) Hist("Muon1_pfIso", "2m")->Fill(mu.pfDeltaCorrRelIso(), EventWeight);
        if(i==1) Hist("Muon2_pfIso", "2m")->Fill(mu.pfDeltaCorrRelIso(), EventWeight);
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
    std::vector<TLorentzVector> JetsVectRes;
    std::vector<TLorentzVector> JetsVectScaleUp;
    std::vector<TLorentzVector> JetsVectScaleDown;
    std::vector<TLorentzVector> JetsVectResUp;
    std::vector<TLorentzVector> JetsVectResDown;
    for(int i = 0; i < m_jetAK4.N; ++i) {
        UZH::Jet jet(&m_jetAK4, i);
        if(jet.pt() < m_AK4PtCut || fabs(jet.eta()) > m_AK4EtaCut || !jet.IDLoose()) continue;
        if(!jet.IDLoose()) continue;
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
    nJets = JetsVect.size();
    MHT = sqrt(HTx*HTx + HTy*HTy);
    MHTNoMu = MHT;
    
    // --- MET ---
    UZH::MissingEt MET( &m_missingEt, 0 );
    TLorentzVector MET_tlv;
    MET_tlv.SetPtEtaPhiE(MET.et(), 0., MET.phi(), MET.et());
    METNoMu = MuonVect.size() == 0 ? MET.et() : sqrt(pow(MET_tlv.Px() + MuonVect[0].tlv().Px(), 2) + pow(MET_tlv.Py() + MuonVect[0].tlv().Py(), 2));
    MinMETMHT = std::min(float(MET.et()), MHT);
    MinMETNoMuMHTNoMu = std::min(METNoMu, MHTNoMu);
    for(int i = 0; i < nJets; i++) if(fabs(MET_tlv.DeltaPhi(JetsVect[i].tlv())) < MinJetMetDPhi) MinJetMetDPhi = fabs(MET_tlv.DeltaPhi(JetsVect[i].tlv()));
    if(nJets == 0) MinJetMetDPhi = -1.;
    
    // --- GEN ---
    TLorentzVector GenH;
    TLorentzVector GenV;
    TLorentzVector GenX;
    TLorentzVector LepP;
    TLorentzVector LepM;
    TLorentzVector BqaP;
    TLorentzVector BqaM;
    bool isZjets = ((sample.find("DY") != std::string::npos || sample.find("Z") != std::string::npos) && sample.find("JetsTo") != std::string::npos);
    bool isWjets = (sample.find("WJetsToLNu") != std::string::npos);
    int GenVpdgId(0), GenLepPpdgId(0), GenLepMpdgId(0), HjetIndex(-1), VjetIndex(-1), Nb21(0), Nb23(0);
    float GenVPt(-1.), GenHT(0.), GenTopPt(-1.), GenAntiTopPt(-1.), TopPtWeight(1.), AntiTopPtWeight(1.);
    // Z and W electroweak corrections
    if(isMC && (m_isSignal || isZjets || isWjets)) {
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
            if(abs(gen.pdgId()) == 5 && gen.status()==23) Nb23++;
            if(abs(gen.pdgId()) == 5 && gen.status()==21) Nb21++;
        }
        LheNb = Nb21 > 0 ? (Nb23+Nb21)/2 : Nb23;
        if(GenV.Pt()>0.) GenVPt = GenV.Pt();
        else if(LepP.Pt()>0. && LepM.Pt()>0.) {GenVPt = (LepP+LepM).Pt(); GenVpdgId = (GenLepPpdgId == -GenLepMpdgId) ? 23 : 24;}
        
        if(GenVPt > 0. && isZjets) ZewkWeight *= m_ScaleFactorTool.GetEwkZ(GenVPt);
        if(GenVPt > 0. && isWjets)  WewkWeight *= m_ScaleFactorTool.GetEwkW(GenVPt);
        EventWeight *= WewkWeight * ZewkWeight;
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
    QCDWeightUp = std::min(std::min(m_eventInfo.genFacWeightUp, m_eventInfo.genRenWeightUp), m_eventInfo.genFacRenWeightUp);
    QCDWeightDown = std::max(std::max(m_eventInfo.genFacWeightDown, m_eventInfo.genRenWeightDown), m_eventInfo.genFacRenWeightDown);
    
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
    if(!triggerMap["SingleMu"] && !triggerMap["SingleIsoMu"] && !triggerMap["SingleEle"] && !triggerMap["SingleIsoEle"] && !triggerMap["MET"] && !triggerMap["METMHT"] && !triggerMap["METMHTNoMu"] && !triggerMap["HT"] && !triggerMap["HTWJ"]) {m_logger << INFO << " - No trigger"  << SLogger::endmsg; throw SError( SError::SkipEvent ); }
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
            if((MuonVect[0].tlv() + MuonVect[1].tlv()).M() < 50.) {m_logger << INFO << " - No Z candidate"  << SLogger::endmsg;}
            else {
                // Check trigger consistency
                if(!isMC && !triggerMap["SingleIsoMu"]) { m_logger << INFO << " - Trigger inconsistency" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
                // SF
                TriggerWeight *= m_ScaleFactorTool.GetTrigSingleIsoMuon(MuonVect[0].pt(), MuonVect[0].eta());
                LeptonWeight *= m_ScaleFactorTool.GetMuonTightId(MuonVect[0].pt(), MuonVect[0].eta());
                LeptonWeight *= m_ScaleFactorTool.GetMuonLooseId(MuonVect[1].pt(), MuonVect[1].eta());
                LeptonWeight *= m_ScaleFactorTool.GetMuonLoosePFIso(MuonVect[0].pt(), MuonVect[0].eta());
                LeptonWeight *= m_ScaleFactorTool.GetMuonLoosePFIso(MuonVect[1].pt(), MuonVect[1].eta());
                LeptonWeight *= pow(m_ScaleFactorTool.GetMuonTrk(nPV), 2);
                TriggerWeightUp *= m_ScaleFactorTool.GetTrigSingleIsoMuon(MuonVect[0].pt(), MuonVect[0].eta(), +1);
                LeptonWeightUp *= m_ScaleFactorTool.GetMuonTightId(MuonVect[0].pt(), MuonVect[0].eta(), +1);
                LeptonWeightUp *= m_ScaleFactorTool.GetMuonLooseId(MuonVect[1].pt(), MuonVect[1].eta(), +1);
                LeptonWeightUp *= m_ScaleFactorTool.GetMuonLoosePFIso(MuonVect[0].pt(), MuonVect[0].eta(), +1);
                LeptonWeightUp *= m_ScaleFactorTool.GetMuonLoosePFIso(MuonVect[1].pt(), MuonVect[1].eta(), +1);
                LeptonWeightUp *= pow(m_ScaleFactorTool.GetMuonTrk(nPV, +1), 2);
                TriggerWeightDown *= m_ScaleFactorTool.GetTrigSingleIsoMuon(MuonVect[0].pt(), MuonVect[0].eta(), -1);
                LeptonWeightDown *= m_ScaleFactorTool.GetMuonTightId(MuonVect[0].pt(), MuonVect[0].eta(), -1);
                LeptonWeightDown *= m_ScaleFactorTool.GetMuonLooseId(MuonVect[1].pt(), MuonVect[1].eta(), -1);
                LeptonWeightDown *= m_ScaleFactorTool.GetMuonLoosePFIso(MuonVect[0].pt(), MuonVect[0].eta(), -1);
                LeptonWeightDown *= m_ScaleFactorTool.GetMuonLoosePFIso(MuonVect[1].pt(), MuonVect[1].eta(), -1);
                LeptonWeightDown *= pow(m_ScaleFactorTool.GetMuonTrk(nPV, -1), 2);
                EventWeight *= TriggerWeight * LeptonWeight;
                // Build candidate
                Lepton1 = MuonVect[0].tlv();
                Lepton2 = MuonVect[1].tlv();
                Lepton1_pt = MuonVect[0].pt();
                Lepton2_pt = MuonVect[1].pt();
                Lepton1_pfIso = MuonVect[0].pfDeltaCorrRelIso();
                Lepton2_pfIso = MuonVect[1].pfDeltaCorrRelIso();
                Z = Lepton1 + Lepton2;
                // nEvents
                Hist("Events", "2m")->Fill(6., EventWeight);
                Hist("Z_pt", "2m")->Fill(Z.Pt(), EventWeight);
                Hist("Z_mass", "2m")->Fill(Z.M(), EventWeight);
                Hist("Z_massRaw", "2m")->Fill((MuonVect[0].tlv() + MuonVect[1].tlv()).M(), EventWeight);
                if(fabs(MuonVect[0].eta()) < 1.2 && fabs(MuonVect[1].eta()) < 1.2) Hist("Z_massBB", "2m")->Fill(Z.M(), EventWeight);
                if(fabs(MuonVect[0].eta()) < 1.2 && fabs(MuonVect[1].eta()) > 1.2) Hist("Z_massBE", "2m")->Fill(Z.M(), EventWeight);
                if(fabs(MuonVect[0].eta()) > 1.2 && fabs(MuonVect[1].eta()) < 1.2) Hist("Z_massEB", "2m")->Fill(Z.M(), EventWeight);
                if(fabs(MuonVect[0].eta()) > 1.2 && fabs(MuonVect[1].eta()) > 1.2) Hist("Z_massEE", "2m")->Fill(Z.M(), EventWeight);
                if(Z.Pt() < m_VPtCut) {m_logger << INFO << " - No V boost" << SLogger::endmsg;}
                else {
                    Hist("Events", "2m")->Fill(7., EventWeight);
                    isZtoMM = true;
                    m_logger << INFO << " + Z -> mm candidate reconstructed" << SLogger::endmsg;
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
            if((ElecVect[0].tlv() + ElecVect[1].tlv()).M() < 50.) {m_logger << INFO << " - No Z candidate"  << SLogger::endmsg;}
            else {
                // Check trigger consistency
                if(!isMC && !triggerMap["SingleIsoEle"] && !triggerMap["SingleEle"]) { m_logger << INFO << " - Trigger inconsistency" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
                // SF
                TriggerWeight *= std::max(m_ScaleFactorTool.GetTrigSingleEle(ElecVect[0].pt(), ElecVect[0].eta()), m_ScaleFactorTool.GetTrigSingleIsoEle(ElecVect[0].pt(), ElecVect[0].eta()));
                LeptonWeight *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[0].pt(), ElecVect[0].eta());
                LeptonWeight *= m_ScaleFactorTool.GetEleIdLooseWP(ElecVect[1].pt(), ElecVect[1].eta());
                LeptonWeight *= m_ScaleFactorTool.GetEleReco(ElecVect[0].pt(), ElecVect[0].eta());
                LeptonWeight *= m_ScaleFactorTool.GetEleReco(ElecVect[1].pt(), ElecVect[1].eta());
                TriggerWeightUp *= std::max(m_ScaleFactorTool.GetTrigSingleEle(ElecVect[0].pt(), ElecVect[0].eta(), +1), m_ScaleFactorTool.GetTrigSingleIsoEle(ElecVect[0].pt(), ElecVect[0].eta(), +1));
                LeptonWeightUp *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[0].pt(), ElecVect[0].eta(), +1);
                LeptonWeightUp *= m_ScaleFactorTool.GetEleIdLooseWP(ElecVect[1].pt(), ElecVect[1].eta(), +1);
                LeptonWeightUp *= m_ScaleFactorTool.GetEleReco(ElecVect[0].pt(), ElecVect[0].eta(), +1);
                LeptonWeightUp *= m_ScaleFactorTool.GetEleReco(ElecVect[1].pt(), ElecVect[1].eta(), +1);
                TriggerWeightDown *= std::max(m_ScaleFactorTool.GetTrigSingleEle(ElecVect[0].pt(), ElecVect[0].eta(), -1), m_ScaleFactorTool.GetTrigSingleIsoEle(ElecVect[0].pt(), ElecVect[0].eta(), -1));
                LeptonWeightDown *= m_ScaleFactorTool.GetEleIdTightWP(ElecVect[0].pt(), ElecVect[0].eta(), -1);
                LeptonWeightDown *= m_ScaleFactorTool.GetEleIdLooseWP(ElecVect[1].pt(), ElecVect[1].eta(), -1);
                LeptonWeightDown *= m_ScaleFactorTool.GetEleReco(ElecVect[0].pt(), ElecVect[0].eta(), -1);
                LeptonWeightDown *= m_ScaleFactorTool.GetEleReco(ElecVect[1].pt(), ElecVect[1].eta(), -1);
                EventWeight *= TriggerWeight * LeptonWeight;
                // Build candidate
                Lepton1 = CalibratedElecVect[0];
                Lepton2 = CalibratedElecVect[1];
                Lepton1_pt = CalibratedElecVect[0].Pt();
                Lepton2_pt = CalibratedElecVect[1].Pt();
                Lepton1_pfIso = ElecVect[0].pfRhoCorrRelIso03();
                Lepton2_pfIso = ElecVect[1].pfRhoCorrRelIso03();
                Z = Lepton1 + Lepton2;
                // nEvents
                Hist("Events", "2e")->Fill(5., EventWeight);
                Hist("Z_pt", "2e")->Fill(Z.Pt(), EventWeight);
                Hist("Z_mass", "2e")->Fill(Z.M(), EventWeight);
                Hist("Z_massRaw", "2e")->Fill((ElecVect[0].tlv() + ElecVect[1].tlv()).M(), EventWeight);
                if(fabs(ElecVect[0].eta()) < 1.4442 && fabs(ElecVect[1].eta()) < 1.4442) Hist("Z_massBB", "2e")->Fill(Z.M(), EventWeight);
                if(fabs(ElecVect[0].eta()) < 1.4442 && fabs(ElecVect[1].eta()) > 1.4442) Hist("Z_massBE", "2e")->Fill(Z.M(), EventWeight);
                if(fabs(ElecVect[0].eta()) > 1.4442 && fabs(ElecVect[1].eta()) < 1.4442) Hist("Z_massEB", "2e")->Fill(Z.M(), EventWeight);
                if(fabs(ElecVect[0].eta()) > 1.4442 && fabs(ElecVect[1].eta()) > 1.4442) Hist("Z_massEE", "2e")->Fill(Z.M(), EventWeight);
                if(Z.Pt() < m_VPtCut) {m_logger << INFO << " - No V boost" << SLogger::endmsg;}
                else {
                    Hist("Events", "2e")->Fill(6., EventWeight);
                    isZtoEE = true;
                    m_logger << INFO << " + Z -> ee candidate reconstructed" << SLogger::endmsg;
                }
            }
        }
    }
    // ----------- Z TO NEUTRINOS ---------------
    if(!isZtoMM && !isZtoEE) {
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
                    if(nJets > 0 && JetsVect[0].IDTight() && JetsVect[0].chf() > 0.1) Hist("MinJetMetDPhi", "0l")->Fill(MinJetMetDPhi, EventWeight);
                    if(!(MinJetMetDPhi >= 0.5)) m_logger << INFO << " - ZtoNN event failed noise cleaning" << SLogger::endmsg;
                    else {
                        Hist("Events", "0l")->Fill(5., EventWeight);
                        if(A_tmass < m_XTMassLowerCut) m_logger << INFO << " - X candidate low mass" << SLogger::endmsg;
                        else {
                            Hist("Events", "0l")->Fill(6., EventWeight);
                            // Dummy V candidate
                            if(MET.et()!=MET.et()) { m_logger << WARNING << " - MET is nan" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
                            Z.SetPtEtaPhiE(MET.et(), 0., MET.phi(), MET.et());
                            if(isMC) {
                                TriggerWeight *= m_ScaleFactorTool.GetTrigMET(MET.et());
                            }
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
                            isZtoNN=true;
                            m_logger << INFO << " + Z -> nn candidate reconstructed" << SLogger::endmsg;
                        }
                    }
                }
            }
        }
    }
    
    // ----------- Nothing else matters ---------------
    if(!isZtoMM && !isZtoEE && !isZtoNN) { m_logger << INFO << " - No V candidate" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
    
    std::string category("");
    if(isZtoNN) category = "0l";
    else if(isZtoEE) category = "2e";
    else if(isZtoMM) category = "2m";
    
    Hist("EventWeight", category.c_str())->Fill(EventWeight);
    Hist("GenWeight", category.c_str())->Fill(GenWeight);
    Hist("TopWeight", category.c_str())->Fill(TopWeight);
    Hist("PUWeight", category.c_str())->Fill(PUWeight);
    Hist("TriggerWeight", category.c_str())->Fill(TriggerWeight);
    Hist("LeptonWeight", category.c_str())->Fill(LeptonWeight);
    Hist("nJets", category.c_str())->Fill(nJets, EventWeight);
    Hist("HT", category.c_str())->Fill(HT, EventWeight);
    

    // -----------------------------------
    //           HADRONIC HIGGS
    // -----------------------------------

    if(nJets < 2) { m_logger << INFO << " - N Jets < 2" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
    if(isZtoNN && nJets > 0 && !(JetsVect[0].IDTight() && JetsVect[0].chf() > 0.1)) { m_logger << INFO << " - No Jet Id" << SLogger::endmsg; throw SError( SError::SkipEvent ); }
    
    if(isZtoNN) Hist("Events", "0l")->Fill(7., EventWeight);
    if(isZtoEE) Hist("Events", "2e")->Fill(7., EventWeight);
    if(isZtoMM) Hist("Events", "2m")->Fill(8., EventWeight);
    m_logger << INFO << " + N Jets >= 2" << SLogger::endmsg;
    
    std::vector<UZH::Jet> JetsVectSorted(JetsVect.begin(), JetsVect.end());
    std::sort(JetsVectSorted.begin(), JetsVectSorted.end(), SortByCSV);
    std::vector<UZH::Jet> JetsVectSortedLoose(JetsVectSorted.begin()+1, JetsVectSorted.end());
    
    Jet1.SetPtEtaPhiE(JetsVect[0].pt(), JetsVect[0].eta(), JetsVect[0].phi(), JetsVect[0].e());
    Jet1_pt = JetsVect[0].pt();
    Jet1_csv = JetsVectSorted[0].csv();
    Jet2.SetPtEtaPhiE(JetsVect[1].pt(), JetsVect[1].eta(), JetsVect[1].phi(), JetsVect[1].e());
    Jet2_pt = JetsVect[1].pt();
    Jet2_csv = JetsVectSorted[1].csv();
    if(nJets >= 3) {
        Jet3.SetPtEtaPhiE(JetsVect[2].pt(), JetsVect[2].eta(), JetsVect[2].phi(), JetsVect[2].e());
        Jet3_pt = kJet3_pt = JetsVect[2].pt();
        Jet3_csv = JetsVectSorted[2].csv();
    }
    H = Jet1 + Jet2;
    
    
    // --- Kinematic Fit ---
    MEt.SetPtEtaPhiE(MET.et(), 0., MET.phi(), MET.et());
    kMEt.SetPtEtaPhiE(MET.et(), 0., MET.phi(), MET.et());
    
    // Higgs
    float kFactor( 125./H.M() );
    kJet1 = Jet1 * kFactor;
    kJet2 = Jet2 * kFactor;
    kH = kJet1 + kJet2;
    
    kJet1_pt = kJet1.Pt();
    kJet2_pt = kJet2.Pt();
    
    // Z
    if(isZtoEE || isZtoMM) {
        kFactor = 90.18/Z.M();
        kLepton1 = Lepton1 * kFactor;
        kLepton2 = Lepton2 * kFactor;
        kZ = kLepton1 + kLepton2;
    }
    else {
        kLepton1 = Lepton1;
        kLepton2 = Lepton2;
        kZ = MEt;
    }
    
    
    // --- BTV ---
    for(unsigned int i=0; i<1; i++) {
        if(m_bTaggingScaleTool.isTagged_tag( JetsVectSorted[i].csv() )) nBTagJets++; // Count Tight b-tagged jets
    }
    for(unsigned int i=1; i<JetsVectSortedLoose.size(); i++) {
        if( m_bTaggingScaleTool.isTagged_veto( JetsVectSorted[i].csv() )) nBTagJets++; // Count Loose b-tagged jets
    }

    if(isMC) {
        float BTagAK4Weight(1.), BTagAK4WeightUp(1.), BTagAK4WeightDown(1.), BTagAK4WeightBUp(1.), BTagAK4WeightBDown(1.), BTagAK4WeightLUp(1.), BTagAK4WeightLDown(1.), BTagAK4WeightB1(1.), BTagAK4WeightB2(1.), BTagAK4WeightL1(1.), BTagAK4WeightL2(1.);
        // Tight
        BTagAK4Weight *= m_bTaggingScaleTool.getScaleFactor_tag(JetsVectSorted[0]);
        BTagAK4WeightBUp *= m_bTaggingScaleTool.getScaleFactor_tag(JetsVectSorted[0], +1, 0);
        BTagAK4WeightBDown *= m_bTaggingScaleTool.getScaleFactor_tag(JetsVectSorted[0], -1, 0);
        BTagAK4WeightLUp *= m_bTaggingScaleTool.getScaleFactor_tag(JetsVectSorted[0], 0, +1);
        BTagAK4WeightLDown *= m_bTaggingScaleTool.getScaleFactor_tag(JetsVectSorted[0], 0, -1);
        // Loose
        BTagAK4Weight *= m_bTaggingScaleTool.getScaleFactor_veto(JetsVectSortedLoose);
        BTagAK4WeightBUp *= m_bTaggingScaleTool.getScaleFactor_veto(JetsVectSortedLoose, +1, 0);
        BTagAK4WeightBDown *= m_bTaggingScaleTool.getScaleFactor_veto(JetsVectSortedLoose, -1, 0);
        BTagAK4WeightLUp *= m_bTaggingScaleTool.getScaleFactor_veto(JetsVectSortedLoose, 0, +1);
        BTagAK4WeightLDown *= m_bTaggingScaleTool.getScaleFactor_veto(JetsVectSortedLoose, 0, -1);
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
    Hist("BTagWeight", category.c_str())->Fill(BTagWeight);
    
    // -----------------------------------
    //           X and VARIABLES
    // -----------------------------------
    A = Z + H;
    kA = Z + kH;
    
    MET_pt = MET.et();
    MET_phi = MET.phi();
    Z_pt = Z.Pt();
    Z_mass = Z.M();
    H_pt = H.Pt();
    H_mass = H.M();
    A_pt = A.Pt();
    A_mass = A.M();
    kH_pt = kH.Pt();
    kH_mass = kH.M();
    kA_pt = kA.Pt();
    kA_mass = kA.M();
    
    if(isZtoEE || isZtoMM) {
        TLorentzVector L1, L2, B1, B2;
        if(isZtoEE) {L1 = (ElecVect[0].charge() > ElecVect[1].charge()) ? Lepton1 : Lepton2; L2 = (ElecVect[0].charge() > ElecVect[1].charge()) ? Lepton2 : Lepton1;}
        if(isZtoMM) {L1 = (MuonVect[0].charge() > MuonVect[1].charge()) ? Lepton1 : Lepton2; L2 = (MuonVect[0].charge() > MuonVect[1].charge()) ? Lepton2 : Lepton1;}
        B1 = JetsVect[0].charge() > JetsVect[1].charge() ? Jet1 : Jet2; B2 = JetsVect[0].charge() > JetsVect[1].charge() ? Jet2 : Jet1;
        Centrality = m_VariableTool.ReturnCentrality(L1, L2, B1, B2);
        CosThetaStar = m_VariableTool.ReturnCosThetaStar(A, Z);
        CosTheta1 = m_VariableTool.ReturnCosTheta1(Z, L1, L2, B1, B2);
        CosTheta2 = m_VariableTool.ReturnCosTheta2(H, L1, L2, B1, B2);
        Phi = m_VariableTool.ReturnPhi(A, L1, L2, B1, B2);
        Phi1 = m_VariableTool.ReturnPhi1(A, L1, L2);
//        Centrality = m_VariableTool.ReturnCentrality(LepP, LepM, BqaP, BqaM);
//        CosThetaStar = m_VariableTool.ReturnCosThetaStar(GenX, GenV);
//        CosTheta1 = m_VariableTool.ReturnCosTheta1(GenV, LepP, LepM, BqaP, BqaM);
//        CosTheta2 = m_VariableTool.ReturnCosTheta2(GenH, LepP, LepM, BqaP, BqaM);
//        Phi = m_VariableTool.ReturnPhi(GenX, LepP, LepM, BqaP, BqaM);
//        Phi1 = m_VariableTool.ReturnPhi1(GenX, LepP, LepM);
    }
    else if(isZtoNN) {
        A_tmass = sqrt( 2.*MEt.Pt()*H.Pt()*(1.-cos(H.DeltaPhi(MEt))) );
        kA_tmass = sqrt( 2.*MEt.Pt()*kH.Pt()*(1.-cos(kH.DeltaPhi(MEt))) );
    }
    
    if(isZtoNN) {
        Hist("A_tmass", "0l")->Fill(A_tmass, EventWeight);
        Hist("A_tmassKinH", "0l")->Fill(kA_tmass, EventWeight);
    }
    else if(isZtoEE || isZtoMM) {
        Hist("A_mass", category.c_str())->Fill((Z+H).M(), EventWeight);
        Hist("A_massKinZ", category.c_str())->Fill((kZ+H).M(), EventWeight);
        Hist("A_massKinH", category.c_str())->Fill((Z+kH).M(), EventWeight);
        Hist("A_massKinZH", category.c_str())->Fill((kZ+kH).M(), EventWeight);
    }
    
    // -----------------------------------
    //           CLEANING
    // -----------------------------------
    
    // Semileptonic analysis categorization
    if(isZtoEE || isZtoMM || isZtoNN) {
        // Signal Regions
        if(H.M() > m_HMassLowerCut && H.M() < m_HMassUpperCut) {
            // Cut flow
            if(isZtoNN) Hist("Events", "0l")->Fill(8., EventWeight);
            if(isZtoEE) Hist("Events", "2e")->Fill(8., EventWeight);
            if(isZtoMM) Hist("Events", "2m")->Fill(9., EventWeight);
            // Single b-tag
            if(nBTagJets >= 1) {
                if(isZtoNN) Hist("Events", "0l")->Fill(9., EventWeight*BTagWeight);
                if(isZtoEE) Hist("Events", "2e")->Fill(9., EventWeight*BTagWeight);
                if(isZtoMM) Hist("Events", "2m")->Fill(10., EventWeight*BTagWeight);
            }
            // Double b-tag
            if(nBTagJets >= 2) {
                if(isZtoNN) Hist("Events", "0l")->Fill(10., EventWeight*BTagWeight);
                if(isZtoEE) Hist("Events", "2e")->Fill(10., EventWeight*BTagWeight);
                if(isZtoMM) Hist("Events", "2m")->Fill(11., EventWeight*BTagWeight);
            }
            // Triple b-tag
            if(nBTagJets >= 3) {
                if(isZtoNN) Hist("Events", "0l")->Fill(11., EventWeight*BTagWeight);
                if(isZtoEE) Hist("Events", "2e")->Fill(11., EventWeight*BTagWeight);
                if(isZtoMM) Hist("Events", "2m")->Fill(12., EventWeight*BTagWeight);
            }
        }
    }
    
    
    m_logger << INFO << " + Tree filled" << SLogger::endmsg;
    
    return;
}







bool AZhAnalysis::isGoodEvent(int runNumber, int lumiSection) {
  
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


std::map<std::string, bool> AZhAnalysis::getTriggerMap() {
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

bool AZhAnalysis::passTrigger(std::string cat) {
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


bool AZhAnalysis::passMETFilters(bool data) {
  
    // using only what's recommended in https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2
    if( !(m_eventInfo.PV_filter) ) return false;
    if(data) Hist( "METFilters", "0l" )->Fill(1);
    if( !(m_eventInfo.passFilter_CSCHalo) ) return false;
    if(data) Hist( "METFilters", "0l" )->Fill(2);
    if( !(m_eventInfo.passFilter_HBHE) ) return false;
    if(data) Hist( "METFilters", "0l" )->Fill(3);
    if( !(m_eventInfo.passFilter_HBHEIso) ) return false;
    if(data) Hist( "METFilters", "0l" )->Fill(4);
    if( !(m_eventInfo.passFilter_ECALDeadCell) ) return false;
    if(data) Hist( "METFilters", "0l" )->Fill(5);
    if(data) {
        if( !(m_eventInfo.passFilter_EEBadSc) ) return false;
        Hist( "METFilters", "0l" )->Fill(6);
    }
    if( !(m_eventInfo.passFilter_globalTightHalo2016) ) return false;
    if(data) Hist( "METFilters", "0l" )->Fill(7);
    if( !(m_eventInfo.passFilter_muonBadTrack) ) return false;
    if(data) Hist( "METFilters", "0l" )->Fill(8);
    if( !(m_eventInfo.passFilter_chargedHadronTrackResolution) ) return false;       
    if(data) Hist( "METFilters", "0l" )->Fill(9);
    //  if( !(m_eventInfo.passFilter_METFilters) ) return false;
    //   Hist( "METFilters" )->Fill(10);
    if(data) {
        if( (!m_eventInfo.flag_badMuons) ) return false;
        Hist( "METFilters", "0l" )->Fill(10);
        if( (!m_eventInfo.flag_duplicateMuons) ) return false;
        Hist( "METFilters", "0l" )->Fill(11);
        if( (!m_eventInfo.flag_nobadMuons) ) return false;
        Hist( "METFilters", "0l" )->Fill(12);
    }

    return true;
  
}



void AZhAnalysis::clearBranches() {
    EventWeight = GenWeight = ZewkWeight = WewkWeight = TopWeight = QCDWeightUp = QCDWeightDown = PUWeight = PUWeightUp = PUWeightDown = TriggerWeight = TriggerWeightUp = TriggerWeightDown = LeptonWeight = LeptonWeightUp = LeptonWeightDown = BTagWeight = BTagWeightUp = BTagWeightDown = 1.;
    isZtoEE = isZtoMM = isZtoNN = isTveto = false;
    LheV_pt, LheHT, LheNj, LheNl, LheNb = 0;
    nPV = nElectrons = nMuons = nTaus = nPhotons = nJets = nBJets = nBQuarks = nBTagJets = nBVetoJets = 0;
    HT = HTx = HTy = MHT = MET_pt = MET_phi = MHTNoMu = METNoMu = MinMETMHT = MinMETNoMuMHTNoMu = ST = 0.;
    Centrality = CosThetaStar = CosTheta1 = CosTheta2 = Phi = Phi1 = -9.;
    JetMetDPhi = -1.;
    MinJetMetDPhi = 10.;
    
    Lepton1 = Lepton2 = kLepton1 = kLepton2 = Jet1 = Jet2 = Jet3 = kJet1 = kJet2 = kJet3 = MEt = kMEt = Z = H = A = kZ = kH = kA = TLorentzVector();
    Lepton1_pt = Lepton2_pt = Lepton1_pfIso = Lepton2_pfIso = Jet1_pt = Jet2_pt = Jet3_pt = kJet1_pt = kJet2_pt = kJet3_pt = Jet1_csv = Jet2_csv = Jet3_csv = -9.;
    Z_pt = Z_mass = H_pt = H_mass = A_pt = A_mass = A_tmass = kH_pt = kH_mass = kA_pt = kA_mass = kA_tmass = -1.;
}


