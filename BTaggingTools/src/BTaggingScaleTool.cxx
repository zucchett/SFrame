#include "include/BTaggingScaleTool.h"

#include <cstdlib>
#include <limits>

#include <TFile.h>

//
// constructor
//
BTaggingScaleTool::BTaggingScaleTool( SCycleBase* parent, 
                                      const char* name ) : 
  SToolBase( parent ), m_name( name ) {

  SetLogName( name );

  std::string sframe_dir(std::getenv("SFRAME_DIR"));

  // m_calib = 0;
  // m_reader = 0;
  // m_reader_up = 0;
  // m_reader_down = 0;
  
  // if needed, probably best to append WP names a la Loose74X
  // see also use in BeginInputData
  wpCuts.clear();
  wpCuts["Loose"] = 0.5426;
  wpCuts["Medium"] = 0.8484;
  wpCuts["Tight"] =  0.9535;
  
  wpDoubleBCuts.clear();
  wpDoubleBCuts["Loose"] = 0.3;
  wpDoubleBCuts["Medium1"] = 0.6;
  wpDoubleBCuts["Medium2"] = 0.8;
  wpDoubleBCuts["Tight"] =  0.9;
  // wpCuts.clear();
  // wpCuts["Loose"] = 0.460;
  // wpCuts["Medium"] = 0.800;
  // wpCuts["Tight"] = 0.935;
  // wpCuts_veto.clear();
  // wpCuts_veto["Loose"] = 0.460;
  // wpCuts_veto["Medium"] = 0.800;
  // wpCuts_veto["Tight"] = 0.935;

  // wpCuts["Loose"] = 0.60;
  // wpCuts["Medium"] = 0.890;
  // wpCuts["Tight"] = 0.95;
 
  currentWorkingPointCut = -1;
  currentWorkingPointCut_veto = -1;
  currentWorkingPointCut_tag = -1;
  currentWorkingPointCut_tight = -1;
  currentWorkingPointCut_loose = -1;
  m_effMaps.clear();
  m_effMaps_veto.clear();
  m_effMaps_tag.clear();
  m_effMaps_tight.clear();
  m_effMaps_loose.clear();
  
  DeclareProperty( m_name + "_Tagger",    m_tagger = "CSVv2" );
  DeclareProperty( m_name + "_Tagger_veto",    m_tagger_veto = "CSVv2" );
  DeclareProperty( m_name + "_Tagger_tag",    m_tagger_tag = "CSVv2" );
 
  DeclareProperty( m_name + "_WorkingPoint", m_workingPoint = "Loose" );
  DeclareProperty( m_name + "_WorkingPoint_veto", m_workingPoint_veto = "Loose" ); // Loose
  DeclareProperty( m_name + "_WorkingPoint_tag", m_workingPoint_tag = "Tight" );
  
  DeclareProperty( m_name + "_WorkingPoint_loose", m_workingPoint_loose = "Loose" );
  DeclareProperty( m_name + "_WorkingPoint_tight", m_workingPoint_tight = "Tight" );

  DeclareProperty( m_name + "_CsvFile", m_csvFile = sframe_dir + "/../BTaggingTools/csv/subjet_CSVv2_Moriond17_B_H.csv" );
  DeclareProperty( m_name + "_CsvFile_veto", m_csvFile_veto = sframe_dir + "/../BTaggingTools/csv/CSVv2_Moriond17_B_H.csv" );
  DeclareProperty( m_name + "_CsvFile_tag", m_csvFile_tag = sframe_dir + "/../BTaggingTools/csv/CSVv2_Moriond17_B_H.csv" );

  DeclareProperty( m_name + "_MeasurementType_udsg", m_measurementType_udsg = "incl" );//"incl" 
  DeclareProperty( m_name + "_MeasurementType_bc", m_measurementType_bc = "lt" );//"lt"

  DeclareProperty( m_name + "_MeasurementType_veto_udsg", m_measurementType_veto_udsg = "incl" ); //incl//comb
  DeclareProperty( m_name + "_MeasurementType_veto_bc", m_measurementType_veto_bc = "mujets" );

  DeclareProperty( m_name + "_MeasurementType_tag_udsg", m_measurementType_tag_udsg = "incl" ); //incl//comb
  DeclareProperty( m_name + "_MeasurementType_tag_bc", m_measurementType_tag_bc = "mujets" );
  
  DeclareProperty( m_name + "_EffHistDirectory", m_effHistDirectory = "bTagEff" );
  DeclareProperty( m_name + "_EffFile", m_effFile = sframe_dir + "/../BTaggingTools/efficiencies/bTagEffs_Summer16.root" );
  DeclareProperty( m_name + "_EffFile_veto", m_effFile_veto = sframe_dir + "/../BTaggingTools/efficiencies/bTagEffs_Summer16.root" );
  DeclareProperty( m_name + "_EffFile_tag", m_effFile_tag = sframe_dir + "/../BTaggingTools/efficiencies/bTagEffs_Summer16.root" );
  
  DeclareProperty( m_name + "_EffFile_loose", m_effFile_loose = sframe_dir + "/../BTaggingTools/efficiencies/bTagEffs_doubleB_Summer16.root" );
  DeclareProperty( m_name + "_EffFile_tight", m_effFile_tight = sframe_dir + "/../BTaggingTools/efficiencies/bTagEffs_doubleB_Summer16.root" );
}

//
// destructor
//
BTaggingScaleTool::~BTaggingScaleTool() {
  // delete m_calib;
  // delete m_reader;
  // delete m_reader_up;
  // delete m_reader_down;
}

