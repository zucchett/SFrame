#include "../interface/CorrectionTool.h"
#include <cstdlib>
#include <fstream>
#include <limits>

#include "../src/EnergyScaleCorrection_class.cc"

#include <TFile.h>
CorrectionTool::CorrectionTool(SCycleBase* parent, const char* name ): SToolBase( parent ), m_name( name ) {
    SetLogName( name );
    // Electrons
    DeclareProperty(  m_name+"_CorrectionFileName", m_CorrectionFileName = std::string (std::getenv("SFRAME_DIR")) + "/../CorrectionTool/data/Electron/ScalesSmearings/Moriond17_23Jan_ele" );
    // Jet
    DeclareProperty(  m_name+"_JetResFileName", m_JetResFileName = std::string (std::getenv("SFRAME_DIR")) + "/../CorrectionTool/data/Jet/Spring16_25nsV10_MC_SF_AK8PFchs.txt" );
    
    // Mass
//    DeclareProperty(  m_name+"_MassFileName", m_MassFileName = std::string (std::getenv("SFRAME_DIR")) + "/../CorrectionTool/data/Mass/puppiCorr.root" );
//    DeclareProperty(  m_name+"_MassGenHistName", m_MassGenHistName = "puppiJECcorr_gen" );
//    DeclareProperty(  m_name+"_MassBarrelHistName", m_MassBarrelHistName = "puppiJECcorr_reco_0eta1v3" );
//    DeclareProperty(  m_name+"_MassEndcapHistName", m_MassEndcapHistName = "puppiJECcorr_reco_1v3eta2v5" );
//    DeclareProperty(  m_name+"_MassGenFileName", m_MassGenFileName = std::string (std::getenv("SFRAME_DIR")) + "/../CorrectionTool/data/Mass/genCorr.root" );
//    DeclareProperty(  m_name+"_MassGenBarrelHistName", m_MassGenBarrelHistName = "gC_forCorr" );
//    DeclareProperty(  m_name+"_MassGenEndcapHistName", m_MassGenEndcapHistName = "gF_forCorr" );
//    DeclareProperty(  m_name+"_MassRecoFileName", m_MassRecoFileName = std::string (std::getenv("SFRAME_DIR")) + "/../CorrectionTool/data/Mass/recoCorr.root" );
//    DeclareProperty(  m_name+"_MassRecoBarrelHistName", m_MassRecoBarrelHistName = "gC_forCorr" );
//    DeclareProperty(  m_name+"_MassRecoEndcapHistName", m_MassRecoEndcapHistName = "gF_forCorr" );
//    DeclareProperty(  m_name+"_MassResFileName", m_MassResFileName = std::string (std::getenv("SFRAME_DIR")) + "/../CorrectionTool/data/Mass/puppiSoftdropResol_ICHEP.root" );
//    DeclareProperty(  m_name+"_MassResBarrelHistName", m_MassResBarrelHistName = "massResolution_0eta1v3" );
//    DeclareProperty(  m_name+"_MassResEndcapHistName", m_MassResEndcapHistName = "massResolution_1v3eta2v5" );
    DeclareProperty(  m_name+"_MassGenFileName", m_MassGenFileName = std::string (std::getenv("SFRAME_DIR")) + "/../CorrectionTool/data/Mass/puppiCorr_Moriond17.root" );
    DeclareProperty(  m_name+"_MassGenBarrelHistName", m_MassGenBarrelHistName = "puppiJECcorr_gen" );
    DeclareProperty(  m_name+"_MassGenEndcapHistName", m_MassGenEndcapHistName = "puppiJECcorr_gen" );
    DeclareProperty(  m_name+"_MassRecoFileName", m_MassRecoFileName = std::string (std::getenv("SFRAME_DIR")) + "/../CorrectionTool/data/Mass/puppiCorr_Moriond17.root" );
    DeclareProperty(  m_name+"_MassRecoBarrelHistName", m_MassRecoBarrelHistName = "puppiJECcorr_reco_0eta1v3" );
    DeclareProperty(  m_name+"_MassRecoEndcapHistName", m_MassRecoEndcapHistName = "puppiJECcorr_reco_1v3eta2v5" );
    DeclareProperty(  m_name+"_MassResFileName", m_MassResFileName = std::string (std::getenv("SFRAME_DIR")) + "/../CorrectionTool/data/Mass/puppiSoftdropResol_Moriond17.root" );
    DeclareProperty(  m_name+"_MassResBarrelHistName", m_MassResBarrelHistName = "massResolution_0eta1v3" );
    DeclareProperty(  m_name+"_MassResEndcapHistName", m_MassResEndcapHistName = "massResolution_1v3eta2v5" );
}


