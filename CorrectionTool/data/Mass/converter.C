{
  TFile* inFile = new TFile("puppiCorr.root", "READ");
//  inFile->cd();
  inFile->ls();
  TF1* puppiJECcorr_gen = (TF1*)inFile->Get("puppiJECcorr_gen");
  TF1* puppiJECcorr_reco_0eta1v3 = (TF1*)inFile->Get("puppiJECcorr_reco_0eta1v3");
  TF1* puppiJECcorr_reco_1v3eta2v5 = (TF1*)inFile->Get("puppiJECcorr_reco_1v3eta2v5");
  
  TFile* outFile = new TFile("puppiCorr_Moriond17.root", "RECREATE");
  outFile->cd();
  puppiJECcorr_gen->Write("puppiJECcorr_gen");
  puppiJECcorr_reco_0eta1v3->Write("puppiJECcorr_reco_0eta1v3");
  puppiJECcorr_reco_1v3eta2v5->Write("puppiJECcorr_reco_1v3eta2v5");
  outFile->ls();
  outFile->Close();
}