void BTaggingScaleTool::BeginInputData( const SInputData& ) throw( SError ) {

  m_logger << INFO << "Initializing BTagCalibrationStandalone" << SLogger::endmsg;
  m_logger << INFO << "CSV file:    " << m_csvFile << SLogger::endmsg;
  m_logger << INFO << "CSV file for veto:    " << m_csvFile_veto << SLogger::endmsg;
  m_logger << INFO << "CSV file for tag:    " << m_csvFile_tag << SLogger::endmsg;

  m_logger << INFO << "Tagger:         " << m_tagger << SLogger::endmsg;
  m_logger << INFO << "Tagger for veto:         " << m_tagger_veto << SLogger::endmsg;
  m_logger << INFO << "Tagger for tag:         " << m_tagger_tag << SLogger::endmsg;
  
  m_logger << INFO << "WorkingPoint: " << m_workingPoint << SLogger::endmsg;
  m_logger << INFO << "WorkingPoint for veto ak4: " << m_workingPoint_veto << SLogger::endmsg;
  m_logger << INFO << "WorkingPoint for tag ak4: " << m_workingPoint_tag << SLogger::endmsg;
  
  m_logger << INFO << "WorkingPoint for doubleB loose: " << m_workingPoint_loose << SLogger::endmsg;
  m_logger << INFO << "WorkingPoint for doubleB tight: " << m_workingPoint_tight << SLogger::endmsg;

  m_logger << INFO << "MeasurementType veto udsg: " << m_measurementType_veto_udsg << SLogger::endmsg;
  m_logger << INFO << "MeasurementType veto  bc: " << m_measurementType_veto_bc << SLogger::endmsg;
  
  m_logger << INFO << "MeasurementType tag udsg: " << m_measurementType_tag_udsg << SLogger::endmsg;
  m_logger << INFO << "MeasurementType tag  bc: " << m_measurementType_tag_bc << SLogger::endmsg;

  m_logger << INFO << "MeasurementType udsg: " << m_measurementType_udsg << SLogger::endmsg;
  m_logger << INFO << "MeasurementType bc: " << m_measurementType_bc << SLogger::endmsg;
  
  m_logger << INFO << "EffHistDirectory: " << m_effHistDirectory << SLogger::endmsg;
  m_logger << INFO << "Efficiency file: " << m_effFile << SLogger::endmsg;
  m_logger << INFO << "Efficiency file doubleB: " << m_effFile_loose << SLogger::endmsg;
   
  BTagEntry::OperatingPoint wp = BTagEntry::OP_LOOSE;
  if (m_workingPoint.find("Loose") != std::string::npos) {
    wp = BTagEntry::OP_LOOSE;
    currentWorkingPointCut = wpCuts["Loose"];
  }
  else if (m_workingPoint.find("Medium") != std::string::npos) {
    wp = BTagEntry::OP_MEDIUM;
    currentWorkingPointCut = wpCuts["Medium"];
  }
  else if (m_workingPoint.find("Tight") != std::string::npos) {
    wp = BTagEntry::OP_TIGHT;
    currentWorkingPointCut = wpCuts["Tight"];
  }
  else if (m_workingPoint.find("Reshaping") != std::string::npos) {
    wp = BTagEntry::OP_RESHAPING;
    currentWorkingPointCut = wpCuts["Loose"]; //placeholder
    m_logger << WARNING << "Reshaping not yet implemented!" << SLogger::endmsg;
  }
  else {
    throw SError( ("Unknown working point: " + m_workingPoint).c_str(), SError::SkipCycle );
  }
  
  BTagEntry::OperatingPoint wp_veto = BTagEntry::OP_LOOSE;
  if (m_workingPoint_veto.find("Loose") != std::string::npos) {
    wp_veto = BTagEntry::OP_LOOSE;
    currentWorkingPointCut_veto = wpCuts["Loose"];
  }
  else if (m_workingPoint_veto.find("Medium") != std::string::npos) {
   
    m_logger << INFO << " working point medium" << SLogger::endmsg;
    wp_veto = BTagEntry::OP_MEDIUM;
    currentWorkingPointCut_veto = wpCuts["Medium"];

  }
  else if (m_workingPoint_veto.find("Tight") != std::string::npos) {
    wp_veto = BTagEntry::OP_TIGHT;
    currentWorkingPointCut_veto = wpCuts["Tight"];
  }
  else if (m_workingPoint_veto.find("Reshaping") != std::string::npos ) {
    wp_veto = BTagEntry::OP_RESHAPING;
    currentWorkingPointCut_veto = wpCuts["Loose"]; //placeholder
    m_logger << WARNING << "Reshaping not yet implemented!" << SLogger::endmsg;
  }
  else {
    throw SError( ("Unknown working point: " + m_workingPoint_veto).c_str(), SError::SkipCycle );
  }
  
  
  
  BTagEntry::OperatingPoint wp_tag = BTagEntry::OP_LOOSE;
  if (m_workingPoint_tag.find("Loose") != std::string::npos) {
    wp_tag = BTagEntry::OP_LOOSE;
    currentWorkingPointCut_tag = wpCuts["Loose"];
  }
  else if (m_workingPoint_tag.find("Medium") != std::string::npos) {
   
    m_logger << INFO << " working point medium" << SLogger::endmsg;
    wp_tag = BTagEntry::OP_MEDIUM;
    currentWorkingPointCut_tag = wpCuts["Medium"];

  }
  else if (m_workingPoint_tag.find("Tight") != std::string::npos) {
    wp_tag = BTagEntry::OP_TIGHT;
    currentWorkingPointCut_tag = wpCuts["Tight"];
  }
  else if (m_workingPoint_tag.find("Reshaping") != std::string::npos ) {
    wp_tag = BTagEntry::OP_RESHAPING;
    currentWorkingPointCut_tag = wpCuts["Loose"]; //placeholder
    m_logger << WARNING << "Reshaping not yet implemented!" << SLogger::endmsg;
  }
  else {
    throw SError( ("Unknown working point: " + m_workingPoint_tag).c_str(), SError::SkipCycle );
  }
  
  if (m_workingPoint_tight.find("Loose") != std::string::npos) {
    m_logger << INFO << " working point loose" << SLogger::endmsg;
    currentWorkingPointCut_tight = wpDoubleBCuts["Loose"];
  }
  else if (m_workingPoint_tight.find("Medium1") != std::string::npos) {
    m_logger << INFO << " working point medium1" << SLogger::endmsg;
    currentWorkingPointCut_tight = wpDoubleBCuts["Medium1"];
  }
  else if (m_workingPoint_tight.find("Medium2") != std::string::npos) {
    m_logger << INFO << " working point medium2" << SLogger::endmsg;
    currentWorkingPointCut_tight = wpDoubleBCuts["Medium2"];
  }
  else if (m_workingPoint_tight.find("Tight") != std::string::npos) {
    m_logger << INFO << " working point tight" << SLogger::endmsg;
    currentWorkingPointCut_tight = wpDoubleBCuts["Tight"];
  }
  else {
    throw SError( ("Unknown working point: " + m_workingPoint_tight).c_str(), SError::SkipCycle );
  }
  
  if (m_workingPoint_loose.find("Loose") != std::string::npos) {
    m_logger << INFO << " working point tight" << SLogger::endmsg;
    currentWorkingPointCut_loose = wpDoubleBCuts["Loose"];
  }
  else if (m_workingPoint_loose.find("Medium1") != std::string::npos) {
    m_logger << INFO << " working point medium1" << SLogger::endmsg;
    currentWorkingPointCut_loose = wpDoubleBCuts["Medium1"];
  }
  else if (m_workingPoint_loose.find("Medium2") != std::string::npos) {
    m_logger << INFO << " working point medium2" << SLogger::endmsg;
    currentWorkingPointCut_loose = wpDoubleBCuts["Medium2"];
  }
  else if (m_workingPoint_loose.find("Tight") != std::string::npos) {
    m_logger << INFO << " working point tight" << SLogger::endmsg;
    currentWorkingPointCut_loose = wpDoubleBCuts["Tight"];
  }
  else {
    throw SError( ("Unknown working point: " + m_workingPoint_loose).c_str(), SError::SkipCycle );
  }
  
  BTagCalibration m_calib(m_tagger, m_csvFile);

  m_reader.reset(new BTagCalibrationReader(wp, "central"));
  m_reader_up.reset(new BTagCalibrationReader(wp, "up"));
  m_reader_down.reset(new BTagCalibrationReader(wp, "down"));
  
  m_reader->load(m_calib, BTagEntry::FLAV_B, m_measurementType_bc);
  m_reader->load(m_calib, BTagEntry::FLAV_C, m_measurementType_bc);
  m_reader->load(m_calib, BTagEntry::FLAV_UDSG, m_measurementType_udsg);
  m_reader_up->load(m_calib, BTagEntry::FLAV_B, m_measurementType_bc);
  m_reader_up->load(m_calib, BTagEntry::FLAV_C, m_measurementType_bc);
  m_reader_up->load(m_calib, BTagEntry::FLAV_UDSG, m_measurementType_udsg);
  m_reader_down->load(m_calib, BTagEntry::FLAV_B, m_measurementType_bc);
  m_reader_down->load(m_calib, BTagEntry::FLAV_C, m_measurementType_bc);
  m_reader_down->load(m_calib, BTagEntry::FLAV_UDSG, m_measurementType_udsg);



  BTagCalibration m_calib_veto(m_tagger_veto, m_csvFile_veto );

  m_reader_veto.reset(new BTagCalibrationReader(wp_veto, "central"));
  m_reader_veto_up.reset(new BTagCalibrationReader(wp_veto, "up"));
  m_reader_veto_down.reset(new BTagCalibrationReader(wp_veto, "down"));
  
  m_reader_veto->load(m_calib_veto, BTagEntry::FLAV_B, m_measurementType_veto_bc);
  m_reader_veto->load(m_calib_veto, BTagEntry::FLAV_C, m_measurementType_veto_bc);
  m_reader_veto->load(m_calib_veto, BTagEntry::FLAV_UDSG, m_measurementType_veto_udsg);
  m_reader_veto_up->load(m_calib_veto, BTagEntry::FLAV_B, m_measurementType_veto_bc);
  m_reader_veto_up->load(m_calib_veto, BTagEntry::FLAV_C, m_measurementType_veto_bc);
  m_reader_veto_up->load(m_calib_veto, BTagEntry::FLAV_UDSG, m_measurementType_veto_udsg);
  m_reader_veto_down->load(m_calib_veto, BTagEntry::FLAV_B, m_measurementType_veto_bc);
  m_reader_veto_down->load(m_calib_veto, BTagEntry::FLAV_C, m_measurementType_veto_bc);
  m_reader_veto_down->load(m_calib_veto, BTagEntry::FLAV_UDSG, m_measurementType_veto_udsg);
  // m_reader_veto->load(m_calib_veto, BTagEntry::FLAV_B, m_measurementType_bc);
  // m_reader_veto->load(m_calib_veto, BTagEntry::FLAV_C, m_measurementType_bc);
  // m_reader_veto->load(m_calib_veto, BTagEntry::FLAV_UDSG, m_measurementType_udsg);
  // m_reader_veto_up->load(m_calib_veto, BTagEntry::FLAV_B, m_measurementType_bc);
  // m_reader_veto_up->load(m_calib_veto, BTagEntry::FLAV_C, m_measurementType_bc);
  // m_reader_veto_up->load(m_calib_veto, BTagEntry::FLAV_UDSG, m_measurementType_udsg);
  // m_reader_veto_down->load(m_calib_veto, BTagEntry::FLAV_B, m_measurementType_bc);
  // m_reader_veto_down->load(m_calib_veto, BTagEntry::FLAV_C, m_measurementType_bc);
  // m_reader_veto_down->load(m_calib_veto, BTagEntry::FLAV_UDSG, m_measurementType_udsg);
  
  
  
  BTagCalibration m_calib_tag(m_tagger_tag, m_csvFile_tag );

  m_reader_tag.reset(new BTagCalibrationReader(wp_tag, "central"));
  m_reader_tag_up.reset(new BTagCalibrationReader(wp_tag, "up"));
  m_reader_tag_down.reset(new BTagCalibrationReader(wp_tag, "down"));
  
  m_reader_tag->load(m_calib_tag, BTagEntry::FLAV_B, m_measurementType_tag_bc);
  m_reader_tag->load(m_calib_tag, BTagEntry::FLAV_C, m_measurementType_tag_bc);
  m_reader_tag->load(m_calib_tag, BTagEntry::FLAV_UDSG, m_measurementType_tag_udsg);
  m_reader_tag_up->load(m_calib_tag, BTagEntry::FLAV_B, m_measurementType_tag_bc);
  m_reader_tag_up->load(m_calib_tag, BTagEntry::FLAV_C, m_measurementType_tag_bc);
  m_reader_tag_up->load(m_calib_tag, BTagEntry::FLAV_UDSG, m_measurementType_tag_udsg);
  m_reader_tag_down->load(m_calib_tag, BTagEntry::FLAV_B, m_measurementType_tag_bc);
  m_reader_tag_down->load(m_calib_tag, BTagEntry::FLAV_C, m_measurementType_tag_bc);
  m_reader_tag_down->load(m_calib_tag, BTagEntry::FLAV_UDSG, m_measurementType_tag_udsg);
  // m_reader_tag->load(m_calib_tag, BTagEntry::FLAV_B, m_measurementType_bc);
  // m_reader_tag->load(m_calib_tag, BTagEntry::FLAV_C, m_measurementType_bc);
  // m_reader_tag->load(m_calib_tag, BTagEntry::FLAV_UDSG, m_measurementType_udsg);
  // m_reader_tag_up->load(m_calib_tag, BTagEntry::FLAV_B, m_measurementType_bc);
  // m_reader_tag_up->load(m_calib_tag, BTagEntry::FLAV_C, m_measurementType_bc);
  // m_reader_tag_up->load(m_calib_tag, BTagEntry::FLAV_UDSG, m_measurementType_udsg);
  // m_reader_tag_down->load(m_calib_tag, BTagEntry::FLAV_B, m_measurementType_bc);
  // m_reader_tag_down->load(m_calib_tag, BTagEntry::FLAV_C, m_measurementType_bc);
  // m_reader_tag_down->load(m_calib_tag, BTagEntry::FLAV_UDSG, m_measurementType_udsg);
  

  
  // jet categories for efficiencies
  m_jetCategories = {"jet", "subjet_softdrop"};//"jet",
  m_jetCategories_veto = {"jet_ak4"};
  m_jetCategories_tag = {"jet_ak4"};
  m_flavours = {"b", "c", "udsg"};
  
  // read in efficiencies
  readEfficiencies();

  return;

}


