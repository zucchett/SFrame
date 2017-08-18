#include "../interface/ScaleFactorTool.h"
#include <cstdlib>
#include <limits>

#include <TFile.h>
ScaleFactorTool::ScaleFactorTool(SCycleBase* parent, const char* name ): SToolBase( parent ), m_name( name ) {
    SetLogName( name );
    // Electron Trigger
    DeclareProperty( m_name+"_TrigSingleEleFileName", m_TrigSingleEleFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/Electron/Ele115_passingTight_2016.root" );
    DeclareProperty( m_name+"_TrigSingleEleHistNumName", m_TrigSingleEleHistNumName = "EGamma_EffData2D" );
    DeclareProperty( m_name+"_TrigSingleEleHistDenName", m_TrigSingleEleHistDenName = "EGamma_EffMC2D" );
    DeclareProperty( m_name+"_TrigSingleIsoEleFileName", m_TrigSingleIsoEleFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/Electron/Ele32_passingTight_2016.root" );
    DeclareProperty( m_name+"_TrigSingleIsoEleHistNumName", m_TrigSingleIsoEleHistNumName = "EGamma_EffData2D" );
    DeclareProperty( m_name+"_TrigSingleIsoEleHistDenName", m_TrigSingleIsoEleHistDenName = "EGamma_EffMC2D" );
    
    // Electron Id+Iso
    DeclareProperty(  m_name+"_EleIdTightWPFileName", m_EleIdTightWPFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/Electron/egammaEffi.txt_EGM2D_TightWP.root" );
    DeclareProperty(  m_name+"_EleIdTightWPHistNumName", m_EleIdTightWPHistNumName = "EGamma_EffData2D" );
    DeclareProperty(  m_name+"_EleIdTightWPHistDenName", m_EleIdTightWPHistDenName = "EGamma_EffMC2D" );
    DeclareProperty(  m_name+"_EleIdMediumWPFileName", m_EleIdMediumWPFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/Electron/egammaEffi.txt_EGM2D_MediumWP.root" );
    DeclareProperty(  m_name+"_EleIdMediumWPHistNumName", m_EleIdMediumWPHistNumName = "EGamma_EffData2D" );
    DeclareProperty(  m_name+"_EleIdMediumWPHistDenName", m_EleIdMediumWPHistDenName = "EGamma_EffMC2D" );
    DeclareProperty(  m_name+"_EleIdLooseWPFileName", m_EleIdLooseWPFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/Electron/egammaEffi.txt_EGM2D_LooseWP.root" );
    DeclareProperty(  m_name+"_EleIdLooseWPHistNumName", m_EleIdLooseWPHistNumName = "EGamma_EffData2D" );
    DeclareProperty(  m_name+"_EleIdLooseWPHistDenName", m_EleIdLooseWPHistDenName = "EGamma_EffMC2D" );
    DeclareProperty(  m_name+"_EleIdVetoWPFileName", m_EleIdVetoWPFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/Electron/egammaEffi.txt_EGM2D_VetoWP.root" );
    DeclareProperty(  m_name+"_EleIdVetoWPHistNumName", m_EleIdVetoWPHistNumName = "EGamma_EffData2D" );
    DeclareProperty(  m_name+"_EleIdVetoWPHistDenName", m_EleIdVetoWPHistDenName = "EGamma_EffMC2D" );
    DeclareProperty(  m_name+"_EleRecoFileName", m_EleRecoFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/Electron/egammaEffi.txt_EGM2D_Reco.root" );
    DeclareProperty(  m_name+"_EleRecoHistNumName", m_EleRecoHistNumName = "EGamma_EffData2D" );
    DeclareProperty(  m_name+"_EleRecoHistDenName", m_EleRecoHistDenName = "EGamma_EffMC2D" );
    
    // Muon Trigger
    DeclareProperty( m_name+"_TrigSingleMuonFileName", m_TrigSingleMuonFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/Muon/SingleMuonTrigger_2016.root" );
    DeclareProperty( m_name+"_TrigSingleMuonHistNumName", m_TrigSingleMuonHistNumName = "Mu50_OR_TkMu50_PtEtaBins/efficienciesDATA/pt_abseta_DATA" );
    DeclareProperty( m_name+"_TrigSingleMuonHistDenName", m_TrigSingleMuonHistDenName = "Mu50_OR_TkMu50_PtEtaBins/efficienciesMC/pt_abseta_MC" );
    DeclareProperty( m_name+"_TrigSingleIsoMuonHistNumName", m_TrigSingleIsoMuonHistNumName = "IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesDATA/pt_abseta_DATA" );
    DeclareProperty( m_name+"_TrigSingleIsoMuonHistDenName", m_TrigSingleIsoMuonHistDenName = "IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesMC/pt_abseta_MC" );
    
    // Muon Id+Iso+Trk
    DeclareProperty(  m_name+"_MuonIdFileName", m_MuonIdFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/Muon/MuonId_2016.root" );
    DeclareProperty(  m_name+"_MuonHighptIdHistNumName", m_MuonHighptIdHistNumName = "MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/efficienciesDATA/pair_ne_DATA" );
    DeclareProperty(  m_name+"_MuonHighptIdHistDenName", m_MuonHighptIdHistDenName = "MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/efficienciesMC/pair_ne_MC" );
    DeclareProperty(  m_name+"_MuonTightIdHistNumName", m_MuonTightIdHistNumName = "MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/efficienciesDATA/abseta_pt_DATA" );
    DeclareProperty(  m_name+"_MuonTightIdHistDenName", m_MuonTightIdHistDenName = "MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/efficienciesMC/abseta_pt_MC" );
    DeclareProperty(  m_name+"_MuonMediumIdHistNumName", m_MuonMediumIdHistNumName = "MC_NUM_MediumID_DEN_genTracks_PAR_pt_eta/efficienciesDATA/abseta_pt_DATA" );
    DeclareProperty(  m_name+"_MuonMediumIdHistDenName", m_MuonMediumIdHistDenName = "MC_NUM_MediumID_DEN_genTracks_PAR_pt_eta/efficienciesMC/abseta_pt_MC" );
    DeclareProperty(  m_name+"_MuonLooseIdHistNumName", m_MuonLooseIdHistNumName = "MC_NUM_LooseID_DEN_genTracks_PAR_pt_eta/efficienciesDATA/abseta_pt_DATA" );
    DeclareProperty(  m_name+"_MuonLooseIdHistDenName", m_MuonLooseIdHistDenName = "MC_NUM_LooseID_DEN_genTracks_PAR_pt_eta/efficienciesMC/abseta_pt_MC" );
    DeclareProperty(  m_name+"_MuonTrkIdFileName", m_MuonTrkIdFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/Muon/trackHighPtID_effSF_80X.root" );
    DeclareProperty(  m_name+"_MuonTrkIdHistNumName", m_MuonTrkIdHistNumName = "eff_trackHighPt_80Xdata_pteta" ); // Despite the name, it's etapt
    DeclareProperty(  m_name+"_MuonTrkIdHistDenName", m_MuonTrkIdHistDenName = "eff_trackHighPt_80Xmc_pteta" ); // Despite the name, it's etapt
    
    DeclareProperty(  m_name+"_MuonIsoFileName", m_MuonIsoFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/Muon/MuonIso_2016.root" );
    DeclareProperty(  m_name+"_MuonLoosePFIsoHistNumName", m_MuonLoosePFIsoHistNumName = "LooseISO_LooseID_pt_eta/efficienciesDATA/abseta_pt_DATA" );
    DeclareProperty(  m_name+"_MuonLoosePFIsoHistDenName", m_MuonLoosePFIsoHistDenName = "LooseISO_LooseID_pt_eta/efficienciesMC/abseta_pt_MC" );
    DeclareProperty(  m_name+"_MuonTightPFIsoHistNumName", m_MuonTightPFIsoHistNumName = "TightISO_TightID_pt_eta/efficienciesDATA/abseta_pt_DATA" );
    DeclareProperty(  m_name+"_MuonTightPFIsoHistDenName", m_MuonTightPFIsoHistDenName = "TightISO_TightID_pt_eta/efficienciesMC/abseta_pt_MC" );
    DeclareProperty(  m_name+"_MuonLooseTrkIsoHistNumName", m_MuonLooseTrkIsoHistNumName = "tkLooseISO_highptID_newpt_eta/efficienciesDATA/pair_ne_DATA" );
    DeclareProperty(  m_name+"_MuonLooseTrkIsoHistDenName", m_MuonLooseTrkIsoHistDenName = "tkLooseISO_highptID_newpt_eta/efficienciesMC/pair_ne_MC" );
    DeclareProperty(  m_name+"_MuonTightTrkIsoHistNumName", m_MuonTightTrkIsoHistNumName = "tkLooseISO_highptID_newpt_eta/efficienciesDATA/pair_ne_DATA" );
    DeclareProperty(  m_name+"_MuonTightTrkIsoHistDenName", m_MuonTightTrkIsoHistDenName = "tkLooseISO_highptID_newpt_eta/efficienciesMC/pair_ne_MC" );
    
    DeclareProperty(  m_name+"_MuonTrkFileName", m_MuonTrkFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/Muon/Tracking_EfficienciesAndSF_BCDEFGH.root" );
    DeclareProperty(  m_name+"_MuonTrkHistName", m_MuonTrkHistName = "ratio_eff_vtx_dr030e030_corr" ); // as a function of nPV
    
    // MET
    DeclareProperty( m_name+"_TrigMETFileName", m_TrigMETFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/MET/METTrigger_2016.root" );
    DeclareProperty( m_name+"_TrigMETHistNumName", m_TrigMETHistNumName = "HLT_PFMET_OR_vs_SingleElectron" );
    DeclareProperty( m_name+"_TrigMETHistDenName", m_TrigMETHistDenName = "HLT_PFMET_OR_vs_SingleElectron" );
    
    // Gen
    DeclareProperty(  m_name+"_EWKFileName", m_EWKFileName = std::string (std::getenv("SFRAME_DIR")) + "/../ScaleFactorTool/data/Gen/scalefactors_v4.root" );
    DeclareProperty(  m_name+"_WHistName", m_WHistName = "w_ewkcorr/w_ewkcorr_func" );
    DeclareProperty(  m_name+"_ZHistName", m_ZHistName = "z_ewkcorr/z_ewkcorr_func" );
    
}


void ScaleFactorTool::BeginInputData( const SInputData& ) throw( SError ) {
    
//    // Electron Trigger
//    m_logger << INFO << "TrigSingleEleFileName:\t" << m_TrigSingleEleFileName << SLogger::endmsg;
//    m_logger << INFO << "TrigSingleEleHistName:\t" << m_TrigSingleEleHistName << SLogger::endmsg;
//    m_logger << INFO << "TrigSingleIsoEleFileName:\t" << m_TrigSingleIsoEleFileName << SLogger::endmsg;
//    m_logger << INFO << "TrigSingleIsoEleHistName:\t" << m_TrigSingleIsoEleHistName << SLogger::endmsg;
//    
//    // Electron Id+Iso
//    m_logger << INFO << "EleIdTightWPFileName:\t" << m_EleIdTightWPFileName << SLogger::endmsg;
//    m_logger << INFO << "EleIdTightWPHistName:\t" << m_EleIdTightWPHistName << SLogger::endmsg;
//    m_logger << INFO << "EleIdMediumWPFileName:\t" << m_EleIdMediumWPFileName << SLogger::endmsg;
//    m_logger << INFO << "EleIdMediumWPHistName:\t" << m_EleIdMediumWPHistName << SLogger::endmsg;
//    m_logger << INFO << "EleIdLooseWPFileName:\t" << m_EleIdLooseWPFileName << SLogger::endmsg;
//    m_logger << INFO << "EleIdLooseWPHistName:\t" << m_EleIdLooseWPHistName << SLogger::endmsg;
//    m_logger << INFO << "EleIdVetoWPFileName:\t" << m_EleIdVetoWPFileName << SLogger::endmsg;
//    m_logger << INFO << "EleIdVetoWPHistName:\t" << m_EleIdVetoWPHistName << SLogger::endmsg;
//    m_logger << INFO << "EleRecoFileName:\t" << m_EleRecoFileName << SLogger::endmsg;
//    m_logger << INFO << "EleRecoHistName:\t" << m_EleRecoHistName << SLogger::endmsg;
//    
//    // Muon Trigger
//    m_logger << INFO << "TrigSingleMuonFileName:\t" << m_TrigSingleMuonFileName << SLogger::endmsg;
//    m_logger << INFO << "TrigSingleMuonHistName:\t" << m_TrigSingleMuonHistName << SLogger::endmsg;
//    m_logger << INFO << "TrigSingleIsoMuonHistName:\t" << m_TrigSingleIsoMuonHistName << SLogger::endmsg;
//    
//    // Muon Id+Iso+Trk
//    m_logger << INFO << "MuonIdFileName:\t" << m_MuonIdFileName << SLogger::endmsg;
//    m_logger << INFO << "MuonHighptIdHistName:\t" << m_MuonHighptIdHistName << SLogger::endmsg;
//    m_logger << INFO << "MuonTightIdHistName:\t" << m_MuonTightIdHistName << SLogger::endmsg;
//    m_logger << INFO << "MuonMediumIdHistName:\t" << m_MuonMediumIdHistName << SLogger::endmsg;
//    m_logger << INFO << "MuonLooseIdHistName:\t" << m_MuonLooseIdHistName << SLogger::endmsg;
//    m_logger << INFO << "MuonTrkIdFileName:\t" << m_MuonTrkIdFileName << SLogger::endmsg;
//    m_logger << INFO << "MuonTrkIdHistName:\t" << m_MuonTrkIdHistName << SLogger::endmsg;
//    
//    m_logger << INFO << "MuonIsoFileName:\t" << m_MuonIsoFileName << SLogger::endmsg;
//    m_logger << INFO << "MuonLoosePFIsoHistName:\t" << m_MuonLoosePFIsoHistName << SLogger::endmsg;
//    m_logger << INFO << "MuonTightPFIsoHistName:\t" << m_MuonTightPFIsoHistName << SLogger::endmsg;
//    m_logger << INFO << "MuonLooseTrkIsoHistName:\t" << m_MuonLooseTrkIsoHistName << SLogger::endmsg;
//    m_logger << INFO << "MuonTightTrkIsoHistName:\t" << m_MuonTightTrkIsoHistName << SLogger::endmsg;
//    
//    m_logger << INFO << "MuonTrkFileName:\t" << m_MuonTrkFileName << SLogger::endmsg;
//    m_logger << INFO << "MuonTrkHistName:\t" << m_MuonTrkHistName << SLogger::endmsg;
//    
//    // MET
//    m_logger << INFO << "TrigMETFileName:\t" << m_TrigMETFileName << SLogger::endmsg;
//    m_logger << INFO << "TrigMETHistName:\t" << m_TrigMETHistName << SLogger::endmsg;

//    // Gen
//    m_logger << INFO << "EWKFileName:\t" << m_EWKFileName << SLogger::endmsg;
//    m_logger << INFO << "WHistName:\t" << m_WHistName << SLogger::endmsg;
//    m_logger << INFO << "ZHistName:\t" << m_ZHistName << SLogger::endmsg;
    
    // ********** Open files **********
    
    
    // Electron trigger
    auto TrigSingleEleFile = TFile::Open(m_TrigSingleEleFileName.c_str(), "READ");
    if(TrigSingleEleFile) {
        TrigSingleEleNum = (TH2D*)TrigSingleEleFile->Get(m_TrigSingleEleHistNumName.c_str()); 
        if(TrigSingleEleNum) TrigSingleEleNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_TrigSingleEleHistNumName << " does not exist. Please check." << SLogger::endmsg;
        TrigSingleEleDen = (TH2D*)TrigSingleEleFile->Get(m_TrigSingleEleHistDenName.c_str()); 
        if(TrigSingleEleDen) TrigSingleEleDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_TrigSingleEleHistDenName << " does not exist. Please check." << SLogger::endmsg;
        TrigSingleEleFile->Close();
        TrigSingleEle = (TH2D*)TrigSingleEleNum->Clone((m_TrigSingleEleHistNumName+"_SF").c_str());
        TrigSingleEle->Divide(TrigSingleEleDen);
    }
    else m_logger << WARNING << "‎File " << m_TrigSingleEleFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "TrigSingleEleFile opened" << SLogger::endmsg;
    
    auto TrigSingleIsoEleFile = TFile::Open(m_TrigSingleIsoEleFileName.c_str(), "READ");
    if(TrigSingleIsoEleFile) {
        TrigSingleIsoEleNum = (TH2D*)TrigSingleIsoEleFile->Get(m_TrigSingleIsoEleHistNumName.c_str()); 
        if(TrigSingleIsoEleNum) TrigSingleIsoEleNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_TrigSingleIsoEleHistNumName << " does not exist. Please check." << SLogger::endmsg;
        TrigSingleIsoEleDen = (TH2D*)TrigSingleIsoEleFile->Get(m_TrigSingleIsoEleHistDenName.c_str()); 
        if(TrigSingleIsoEleDen) TrigSingleIsoEleDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_TrigSingleIsoEleHistDenName << " does not exist. Please check." << SLogger::endmsg;
        TrigSingleIsoEleFile->Close();
        TrigSingleIsoEle = (TH2D*)TrigSingleIsoEleNum->Clone((m_TrigSingleIsoEleHistNumName+"_SF").c_str());
        TrigSingleIsoEle->Divide(TrigSingleIsoEleDen);
    }
    else m_logger << WARNING << "‎File " << m_TrigSingleIsoEleFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "TrigSingleIsoEleFile opened" << SLogger::endmsg;
    
    // Electrons
    auto EleIdTightWPFile = TFile::Open(m_EleIdTightWPFileName.c_str(), "READ");
    if(EleIdTightWPFile) {
        EleIdTightWPNum = (TH2D*)EleIdTightWPFile->Get(m_EleIdTightWPHistNumName.c_str()); 
        if(EleIdTightWPNum) EleIdTightWPNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_EleIdTightWPHistNumName << " does not exist. Please check." << SLogger::endmsg;
        EleIdTightWPDen = (TH2D*)EleIdTightWPFile->Get(m_EleIdTightWPHistDenName.c_str()); 
        if(EleIdTightWPDen) EleIdTightWPDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_EleIdTightWPHistDenName << " does not exist. Please check." << SLogger::endmsg;
        EleIdTightWPFile->Close();
        EleIdTightWP = (TH2D*)EleIdTightWPNum->Clone((m_EleIdTightWPHistNumName+"_SF").c_str());
        EleIdTightWP->Divide(EleIdTightWPDen);
    }
    else m_logger << WARNING << "‎File " << m_EleIdTightWPFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "EleIdTightWPFile opened" << SLogger::endmsg;
    
    auto EleIdMediumWPFile = TFile::Open(m_EleIdMediumWPFileName.c_str(), "READ");
    if(EleIdMediumWPFile) {
        EleIdMediumWPNum = (TH2D*)EleIdMediumWPFile->Get(m_EleIdMediumWPHistNumName.c_str()); 
        if(EleIdMediumWPNum) EleIdMediumWPNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_EleIdMediumWPHistNumName << " does not exist. Please check." << SLogger::endmsg;
        EleIdMediumWPDen = (TH2D*)EleIdMediumWPFile->Get(m_EleIdMediumWPHistDenName.c_str()); 
        if(EleIdMediumWPDen) EleIdMediumWPDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_EleIdMediumWPHistDenName << " does not exist. Please check." << SLogger::endmsg;
        EleIdMediumWPFile->Close();
        EleIdMediumWP = (TH2D*)EleIdMediumWPNum->Clone((m_EleIdMediumWPHistNumName+"_SF").c_str());
        EleIdMediumWP->Divide(EleIdMediumWPDen);
    }
    else m_logger << WARNING << "‎File " << m_EleIdMediumWPFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "EleIdMediumWPFile opened" << SLogger::endmsg;
    
    auto EleIdLooseWPFile = TFile::Open(m_EleIdLooseWPFileName.c_str(), "READ");
    if(EleIdLooseWPFile) {
        EleIdLooseWPNum = (TH2D*)EleIdLooseWPFile->Get(m_EleIdLooseWPHistNumName.c_str()); 
        if(EleIdLooseWPNum) EleIdLooseWPNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_EleIdLooseWPHistNumName << " does not exist. Please check." << SLogger::endmsg;
        EleIdLooseWPDen = (TH2D*)EleIdLooseWPFile->Get(m_EleIdLooseWPHistDenName.c_str()); 
        if(EleIdLooseWPDen) EleIdLooseWPDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_EleIdLooseWPHistDenName << " does not exist. Please check." << SLogger::endmsg;
        EleIdLooseWPFile->Close();
        EleIdLooseWP = (TH2D*)EleIdLooseWPNum->Clone((m_EleIdLooseWPHistNumName+"_SF").c_str());
        EleIdLooseWP->Divide(EleIdLooseWPDen);
    }
    else m_logger << WARNING << "‎File " << m_EleIdLooseWPFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "EleIdLooseWPFile opened" << SLogger::endmsg;
    
    auto EleIdVetoWPFile = TFile::Open(m_EleIdVetoWPFileName.c_str(), "READ");
    if(EleIdVetoWPFile) {
        EleIdVetoWPNum = (TH2D*)EleIdVetoWPFile->Get(m_EleIdVetoWPHistNumName.c_str()); 
        if(EleIdVetoWPNum) EleIdVetoWPNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_EleIdVetoWPHistNumName << " does not exist. Please check." << SLogger::endmsg;
        EleIdVetoWPDen = (TH2D*)EleIdVetoWPFile->Get(m_EleIdVetoWPHistDenName.c_str()); 
        if(EleIdVetoWPDen) EleIdVetoWPDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_EleIdVetoWPHistDenName << " does not exist. Please check." << SLogger::endmsg;
        EleIdVetoWPFile->Close();
        EleIdVetoWP = (TH2D*)EleIdVetoWPNum->Clone((m_EleIdVetoWPHistNumName+"_SF").c_str());
        EleIdVetoWP->Divide(EleIdVetoWPDen);
    }
    else m_logger << WARNING << "‎File " << m_EleIdVetoWPFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "EleIdVetoWPFile opened" << SLogger::endmsg;
    
    auto EleRecoFile = TFile::Open(m_EleRecoFileName.c_str(), "READ");
    if(EleRecoFile) {
        EleRecoNum = (TH2D*)EleRecoFile->Get(m_EleRecoHistNumName.c_str()); 
        if(EleRecoNum) EleRecoNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_EleRecoHistNumName << " does not exist. Please check." << SLogger::endmsg;
        EleRecoDen = (TH2D*)EleRecoFile->Get(m_EleRecoHistDenName.c_str()); 
        if(EleRecoDen) EleRecoDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_EleRecoHistDenName << " does not exist. Please check." << SLogger::endmsg;
        EleRecoFile->Close();
        EleReco = (TH2D*)EleRecoNum->Clone((m_EleRecoHistNumName+"_SF").c_str());
        EleReco->Divide(EleRecoDen);
    }
    else m_logger << WARNING << "‎File " << m_EleRecoFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "EleRecoFile opened" << SLogger::endmsg;
    
    
    // Muon trigger
    auto TrigSingleMuonFile = TFile::Open(m_TrigSingleMuonFileName.c_str(), "READ");
    if(TrigSingleMuonFile) {
        TrigSingleMuonNum = (TH2F*)TrigSingleMuonFile->Get(m_TrigSingleMuonHistNumName.c_str());
        if(TrigSingleMuonNum) TrigSingleMuonNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_TrigSingleMuonHistNumName << " does not exist. Please check." << SLogger::endmsg;
        TrigSingleMuonDen = (TH2F*)TrigSingleMuonFile->Get(m_TrigSingleMuonHistDenName.c_str());
        if(TrigSingleMuonDen) TrigSingleMuonDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_TrigSingleMuonHistDenName << " does not exist. Please check." << SLogger::endmsg;
        
        TrigSingleIsoMuonNum = (TH2F*)TrigSingleMuonFile->Get(m_TrigSingleIsoMuonHistNumName.c_str());
        if(TrigSingleIsoMuonNum) TrigSingleIsoMuonNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_TrigSingleIsoMuonHistNumName << " does not exist. Please check." << SLogger::endmsg;
        TrigSingleIsoMuonDen = (TH2F*)TrigSingleMuonFile->Get(m_TrigSingleIsoMuonHistDenName.c_str());
        if(TrigSingleIsoMuonDen) TrigSingleIsoMuonDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_TrigSingleIsoMuonHistDenName << " does not exist. Please check." << SLogger::endmsg;
        TrigSingleMuonFile->Close();
        
        TrigSingleMuon = (TH2F*)TrigSingleMuonNum->Clone((m_TrigSingleMuonHistNumName+"_SF").c_str());
        TrigSingleMuon->Divide(TrigSingleMuonDen);
        TrigSingleIsoMuon = (TH2F*)TrigSingleIsoMuonNum->Clone((m_TrigSingleIsoMuonHistNumName+"_SF").c_str());
        TrigSingleIsoMuon->Divide(TrigSingleIsoMuonDen);
    }
    else m_logger << WARNING << "‎File " << m_TrigSingleMuonFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "TrigSingleMuonFile opened" << SLogger::endmsg;
    
    // Muon Id
    auto MuonIdFile = TFile::Open(m_MuonIdFileName.c_str(), "READ");
    if(MuonIdFile) {
        MuonHighptIdNum = (TH2F*)MuonIdFile->Get(m_MuonHighptIdHistNumName.c_str());
        if(MuonHighptIdNum) MuonHighptIdNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonHighptIdHistNumName << " does not exist. Please check." << SLogger::endmsg;
        MuonHighptIdDen = (TH2F*)MuonIdFile->Get(m_MuonHighptIdHistDenName.c_str());
        if(MuonHighptIdDen) MuonHighptIdDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonHighptIdHistDenName << " does not exist. Please check." << SLogger::endmsg;
        
        MuonTightIdNum = (TH2F*)MuonIdFile->Get(m_MuonTightIdHistNumName.c_str());
        if(MuonTightIdNum) MuonTightIdNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonTightIdHistNumName << " does not exist. Please check." << SLogger::endmsg;
        MuonTightIdDen = (TH2F*)MuonIdFile->Get(m_MuonTightIdHistDenName.c_str());
        if(MuonTightIdDen) MuonTightIdDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonTightIdHistDenName << " does not exist. Please check." << SLogger::endmsg;
        
        MuonMediumIdNum = (TH2F*)MuonIdFile->Get(m_MuonMediumIdHistNumName.c_str());
        if(MuonMediumIdNum) MuonMediumIdNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonMediumIdHistNumName << " does not exist. Please check." << SLogger::endmsg;
        MuonMediumIdDen = (TH2F*)MuonIdFile->Get(m_MuonMediumIdHistDenName.c_str());
        if(MuonMediumIdDen) MuonMediumIdDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonMediumIdHistDenName << " does not exist. Please check." << SLogger::endmsg;
        
        
        MuonLooseIdNum = (TH2F*)MuonIdFile->Get(m_MuonLooseIdHistNumName.c_str());
        if(MuonLooseIdNum) MuonLooseIdNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonLooseIdHistNumName << " does not exist. Please check." << SLogger::endmsg;
        MuonLooseIdDen = (TH2F*)MuonIdFile->Get(m_MuonLooseIdHistDenName.c_str());
        if(MuonLooseIdDen) MuonLooseIdDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonLooseIdHistDenName << " does not exist. Please check." << SLogger::endmsg;
        
        MuonIdFile->Close();
        
        MuonHighptId = (TH2F*)MuonHighptIdNum->Clone((m_MuonHighptIdHistNumName+"_SF").c_str());
        MuonHighptId->Divide(MuonHighptIdDen);
        MuonTightId = (TH2F*)MuonTightIdNum->Clone((m_MuonTightIdHistNumName+"_SF").c_str());
        MuonTightId->Divide(MuonTightIdDen);
        MuonMediumId = (TH2F*)MuonMediumIdNum->Clone((m_MuonMediumIdHistNumName+"_SF").c_str());
        MuonMediumId->Divide(MuonMediumIdDen);
        MuonLooseId = (TH2F*)MuonLooseIdNum->Clone((m_MuonLooseIdHistNumName+"_SF").c_str());
        MuonLooseId->Divide(MuonLooseIdDen);
    }
    else m_logger << WARNING << "‎File " << m_MuonIdFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "MuonIdFile opened" << SLogger::endmsg;
    
    // Muon Trk High Pt Id
    auto MuonTrkIdFile = TFile::Open(m_MuonTrkIdFileName.c_str(), "READ");
    if(MuonTrkIdFile) {
        MuonTrkIdNum = (TH2F*)MuonTrkIdFile->Get(m_MuonTrkIdHistNumName.c_str());
        if(MuonTrkIdNum) MuonTrkIdNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonTrkIdHistNumName << " does not exist. Please check." << SLogger::endmsg;
        MuonTrkIdDen = (TH2F*)MuonTrkIdFile->Get(m_MuonTrkIdHistDenName.c_str());
        if(MuonTrkIdDen) MuonTrkIdDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonTrkIdHistDenName << " does not exist. Please check." << SLogger::endmsg;
        MuonTrkIdFile->Close();
        MuonTrkId = (TH2F*)MuonTrkIdNum->Clone((m_MuonTrkIdHistNumName+"_SF").c_str());
        MuonTrkId->Divide(MuonTrkIdDen);
    }
    else m_logger << WARNING << "‎File " << m_MuonTrkIdFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "MuonTrkIdFile opened" << SLogger::endmsg;
    
    // Muon Iso
    auto MuonIsoFile = TFile::Open(m_MuonIsoFileName.c_str(), "READ");
    if(MuonIsoFile) {
        MuonLoosePFIsoNum = (TH2F*)MuonIsoFile->Get(m_MuonLoosePFIsoHistNumName.c_str());
        if(MuonLoosePFIsoNum) MuonLoosePFIsoNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonLoosePFIsoHistNumName << " does not exist. Please check." << SLogger::endmsg;
        MuonLoosePFIsoDen = (TH2F*)MuonIsoFile->Get(m_MuonLoosePFIsoHistDenName.c_str());
        if(MuonLoosePFIsoDen) MuonLoosePFIsoDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonLoosePFIsoHistDenName << " does not exist. Please check." << SLogger::endmsg;
        
        MuonTightPFIsoNum = (TH2F*)MuonIsoFile->Get(m_MuonTightPFIsoHistNumName.c_str());
        if(MuonTightPFIsoNum) MuonTightPFIsoNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonTightPFIsoHistNumName << " does not exist. Please check." << SLogger::endmsg;
        MuonTightPFIsoDen = (TH2F*)MuonIsoFile->Get(m_MuonTightPFIsoHistDenName.c_str());
        if(MuonTightPFIsoDen) MuonTightPFIsoDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonTightPFIsoHistDenName << " does not exist. Please check." << SLogger::endmsg;
        
        MuonLooseTrkIsoNum = (TH2F*)MuonIsoFile->Get(m_MuonLooseTrkIsoHistNumName.c_str());
        if(MuonLooseTrkIsoNum) MuonLooseTrkIsoNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonLooseTrkIsoHistNumName << " does not exist. Please check." << SLogger::endmsg;
        MuonLooseTrkIsoDen = (TH2F*)MuonIsoFile->Get(m_MuonLooseTrkIsoHistDenName.c_str());
        if(MuonLooseTrkIsoDen) MuonLooseTrkIsoDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonLooseTrkIsoHistDenName << " does not exist. Please check." << SLogger::endmsg;
        
        MuonTightTrkIsoNum = (TH2F*)MuonIsoFile->Get(m_MuonTightTrkIsoHistNumName.c_str());
        if(MuonTightTrkIsoNum) MuonTightTrkIsoNum->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonTightTrkIsoHistNumName << " does not exist. Please check." << SLogger::endmsg;
        MuonTightTrkIsoDen = (TH2F*)MuonIsoFile->Get(m_MuonTightTrkIsoHistDenName.c_str());
        if(MuonTightTrkIsoDen) MuonTightTrkIsoDen->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MuonTightTrkIsoHistDenName << " does not exist. Please check." << SLogger::endmsg;
        
        MuonIsoFile->Close();
        
        MuonLoosePFIso = (TH2F*)MuonLoosePFIsoNum->Clone((m_MuonLoosePFIsoHistNumName+"_SF").c_str());
        MuonLoosePFIso->Divide(MuonLoosePFIsoDen);
        MuonTightPFIso = (TH2F*)MuonTightPFIsoNum->Clone((m_MuonTightPFIsoHistNumName+"_SF").c_str());
        MuonTightPFIso->Divide(MuonTightPFIsoDen);
        MuonLooseTrkIso = (TH2F*)MuonLooseTrkIsoNum->Clone((m_MuonLooseTrkIsoHistNumName+"_SF").c_str());
        MuonLooseTrkIso->Divide(MuonLooseTrkIsoDen);
        MuonTightTrkIso = (TH2F*)MuonTightTrkIsoNum->Clone((m_MuonTightTrkIsoHistNumName+"_SF").c_str());
        MuonTightTrkIso->Divide(MuonTightTrkIsoDen);
    }
    else m_logger << WARNING << "‎File " << m_MuonIsoFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "MuonIsoFile opened" << SLogger::endmsg;
    
    // Muon Tracking SF
    auto MuonTrkFile = TFile::Open(m_MuonTrkFileName.c_str(), "READ");
    if(MuonTrkFile) {
        MuonTrk = (TGraphAsymmErrors*)MuonTrkFile->Get(m_MuonTrkHistName.c_str());
        //if(MuonTrk) MuonTrk->SetDirectory(0);
        //else m_logger << WARNING << "‎Hist " << m_MuonTrkHistName << " does not exist. Please check." << SLogger::endmsg;
        MuonTrkFile->Close();
    }
    else m_logger << WARNING << "‎File " << m_MuonTrkFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "MuonTrkFile opened" << SLogger::endmsg;
    
    
    // MET
    auto TrigMETFile = TFile::Open(m_TrigMETFileName.c_str(), "READ");
    if(TrigMETFile) {
        TrigMET = (TGraphAsymmErrors*)TrigMETFile->Get(m_TrigMETHistNumName.c_str());
        //if(TrigMET) TrigMET->SetDirectory(0);
        //else m_logger << WARNING << "‎Hist " << m_TrigMETHistName << " does not exist. Please check." << SLogger::endmsg;
        TrigMETFile->Close();
    }
    else m_logger << WARNING << "‎File " << m_TrigMETFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "TrigMETFile opened" << SLogger::endmsg;
    
    
    // Gen
    auto EWKFile = TFile::Open(m_EWKFileName.c_str(), "READ");
    if(EWKFile) {
        W = (TF1*)EWKFile->Get(m_WHistName.c_str());
        Z = (TF1*)EWKFile->Get(m_ZHistName.c_str());
  //  W->SetDirectory(0);
  //  Z->SetDirectory(0);
        EWKFile->Close();
    }
    else m_logger << WARNING << "‎File " << m_EWKFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "EWKFile opened" << SLogger::endmsg;
    
    return;
}



ScaleFactorTool::~ScaleFactorTool() {

}

// Electron trigger

double ScaleFactorTool::GetTrigSingleEle(double pt, double eta, int sigma) {
    if(!TrigSingleEle) return -1.;
    if(eta > TrigSingleEle->GetXaxis()->GetXmax()) eta = TrigSingleEle->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < TrigSingleEle->GetXaxis()->GetXmin()) eta = TrigSingleEle->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > TrigSingleEle->GetYaxis()->GetXmax()) pt = TrigSingleEle->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < TrigSingleEle->GetYaxis()->GetXmin()) pt = TrigSingleEle->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = TrigSingleEle->FindBin(eta, pt);
    if(sigma == +1) return TrigSingleEle->Interpolate(eta, pt) + TrigSingleEle->GetBinError(bin);
    if(sigma == -1) return TrigSingleEle->Interpolate(eta, pt) - TrigSingleEle->GetBinError(bin);
    return TrigSingleEle->Interpolate(eta, pt);
}

double ScaleFactorTool::GetTrigSingleEleEffData(double pt, double eta, int sigma) {
    if(!TrigSingleEleNum) return -1.;
    if(eta > TrigSingleEleNum->GetXaxis()->GetXmax()) eta = TrigSingleEleNum->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < TrigSingleEleNum->GetXaxis()->GetXmin()) eta = TrigSingleEleNum->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > TrigSingleEleNum->GetYaxis()->GetXmax()) pt = TrigSingleEleNum->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < TrigSingleEleNum->GetYaxis()->GetXmin()) pt = TrigSingleEleNum->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = TrigSingleEleNum->FindBin(eta, pt);
    if(sigma == +1) return TrigSingleEleNum->Interpolate(eta, pt) + TrigSingleEleNum->GetBinError(bin);
    if(sigma == -1) return TrigSingleEleNum->Interpolate(eta, pt) - TrigSingleEleNum->GetBinError(bin);
    return TrigSingleEleNum->Interpolate(eta, pt);
}

double ScaleFactorTool::GetTrigSingleEleEffMC(double pt, double eta, int sigma) {
    if(!TrigSingleEleDen) return -1.;
    if(eta > TrigSingleEleDen->GetXaxis()->GetXmax()) eta = TrigSingleEleDen->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < TrigSingleEleDen->GetXaxis()->GetXmin()) eta = TrigSingleEleDen->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > TrigSingleEleDen->GetYaxis()->GetXmax()) pt = TrigSingleEleDen->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < TrigSingleEleDen->GetYaxis()->GetXmin()) pt = TrigSingleEleDen->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = TrigSingleEleDen->FindBin(eta, pt);
    if(sigma == +1) return TrigSingleEleDen->Interpolate(eta, pt) + TrigSingleEleDen->GetBinError(bin);
    if(sigma == -1) return TrigSingleEleDen->Interpolate(eta, pt) - TrigSingleEleDen->GetBinError(bin);
    return TrigSingleEleDen->Interpolate(eta, pt);
}


double ScaleFactorTool::GetTrigSingleIsoEle(double pt, double eta, int sigma) {
    if(!TrigSingleIsoEle) return -1.;
    if(eta > TrigSingleIsoEle->GetXaxis()->GetXmax()) eta = TrigSingleIsoEle->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < TrigSingleIsoEle->GetXaxis()->GetXmin()) eta = TrigSingleIsoEle->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > TrigSingleIsoEle->GetYaxis()->GetXmax()) pt = TrigSingleIsoEle->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < TrigSingleIsoEle->GetYaxis()->GetXmin()) pt = TrigSingleIsoEle->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = TrigSingleIsoEle->FindBin(eta, pt);
    if(sigma == +1) return TrigSingleIsoEle->Interpolate(eta, pt) + TrigSingleIsoEle->GetBinError(bin);
    if(sigma == -1) return TrigSingleIsoEle->Interpolate(eta, pt) - TrigSingleIsoEle->GetBinError(bin);
    return TrigSingleIsoEle->Interpolate(eta, pt);
}

double ScaleFactorTool::GetTrigSingleIsoEleEffData(double pt, double eta, int sigma) {
    if(!TrigSingleIsoEleNum) return -1.;
    if(eta > TrigSingleIsoEleNum->GetXaxis()->GetXmax()) eta = TrigSingleIsoEleNum->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < TrigSingleIsoEleNum->GetXaxis()->GetXmin()) eta = TrigSingleIsoEleNum->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > TrigSingleIsoEleNum->GetYaxis()->GetXmax()) pt = TrigSingleIsoEleNum->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < TrigSingleIsoEleNum->GetYaxis()->GetXmin()) pt = TrigSingleIsoEleNum->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = TrigSingleIsoEleNum->FindBin(eta, pt);
    if(sigma == +1) return TrigSingleIsoEleNum->Interpolate(eta, pt) + TrigSingleIsoEleNum->GetBinError(bin);
    if(sigma == -1) return TrigSingleIsoEleNum->Interpolate(eta, pt) - TrigSingleIsoEleNum->GetBinError(bin);
    return TrigSingleIsoEleNum->Interpolate(eta, pt);
}

