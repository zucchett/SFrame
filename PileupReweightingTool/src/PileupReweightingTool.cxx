#include "../include/PileupReweightingTool.h"
#include <iostream>
//
// constructor
//
PileupReweightingTool::PileupReweightingTool( SCycleBase* parent, const char* name ) : SToolBase( parent ), m_name( name ) { //, m_puWeight()

    SetLogName( name );
    //$SFRAME_DIR
    DeclareProperty( m_name + "_MCRootFileName", m_MCRootFileName = "$SFRAME_DIR/../PileupReweightingTool/histograms/Moriond17_MC.root" );
    DeclareProperty( m_name + "_MCRootHistName", m_MCRootHistName = "pileup" );
    DeclareProperty( m_name + "_DataRootFileName", m_DataRootFileName = "$SFRAME_DIR/../PileupReweightingTool/histograms/Moriond17_69200.root" );
    DeclareProperty( m_name + "_DataRootHistName", m_DataRootHistName = "pileup" );
    DeclareProperty( m_name + "_DataRootFileNameUp", m_DataRootFileNameUp = "$SFRAME_DIR/../PileupReweightingTool/histograms/Moriond17_72383.root" );
    DeclareProperty( m_name + "_DataRootHistNameUp", m_DataRootHistNameUp = "pileup" );
    DeclareProperty( m_name + "_DataRootFileNameDown", m_DataRootFileNameDown = "$SFRAME_DIR/../PileupReweightingTool/histograms/Moriond17_66156.root" );
    DeclareProperty( m_name + "_DataRootHistNameDown", m_DataRootHistNameDown = "pileup" );

}

//
// destructor
//
PileupReweightingTool::~PileupReweightingTool() {}


//
// initialize before event loop
//
void PileupReweightingTool::BeginInputData( const SInputData& ) throw( SError ) {
  
    m_logger << INFO << "MCRootFileName: " << m_MCRootFileName << SLogger::endmsg;
    m_logger << INFO << "MCRootHistName: " << m_MCRootHistName << SLogger::endmsg;
    m_logger << INFO << "DataRootFileName: " << m_DataRootFileName << SLogger::endmsg;
    m_logger << INFO << "DataRootHistName: " << m_DataRootHistName << SLogger::endmsg;
    m_logger << INFO << "DataRootFileNameUp: " << m_DataRootFileNameUp << SLogger::endmsg;
    m_logger << INFO << "DataRootHistNameUp: " << m_DataRootHistNameUp << SLogger::endmsg;
    m_logger << INFO << "DataRootFileNameDown: " << m_DataRootFileNameDown << SLogger::endmsg;
    m_logger << INFO << "DataRootHistNameDown: " << m_DataRootHistNameDown << SLogger::endmsg;
    
    
    MCRootFile = new TFile(m_MCRootFileName.c_str(), "READ");
    if(MCRootFile) {
        MCRootHist = (TH1F*)MCRootFile->Get(m_MCRootHistName.c_str());
        if(MCRootHist) MCRootHist->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_MCRootHistName << " does not exist. Please check." << SLogger::endmsg;
        MCRootFile->Close();
    }
    else m_logger << WARNING << "‎File " << m_MCRootFileName << " does not exist. Please check." << SLogger::endmsg;
    
    DataRootFile = new TFile(m_DataRootFileName.c_str(), "READ");
    if(DataRootFile) {
        DataRootHist = (TH1F*)DataRootFile->Get(m_DataRootHistName.c_str());
        if(DataRootHist) DataRootHist->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_DataRootHistName << " does not exist. Please check." << SLogger::endmsg;
        DataRootFile->Close();
    }
    else m_logger << WARNING << "‎File " << m_DataRootFileName << " does not exist. Please check." << SLogger::endmsg;
    
    DataRootFileUp = new TFile(m_DataRootFileNameUp.c_str(), "READ");
    if(DataRootFileUp) {
        DataRootHistUp = (TH1F*)DataRootFileUp->Get(m_DataRootHistNameUp.c_str());
        if(DataRootHistUp) DataRootHistUp->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_DataRootHistNameUp << " does not exist. Please check." << SLogger::endmsg;
        DataRootFileUp->Close();
    }
    else m_logger << WARNING << "‎File " << m_DataRootFileNameUp << " does not exist. Please check." << SLogger::endmsg;
    
    DataRootFileDown = new TFile(m_DataRootFileNameDown.c_str(), "READ");
    if(DataRootFileDown) {
        DataRootHistDown = (TH1F*)DataRootFileDown->Get(m_DataRootHistNameDown.c_str());
        if(DataRootHistDown) DataRootHistDown->SetDirectory(0);
        else m_logger << WARNING << "‎Hist " << m_DataRootHistNameDown << " does not exist. Please check." << SLogger::endmsg;
        DataRootFileDown->Close();
    }
    else m_logger << WARNING << "‎File " << m_DataRootFileNameDown << " does not exist. Please check." << SLogger::endmsg;
    
    // Normalize to unity
    MCRootHist->Scale(1./MCRootHist->Integral());
    DataRootHist->Scale(1./DataRootHist->Integral());
    DataRootHistUp->Scale(1./DataRootHistUp->Integral());
    DataRootHistDown->Scale(1./DataRootHistDown->Integral());
    
    if(MCRootHist->GetNbinsX() != DataRootHist->GetNbinsX()) m_logger << WARNING << "Pileup histograms binning is different" << SLogger::endmsg;
    m_logger << INFO << "Pileup weights initialised" << SLogger::endmsg;
     
    return;
}

//
// get the weight
//
double PileupReweightingTool::getPileUpweight(int npu, int sigma){
    //double weight = m_puWeight.getPUWeight( mu );
    float data(1.), mc(0.);
    if(sigma==+1) data = DataRootHistUp->GetBinContent(DataRootHistUp->FindBin(npu));
    else if(sigma==-1) data = DataRootHistDown->GetBinContent(DataRootHistDown->FindBin(npu));
    else data = DataRootHist->GetBinContent(DataRootHist->FindBin(npu));
    mc = MCRootHist->GetBinContent(MCRootHist->FindBin(npu));

    if(mc > 0.) return data / mc;

    m_logger << WARNING << "Non-defined Pileup Weight: " << mc << SLogger::endmsg;
    return 1;
}
