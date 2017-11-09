#include "../include/kfactors.h"

//--------------------------------------------------------------------------------------
double getEWKW(const double pt)
{
  double weight = 1;
  if     (pt < 170)              { weight = 0.94269; }
  else if(pt>=170  && pt < 200)  { weight = 0.902615; }
  else if(pt>=200  && pt < 230)  { weight = 0.898827; }
  else if(pt>=230  && pt < 260)  { weight = 0.959081; }
  else if(pt>=260  && pt < 290)  { weight = 0.891248; }
  else if(pt>=290  && pt < 320)  { weight = 0.860188; }
  else if(pt>=320  && pt < 350)  { weight = 0.884811; }
  else if(pt>=350  && pt < 390)  { weight = 0.868131; }
  else if(pt>=390  && pt < 430)  { weight = 0.848655; }
  else if(pt>=430  && pt < 470)  { weight = 0.806186; }
  else if(pt>=470  && pt < 510)  { weight = 0.848507; }
  else if(pt>=510  && pt < 550)  { weight = 0.83763; }
  else if(pt>=550  && pt < 590)  { weight = 0.792152; }
  else if(pt>=590  && pt < 640)  { weight = 0.730731; }
  else if(pt>=640  && pt < 690)  { weight = 0.778061; }
  else if(pt>=690  && pt < 740)  { weight = 0.771811; }
  else if(pt>=740  && pt < 790)  { weight = 0.795004; }
  else if(pt>=790  && pt < 840)  { weight = 0.757859; }
  else if(pt>=840  && pt < 900)  { weight = 0.709571; }
  else if(pt>=900  && pt < 960)  { weight = 0.702751; }
  else if(pt>=960  && pt < 1020) { weight = 0.657821; }
  else if(pt>=1020 && pt < 1090) { weight = 0.762559; }
  else if(pt>=1090 && pt < 1160) { weight = 0.845925; }
  else if(pt>=1160)              { weight = 0.674034; }

  return weight;
}

double getQCDW(const double pt)
{
  double weight = 1;
  if     (pt < 170)              { weight = 1.43896; }
  else if(pt>=170  && pt < 200)  { weight = 1.45307; }
  else if(pt>=200  && pt < 230)  { weight = 1.41551; }
  else if(pt>=230  && pt < 260)  { weight = 1.42199; }
  else if(pt>=260  && pt < 290)  { weight = 1.3477; }
  else if(pt>=290  && pt < 320)  { weight = 1.35302; }
  else if(pt>=320  && pt < 350)  { weight = 1.34289; }
  else if(pt>=350  && pt < 390)  { weight = 1.32474; }
  else if(pt>=390  && pt < 430)  { weight = 1.23267; }
  else if(pt>=430  && pt < 470)  { weight = 1.22641; }
  else if(pt>=470  && pt < 510)  { weight = 1.23149; }
  else if(pt>=510  && pt < 550)  { weight = 1.21593; }
  else if(pt>=550  && pt < 590)  { weight = 1.16506; }
  else if(pt>=590  && pt < 640)  { weight = 1.01718; }
  else if(pt>=640  && pt < 690)  { weight = 1.01575; }
  else if(pt>=690  && pt < 740)  { weight = 1.05425; }
  else if(pt>=740  && pt < 790)  { weight = 1.05992; }
  else if(pt>=790  && pt < 840)  { weight = 1.01503; }
  else if(pt>=840  && pt < 900)  { weight = 1.01761; }
  else if(pt>=900  && pt < 960)  { weight = 0.947194; }
  else if(pt>=960  && pt < 1020) { weight = 0.932754; }
  else if(pt>=1020 && pt < 1090) { weight = 1.00849; }
  else if(pt>=1090 && pt < 1160) { weight = 0.94805; }
  else if(pt>=1160)              { weight = 0.86956; }

  return weight;
}