double ScaleFactorTool::GetTrigSingleIsoEleEffMC(double pt, double eta, int sigma) {
    if(!TrigSingleIsoEleDen) return -1.;
    if(eta > TrigSingleIsoEleDen->GetXaxis()->GetXmax()) eta = TrigSingleIsoEleDen->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < TrigSingleIsoEleDen->GetXaxis()->GetXmin()) eta = TrigSingleIsoEleDen->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > TrigSingleIsoEleDen->GetYaxis()->GetXmax()) pt = TrigSingleIsoEleDen->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < TrigSingleIsoEleDen->GetYaxis()->GetXmin()) pt = TrigSingleIsoEleDen->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = TrigSingleIsoEleDen->FindBin(eta, pt);
    if(sigma == +1) return TrigSingleIsoEleDen->Interpolate(eta, pt) + TrigSingleIsoEleDen->GetBinError(bin);
    if(sigma == -1) return TrigSingleIsoEleDen->Interpolate(eta, pt) - TrigSingleIsoEleDen->GetBinError(bin);
    return TrigSingleIsoEleDen->Interpolate(eta, pt);
}


// Electron Id+Iso+Trk

double ScaleFactorTool::GetEleIdTightWP(double pt, double eta, int sigma) {
    if(!EleIdTightWP) return -1.;
    if(eta > EleIdTightWP->GetXaxis()->GetXmax()) eta = EleIdTightWP->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < EleIdTightWP->GetXaxis()->GetXmin()) eta = EleIdTightWP->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > EleIdTightWP->GetYaxis()->GetXmax()) pt = EleIdTightWP->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < EleIdTightWP->GetYaxis()->GetXmin()) pt = EleIdTightWP->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = EleIdTightWP->FindBin(eta, pt);
    if(sigma == +1) return EleIdTightWP->Interpolate(eta, pt) + EleIdTightWP->GetBinError(bin);
    if(sigma == -1) return EleIdTightWP->Interpolate(eta, pt) - EleIdTightWP->GetBinError(bin);
    return EleIdTightWP->Interpolate(eta, pt);
}