void CorrectionTool::BeginInputData( const SInputData& ) throw( SError ) {

    // Electrons
    m_logger << INFO << "CorrectionFileName:\t" << m_CorrectionFileName << SLogger::endmsg;
    
    // Jets
    m_logger << INFO << "JetResFileName:\t" << m_JetResFileName << SLogger::endmsg;
    
    // Mass
    m_logger << INFO << "MassGenFileName:\t" << m_MassGenFileName << SLogger::endmsg;
    m_logger << INFO << "MassGenBarrelHistName:\t" << m_MassGenBarrelHistName << SLogger::endmsg;
    m_logger << INFO << "MassGenEndcapHistName:\t" << m_MassGenEndcapHistName << SLogger::endmsg;
    m_logger << INFO << "MassRecoFileName:\t" << m_MassRecoFileName << SLogger::endmsg;
    m_logger << INFO << "MassRecoBarrelHistName:\t" << m_MassRecoBarrelHistName << SLogger::endmsg;
    m_logger << INFO << "MassRecoEndcapHistName:\t" << m_MassRecoEndcapHistName << SLogger::endmsg;
    m_logger << INFO << "MassResFileName:\t" << m_MassResFileName << SLogger::endmsg;
    m_logger << INFO << "MassResBarrelHistName:\t" << m_MassResBarrelHistName << SLogger::endmsg;
    m_logger << INFO << "MassResEndcapHistName:\t" << m_MassResEndcapHistName << SLogger::endmsg;
    
    
    // ********** Open files **********
    
    // Jets
    std::ifstream JetResFile(m_JetResFileName);
    std::string line;
    getline(JetResFile, line);
    float minEta, maxEta, none, res, resDown, resUp;
    while(JetResFile >> minEta >> maxEta >> none >> res >> resDown >> resUp) {
        float* array = new float[6]{minEta, maxEta, none, res, resDown, resUp};
        JetRes.push_back(array);
    }
    m_logger << VERBOSE << "JetResFile opened, found " << JetRes.size() << " lines" << SLogger::endmsg;
    
    // Gen
    auto MassGenFile = TFile::Open(m_MassGenFileName.c_str(), "READ");
    if(MassGenFile) {
        MassGenBarrel = (TF1*)MassGenFile->Get(m_MassGenBarrelHistName.c_str());
        MassGenEndcap = (TF1*)MassGenFile->Get(m_MassGenEndcapHistName.c_str());
  //  MassGenBarrel->SetDirectory(0);
  //  MassGenEndcap->SetDirectory(0);
        MassGenFile->Close();
    }
    else m_logger << WARNING << "‎File " << m_MassGenFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "MassGenFile opened" << SLogger::endmsg;
    
    // Reco
    auto MassRecoFile = TFile::Open(m_MassRecoFileName.c_str(), "READ");
    if(MassRecoFile) {
        MassRecoBarrel = (TF1*)MassRecoFile->Get(m_MassRecoBarrelHistName.c_str());
        MassRecoEndcap = (TF1*)MassRecoFile->Get(m_MassRecoEndcapHistName.c_str());
  //  MassRecoBarrel->SetDirectory(0);
  //  MassRecoEndcap->SetDirectory(0);
        MassRecoFile->Close();
    }
    else m_logger << WARNING << "‎File " << m_MassRecoFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "MassRecoFile opened" << SLogger::endmsg;
    
    // Mass resolution
    auto MassResFile = TFile::Open(m_MassResFileName.c_str(), "READ");
    if(MassResFile) {
        MassResBarrel = (TF1*)MassResFile->Get(m_MassResBarrelHistName.c_str());
        MassResEndcap = (TF1*)MassResFile->Get(m_MassResEndcapHistName.c_str());
  //  MassResBarrel->SetDirectory(0);
  //  MassResEndcap->SetDirectory(0);
        MassResFile->Close();
    }
    else m_logger << WARNING << "‎File " << m_MassResFileName << " does not exist. Please check." << SLogger::endmsg;
    m_logger << VERBOSE << "MassResFile opened" << SLogger::endmsg;
    
    trandom = new TRandom3();
    
    eScaler = new EnergyScaleCorrection_class(m_CorrectionFileName);
    eScaler->doScale = true;
    eScaler->doSmearings = true;
    
    return;
}



CorrectionTool::~CorrectionTool() {
    delete trandom;
    delete eScaler;
}

// Electron