double BTaggingScaleTool::getScaleFactor( const double& pt, const double& eta, const int& flavour, bool isTagged, const double& sigma_bc, const double& sigma_udsg, const TString& jetCategory ) {

  // Flavor
  BTagEntry::JetFlavor flavorEnum = BTagEntry::FLAV_UDSG;
  if  ( fabs(flavour)==5) flavorEnum = BTagEntry::FLAV_B;
  if  ( fabs(flavour)==15) flavorEnum = BTagEntry::FLAV_C;
  if  ( fabs(flavour)==4) flavorEnum = BTagEntry::FLAV_C;

  double MaxEta = 2.4;
  double abs_eta = fabs(eta);
  if (abs_eta > MaxEta) {
    // outside tracker range
    return 1.;
  }
  
  // range checking, double uncertainty if beyond
  std::pair<float, float> sf_bounds = m_reader->min_max_pt(flavorEnum, abs_eta);
  
  m_logger << DEBUG << "     flavor " << flavorEnum << " - " << sf_bounds.first << " " << sf_bounds.second <<" jet pt "<< pt<<  SLogger::endmsg;

  float pt_for_eval = pt;
  bool is_out_of_bounds = false;
  if (pt < sf_bounds.first) {
    pt_for_eval = sf_bounds.first + 1e-5;
    is_out_of_bounds = true;
  } else if (pt >= sf_bounds.second) {
    pt_for_eval = sf_bounds.second - 0.1;
    is_out_of_bounds = true;
  }
  m_logger << DEBUG << "*******  out of band ? " << is_out_of_bounds << "so pt_for_eval_used " <<  pt_for_eval << SLogger::endmsg;
  double sigmaScale_bc = sigma_bc;
  double sigmaScale_udsg = sigma_udsg;
  // double uncertainty in case jet outside normal kinematics
  if (is_out_of_bounds) {
    m_logger << DEBUG << sf_bounds.first << " - " << sf_bounds.second << SLogger::endmsg;
    m_logger << DEBUG << "out of bounds, using: " << pt_for_eval << " and " << abs_eta << SLogger::endmsg;
    sigmaScale_bc *= 2;
    sigmaScale_udsg *= 2;
  }
  
  m_logger << DEBUG << "getting scale factor " << SLogger::endmsg;
  double scalefactor = m_reader->eval(flavorEnum, eta, pt_for_eval);
  m_logger << DEBUG << "scale factor: " << scalefactor << SLogger::endmsg;
  if ((flavour == 5) || (flavour == 4)) {
    if ((sigma_bc > std::numeric_limits<double>::epsilon()) || (sigma_bc < -std::numeric_limits<double>::epsilon())) {
      // m_logger << DEBUG << "limit: " << std::numeric_limits<double>::epsilon() << " value: " << sigma << SLogger::endmsg;
      if (sigma_bc > 0) {
        double scalefactor_up =  m_reader_up->eval(flavorEnum, eta, pt_for_eval);
        scalefactor = sigmaScale_bc*(scalefactor_up - scalefactor) + scalefactor;
      }
      else {
        double scalefactor_down =  m_reader_down->eval(flavorEnum, eta, pt_for_eval);
        scalefactor = fabs(sigmaScale_bc)*(scalefactor_down - scalefactor) + scalefactor;
      }
    }
  }
  else {
    if ((sigma_udsg > std::numeric_limits<double>::epsilon()) || (sigma_udsg < -std::numeric_limits<double>::epsilon())) {
      // m_logger << DEBUG << "limit: " << std::numeric_limits<double>::epsilon() << " value: " << sigma << SLogger::endmsg;
      if (sigma_udsg > 0) {
        double scalefactor_up =  m_reader_up->eval(flavorEnum, eta, pt_for_eval);
        scalefactor = sigmaScale_udsg*(scalefactor_up - scalefactor) + scalefactor;
      }
      else {
        double scalefactor_down =  m_reader_down->eval(flavorEnum, eta, pt_for_eval);
        scalefactor = fabs(sigmaScale_udsg)*(scalefactor_down - scalefactor) + scalefactor;
      }
    }
  }
  if (scalefactor == 0) {
    throw SError( "Scale factor returned is zero!", SError::SkipCycle );
  }
  
  m_logger << DEBUG << "getting final weight for flav " << flavorEnum << SLogger::endmsg;
  
  double jetweight = 1.;
  // set effMC close to one for now, need to use real value map later
  double effMC = getEfficiency(pt, eta, flavour, jetCategory);
  
  if (isTagged) {
    m_logger << DEBUG << "     Jet is tagged " << SLogger::endmsg;
    jetweight *= scalefactor;
  }
  else {
    m_logger << DEBUG << "     Jet is not tagged " << SLogger::endmsg;
    jetweight *= (1 - (scalefactor * effMC)) / (1 - effMC);
  }
  
  m_logger << DEBUG << " jetweight " << jetweight << SLogger::endmsg;

  return jetweight;
}