double getRenUpW(const double pt)
{
  double weight = 1;
  if     (pt < 170)              { weight = 1.33939; }
  else if(pt>=170  && pt < 200)  { weight = 1.34649; }
  else if(pt>=200  && pt < 230)  { weight = 1.3115; }
  else if(pt>=230  && pt < 260)  { weight = 1.30782; }
  else if(pt>=260  && pt < 290)  { weight = 1.24619; }
  else if(pt>=290  && pt < 320)  { weight = 1.24531; }
  else if(pt>=320  && pt < 350)  { weight = 1.23472; }
  else if(pt>=350  && pt < 390)  { weight = 1.20709; }
  else if(pt>=390  && pt < 430)  { weight = 1.13368; }
  else if(pt>=430  && pt < 470)  { weight = 1.12227; }
  else if(pt>=470  && pt < 510)  { weight = 1.12612; }
  else if(pt>=510  && pt < 550)  { weight = 1.1118; }
  else if(pt>=550  && pt < 590)  { weight = 1.06549; }
  else if(pt>=590  && pt < 640)  { weight = 0.931838; }
  else if(pt>=640  && pt < 690)  { weight = 0.929282; }
  else if(pt>=690  && pt < 740)  { weight = 0.959553; }
  else if(pt>=740  && pt < 790)  { weight = 0.955823; }
  else if(pt>=790  && pt < 840)  { weight = 0.920614; }
  else if(pt>=840  && pt < 900)  { weight = 0.917243; }
  else if(pt>=900  && pt < 960)  { weight = 0.855649; }
  else if(pt>=960  && pt < 1020) { weight = 0.84587; }
  else if(pt>=1020 && pt < 1090) { weight = 0.906862; }
  else if(pt>=1090 && pt < 1160) { weight = 0.858763; }
  else if(pt>=1160)              { weight = 0.794909; }

  return weight;
}

double getRenDownW(const double pt)
{
  double weight = 1; 
  if     (pt < 170)              { weight = 1.52924; }
  else if(pt>=170  && pt < 200)  { weight = 1.55189; }
  else if(pt>=200  && pt < 230)  { weight = 1.51041; }
  else if(pt>=230  && pt < 260)  { weight = 1.53402; }
  else if(pt>=260  && pt < 290)  { weight = 1.44328; }
  else if(pt>=290  && pt < 320)  { weight = 1.45859; }
  else if(pt>=320  && pt < 350)  { weight = 1.4486; }
  else if(pt>=350  && pt < 390)  { weight = 1.44818; }
  else if(pt>=390  && pt < 430)  { weight = 1.33058; }
  else if(pt>=430  && pt < 470)  { weight = 1.33239; }
  else if(pt>=470  && pt < 510)  { weight = 1.33976; }
  else if(pt>=510  && pt < 550)  { weight = 1.32336; }
  else if(pt>=550  && pt < 590)  { weight = 1.26854; }
  else if(pt>=590  && pt < 640)  { weight = 1.10489; }
  else if(pt>=640  && pt < 690)  { weight = 1.1055; }
  else if(pt>=690  && pt < 740)  { weight = 1.15559; }
  else if(pt>=740  && pt < 790)  { weight = 1.17682; }
  else if(pt>=790  && pt < 840)  { weight = 1.118; }
  else if(pt>=840  && pt < 900)  { weight = 1.13097; }
  else if(pt>=900  && pt < 960)  { weight = 1.04988; }
  else if(pt>=960  && pt < 1020) { weight = 1.02796; }
  else if(pt>=1020 && pt < 1090) { weight = 1.12438; }
  else if(pt>=1090 && pt < 1160) { weight = 1.04704; }
  else if(pt>=1160)              { weight = 0.94791; }

  return weight;
}

double getFacUpW(const double pt)
{
  double weight = 1; 
  if     (pt < 170)              { weight = 1.44488; }
  else if(pt>=170  && pt < 200)  { weight = 1.45847; }
  else if(pt>=200  && pt < 230)  { weight = 1.41004; }
  else if(pt>=230  && pt < 260)  { weight = 1.39864; }
  else if(pt>=260  && pt < 290)  { weight = 1.3391; }
  else if(pt>=290  && pt < 320)  { weight = 1.33663; }
  else if(pt>=320  && pt < 350)  { weight = 1.32073; }
  else if(pt>=350  && pt < 390)  { weight = 1.3076; }
  else if(pt>=390  && pt < 430)  { weight = 1.20904; }
  else if(pt>=430  && pt < 470)  { weight = 1.20066; }
  else if(pt>=470  && pt < 510)  { weight = 1.20462; }
  else if(pt>=510  && pt < 550)  { weight = 1.18286; }
  else if(pt>=550  && pt < 590)  { weight = 1.12586; }
  else if(pt>=590  && pt < 640)  { weight = 0.990615; }
  else if(pt>=640  && pt < 690)  { weight = 0.984473; }
  else if(pt>=690  && pt < 740)  { weight = 1.0171; }
  else if(pt>=740  && pt < 790)  { weight = 1.01706; }
  else if(pt>=790  && pt < 840)  { weight = 0.986107; }
  else if(pt>=840  && pt < 900)  { weight = 0.972452; }
  else if(pt>=900  && pt < 960)  { weight = 0.910183; }
  else if(pt>=960  && pt < 1020) { weight = 0.885284; }
  else if(pt>=1020 && pt < 1090) { weight = 0.950662; }
  else if(pt>=1090 && pt < 1160) { weight = 0.89605; }
  else if(pt>=1160)              { weight = 0.823212; }

  return weight;
}