double ScaleFactorTool::GetEleIdMediumWP(double pt, double eta, int sigma) {
    if(!EleIdMediumWP) return -1.;
    if(eta > EleIdMediumWP->GetXaxis()->GetXmax()) eta = EleIdMediumWP->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < EleIdMediumWP->GetXaxis()->GetXmin()) eta = EleIdMediumWP->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > EleIdMediumWP->GetYaxis()->GetXmax()) pt = EleIdMediumWP->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < EleIdMediumWP->GetYaxis()->GetXmin()) pt = EleIdMediumWP->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = EleIdMediumWP->FindBin(eta, pt);
    if(sigma == +1) return EleIdMediumWP->Interpolate(eta, pt) + EleIdMediumWP->GetBinError(bin);
    if(sigma == -1) return EleIdMediumWP->Interpolate(eta, pt) - EleIdMediumWP->GetBinError(bin);
    return EleIdMediumWP->Interpolate(eta, pt);
}

double ScaleFactorTool::GetEleIdLooseWP(double pt, double eta, int sigma) {
    if(!EleIdLooseWP) return -1.;
    if(eta > EleIdLooseWP->GetXaxis()->GetXmax()) eta = EleIdLooseWP->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < EleIdLooseWP->GetXaxis()->GetXmin()) eta = EleIdLooseWP->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > EleIdLooseWP->GetYaxis()->GetXmax()) pt = EleIdLooseWP->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < EleIdLooseWP->GetYaxis()->GetXmin()) pt = EleIdLooseWP->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = EleIdLooseWP->FindBin(eta, pt);
    if(sigma == +1) return EleIdLooseWP->Interpolate(eta, pt) + EleIdLooseWP->GetBinError(bin);
    if(sigma == -1) return EleIdLooseWP->Interpolate(eta, pt) - EleIdLooseWP->GetBinError(bin);
    return EleIdLooseWP->Interpolate(eta, pt);
}

