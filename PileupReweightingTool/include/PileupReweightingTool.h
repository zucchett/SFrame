#ifndef PileupReweightingTool_h
#define PileupReweightingTool_h

#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
// SFrame include(s):
#include "core/include/SError.h"
#include "plug-ins/include/SToolBase.h"

//#include "PUWeight.h"

//namespace Root{
//  class TPileupReweighting;
//}

class PileupReweightingTool : public SToolBase {

    public:
        /// constructor
        PileupReweightingTool( SCycleBase* parent, const char* name = "PileupReweightingTool" );

        /// destructor
        ~PileupReweightingTool();

        /// open input file
        void BeginInputData( const SInputData& id ) throw( SError );

        /// get weight
        double getPileUpweight(int, int=0);


    private:
        std::string m_name;                 ///< name of the tool
        //PUWeight m_puWeight;      ///< instance of PUWeight class

        // tool properties
        std::string m_MCRootFileName;
        std::string m_MCRootHistName;
        std::string m_DataRootFileName;
        std::string m_DataRootHistName;
        std::string m_DataRootFileNameUp;
        std::string m_DataRootHistNameUp;
        std::string m_DataRootFileNameDown;
        std::string m_DataRootHistNameDown;
        TFile* MCRootFile;
        TFile* DataRootFile;
        TFile* DataRootFileUp;
        TFile* DataRootFileDown;
        TH1F* MCRootHist;
        TH1F* DataRootHist;
        TH1F* DataRootHistUp;
        TH1F* DataRootHistDown;
        
};


#endif //  __PILEUPREWEIGHTINGTTOOL_H__