double BTaggingScaleTool::getScaleFactor_veto( const double& pt, const double& eta, const int& flavour, bool isTagged, const double& sigma_bc, const double& sigma_udsg, const TString& jetCategory ) {
  m_logger << DEBUG << "     flavor " <<  flavour<<  SLogger::endmsg;
  // Flavor
  BTagEntry::JetFlavor flavorEnum = BTagEntry::FLAV_UDSG;
  
  if  ( fabs(flavour)==5) flavorEnum = BTagEntry::FLAV_B;
  if  ( fabs(flavour)==15) flavorEnum = BTagEntry::FLAV_C;
  if  ( fabs(flavour)==4) flavorEnum = BTagEntry::FLAV_C;

  double MaxEta = 2.4;
  double abs_eta = fabs(eta);
  if (abs_eta > MaxEta) {
    // outside tracker range
    return 1.;
  }
  
  // range checking, double uncertainty if beyond
  std::pair<float, float> sf_bounds = m_reader_veto->min_max_pt(flavorEnum, abs_eta);
  
  m_logger << DEBUG << "     flavorEnum  " << flavorEnum << " ;pt bound min " << sf_bounds.first << " ;pt bound max " << sf_bounds.second <<" jet pt "<< pt<<  SLogger::endmsg;

  float pt_for_eval = pt;
  bool is_out_of_bounds = false;
  if (pt < sf_bounds.first) {
    pt_for_eval = sf_bounds.first + 1e-5;
    is_out_of_bounds = true;
  } else if (pt >= sf_bounds.second) {
    pt_for_eval = sf_bounds.second - 0.1;
    is_out_of_bounds = true;
  }
  m_logger << DEBUG << "*******  out of band ? " << is_out_of_bounds << "so pt_for_eval_used " <<  pt_for_eval << SLogger::endmsg;
  double sigmaScale_bc = sigma_bc;
  double sigmaScale_udsg = sigma_udsg;
  // double uncertainty in case jet outside normal kinematics
  if (is_out_of_bounds) {
    m_logger << DEBUG << sf_bounds.first << " - " << sf_bounds.second << SLogger::endmsg;
    m_logger << DEBUG << "out of bounds, using: " << pt_for_eval << " and " << abs_eta << SLogger::endmsg;
    sigmaScale_bc *= 2;
    sigmaScale_udsg *= 2;
  }
  
 
  double scalefactor = m_reader_veto->eval(flavorEnum, eta, pt_for_eval);
  m_logger << DEBUG << "scale factor: " << scalefactor << SLogger::endmsg;
  if ((flavour == 5) || (flavour == 4)) {
    if ((sigma_bc > std::numeric_limits<double>::epsilon()) || (sigma_bc < -std::numeric_limits<double>::epsilon())) {
      // m_logger << DEBUG << "limit: " << std::numeric_limits<double>::epsilon() << " value: " << sigma << SLogger::endmsg;
      if (sigma_bc > 0) {
        double scalefactor_up =  m_reader_veto_up->eval(flavorEnum, eta, pt_for_eval);
        scalefactor = sigmaScale_bc*(scalefactor_up - scalefactor) + scalefactor;
      }
      else {
        double scalefactor_down =  m_reader_veto_down->eval(flavorEnum, eta, pt_for_eval);
        scalefactor = fabs(sigmaScale_bc)*(scalefactor_down - scalefactor) + scalefactor;
      }
    }
  }
  else {
    if ((sigma_udsg > std::numeric_limits<double>::epsilon()) || (sigma_udsg < -std::numeric_limits<double>::epsilon())) {
      // m_logger << DEBUG << "limit: " << std::numeric_limits<double>::epsilon() << " value: " << sigma << SLogger::endmsg;
      if (sigma_udsg > 0) {
        double scalefactor_up =  m_reader_veto_up->eval(flavorEnum, eta, pt_for_eval);
        scalefactor = sigmaScale_udsg*(scalefactor_up - scalefactor) + scalefactor;
      }
      else {
        double scalefactor_down =  m_reader_veto_down->eval(flavorEnum, eta, pt_for_eval);
        scalefactor = fabs(sigmaScale_udsg)*(scalefactor_down - scalefactor) + scalefactor;
      }
    }
  }
  if (scalefactor == 0) {
    throw SError( "Scale factor returned is zero!", SError::SkipCycle );
  }
  
  m_logger << DEBUG << "getting final weight for flavorEnum  " << flavorEnum << SLogger::endmsg;
  
  double jetweight = 1.;
  // set effMC close to one for now, need to use real value map later
  double effMC = getEfficiency(pt, eta, flavour, "jet_ak4");/// maybe change "jet_ak4_
  m_logger << DEBUG << "    Efficiency " << effMC <<SLogger::endmsg;
  if (isTagged) {
    m_logger << DEBUG << "     Jet is tagged " << SLogger::endmsg;
    jetweight *= scalefactor;
  }
  else {
    m_logger << DEBUG << "     Jet is not tagged " << SLogger::endmsg;
    jetweight *= (1 - (scalefactor * effMC)) / (1 - effMC);
  }
  
  m_logger << DEBUG << " jetweight " << jetweight << SLogger::endmsg;

  return jetweight;
}



double BTaggingScaleTool::getScaleFactor_tag( const double& pt, const double& eta, const int& flavour, bool isTagged, const double& sigma_bc, const double& sigma_udsg, const TString& jetCategory ) {
  m_logger << DEBUG << "     flavor " <<  flavour<<  SLogger::endmsg;
  // Flavor
  BTagEntry::JetFlavor flavorEnum = BTagEntry::FLAV_UDSG;
  
  if  ( fabs(flavour)==5) flavorEnum = BTagEntry::FLAV_B;
  if  ( fabs(flavour)==15) flavorEnum = BTagEntry::FLAV_C;
  if  ( fabs(flavour)==4) flavorEnum = BTagEntry::FLAV_C;

  double MaxEta = 2.4;
  double abs_eta = fabs(eta);
  if (abs_eta > MaxEta) {
    // outside tracker range
    return 1.;
  }
  
  // range checking, double uncertainty if beyond
  std::pair<float, float> sf_bounds = m_reader_tag->min_max_pt(flavorEnum, abs_eta);
  
  m_logger << DEBUG << "     flavorEnum  " << flavorEnum << " ;pt bound min " << sf_bounds.first << " ;pt bound max " << sf_bounds.second <<" jet pt "<< pt<<  SLogger::endmsg;

  float pt_for_eval = pt;
  bool is_out_of_bounds = false;
  if (pt < sf_bounds.first) {
    pt_for_eval = sf_bounds.first + 1e-5;
    is_out_of_bounds = true;
  } else if (pt >= sf_bounds.second) {
    pt_for_eval = sf_bounds.second - 0.1;
    is_out_of_bounds = true;
  }
  m_logger << DEBUG << "*******  out of band ? " << is_out_of_bounds << "so pt_for_eval_used " <<  pt_for_eval << SLogger::endmsg;
  double sigmaScale_bc = sigma_bc;
  double sigmaScale_udsg = sigma_udsg;
  // double uncertainty in case jet outside normal kinematics
  if (is_out_of_bounds) {
    m_logger << DEBUG << sf_bounds.first << " - " << sf_bounds.second << SLogger::endmsg;
    m_logger << DEBUG << "out of bounds, using: " << pt_for_eval << " and " << abs_eta << SLogger::endmsg;
    sigmaScale_bc *= 2;
    sigmaScale_udsg *= 2;
  }
  
 
  double scalefactor = m_reader_tag->eval(flavorEnum, eta, pt_for_eval);
  m_logger << DEBUG << "scale factor: " << scalefactor << SLogger::endmsg;
  if ((flavour == 5) || (flavour == 4)) {
    if ((sigma_bc > std::numeric_limits<double>::epsilon()) || (sigma_bc < -std::numeric_limits<double>::epsilon())) {
      // m_logger << DEBUG << "limit: " << std::numeric_limits<double>::epsilon() << " value: " << sigma << SLogger::endmsg;
      if (sigma_bc > 0) {
        double scalefactor_up =  m_reader_tag_up->eval(flavorEnum, eta, pt_for_eval);
        scalefactor = sigmaScale_bc*(scalefactor_up - scalefactor) + scalefactor;
      }
      else {
        double scalefactor_down =  m_reader_tag_down->eval(flavorEnum, eta, pt_for_eval);
        scalefactor = fabs(sigmaScale_bc)*(scalefactor_down - scalefactor) + scalefactor;
      }
    }
  }
  else {
    if ((sigma_udsg > std::numeric_limits<double>::epsilon()) || (sigma_udsg < -std::numeric_limits<double>::epsilon())) {
      // m_logger << DEBUG << "limit: " << std::numeric_limits<double>::epsilon() << " value: " << sigma << SLogger::endmsg;
      if (sigma_udsg > 0) {
        double scalefactor_up =  m_reader_tag_up->eval(flavorEnum, eta, pt_for_eval);
        scalefactor = sigmaScale_udsg*(scalefactor_up - scalefactor) + scalefactor;
      }
      else {
        double scalefactor_down =  m_reader_tag_down->eval(flavorEnum, eta, pt_for_eval);
        scalefactor = fabs(sigmaScale_udsg)*(scalefactor_down - scalefactor) + scalefactor;
      }
    }
  }
  if (scalefactor == 0) {
    throw SError( "Scale factor returned is zero!", SError::SkipCycle );
  }
  
  m_logger << DEBUG << "getting final weight for flavorEnum  " << flavorEnum << SLogger::endmsg;
  
  double jetweight = 1.;
  // set effMC close to one for now, need to use real value map later
  double effMC = getEfficiency(pt, eta, flavour, "jet_ak4");/// maybe change "jet_ak4_
  m_logger << DEBUG << "    Efficiency " << effMC <<SLogger::endmsg;
  if (isTagged) {
    m_logger << DEBUG << "     Jet is tagged " << SLogger::endmsg;
    jetweight *= scalefactor;
  }
  else {
    m_logger << DEBUG << "     Jet is not tagged " << SLogger::endmsg;
    jetweight *= (1 - (scalefactor * effMC)) / (1 - effMC);
  }
  
  m_logger << DEBUG << " jetweight " << jetweight << SLogger::endmsg;

  return jetweight;
}