double ScaleFactorTool::GetEleIdVetoWP(double pt, double eta, int sigma) {
    if(!EleIdVetoWP) return -1.;
    if(eta > EleIdVetoWP->GetXaxis()->GetXmax()) eta = EleIdVetoWP->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < EleIdVetoWP->GetXaxis()->GetXmin()) eta = EleIdVetoWP->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > EleIdVetoWP->GetYaxis()->GetXmax()) pt = EleIdVetoWP->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < EleIdVetoWP->GetYaxis()->GetXmin()) pt = EleIdVetoWP->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = EleIdVetoWP->FindBin(eta, pt);
    if(sigma == +1) return EleIdVetoWP->Interpolate(eta, pt) + EleIdVetoWP->GetBinError(bin);
    if(sigma == -1) return EleIdVetoWP->Interpolate(eta, pt) - EleIdVetoWP->GetBinError(bin);
    return EleIdVetoWP->Interpolate(eta, pt);
}

double ScaleFactorTool::GetEleReco(double pt, double eta, int sigma) {
    if(!EleReco) return -1.;
    if(eta > EleReco->GetXaxis()->GetXmax()) eta = EleReco->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < EleReco->GetXaxis()->GetXmin()) eta = EleReco->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > EleReco->GetYaxis()->GetXmax()) pt = EleReco->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < EleReco->GetYaxis()->GetXmin()) pt = EleReco->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = EleReco->FindBin(eta, pt);
    if(sigma == +1) return EleReco->Interpolate(eta, pt) + EleReco->GetBinError(bin);
    if(sigma == -1) return EleReco->Interpolate(eta, pt) - EleReco->GetBinError(bin);
    return EleReco->Interpolate(eta, pt);
}