double getFacDownW(const double pt)
{
  double weight = 1; 
  if     (pt < 170)              { weight = 1.43694; }
  else if(pt>=170  && pt < 200)  { weight = 1.45181; }
  else if(pt>=200  && pt < 230)  { weight = 1.42649; }
  else if(pt>=230  && pt < 260)  { weight = 1.4513; }
  else if(pt>=260  && pt < 290)  { weight = 1.3624; }
  else if(pt>=290  && pt < 320)  { weight = 1.37472; }
  else if(pt>=320  && pt < 350)  { weight = 1.3718; }
  else if(pt>=350  && pt < 390)  { weight = 1.34637; }
  else if(pt>=390  && pt < 430)  { weight = 1.26276; }
  else if(pt>=430  && pt < 470)  { weight = 1.25762; }
  else if(pt>=470  && pt < 510)  { weight = 1.26389; }
  else if(pt>=510  && pt < 550)  { weight = 1.25504; }
  else if(pt>=550  && pt < 590)  { weight = 1.2116; }
  else if(pt>=590  && pt < 640)  { weight = 1.04846; }
  else if(pt>=640  && pt < 690)  { weight = 1.052; }
  else if(pt>=690  && pt < 740)  { weight = 1.09788; }
  else if(pt>=740  && pt < 790)  { weight = 1.10967; }
  else if(pt>=790  && pt < 840)  { weight = 1.046; }
  else if(pt>=840  && pt < 900)  { weight = 1.06961; }
  else if(pt>=900  && pt < 960)  { weight = 0.989001; }
  else if(pt>=960  && pt < 1020) { weight = 0.987722; }
  else if(pt>=1020 && pt < 1090) { weight = 1.07552; }
  else if(pt>=1090 && pt < 1160) { weight = 1.00802; }
  else if(pt>=1160)              { weight = 0.922172; }

  return weight;
}

//--------------------------------------------------------------------------------------
double getEWKZ(const double pt)
{
  double weight = 1;
  if     (pt < 170)              { weight = 0.970592; }
  else if(pt>=170  && pt < 200)  { weight = 0.964424; }
  else if(pt>=200  && pt < 230)  { weight = 0.956695; }
  else if(pt>=230  && pt < 260)  { weight = 0.948747; }
  else if(pt>=260  && pt < 290)  { weight = 0.941761; }
  else if(pt>=290  && pt < 320)  { weight = 0.934246; }
  else if(pt>=320  && pt < 350)  { weight = 0.927089; }
  else if(pt>=350  && pt < 390)  { weight = 0.919181; }
  else if(pt>=390  && pt < 430)  { weight = 0.909926; }
  else if(pt>=430  && pt < 470)  { weight = 0.900911; }
  else if(pt>=470  && pt < 510)  { weight = 0.892561; }
  else if(pt>=510  && pt < 550)  { weight = 0.884353; }
  else if(pt>=550  && pt < 590)  { weight = 0.8761; }
  else if(pt>=590  && pt < 640)  { weight = 0.867687; }
  else if(pt>=640  && pt < 690)  { weight = 0.858047; }
  else if(pt>=690  && pt < 740)  { weight = 0.849014; }
  else if(pt>=740  && pt < 790)  { weight = 0.840317; }
  else if(pt>=790  && pt < 840)  { weight = 0.832017; }
  else if(pt>=840  && pt < 900)  { weight = 0.823545; }
  else if(pt>=900  && pt < 960)  { weight = 0.814596; }
  else if(pt>=960  && pt < 1020) { weight = 0.806229; }
  else if(pt>=1020 && pt < 1090) { weight = 0.798038; }
  else if(pt>=1090 && pt < 1160) { weight = 0.789694; }
  else if(pt>=1160)              { weight = 0.781163; }

  return weight;
}