double BTaggingScaleTool::getDoubleBScaleFactor( double pt, double eta, int flavour, float dbt, int sigma ) {
  m_logger << DEBUG << "     DoubleB with flavor " <<  flavour<<  SLogger::endmsg;
  // Flavor
  BTagEntry::JetFlavor flavorEnum = BTagEntry::FLAV_UDSG;
  
  if  ( fabs(flavour)==5) flavorEnum = BTagEntry::FLAV_B;
  if  ( fabs(flavour)==15) flavorEnum = BTagEntry::FLAV_C;
  if  ( fabs(flavour)==4) flavorEnum = BTagEntry::FLAV_C;
  
  bool isLoose = isDoubleBTagged_loose(dbt);
  bool isTight = isDoubleBTagged_tight(dbt);
  
  double sfLoose = getScaleFactorDoubleBTagger(m_workingPoint_loose, pt, sigma);
  double sfTight = getScaleFactorDoubleBTagger(m_workingPoint_tight, pt, sigma);
  
  std::string nameLoose(("doubleB_" + flavourToString(flavour) + "_" + m_workingPoint_loose).Data());
  std::string nameTight(("doubleB_" + flavourToString(flavour) + "_" + m_workingPoint_tight).Data());
  
  eta = std::min(eta, m_effMaps_loose[nameLoose].GetYaxis()->GetXmax()-0.1);
  eta = std::max(eta, m_effMaps_loose[nameLoose].GetYaxis()->GetXmin()+0.1);
  pt = std::min(pt, m_effMaps_loose[nameLoose].GetXaxis()->GetXmax()-0.1);

  double effLoose = m_effMaps_loose[nameLoose].GetBinContent(m_effMaps_loose[nameLoose].FindBin(pt, eta));//getEfficiency(pt, eta, flavour, "doubleB");
  double effTight = m_effMaps_tight[nameTight].GetBinContent(m_effMaps_tight[nameTight].FindBin(pt, eta));//getEfficiency(pt, eta, flavour, "doubleB");
  
  m_logger << INFO << "     DoubleB " << dbt << " -> tag: " << isLoose << "/" << isTight << ", sf: " << sfLoose << "/" << sfTight << ", eff: " << effLoose << "/" << effTight << SLogger::endmsg;
  
  double jetweight(1.);
  if(isTight) {
    m_logger << DEBUG << "     Jet is Tight" << SLogger::endmsg;
    jetweight = sfTight;
  }
  else if(isLoose) {
    m_logger << DEBUG << "     Jet is between Loose and Tight" << SLogger::endmsg;
    jetweight = (effLoose - effTight) != 0 ? (sfLoose*effLoose - sfTight*effTight) / (effLoose - effTight) : 1.;
  }
  else {
    m_logger << DEBUG << "     Jet is not Loose" << SLogger::endmsg;
    jetweight = (1. - effLoose) != 0 ? (1. - sfLoose*effLoose) / (1. - effLoose) : 1.;
  }
  
  return jetweight;
}





double BTaggingScaleTool::getScaleFactor( const UZH::Jet& jet, const double& sigma_bc, const double& sigma_udsg, const TString& jetCategory ) {

  double jetweight = getScaleFactor(jet.pt(), jet.eta(), jet.hadronFlavour(), isTagged(jet), sigma_bc, sigma_udsg, jetCategory);

  return jetweight;
  
}
double BTaggingScaleTool::getScaleFactor_veto( const UZH::Jet& jet, const double& sigma_bc, const double& sigma_udsg, const TString& jetCategory ) {

  double jetweight = getScaleFactor_veto(jet.pt(), jet.eta(), jet.hadronFlavour(), isTagged_veto(jet), sigma_bc, sigma_udsg, jetCategory);

  return jetweight;
  
}
double BTaggingScaleTool::getScaleFactor_tag( const UZH::Jet& jet, const double& sigma_bc, const double& sigma_udsg, const TString& jetCategory ) {

  double jetweight = getScaleFactor_tag(jet.pt(), jet.eta(), jet.hadronFlavour(), isTagged_tag(jet), sigma_bc, sigma_udsg, jetCategory);

  return jetweight;
  
}



double BTaggingScaleTool::getSoftdropSubjetScaleFactor( const UZH::Jet& jet, const double& sigma_bc, const double& sigma_udsg, const TString& jetCategory ) {

  double jetweight = 1;
  
  for (int i = 0; i < jet.subjet_softdrop_N(); ++i) {
    m_logger << DEBUG << "Looking at softdrop subjet " << i
	     << ", pT=" << jet.subjet_softdrop_pt()[i] << ", eta=" << jet.subjet_softdrop_eta()[i]
	     << SLogger::endmsg;
    jetweight *= getScaleFactor(jet.subjet_softdrop_pt()[i], jet.subjet_softdrop_eta()[i], jet.subjet_softdrop_hadronFlavour()[i], isTagged(jet.subjet_softdrop_csv()[i]), sigma_bc, sigma_udsg, jetCategory);
  }

  return jetweight;
  
}


//
// return scale for Jet collection
//
double BTaggingScaleTool::getScaleFactor( const UZH::JetVec& vJets, const double& sigma_bc, const double& sigma_udsg, const TString& jetCategory) {

  double scale = 1.;
  
  m_logger << DEBUG << "BTaggingScaleTool::getScaleFactor" << SLogger::endmsg;

  for (std::vector< UZH::Jet>::const_iterator itJet = vJets.begin(); itJet < vJets.end(); ++itJet) {
    m_logger << DEBUG << "Looking at jet " << itJet - vJets.begin()
	     << ", pT=" << (*itJet).pt() << ", eta=" << (*itJet).eta()
	     << SLogger::endmsg;

    scale *= getScaleFactor(*itJet, sigma_bc, sigma_udsg, jetCategory);
  }  

  m_logger << DEBUG << "BTaggingScaleTool::getScaleFactor done" << SLogger::endmsg;
  return scale;

}

double BTaggingScaleTool::getScaleFactor_veto( const UZH::JetVec& vJets, const double& sigma_bc, const double& sigma_udsg, const TString& jetCategory) {

  double scale = 1.;
  
  m_logger << DEBUG << "BTaggingScaleTool::getScaleFactor_veto" << SLogger::endmsg;

  for (std::vector< UZH::Jet>::const_iterator itJet = vJets.begin(); itJet < vJets.end(); ++itJet) {
    m_logger << DEBUG << "Looking at jet " << itJet - vJets.begin()
	     << ", pT=" << (*itJet).pt() << ", eta=" << (*itJet).eta()
	     << SLogger::endmsg;

    scale *= getScaleFactor_veto(*itJet, sigma_bc, sigma_udsg, jetCategory);
  }  

  m_logger << DEBUG << "BTaggingScaleTool::getScaleFactor_veto done and wigth tot is " << scale<<  SLogger::endmsg;
  return scale;

}

double BTaggingScaleTool::getScaleFactor_tag( const UZH::JetVec& vJets, const double& sigma_bc, const double& sigma_udsg, const TString& jetCategory) {

  double scale = 1.;
  
  m_logger << DEBUG << "BTaggingScaleTool::getScaleFactor_tag" << SLogger::endmsg;

  for (std::vector< UZH::Jet>::const_iterator itJet = vJets.begin(); itJet < vJets.end(); ++itJet) {
    m_logger << DEBUG << "Looking at jet " << itJet - vJets.begin()
	     << ", pT=" << (*itJet).pt() << ", eta=" << (*itJet).eta()
	     << SLogger::endmsg;

    scale *= getScaleFactor_tag(*itJet, sigma_bc, sigma_udsg, jetCategory);
  }  

  m_logger << DEBUG << "BTaggingScaleTool::getScaleFactor_tag done and wigth tot is " << scale<<  SLogger::endmsg;
  return scale;

}