// Muon trigger

double ScaleFactorTool::GetTrigSingleMuon(double pt, double eta, int sigma) {
    if(!TrigSingleMuon) return -1.;
    eta = fabs(eta);
    if(eta > TrigSingleMuon->GetYaxis()->GetXmax()) eta = TrigSingleMuon->GetYaxis()->GetXmax() - 1.e-2;
    if(eta < TrigSingleMuon->GetYaxis()->GetXmin()) eta = TrigSingleMuon->GetYaxis()->GetXmin() + 1.e-2;
    if(pt > TrigSingleMuon->GetXaxis()->GetXmax()) pt = TrigSingleMuon->GetXaxis()->GetXmax() - 1.e-2;
    if(pt < TrigSingleMuon->GetXaxis()->GetXmin()) pt = TrigSingleMuon->GetXaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = TrigSingleMuon->FindBin(pt, eta);
    if(sigma == +1) return TrigSingleMuon->Interpolate(pt, eta) + TrigSingleMuon->GetBinError(bin);
    if(sigma == -1) return TrigSingleMuon->Interpolate(pt, eta) - TrigSingleMuon->GetBinError(bin);
    return TrigSingleMuon->Interpolate(pt, eta);
}

double ScaleFactorTool::GetTrigSingleMuonEffData(double pt, double eta, int sigma) {
    if(!TrigSingleMuonNum) return -1.;
    eta = fabs(eta);
    if(eta > TrigSingleMuonNum->GetYaxis()->GetXmax()) eta = TrigSingleMuonNum->GetYaxis()->GetXmax() - 1.e-2;
    if(eta < TrigSingleMuonNum->GetYaxis()->GetXmin()) eta = TrigSingleMuonNum->GetYaxis()->GetXmin() + 1.e-2;
    if(pt > TrigSingleMuonNum->GetXaxis()->GetXmax()) pt = TrigSingleMuonNum->GetXaxis()->GetXmax() - 1.e-2;
    if(pt < TrigSingleMuonNum->GetXaxis()->GetXmin()) pt = TrigSingleMuonNum->GetXaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = TrigSingleMuonNum->FindBin(pt, eta);
    if(sigma == +1) return TrigSingleMuonNum->Interpolate(pt, eta) + TrigSingleMuonNum->GetBinError(bin);
    if(sigma == -1) return TrigSingleMuonNum->Interpolate(pt, eta) - TrigSingleMuonNum->GetBinError(bin);
    return TrigSingleMuonNum->Interpolate(pt, eta);
}

