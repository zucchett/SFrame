// $Id: SETUP.C 376 2013-12-02 16:35:13Z krasznaa $
/***************************************************************************
 * @Project: SFrame - ROOT-based analysis framework for ATLAS
 * @Package: User
 *
 * @author Stefan Ask       <Stefan.Ask@cern.ch>           - Manchester
 * @author David Berge      <David.Berge@cern.ch>          - CERN
 * @author Johannes Haller  <Johannes.Haller@cern.ch>      - Hamburg
 * @author A. Krasznahorkay <Attila.Krasznahorkay@cern.ch> - CERN/Debrecen
 *
 ***************************************************************************/

// ROOT include(s):
#include <TSystem.h>
#include <TString.h>

/**
 * This function loads all the libraries needed for SFrameUser (including
 * SFrameUser itself) on the PROOF master and worker nodes.
 */
int SETUP() {

   /// Add global path if provided -> skip worker compilation!
   const TString path = gSystem->Getenv( "ROOTPROOFLIBDIR" );
   if( path != "" ) {
      const TString libpath = gSystem->GetDynamicPath();
      if( ! libpath.Contains( path ) ) {
         gSystem->AddDynamicPath( path );
      }
   }

   if( gSystem->Load( "libTree" ) == -1 ) return -1;
   if( gSystem->Load( "libHist" ) == -1 ) return -1;
   if( gSystem->Load( "libGraf" ) == -1 ) return -1;
   if( gSystem->Load( "libSFrameUser" ) == -1 ) return -1;

   return 0;
}