//
// return scale for Jet collection
//
double BTaggingScaleTool::getSoftdropSubjetScaleFactor( const UZH::JetVec& vJets, const double& sigma_bc, const double& sigma_udsg, const TString& jetCategory ) {

  double scale = 1.;
  
  m_logger << DEBUG << "BTaggingScaleTool::getSoftdropSubjetScaleFactor" << SLogger::endmsg;

  for (std::vector< UZH::Jet>::const_iterator itJet = vJets.begin(); itJet < vJets.end(); ++itJet) {
    m_logger << DEBUG << "Looking at jet " << itJet - vJets.begin()
	     << ", pT=" << (*itJet).pt() << ", eta=" << (*itJet).eta()
	     << SLogger::endmsg;

    scale *= getSoftdropSubjetScaleFactor(*itJet, sigma_bc, sigma_udsg, jetCategory);
  }  

  m_logger << DEBUG << "BTaggingScaleTool::getSoftdropSubjetScaleFactor done" << SLogger::endmsg;
  return scale;

}



double BTaggingScaleTool::getScaleFactorDoubleBTagger(std::string wp, double pt, int i) {
    if(wp=="Loose") {
      if(pt < 250) return 0.96 + i*2*(i>0 ? 0.03 : 0.02);
      else if(pt < 350) return 0.96 + i*(i>0 ? 0.03 : 0.02);
      else if(pt < 430) return 1.00 + i*(i>0 ? 0.04 : 0.03);
      else if(pt < 840) return 1.01 + i*(i>0 ? 0.02 : 0.04);
      else return 1.01 + i*2*(i>0 ? 0.02 : 0.04);
      return 1.;
    }
    else if(wp=="Medium1") {
      if(pt < 250) return 0.93 + i*2*(i>0 ? 0.03 : 0.02);
      else if(pt < 350) return 0.93 + i*(i>0 ? 0.03 : 0.02);
      else if(pt < 430) return 1.01 + i*(i>0 ? 0.03 : 0.03);
      else if(pt < 840) return 0.99 + i*(i>0 ? 0.02 : 0.04);
      else return 0.99 + i*2*(i>0 ? 0.02 : 0.04);
      return 1.;
    }
    else if(wp=="Medium2") {
      if(pt < 250) return 0.92 + i*2*(i>0 ? 0.03 : 0.03);
      else if(pt < 350) return 0.92 + i*(i>0 ? 0.03 : 0.03);
      else if(pt < 430) return 1.01 + i*(i>0 ? 0.03 : 0.04);
      else if(pt < 840) return 0.92 + i*(i>0 ? 0.03 : 0.05);
      else return 0.92 + i*2*(i>0 ? 0.03 : 0.05);
      return 1.;
    }
    else if(wp=="Tight") {
      if(pt < 250) return 0.85 + i*2*(i>0 ? 0.03 : 0.03);
      else if(pt < 350) return 0.85 + i*(i>0 ? 0.03 : 0.03);
      else if(pt < 840) return 0.91 + i*(i>0 ? 0.03 : 0.04);
      else return 0.91 + i*2*(i>0 ? 0.03 : 0.04);
      return 1.;
    }
    m_logger << ERROR << "Working point " << wp << " not recognized" << SLogger::endmsg;
    return 1.;
}


/// function to book histograms for efficiencies
void BTaggingScaleTool::bookHistograms() {
  
  const int nPtBins = 18;
  const int nEtaBins = 4;
  float ptBins[nPtBins+1] = {10, 20, 30, 40, 50, 70, 100, 150, 200, 250, 300, 400, 500, 670, 1000, 1250, 1500, 2000, 3000};
  float etaBins[nEtaBins+1] = {-2.5, -1.5, 0, 1.5, 2.5};
  
  for (std::vector<TString>::const_iterator jetCat = m_jetCategories.begin(); jetCat != m_jetCategories.end(); ++jetCat) {
    for (std::vector<TString>::const_iterator flav = m_flavours.begin(); flav != m_flavours.end(); ++flav) {
      Book( TH2F( *jetCat + "_" + *flav + "_" + m_workingPoint, *jetCat + "_" + *flav + "_" + m_workingPoint, nPtBins, ptBins, nEtaBins, etaBins ), m_effHistDirectory.c_str() );
      Book( TH2F( *jetCat + "_" + *flav + "_all", *jetCat + "_" + *flav + "_all", nPtBins, ptBins, nEtaBins, etaBins ), m_effHistDirectory.c_str() );
    }
  }

  for (std::vector<TString>::const_iterator flav = m_flavours.begin(); flav != m_flavours.end(); ++flav) {
    Book( TH2F("jet_ak4_" + *flav + "_" + m_workingPoint_veto, "jet_ak4_" + *flav + "_" + m_workingPoint_veto, nPtBins, ptBins, nEtaBins, etaBins ), m_effHistDirectory.c_str() );
    Book( TH2F("jet_ak4_" + *flav + "_" + m_workingPoint_tag, "jet_ak4_" + *flav + "_" + m_workingPoint_tag, nPtBins, ptBins, nEtaBins, etaBins ), m_effHistDirectory.c_str() );
    Book( TH2F( "jet_ak4_" + *flav + "_all", "jet_ak4_" + *flav + "_all", nPtBins, ptBins, nEtaBins, etaBins ), m_effHistDirectory.c_str() );
    
    Book( TH2F("doubleB_" + *flav + "_" + m_workingPoint_loose, "doubleB_" + *flav + "_" + m_workingPoint_loose, nPtBins, ptBins, nEtaBins, etaBins ), m_effHistDirectory.c_str() );
    Book( TH2F("doubleB_" + *flav + "_" + m_workingPoint_tight, "doubleB_" + *flav + "_" + m_workingPoint_tight, nPtBins, ptBins, nEtaBins, etaBins ), m_effHistDirectory.c_str() );
    Book( TH2F("doubleB_" + *flav + "_all", "doubleB_" + *flav + "_all", nPtBins, ptBins, nEtaBins, etaBins ), m_effHistDirectory.c_str() );
  }

  
}


/// function to fill jet b-tagging efficiencies
void BTaggingScaleTool::fillEfficiencies( const UZH::JetVec& vJets ) {
  
  for (std::vector< UZH::Jet>::const_iterator itJet = vJets.begin(); itJet < vJets.end(); ++itJet) {
    m_logger << DEBUG << "Looking at jet " << itJet - vJets.begin()
	     << ", pT=" << (*itJet).pt() << ", eta=" << (*itJet).eta()
	     << SLogger::endmsg;
    TString flavourString = flavourToString(itJet->hadronFlavour());
    if (isTagged(*itJet)) {
      Hist( "jet_" + flavourString + "_" + m_workingPoint, m_effHistDirectory.c_str() )->Fill( itJet->pt(), itJet->eta() );
    }
    Hist( "jet_" + flavourString + "_all", m_effHistDirectory.c_str() )->Fill( itJet->pt(), itJet->eta() );
  }
  
}

/// function to fill jet b-tagging efficiencies for Ak4 used in veto
void BTaggingScaleTool::fillEfficiencies_veto( const UZH::JetVec& vJets ) {
  
  for (std::vector< UZH::Jet>::const_iterator itJet = vJets.begin(); itJet < vJets.end(); ++itJet) {
    m_logger << DEBUG << "Looking at jet " << itJet - vJets.begin()
	     << ", pT=" << (*itJet).pt() << ", eta=" << (*itJet).eta()
	     << SLogger::endmsg;
    TString flavourString = flavourToString(itJet->hadronFlavour());
    if (isTagged_veto(*itJet)) {
      Hist( "jet_ak4_" + flavourString + "_" + m_workingPoint_veto, m_effHistDirectory.c_str() )->Fill( itJet->pt(), itJet->eta() );
    }
    Hist( "jet_ak4_" + flavourString + "_all", m_effHistDirectory.c_str() )->Fill( itJet->pt(), itJet->eta() );
  }
  
}

/// function to fill jet b-tagging efficiencies for Ak4 used in tag
void BTaggingScaleTool::fillEfficiencies_tag( const UZH::JetVec& vJets ) {
  
  for (std::vector< UZH::Jet>::const_iterator itJet = vJets.begin(); itJet < vJets.end(); ++itJet) {
    m_logger << DEBUG << "Looking at jet " << itJet - vJets.begin()
	     << ", pT=" << (*itJet).pt() << ", eta=" << (*itJet).eta()
	     << SLogger::endmsg;
    TString flavourString = flavourToString(itJet->hadronFlavour());
    if (isTagged_tag(*itJet)) {
      Hist( "jet_ak4_" + flavourString + "_" + m_workingPoint_tag, m_effHistDirectory.c_str() )->Fill( itJet->pt(), itJet->eta() );
    }
    Hist( "jet_ak4_" + flavourString + "_all", m_effHistDirectory.c_str() )->Fill( itJet->pt(), itJet->eta() );
  }
  
}