double ScaleFactorTool::GetTrigSingleMuonEffMC(double pt, double eta, int sigma) {
    if(!TrigSingleMuonDen) return -1.;
    eta = fabs(eta);
    if(eta > TrigSingleMuonDen->GetYaxis()->GetXmax()) eta = TrigSingleMuonDen->GetYaxis()->GetXmax() - 1.e-2;
    if(eta < TrigSingleMuonDen->GetYaxis()->GetXmin()) eta = TrigSingleMuonDen->GetYaxis()->GetXmin() + 1.e-2;
    if(pt > TrigSingleMuonDen->GetXaxis()->GetXmax()) pt = TrigSingleMuonDen->GetXaxis()->GetXmax() - 1.e-2;
    if(pt < TrigSingleMuonDen->GetXaxis()->GetXmin()) pt = TrigSingleMuonDen->GetXaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = TrigSingleMuonDen->FindBin(pt, eta);
    if(sigma == +1) return TrigSingleMuonDen->Interpolate(pt, eta) + TrigSingleMuonDen->GetBinError(bin);
    if(sigma == -1) return TrigSingleMuonDen->Interpolate(pt, eta) - TrigSingleMuonDen->GetBinError(bin);
    return TrigSingleMuonDen->Interpolate(pt, eta);
}

double ScaleFactorTool::GetTrigSingleIsoMuon(double pt, double eta, int sigma) {
    if(!TrigSingleIsoMuon) return -1.;
    eta = fabs(eta);
    if(eta > TrigSingleIsoMuon->GetYaxis()->GetXmax()) eta = TrigSingleIsoMuon->GetYaxis()->GetXmax() - 1.e-2;
    if(eta < TrigSingleIsoMuon->GetYaxis()->GetXmin()) eta = TrigSingleIsoMuon->GetYaxis()->GetXmin() + 1.e-2;
    if(pt > TrigSingleIsoMuon->GetXaxis()->GetXmax()) pt = TrigSingleIsoMuon->GetXaxis()->GetXmax() - 1.e-2;
    if(pt < TrigSingleIsoMuon->GetXaxis()->GetXmin()) pt = TrigSingleIsoMuon->GetXaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = TrigSingleIsoMuon->FindBin(pt, eta);
    if(sigma == +1) return TrigSingleIsoMuon->Interpolate(pt, eta) + TrigSingleIsoMuon->GetBinError(bin);
    if(sigma == -1) return TrigSingleIsoMuon->Interpolate(pt, eta) - TrigSingleIsoMuon->GetBinError(bin);
    return TrigSingleIsoMuon->Interpolate(pt, eta);
}