double getQCDZ(const double pt)
{
  double weight = 1;
  if     (pt < 170)              { weight = 1.47528; }
  else if(pt>=170  && pt < 200)  { weight = 1.5428; }
  else if(pt>=200  && pt < 230)  { weight = 1.49376; }
  else if(pt>=230  && pt < 260)  { weight = 1.39119; }
  else if(pt>=260  && pt < 290)  { weight = 1.40538; }
  else if(pt>=290  && pt < 320)  { weight = 1.44661; }
  else if(pt>=320  && pt < 350)  { weight = 1.38176; }
  else if(pt>=350  && pt < 390)  { weight = 1.37381; }
  else if(pt>=390  && pt < 430)  { weight = 1.29145; }
  else if(pt>=430  && pt < 470)  { weight = 1.33452; }
  else if(pt>=470  && pt < 510)  { weight = 1.25765; }
  else if(pt>=510  && pt < 550)  { weight = 1.24265; }
  else if(pt>=550  && pt < 590)  { weight = 1.24331; }
  else if(pt>=590  && pt < 640)  { weight = 1.16187; }
  else if(pt>=640  && pt < 690)  { weight = 1.07349; }
  else if(pt>=690  && pt < 740)  { weight = 1.10748; }
  else if(pt>=740  && pt < 790)  { weight = 1.06617; }
  else if(pt>=790  && pt < 840)  { weight = 1.05616; }
  else if(pt>=840  && pt < 900)  { weight = 1.1149; }
  else if(pt>=900  && pt < 960)  { weight = 1.03164; }
  else if(pt>=960  && pt < 1020) { weight = 1.06872; }
  else if(pt>=1020 && pt < 1090) { weight = 0.981645; }
  else if(pt>=1090 && pt < 1160) { weight = 0.81729; }
  else if(pt>=1160)              { weight = 0.924246; }

  return weight;
}

double getRenUpZ(const double pt)
{
  double weight = 1;
  if     (pt < 170)              { weight = 1.38864; }
  else if(pt>=170  && pt < 200)  { weight = 1.43444; }
  else if(pt>=200  && pt < 230)  { weight = 1.39055; }
  else if(pt>=230  && pt < 260)  { weight = 1.2934; }
  else if(pt>=260  && pt < 290)  { weight = 1.30618; }
  else if(pt>=290  && pt < 320)  { weight = 1.33453; }
  else if(pt>=320  && pt < 350)  { weight = 1.27239; }
  else if(pt>=350  && pt < 390)  { weight = 1.25981; }
  else if(pt>=390  && pt < 430)  { weight = 1.18738; }
  else if(pt>=430  && pt < 470)  { weight = 1.21686; }
  else if(pt>=470  && pt < 510)  { weight = 1.14351; }
  else if(pt>=510  && pt < 550)  { weight = 1.13305; }
  else if(pt>=550  && pt < 590)  { weight = 1.12346; }
  else if(pt>=590  && pt < 640)  { weight = 1.05153; }
  else if(pt>=640  && pt < 690)  { weight = 0.98056; }
  else if(pt>=690  && pt < 740)  { weight = 1.00651; }
  else if(pt>=740  && pt < 790)  { weight = 0.96642; }
  else if(pt>=790  && pt < 840)  { weight = 0.956161; }
  else if(pt>=840  && pt < 900)  { weight = 0.998079; }
  else if(pt>=900  && pt < 960)  { weight = 0.927421; }
  else if(pt>=960  && pt < 1020) { weight = 0.954094; }
  else if(pt>=1020 && pt < 1090) { weight = 0.883954; }
  else if(pt>=1090 && pt < 1160) { weight = 0.729348; }
  else if(pt>=1160)              { weight = 0.833531; }

  return weight;
}

double getRenDownZ(const double pt)
{
  double weight = 1;
  if     (pt < 170)              { weight = 1.55029; }
  else if(pt>=170  && pt < 200)  { weight = 1.64816; }
  else if(pt>=200  && pt < 230)  { weight = 1.59095; }
  else if(pt>=230  && pt < 260)  { weight = 1.48394; }
  else if(pt>=260  && pt < 290)  { weight = 1.49876; }
  else if(pt>=290  && pt < 320)  { weight = 1.55923; }
  else if(pt>=320  && pt < 350)  { weight = 1.49283; }
  else if(pt>=350  && pt < 390)  { weight = 1.49279; }
  else if(pt>=390  && pt < 430)  { weight = 1.39829; }
  else if(pt>=430  && pt < 470)  { weight = 1.46136; }
  else if(pt>=470  && pt < 510)  { weight = 1.38252; }
  else if(pt>=510  && pt < 550)  { weight = 1.36081; }
  else if(pt>=550  && pt < 590)  { weight = 1.37848; }
  else if(pt>=590  && pt < 640)  { weight = 1.28575; }
  else if(pt>=640  && pt < 690)  { weight = 1.17335; }
  else if(pt>=690  && pt < 740)  { weight = 1.21841; }
  else if(pt>=740  && pt < 790)  { weight = 1.17707; }
  else if(pt>=790  && pt < 840)  { weight = 1.16846; }
  else if(pt>=840  && pt < 900)  { weight = 1.25225; }
  else if(pt>=900  && pt < 960)  { weight = 1.15246; }
  else if(pt>=960  && pt < 1020) { weight = 1.20461; }
  else if(pt>=1020 && pt < 1090) { weight = 1.09389; }
  else if(pt>=1090 && pt < 1160) { weight = 0.921666; }
  else if(pt>=1160)              { weight = 1.02897; }

  return weight;
}