/// function to fill subjet b-tagging efficiencies
void BTaggingScaleTool::fillSoftdropSubjetEfficiencies( const UZH::JetVec& vJets ) {
  
  for (std::vector< UZH::Jet>::const_iterator itJet = vJets.begin(); itJet < vJets.end(); ++itJet) {
    m_logger << DEBUG << "Looking at jet " << itJet - vJets.begin()
	     << ", pT=" << (*itJet).pt() << ", eta=" << (*itJet).eta()
	     << SLogger::endmsg;
    for (int i = 0; i < itJet->subjet_softdrop_N(); ++i) {
      m_logger << DEBUG << "Looking at softdrop subjet " << i
  	     << ", pT=" << itJet->subjet_softdrop_pt()[i] << ", eta=" << itJet->subjet_softdrop_eta()[i]
  	     << SLogger::endmsg;
      TString flavourString = flavourToString(itJet->subjet_softdrop_hadronFlavour()[i]);
      if (isTagged(itJet->subjet_softdrop_csv()[i])) {
        Hist( "subjet_softdrop_" + flavourString + "_" + m_workingPoint, m_effHistDirectory.c_str() )->Fill( itJet->subjet_softdrop_pt()[i], itJet->subjet_softdrop_eta()[i] );
      }
      Hist( "subjet_softdrop_" + flavourString + "_all", m_effHistDirectory.c_str() )->Fill( itJet->subjet_softdrop_pt()[i], itJet->subjet_softdrop_eta()[i] );
    }
  }
  
}

/// function to fill jet b-tagging efficiencies
void BTaggingScaleTool::fillDoubleBEfficiencies_loose( const UZH::JetVec& vJets ) {
  
  for (std::vector< UZH::Jet>::const_iterator itJet = vJets.begin(); itJet < vJets.end(); ++itJet) {
    m_logger << DEBUG << "Looking at jet " << itJet - vJets.begin()
	     << ", pT=" << (*itJet).pt() << ", eta=" << (*itJet).eta()
	     << SLogger::endmsg;
    TString flavourString = flavourToString(itJet->hadronFlavour());
    if (isDoubleBTagged_loose(*itJet)) {
      Hist( "doubleB_" + flavourString + "_" + m_workingPoint_loose, m_effHistDirectory.c_str() )->Fill( itJet->pt(), itJet->eta() );
    }
    Hist( "doubleB_" + flavourString + "_all", m_effHistDirectory.c_str() )->Fill( itJet->pt(), itJet->eta() );
  }
  
}

/// function to fill jet b-tagging efficiencies
void BTaggingScaleTool::fillDoubleBEfficiencies_tight( const UZH::JetVec& vJets ) {
  
  for (std::vector< UZH::Jet>::const_iterator itJet = vJets.begin(); itJet < vJets.end(); ++itJet) {
    m_logger << DEBUG << "Looking at jet " << itJet - vJets.begin()
	     << ", pT=" << (*itJet).pt() << ", eta=" << (*itJet).eta()
	     << SLogger::endmsg;
    TString flavourString = flavourToString(itJet->hadronFlavour());
    if (isDoubleBTagged_tight(*itJet)) {
      Hist( "doubleB_" + flavourString + "_" + m_workingPoint_tight, m_effHistDirectory.c_str() )->Fill( itJet->pt(), itJet->eta() );
    }
    Hist( "doubleB_" + flavourString + "_all", m_effHistDirectory.c_str() )->Fill( itJet->pt(), itJet->eta() );
  }
  
}


/// function to read efficiencies
void BTaggingScaleTool::readEfficiencies() {
  
  m_logger << INFO << "Reading in b-tagging efficiencies from file " << m_effFile << SLogger::endmsg;
  auto inFile = TFile::Open(m_effFile.c_str());
  
  for (std::vector<TString>::iterator jetCat = m_jetCategories.begin(); jetCat != m_jetCategories.end(); ++jetCat) {
    for (std::vector<TString>::const_iterator flav = m_flavours.begin(); flav != m_flavours.end(); ++flav) {
      auto hPass = (TH2F*) inFile->Get( m_effHistDirectory + "/" + *jetCat + "_" + *flav + "_" + m_workingPoint);
      auto hAll = (TH2F*) inFile->Get( m_effHistDirectory + "/" + *jetCat + "_" + *flav + "_all");
      if(hPass && hAll) {
        TH2F hEff = *((TH2F*) hPass->Clone( m_effHistDirectory + "_" + *jetCat + "_" + *flav + "_" + m_workingPoint ));
        hEff.Divide(hAll);
        // delete hPass;
        // delete hAll;
        m_effMaps[(*jetCat + "_" + *flav + "_" + m_workingPoint).Data()] = hEff;
        m_logger << DEBUG << "effi TH2D binsx: " << hEff.GetNbinsX() << " binsy: " << hEff.GetNbinsY() << SLogger::endmsg;
      }
      else {
        m_logger << WARNING << "effi TH2D histo: " << m_effHistDirectory + "/" + *jetCat + "_" + *flav + "_" + m_workingPoint << " does not exist" << SLogger::endmsg;
      }
    }
  }
  inFile->Close();
  // delete inFile;
  
//  m_logger << INFO << "For Veto: Reading in b-tagging efficiencies from file " << m_effFile_veto << SLogger::endmsg;
//  auto inFile_veto = TFile::Open(m_effFile_veto.c_str());
//  
//  for (std::vector<TString>::const_iterator flav = m_flavours.begin(); flav != m_flavours.end(); ++flav) {
//    auto hPass_veto = (TH2F*) inFile_veto->Get( m_effHistDirectory + "/" + "jet_ak4_" + *flav + "_" + m_workingPoint_veto);
//    auto hAll_veto = (TH2F*) inFile_veto->Get( m_effHistDirectory + "/" + "jet_ak4_" + *flav + "_all");
//    TH2F hEff_veto = *((TH2F*) hPass_veto->Clone( m_effHistDirectory + "_jet_ak4_" + *flav + "_" + m_workingPoint_veto ));
//    hEff_veto.Divide(hAll_veto);
//    // delete hPass;
//    // delete hAll;
//    m_effMaps_veto[( "jet_ak4_" + *flav + "_" + m_workingPoint_veto).Data()] = hEff_veto;
//    m_logger << DEBUG << "effi Veto TH2D binsx: " << hEff_veto.GetNbinsX() << " binsy: " << hEff_veto.GetNbinsY() << SLogger::endmsg;
//  }
//  
//  inFile_veto->Close();

//  
//  m_logger << INFO << "For Tag: Reading in b-tagging efficiencies from file " << m_effFile_tag << SLogger::endmsg;
//  auto inFile_tag = TFile::Open(m_effFile_tag.c_str());
//  
//  for (std::vector<TString>::const_iterator flav = m_flavours.begin(); flav != m_flavours.end(); ++flav) {
//    auto hPass_tag = (TH2F*) inFile_tag->Get( m_effHistDirectory + "/" + "jet_ak4_" + *flav + "_" + m_workingPoint_tag);
//    auto hAll_tag = (TH2F*) inFile_tag->Get( m_effHistDirectory + "/" + "jet_ak4_" + *flav + "_all");
//    TH2F hEff_tag = *((TH2F*) hPass_tag->Clone( m_effHistDirectory + "_jet_ak4_" + *flav + "_" + m_workingPoint_tag ));
//    hEff_tag.Divide(hAll_tag);
//    // delete hPass;
//    // delete hAll;
//    m_effMaps_tag[( "jet_ak4_" + *flav + "_" + m_workingPoint_tag).Data()] = hEff_tag;
//    m_logger << DEBUG << "effi Tag TH2D binsx: " << hEff_tag.GetNbinsX() << " binsy: " << hEff_tag.GetNbinsY() << SLogger::endmsg;
//  }
//  
//  inFile_tag->Close();

  m_logger << INFO << "For doubleB Loose: Reading in b-tagging efficiencies from file " << m_effFile_loose << SLogger::endmsg;
  auto inFile_loose = TFile::Open(m_effFile_loose.c_str());
  
  for (std::vector<TString>::const_iterator flav = m_flavours.begin(); flav != m_flavours.end(); ++flav) {
    auto hPass_loose = (TH2F*) inFile_loose->Get( m_effHistDirectory + "/" + "doubleB_" + *flav + "_" + m_workingPoint_loose);
    auto hAll_loose = (TH2F*) inFile_loose->Get( m_effHistDirectory + "/" + "doubleB_" + *flav + "_all");
    if(hPass_loose && hAll_loose) {
      TH2F hEff_loose = *((TH2F*) hPass_loose->Clone( m_effHistDirectory + "_doubleB_" + *flav + "_" + m_workingPoint_loose ));
      hEff_loose.Divide(hAll_loose);
      // delete hPass;
      // delete hAll;
      m_effMaps_loose[( "doubleB_" + *flav + "_" + m_workingPoint_loose).Data()] = hEff_loose;
      m_logger << DEBUG << "effi Loose TH2D binsx: " << hEff_loose.GetNbinsX() << " binsy: " << hEff_loose.GetNbinsY() << SLogger::endmsg;
    }
    else {
      m_logger << WARNING << "effi TH2D histo: " << m_effHistDirectory + "/" + "doubleB_" + *flav + "_" + m_workingPoint_loose << " does not exist" << SLogger::endmsg;
    }
  }
  
  inFile_loose->Close();
  
  m_logger << INFO << "For doubleB tight: Reading in b-tagging efficiencies from file " << m_effFile_tight << SLogger::endmsg;
  auto inFile_tight = TFile::Open(m_effFile_tight.c_str());
  
  for (std::vector<TString>::const_iterator flav = m_flavours.begin(); flav != m_flavours.end(); ++flav) {
    auto hPass_tight = (TH2F*) inFile_tight->Get( m_effHistDirectory + "/" + "doubleB_" + *flav + "_" + m_workingPoint_tight);
    auto hAll_tight = (TH2F*) inFile_tight->Get( m_effHistDirectory + "/" + "doubleB_" + *flav + "_all");
    if(hPass_tight && hAll_tight) {
      TH2F hEff_tight = *((TH2F*) hPass_tight->Clone( m_effHistDirectory + "_doubleB_" + *flav + "_" + m_workingPoint_tight ));
      hEff_tight.Divide(hAll_tight);
      // delete hPass;
      // delete hAll;
      m_effMaps_tight[( "doubleB_" + *flav + "_" + m_workingPoint_tight).Data()] = hEff_tight;
      m_logger << DEBUG << "effi tight TH2D binsx: " << hEff_tight.GetNbinsX() << " binsy: " << hEff_tight.GetNbinsY() << SLogger::endmsg;
    }
    else {
      m_logger << WARNING << "effi TH2D histo: " << m_effHistDirectory + "/" + "doubleB_" + *flav + "_" + m_workingPoint_tight << " does not exist" << SLogger::endmsg;
    }
  }
  
  inFile_tight->Close();
}