double ScaleFactorTool::GetTrigSingleIsoMuonEffData(double pt, double eta, int sigma) {
    if(!TrigSingleIsoMuonNum) return -1.;
    eta = fabs(eta);
    if(eta > TrigSingleIsoMuonNum->GetYaxis()->GetXmax()) eta = TrigSingleIsoMuonNum->GetYaxis()->GetXmax() - 1.e-2;
    if(eta < TrigSingleIsoMuonNum->GetYaxis()->GetXmin()) eta = TrigSingleIsoMuonNum->GetYaxis()->GetXmin() + 1.e-2;
    if(pt > TrigSingleIsoMuonNum->GetXaxis()->GetXmax()) pt = TrigSingleIsoMuonNum->GetXaxis()->GetXmax() - 1.e-2;
    if(pt < TrigSingleIsoMuonNum->GetXaxis()->GetXmin()) pt = TrigSingleIsoMuonNum->GetXaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = TrigSingleIsoMuonNum->FindBin(pt, eta);
    if(sigma == +1) return TrigSingleIsoMuonNum->Interpolate(pt, eta) + TrigSingleIsoMuonNum->GetBinError(bin);
    if(sigma == -1) return TrigSingleIsoMuonNum->Interpolate(pt, eta) - TrigSingleIsoMuonNum->GetBinError(bin);
    return TrigSingleIsoMuonNum->Interpolate(pt, eta);
}

double ScaleFactorTool::GetTrigSingleIsoMuonEffMC(double pt, double eta, int sigma) {
    if(!TrigSingleIsoMuonDen) return -1.;
    eta = fabs(eta);
    if(eta > TrigSingleIsoMuonDen->GetYaxis()->GetXmax()) eta = TrigSingleIsoMuonDen->GetYaxis()->GetXmax() - 1.e-2;
    if(eta < TrigSingleIsoMuonDen->GetYaxis()->GetXmin()) eta = TrigSingleIsoMuonDen->GetYaxis()->GetXmin() + 1.e-2;
    if(pt > TrigSingleIsoMuonDen->GetXaxis()->GetXmax()) pt = TrigSingleIsoMuonDen->GetXaxis()->GetXmax() - 1.e-2;
    if(pt < TrigSingleIsoMuonDen->GetXaxis()->GetXmin()) pt = TrigSingleIsoMuonDen->GetXaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = TrigSingleIsoMuonDen->FindBin(pt, eta);
    if(sigma == +1) return TrigSingleIsoMuonDen->Interpolate(pt, eta) + TrigSingleIsoMuonDen->GetBinError(bin);
    if(sigma == -1) return TrigSingleIsoMuonDen->Interpolate(pt, eta) - TrigSingleIsoMuonDen->GetBinError(bin);
    return TrigSingleIsoMuonDen->Interpolate(pt, eta);
}

// Muon Id

double ScaleFactorTool::GetMuonHighptId(double pt, double eta, int sigma) {
    if(!MuonHighptId) return -1.;
    eta = fabs(eta);
    if(eta > MuonHighptId->GetXaxis()->GetXmax()) eta = MuonHighptId->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < MuonHighptId->GetXaxis()->GetXmin()) eta = MuonHighptId->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > MuonHighptId->GetYaxis()->GetXmax()) pt = MuonHighptId->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < MuonHighptId->GetYaxis()->GetXmin()) pt = MuonHighptId->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = MuonHighptId->FindBin(eta, pt);
    if(sigma == +1) return MuonHighptId->Interpolate(eta, pt) + MuonHighptId->GetBinError(bin);
    if(sigma == -1) return MuonHighptId->Interpolate(eta, pt) - MuonHighptId->GetBinError(bin);
    return MuonHighptId->Interpolate(eta, pt);
}

double ScaleFactorTool::GetMuonTightId(double pt, double eta, int sigma) {
    if(!MuonTightId) return -1.;
    eta = fabs(eta);
    if(eta > MuonTightId->GetXaxis()->GetXmax()) eta = MuonTightId->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < MuonTightId->GetXaxis()->GetXmin()) eta = MuonTightId->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > MuonTightId->GetYaxis()->GetXmax()) pt = MuonTightId->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < MuonTightId->GetYaxis()->GetXmin()) pt = MuonTightId->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = MuonTightId->FindBin(eta, pt);
    if(sigma == +1) return MuonTightId->Interpolate(eta, pt) + MuonTightId->GetBinError(bin);
    if(sigma == -1) return MuonTightId->Interpolate(eta, pt) - MuonTightId->GetBinError(bin);
    return MuonTightId->Interpolate(eta, pt);
}

double ScaleFactorTool::GetMuonMediumId(double pt, double eta, int sigma) {
    if(!MuonMediumId) return -1.;
    eta = fabs(eta);
    if(eta > MuonMediumId->GetXaxis()->GetXmax()) eta = MuonMediumId->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < MuonMediumId->GetXaxis()->GetXmin()) eta = MuonMediumId->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > MuonMediumId->GetYaxis()->GetXmax()) pt = MuonMediumId->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < MuonMediumId->GetYaxis()->GetXmin()) pt = MuonMediumId->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = MuonMediumId->FindBin(eta, pt);
    if(sigma == +1) return MuonMediumId->Interpolate(eta, pt) + MuonMediumId->GetBinError(bin);
    if(sigma == -1) return MuonMediumId->Interpolate(eta, pt) - MuonMediumId->GetBinError(bin);
    return MuonMediumId->Interpolate(eta, pt);
}

double ScaleFactorTool::GetMuonLooseId(double pt, double eta, int sigma) {
    if(!MuonLooseId) return -1.;
    eta = fabs(eta);
    if(eta > MuonLooseId->GetXaxis()->GetXmax()) eta = MuonLooseId->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < MuonLooseId->GetXaxis()->GetXmin()) eta = MuonLooseId->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > MuonLooseId->GetYaxis()->GetXmax()) pt = MuonLooseId->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < MuonLooseId->GetYaxis()->GetXmin()) pt = MuonLooseId->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = MuonLooseId->FindBin(eta, pt);
    if(sigma == +1) return MuonLooseId->Interpolate(eta, pt) + MuonLooseId->GetBinError(bin);
    if(sigma == -1) return MuonLooseId->Interpolate(eta, pt) - MuonLooseId->GetBinError(bin);
    return MuonLooseId->Interpolate(eta, pt);
}

// Muon Iso

double ScaleFactorTool::GetMuonLoosePFIso(double pt, double eta, int sigma) {
    if(!MuonLoosePFIso) return -1.;
    eta = fabs(eta);
    if(eta > MuonLoosePFIso->GetXaxis()->GetXmax()) eta = MuonLoosePFIso->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < MuonLoosePFIso->GetXaxis()->GetXmin()) eta = MuonLoosePFIso->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > MuonLoosePFIso->GetYaxis()->GetXmax()) pt = MuonLoosePFIso->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < MuonLoosePFIso->GetYaxis()->GetXmin()) pt = MuonLoosePFIso->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = MuonLoosePFIso->FindBin(eta, pt);
    if(sigma == +1) return MuonLoosePFIso->Interpolate(eta, pt) + MuonLoosePFIso->GetBinError(bin);
    if(sigma == -1) return MuonLoosePFIso->Interpolate(eta, pt) - MuonLoosePFIso->GetBinError(bin);
    return MuonLoosePFIso->Interpolate(eta, pt);
}

double ScaleFactorTool::GetMuonTightPFIso(double pt, double eta, int sigma) {
    if(!MuonTightPFIso) return -1.;
    eta = fabs(eta);
    if(eta > MuonTightPFIso->GetXaxis()->GetXmax()) eta = MuonTightPFIso->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < MuonTightPFIso->GetXaxis()->GetXmin()) eta = MuonTightPFIso->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > MuonTightPFIso->GetYaxis()->GetXmax()) pt = MuonTightPFIso->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < MuonTightPFIso->GetYaxis()->GetXmin()) pt = MuonTightPFIso->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = MuonTightPFIso->FindBin(eta, pt);
    if(sigma == +1) return MuonTightPFIso->Interpolate(eta, pt) + MuonTightPFIso->GetBinError(bin);
    if(sigma == -1) return MuonTightPFIso->Interpolate(eta, pt) - MuonTightPFIso->GetBinError(bin);
    return MuonTightPFIso->Interpolate(eta, pt);
}