double getFacUpZ(const double pt)
{
  double weight = 1;
  if     (pt < 170)              { weight = 1.48202; }
  else if(pt>=170  && pt < 200)  { weight = 1.54551; }
  else if(pt>=200  && pt < 230)  { weight = 1.49889; }
  else if(pt>=230  && pt < 260)  { weight = 1.37781; }
  else if(pt>=260  && pt < 290)  { weight = 1.39565; }
  else if(pt>=290  && pt < 320)  { weight = 1.42918; }
  else if(pt>=320  && pt < 350)  { weight = 1.35817; }
  else if(pt>=350  && pt < 390)  { weight = 1.34966; }
  else if(pt>=390  && pt < 430)  { weight = 1.26377; }
  else if(pt>=430  && pt < 470)  { weight = 1.30245; }
  else if(pt>=470  && pt < 510)  { weight = 1.22426; }
  else if(pt>=510  && pt < 550)  { weight = 1.20589; }
  else if(pt>=550  && pt < 590)  { weight = 1.20726; }
  else if(pt>=590  && pt < 640)  { weight = 1.12468; }
  else if(pt>=640  && pt < 690)  { weight = 1.04086; }
  else if(pt>=690  && pt < 740)  { weight = 1.06898; }
  else if(pt>=740  && pt < 790)  { weight = 1.02577; }
  else if(pt>=790  && pt < 840)  { weight = 1.01591; }
  else if(pt>=840  && pt < 900)  { weight = 1.06486; }
  else if(pt>=900  && pt < 960)  { weight = 0.986133; }
  else if(pt>=960  && pt < 1020) { weight = 1.01224; }
  else if(pt>=1020 && pt < 1090) { weight = 0.937427; }
  else if(pt>=1090 && pt < 1160) { weight = 0.77774; }
  else if(pt>=1160)              { weight = 0.87435; }

  return weight;
}

double getFacDownZ(const double pt)
{
  double weight = 1;
  if     (pt < 170)              { weight = 1.47257; }
  else if(pt>=170  && pt < 200)  { weight = 1.54471; }
  else if(pt>=200  && pt < 230)  { weight = 1.49357; }
  else if(pt>=230  && pt < 260)  { weight = 1.41154; }
  else if(pt>=260  && pt < 290)  { weight = 1.42113; }
  else if(pt>=290  && pt < 320)  { weight = 1.46933; }
  else if(pt>=320  && pt < 350)  { weight = 1.41145; }
  else if(pt>=350  && pt < 390)  { weight = 1.40379; }
  else if(pt>=390  && pt < 430)  { weight = 1.32597; }
  else if(pt>=430  && pt < 470)  { weight = 1.37278; }
  else if(pt>=470  && pt < 510)  { weight = 1.2973; }
  else if(pt>=510  && pt < 550)  { weight = 1.28653; }
  else if(pt>=550  && pt < 590)  { weight = 1.28624; }
  else if(pt>=590  && pt < 640)  { weight = 1.20542; }
  else if(pt>=640  && pt < 690)  { weight = 1.11142; }
  else if(pt>=690  && pt < 740)  { weight = 1.1518; }
  else if(pt>=740  && pt < 790)  { weight = 1.11242; }
  else if(pt>=790  && pt < 840)  { weight = 1.1022; }
  else if(pt>=840  && pt < 900)  { weight = 1.17247; }
  else if(pt>=900  && pt < 960)  { weight = 1.08313; }
  else if(pt>=960  && pt < 1020) { weight = 1.13457; }
  else if(pt>=1020 && pt < 1090) { weight = 1.03169; }
  else if(pt>=1090 && pt < 1160) { weight = 0.862166; }
  else if(pt>=1160)              { weight = 0.981254; }

  return weight;
}