double BTaggingScaleTool::getEfficiency( const double& pt, const double& eta, const int& flavour, const TString& jetCategory ) {
  double eff = 1.;
  int binx(0), biny(0);
  if (jetCategory!="jet_ak4"){
 
    TH2F thisHist = m_effMaps[(jetCategory + "_" + flavourToString(flavour) + "_" + m_workingPoint).Data()];
    m_logger << DEBUG << /*thisHist << " " << */ thisHist.GetName() << SLogger::endmsg;
    binx = thisHist.GetXaxis()->FindBin(pt);
    biny = thisHist.GetYaxis()->FindBin(eta);
    m_logger << DEBUG << "binx = " << binx << " biny = " << biny << SLogger::endmsg;
    m_logger << DEBUG << "maxx = " << thisHist.GetNbinsX() << " maxy = " << thisHist.GetNbinsY() << SLogger::endmsg;
    // implement check for overflow
    eff = thisHist.GetBinContent(binx, biny);
    m_logger << DEBUG << "For "<< jetCategory << " with pt = " << pt << ", eta = " << eta << ", flavour = " << flavour << " returning efficiency =" << eff << SLogger::endmsg;
 
  }else{
    
    TH2F thisHist_veto = m_effMaps_veto[("jet_ak4_" + flavourToString(flavour) + "_" + m_workingPoint_veto).Data()];
    m_logger << DEBUG << /*thisHist << " " << */ thisHist_veto.GetName() << SLogger::endmsg;
    binx = thisHist_veto.GetXaxis()->FindBin(pt);
    biny = thisHist_veto.GetYaxis()->FindBin(eta);
    m_logger << DEBUG << "binx = " << binx << " biny = " << biny << SLogger::endmsg;
    m_logger << DEBUG << "maxx = " << thisHist_veto.GetNbinsX() << " maxy = " << thisHist_veto.GetNbinsY() << SLogger::endmsg;
    // implement check for overflow
    eff = thisHist_veto.GetBinContent(binx, biny);
    m_logger << DEBUG << "For "<< jetCategory << " with pt = " << pt << ", eta = " << eta << ", flavour = " << flavour << " returning efficiency =" << eff << SLogger::endmsg;
    
    TH2F thisHist_tag = m_effMaps_tag[("jet_ak4_" + flavourToString(flavour) + "_" + m_workingPoint_tag).Data()];
    m_logger << DEBUG << /*thisHist << " " << */ thisHist_tag.GetName() << SLogger::endmsg;
    binx = thisHist_tag.GetXaxis()->FindBin(pt);
    biny = thisHist_tag.GetYaxis()->FindBin(eta);
    m_logger << DEBUG << "binx = " << binx << " biny = " << biny << SLogger::endmsg;
    m_logger << DEBUG << "maxx = " << thisHist_tag.GetNbinsX() << " maxy = " << thisHist_tag.GetNbinsY() << SLogger::endmsg;
    // implement check for overflow
    eff = thisHist_tag.GetBinContent(binx, biny);
    m_logger << DEBUG << "For "<< jetCategory << " with pt = " << pt << ", eta = " << eta << ", flavour = " << flavour << " returning efficiency =" << eff << SLogger::endmsg;
  }
  return eff;
  
}



TString BTaggingScaleTool::flavourToString( const int& flavour ) {
  
  TString flavourString = "udsg";
  
  if (flavour == 5) {
    flavourString = "b";
  }
  else if (flavour == 4) {
    flavourString = "c";
  }
  // else if (flavour == 15) {
  //   flavourString = "c"; // Use C scale factors for tau for now.
  // }
  
  return flavourString;
  
}


bool BTaggingScaleTool::isTagged( const UZH::Jet& jet ) {
  
  if (jet.csv() > currentWorkingPointCut) {
    return true;  
  }
  return false;
  
}


bool BTaggingScaleTool::isTagged( const double& csv ) {
  
  if (csv > currentWorkingPointCut) {
    return true;  
  }
  return false;
  
}


bool BTaggingScaleTool::isTagged_veto(  const UZH::Jet& jet  ) {
  
  if (jet.csv() > currentWorkingPointCut_veto) {
    //std::cout << " jet.csv() " <<jet.csv() << " > currentWorkingPointCut_veto " << currentWorkingPointCut_veto << std::endl;
    return true;  
  }
  return false;
  
}
bool BTaggingScaleTool::isTagged_veto( const double& csv ) {
  
  if (csv > currentWorkingPointCut_veto) {
    //std::cout << " csv " <<csv << " > currentWorkingPointCut_veto " << currentWorkingPointCut_veto << std::endl;
    return true;  
  }
  return false;
  
}

bool BTaggingScaleTool::isTagged_tag(  const UZH::Jet& jet  ) {
  
  if (jet.csv() > currentWorkingPointCut_tag) {
    //std::cout << " jet.csv() " <<jet.csv() << " > currentWorkingPointCut_tag " << currentWorkingPointCut_tag << std::endl;
    return true;  
  }
  return false;
  
}
bool BTaggingScaleTool::isTagged_tag( const double& csv ) {
  
  if (csv > currentWorkingPointCut_tag) {
    //std::cout << " csv " <<csv << " > currentWorkingPointCut_tag " << currentWorkingPointCut_tag << std::endl;
    return true;  
  }
  return false;
  
}


bool BTaggingScaleTool::isDoubleBTagged_loose(  const UZH::Jet& jet  ) {
  
  if (jet.Hbbtag() > currentWorkingPointCut_loose) {
    //std::cout << " jet.csv() " <<jet.csv() << " > currentWorkingPointCut_tag " << currentWorkingPointCut_tag << std::endl;
    return true;  
  }
  return false;
  
}

bool BTaggingScaleTool::isDoubleBTagged_loose( const double& dbt ) {
  
  if (dbt > currentWorkingPointCut_loose) {
    //std::cout << " csv " <<csv << " > currentWorkingPointCut_tag " << currentWorkingPointCut_tag << std::endl;
    return true;  
  }
  return false;
  
}

bool BTaggingScaleTool::isDoubleBTagged_tight(  const UZH::Jet& jet  ) {
  
  if (jet.Hbbtag() > currentWorkingPointCut_tight) {
    //std::cout << " jet.csv() " <<jet.csv() << " > currentWorkingPointCut_tag " << currentWorkingPointCut_tag << std::endl;
    return true;  
  }
  return false;
  
}


bool BTaggingScaleTool::isDoubleBTagged_tight( const double& dbt ) {
  
  if (dbt > currentWorkingPointCut_tight) {
    //std::cout << " csv " <<csv << " > currentWorkingPointCut_tag " << currentWorkingPointCut_tag << std::endl;
    return true;  
  }
  return false;
  
}