double ScaleFactorTool::GetMuonLooseTrkIso(double pt, double eta, int sigma) {
    if(!MuonLooseTrkIso) return -1.;
    eta = fabs(eta);
    if(eta > MuonLooseTrkIso->GetXaxis()->GetXmax()) eta = MuonLooseTrkIso->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < MuonLooseTrkIso->GetXaxis()->GetXmin()) eta = MuonLooseTrkIso->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > MuonLooseTrkIso->GetYaxis()->GetXmax()) pt = MuonLooseTrkIso->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < MuonLooseTrkIso->GetYaxis()->GetXmin()) pt = MuonLooseTrkIso->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = MuonLooseTrkIso->FindBin(eta, pt);
    if(sigma == +1) return MuonLooseTrkIso->Interpolate(eta, pt) + MuonLooseTrkIso->GetBinError(bin);
    if(sigma == -1) return MuonLooseTrkIso->Interpolate(eta, pt) - MuonLooseTrkIso->GetBinError(bin);
    return MuonLooseTrkIso->Interpolate(eta, pt);
}

double ScaleFactorTool::GetMuonTightTrkIso(double pt, double eta, int sigma) {
    if(!MuonTightTrkIso) return -1.;
    eta = fabs(eta);
    if(eta > MuonTightTrkIso->GetXaxis()->GetXmax()) eta = MuonTightTrkIso->GetXaxis()->GetXmax() - 1.e-2;
    if(eta < MuonTightTrkIso->GetXaxis()->GetXmin()) eta = MuonTightTrkIso->GetXaxis()->GetXmin() + 1.e-2;
    if(pt > MuonTightTrkIso->GetYaxis()->GetXmax()) pt = MuonTightTrkIso->GetYaxis()->GetXmax() - 1.e-2;
    if(pt < MuonTightTrkIso->GetYaxis()->GetXmin()) pt = MuonTightTrkIso->GetYaxis()->GetXmin() + 1.e-2;
    int bin(0);
    if(sigma != 0) bin = MuonTightTrkIso->FindBin(eta, pt);
    if(sigma == +1) return MuonTightTrkIso->Interpolate(eta, pt) + MuonTightTrkIso->GetBinError(bin);
    if(sigma == -1) return MuonTightTrkIso->Interpolate(eta, pt) - MuonTightTrkIso->GetBinError(bin);
    return MuonTightTrkIso->Interpolate(eta, pt);
}

// Muon Trk

double ScaleFactorTool::GetMuonTrkId(double pt, double eta, int sigma) {
    if(!MuonTrkId) return -1.;
    eta = fabs(eta);
    if(eta > MuonTrkId->GetXaxis()->GetXmax()) eta = MuonTrkId->GetXaxis()->GetXmax() - 1.e-1;
    if(eta < MuonTrkId->GetXaxis()->GetXmin()) eta = MuonTrkId->GetXaxis()->GetXmin() + 1.e-1;
    if(pt > MuonTrkId->GetYaxis()->GetXmax()) pt = MuonTrkId->GetYaxis()->GetXmax() - 1.e-1;
    if(pt < MuonTrkId->GetYaxis()->GetXmin()) pt = MuonTrkId->GetYaxis()->GetXmin() + 1.e-1;
    int bin(0);
    if(sigma != 0) bin = MuonTrkId->FindBin(eta, pt);
    if(sigma == +1) return MuonTrkId->Interpolate(eta, pt) + MuonTrkId->GetBinError(bin);
    if(sigma == -1) return MuonTrkId->Interpolate(eta, pt) - MuonTrkId->GetBinError(bin);
    return MuonTrkId->Interpolate(eta, pt);
}

// Muon Tracking SF

double ScaleFactorTool::GetMuonTrk(int nPV, int sigma) {
    if(!MuonTrk) return -1.;
    if(nPV < MuonTrk->GetX()[0]) nPV = MuonTrk->GetX()[0];
    if(nPV > MuonTrk->GetX()[MuonTrk->GetN()-1]) nPV = MuonTrk->GetX()[MuonTrk->GetN()];
    float sf = MuonTrk->Eval(nPV);
    if(sigma!=0) {
        int bin(0);
        double max(1.e6);
        for(int i=0; i<MuonTrk->GetN(); i++) if(fabs(nPV - MuonTrk->GetX()[i]) < max) {max = fabs(nPV - MuonTrk->GetX()[i]); bin = i;}
        if(sigma > 0) sf = MuonTrk->Eval(nPV) + sigma*MuonTrk->GetErrorYhigh(bin);
        else if(sigma < 0) sf = MuonTrk->Eval(nPV) + sigma*MuonTrk->GetErrorYlow(bin);
    }
    if(sf < 0.5 || sf > 1.5) sf = 1.;
    return sf;
}


// MET

double ScaleFactorTool::GetTrigMET(double met, int sigma) {
    if(!TrigMET) return -1.;
    if(met < TrigMET->GetX()[0]) return 0.;
    if(met > TrigMET->GetX()[TrigMET->GetN()-1]) met = TrigMET->GetX()[TrigMET->GetN()-1] -1.;
    return TrigMET->Eval(met);
}


// Gen

double ScaleFactorTool::GetEwkW(double pt) {
    if(!W) return -1.;
    return W->Eval(pt);
}

double ScaleFactorTool::GetEwkZ(double pt) {
    if(!Z) return -1.;
    return Z->Eval(pt);
}

// Combined

double ScaleFactorTool::GetTrigEle(double pt1, double eta1, double pt2, double eta2, int sigma) {
    float TrigEle1EffData = GetTrigSingleEleEffData(pt1, eta1, sigma);
    float TrigEle2EffData = GetTrigSingleEleEffData(pt2, eta2, sigma);
    float TrigEle1EffMC = GetTrigSingleEleEffMC(pt1, eta1, sigma);
    float TrigEle2EffMC = GetTrigSingleEleEffMC(pt2, eta2, sigma);
    
    float TrigEleEffData = TrigEle1EffData + TrigEle2EffData - TrigEle1EffData*TrigEle2EffData;
    float TrigeleEffMC = TrigEle1EffMC + TrigEle2EffMC - TrigEle1EffMC*TrigEle2EffMC;
    
    return TrigEleEffData/TrigeleEffMC;
}

double ScaleFactorTool::GetTrigEle(double pt, double eta, int sigma) {
    float TrigEleEffData = GetTrigSingleEleEffData(pt, eta, sigma);
    float TrigeleEffMC = GetTrigSingleEleEffMC(pt, eta, sigma);
    
    return TrigEleEffData/TrigeleEffMC;
}


double ScaleFactorTool::GetTrigMuonMET(double pt, double eta, double met, int sigma) {
    float TrigMETEffData = GetTrigMET(met, sigma);
    float TrigMETEffMC = 1.;
    
    float TrigMuonEffData = GetTrigSingleMuonEffData(pt, eta, sigma);
    float TrigMuonEffMC = GetTrigSingleMuonEffMC(pt, eta, sigma);
    
    float TrigEffData = TrigMuonEffData + TrigMETEffData - TrigMuonEffData*TrigMETEffData;
    float TrigEffMC = TrigMuonEffMC + TrigMETEffMC - TrigMuonEffMC*TrigMETEffMC;
    
    return TrigEffData/TrigEffMC;
}

double ScaleFactorTool::GetTrigMuonMET(double pt1, double eta1, double pt2, double eta2, double met, int sigma) {
    float TrigMETEffData = GetTrigMET(met, sigma);
    float TrigMuon1EffData = GetTrigSingleMuonEffData(pt1, eta1, sigma);
    float TrigMuon2EffData = GetTrigSingleMuonEffData(pt2, eta2, sigma);
    float TrigMETEffMC = 1.;
    float TrigMuon1EffMC = GetTrigSingleMuonEffMC(pt1, eta1, sigma);
    float TrigMuon2EffMC = GetTrigSingleMuonEffMC(pt2, eta2, sigma);
    
    float TrigMuonEffData = TrigMuon1EffData + TrigMuon2EffData - TrigMuon1EffData*TrigMuon2EffData;
    float TrigMuonEffMC = TrigMuon1EffMC + TrigMuon2EffMC - TrigMuon1EffMC*TrigMuon2EffMC;
    
    float TrigEffData = TrigMuonEffData + TrigMETEffData - TrigMuonEffData*TrigMETEffData;
    float TrigEffMC = TrigMuonEffMC + TrigMETEffMC - TrigMuonEffMC*TrigMETEffMC;
    
    return TrigEffData/TrigEffMC;
}