double CorrectionTool::GetCorrectedEnergy(double energy, bool isMC, int runNumber, bool isEBEle, double R9Ele, double etaSCEle, double EtEle) {
    
    double scale_corr(1.), sigma(1.), Ecorr(1.);
    
    // Scale the Data
    if(!isMC) {
        scale_corr = eScaler->ScaleCorrection(runNumber, isEBEle, R9Ele, etaSCEle, EtEle, 0, 000);
        //At this point, the new data energy will be:
        energy = energy * scale_corr;
    }

    // Smear the MC
    if(isMC) {
        sigma = eScaler->getSmearingSigma(runNumber, isEBEle, R9Ele, etaSCEle, EtEle, 0, 0, 0);
        //Put the last two inputs at 0,0 for the nominal value of sigma
        //Now smear the MC energy
        energy = energy * trandom->Gaus(1, sigma);
    }
    
//    // Slew rate NO MORE!
//    if(!isMC && isEBEle) {
//        if(energy > 200 && energy <= 300)  Ecorr = 1.0199;
//        else if(energy > 300 && energy <= 400) Ecorr = 1.052;
//        else if(energy > 400 && energy <= 500) Ecorr = 1.015;
//        energy = energy * Ecorr;
//    }
    
    return energy;
}


// Jet
// JER form here:  https://github.com/cms-jet/JERDatabase
TLorentzVector CorrectionTool::GetCorrectedJet(TLorentzVector jet, int sigmaRes) {
    if(!(jet.Pt() > 0.)) return jet;
    
    //if(sigmaRes !=0) {
    float eta( jet.Eta() );
    int etaBin(-1);
    for(unsigned int i=0; i<JetRes.size()-1; i++) if(eta > JetRes[i][0] && eta < JetRes[i+1][1]) etaBin = i;
    if(etaBin < 0) {
        m_logger << WARNING << "Eta bin not found for jet with eta = " << eta << SLogger::endmsg;
        return jet;
    }
    float res(JetRes[etaBin][3]), resUp(JetRes[etaBin][5]-JetRes[etaBin][3]), resDown(JetRes[etaBin][3]-JetRes[etaBin][4]);
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution
    float jer2(res + sigmaRes*(sigmaRes > 0 ? resUp : resDown));
    jer2 = std::max(jer2, float(1.)); // truncated to 0
//    float jer2(sigmaRes > 0 ? resUp : resDown);
    float coeff = trandom->Gaus( 1., sqrt(jer2*jer2-1.)*(res-1.));
    jet *= coeff;
    //}

    return jet;
}

TLorentzVector CorrectionTool::GetCorrectedJet(TLorentzVector jet, float jer_sigma, float jer_sf,  float jer_sf_up, float jer_sf_down, int sigmaRes) {
    if(!(jet.Pt() > 0.)) return jet;
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution
    float sf( jer_sf );
    if(sigmaRes > 0) sf = sigmaRes*jer_sf_up;
    else if(sigmaRes < 0) sf = sigmaRes*jer_sf_down;
    float sigma = jer_sigma * std::sqrt(sf*sf - 1);
    float coeff = 1. + trandom->Gaus( 0., sigma);
    jet *= coeff;
    return jet;
}


// Mass

double CorrectionTool::GetCorrectedMass(double mass, double genmass, double pt, double eta, bool mc, int sigmaScale, int sigmaRes) {
    if(!MassGenBarrel || !MassGenEndcap || !MassRecoBarrel || !MassRecoEndcap) return -1.;
    float resolutionFactor(0.);
    
    // Base correction
    if( fabs(eta) <= 1.3 ) {
        mass *= MassGenBarrel->Eval( pt );
        mass *= MassRecoBarrel->Eval( pt );
    }
    else {
        mass *= MassGenEndcap->Eval( pt );
        mass *= MassRecoEndcap->Eval( pt );
    }
    
    if(mc) {
        // Scale correction
        float jms = 1.000;
        float jmsUnc = 0.0094;
        mass *= jms + sigmaScale*jmsUnc;
        
        // Resolution correction
        float jmr = 1.000;
        float jmrUnc = 0.200;
        float jmr2(jmr + sigmaRes*jmrUnc);

        // Hybrid method
        if(genmass > 0 && fabs(mass-genmass)/mass < 0.3) {
            float delta(genmass-mass);
            delta *= jmr2;
            mass = genmass - delta;
        }
        // Otherwise, stochastic smearing
        else {
            jmr2 = std::max(jmr2, float(1.)); // truncated to 0
            if(MassResBarrel  && MassResEndcap) {
                if( fabs(eta) <= 1.3 ) resolutionFactor = MassResBarrel->Eval( pt );
                else resolutionFactor = MassResBarrel->Eval( pt );
            }
            else {
                // Override resolution factor
                resolutionFactor = 1.101; // +- 0.05
            }
            float sigma( mass*(resolutionFactor-1.)*sqrt(jmr2*jmr2-1.) );
            if(jmr2 > 1.) mass = trandom->Gaus( mass, sigma );
        }
    }
    
    if(mass<=0.) mass = 0.;
    
    return mass;
}

