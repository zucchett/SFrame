void functions() {
    std::cout << "  C++ functions loaded" << std::endl;
}

float DeltaPhi(float phi1, float phi2) {
    float dPhi = fabs(phi1-phi2);
    return dPhi <= 3.14159265 ? dPhi : 2*3.14159265-dPhi;
}

float DeltaR(float eta1, float phi1, float eta2, float phi2) {
    float dEta = eta2-eta1;
    float dPhi = fabs(phi1-phi2);
    if(dPhi > 3.14159265) dPhi = 2*3.14159265-dPhi;
    return sqrt(dEta*dEta + dPhi*dPhi);
}

float Tmass(float pt1, float phi1, float pt2, float phi2) {
    float dPhi = fabs(phi1-phi2);
    if(dPhi > 3.14159265) dPhi = 2*3.14159265-dPhi;
    return sqrt(2*pt1*pt2*(1-cos(dPhi)));
}

float ratio_func(float var1, float var2){
  float r = 0;
  if(var2>0) r = var1/var2;
  return r;
}

