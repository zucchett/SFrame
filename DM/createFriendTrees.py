#! /usr/bin/env python

import os, multiprocessing, math
from array import array
from ROOT import TFile, TH1, TF1, TLorentzVector, TTree, TObject

from itertools import combinations
import heapq

from xsections import xsection

import optparse
usage = 'usage: %prog [options]'
parser = optparse.OptionParser(usage)
parser.add_option('-i', '--input', action='store', type='string', dest='origin', default='testout/')
parser.add_option('-t', '--treefile', action='store', type='string', dest='treefile', default='TT.root')
parser.add_option('-o', '--output', action='store', type='string', dest='target', default='testout2/')
parser.add_option('-f', '--filter', action='store', type='string', dest='filter', default='')
parser.add_option('-s', '--single', action='store_true', dest='single', default=False)
parser.add_option('-v', '--verbose', action='store_true', dest='verbose', default=False)
parser.add_option('-c', '--copytree', action='store_true', dest='copytree', default=False)

(options, args) = parser.parse_args()

origin      = options.origin
target      = options.target
treefile      = options.treefile
filterset   = options.filter
singlecore  = options.single
verboseout  = options.verbose
copytree  = options.copytree

LUMI    = 35867.
ANALYSIS = "DMAnalysis"

if not os.path.exists(origin):
    print 'Origin directory', origin, 'does not exist, aborting...'
    exit()

try: os.stat(target)
except: os.mkdir(target)
try: os.stat(target+'/logs')
except: os.mkdir(target+'/logs')


##############################
def DeltaPhi(phi1, phi2):
    dPhi = math.fabs(phi1-phi2)
    if dPhi > 3.14159265:
        dPhi -= 2*3.14159265
    if dPhi <= -3.14159265:
        dPhi += 2*3.14159265
    return dPhi

def Return_DeltaR(jet,i):
    a = list(combinations(jet, 2))

    DeltaR = [1000,-1000]

    for x in a:
        if x[0][0]<-5 or x[1][0]<-5: continue;
        if x[0][1]<-5 or x[1][1]<-5: continue;

        if(math.sqrt(math.pow((x[0][0]-x[1][0]),2)+math.pow((x[0][1]-x[1][1]),2)) < DeltaR[0]):
            DeltaR[0] = math.sqrt(math.pow((x[0][0]-x[1][0]),2)+math.pow((x[0][1]-x[1][1]),2))

        if(math.sqrt(math.pow((x[0][0]-x[1][0]),2)+math.pow((x[0][1]-x[1][1]),2)) > DeltaR[1]):
            DeltaR[1] = math.sqrt(math.pow((x[0][0]-x[1][0]),2)+math.pow((x[0][1]-x[1][1]),2))

    return DeltaR

def Return_mTbs( jet, metPt, metPhi):
    #setting of the 4vectors seems fine naeively?
    met4Vec = TLorentzVector()
    met4Vec.SetPtEtaPhiE(metPt,0,metPhi,metPt)
    bjet4Vec = TLorentzVector()
    bjet4Vec.SetPtEtaPhiE(jet[0],jet[1],jet[2],jet[3])


    mTb = (met4Vec+bjet4Vec).Mt() # this assumes masses for the b-jet

    #using 4vectors and dPhi method (massless particles)
    mTb0 = math.sqrt(2.*met4Vec.Et()*bjet4Vec.Et()*(1.-math.cos(met4Vec.DeltaPhi(bjet4Vec))))

    return {'mTb_mass':mTb, 'mTb_massless':mTb0}

def Return_minDphiJetBJet(jet,id_csv):
    #setting of the 4vectors seems fine naeively?

    deltaphis = [-100,-100]

    jet_nob = list(jet)
    jet_nob.pop(id_csv)

    deltaphis[0]=math.fabs(DeltaPhi(jet[id_csv][2],jet_nob[0][2]))
    deltaphis[1]=math.fabs(DeltaPhi(jet[id_csv][2],jet_nob[1][2]))

    return deltaphis

def Return_Top(jet,id_csv,metPhi):

    masses = [-1000,-1000,-1000,-1000,-1000,-1000]

    b_tmp = TLorentzVector()
    b_tmp.SetPtEtaPhiE(jet[id_csv][0],jet[id_csv][1],jet[id_csv][2],jet[id_csv][3])
    jet_nob = list(jet)
    jet_nob.pop(id_csv)

    j1dR_tmp = TLorentzVector()
    j1dR_tmp.SetPtEtaPhiE(0,0,0,0)
    j2dR_tmp = TLorentzVector()
    j2dR_tmp.SetPtEtaPhiE(0,0,0,0)

    a = list(combinations(jet_nob, 2))
    min_DeltaR=1000

    for x in a:

        if x[0][0]<-5 or x[1][0]<-5: continue;
        if x[0][1]<-5 or x[1][1]<-5: continue;

        if(math.sqrt(math.pow((x[0][0]-x[1][0]),2)+math.pow((x[0][1]-x[1][1]),2)) < min_DeltaR):
            j1dR_tmp.SetPtEtaPhiE(x[0][0],x[0][1],x[0][2],x[0][3])
            j2dR_tmp.SetPtEtaPhiE(x[1][0],x[1][1],x[1][2],x[1][3])

    topdR_tmp = TLorentzVector()
    topdR_tmp = j1dR_tmp+j2dR_tmp+b_tmp

    WdR_tmp = TLorentzVector()
    WdR_tmp = j1dR_tmp+j2dR_tmp

    masses[0] = topdR_tmp.M()
    masses[1] = WdR_tmp.M()

    masses[2] = math.fabs(DeltaPhi(WdR_tmp.Phi(),b_tmp.Phi()))
    masses[3] = math.fabs(DeltaPhi(topdR_tmp.Phi(),b_tmp.Phi()))
    masses[4] = math.fabs(DeltaPhi(WdR_tmp.Phi(),metPhi))
    masses[5] = math.fabs(DeltaPhi(topdR_tmp.Phi(),metPhi))    

    return masses





def processFile(sample_name, verbose=False):
    sample = sample_name.replace(ANALYSIS+".", "").replace(".root", "")
    isMC = not '2016' in sample
    
    # Unweighted input
    ref_file_name = origin + '/' + sample_name
    if not os.path.exists(ref_file_name): 
        print '  WARNING: file', ref_file_name, 'does not exist, continuing'
        return True
    
    # Weighted output
    new_file_name = target + '/' + sample + '.root'
    #if os.path.exists(new_file_name):
    #    print '  WARNING: weighted file exists, overwriting'
        #return True
    
    new_file = TFile(new_file_name, 'RECREATE')
    new_file.cd()
    
    # Open old file
    ref_file = TFile(ref_file_name, 'READ')
    ref_hist = ref_file.Get('Events')
    try:
        totalEntries = ref_hist.GetBinContent(1)
    except:
        print '  ERROR: nEvents not found in file', sample
        exit(1)

    old_tree = ref_file.Get("tree")
    #check if the tree already has the lumi weights    
    hasLumiWeights = any(True for x in old_tree.GetListOfBranches() if "eventWeightLumi" in x.GetName())

    # Cross section
    
    XS = xsection[sample]['xsec']*xsection[sample]['kfactor']*xsection[sample]['br']
    Leq = LUMI*XS/totalEntries if totalEntries > 0 else 0.
    
    print sample, ": Leq =", (Leq if isMC else "Data")
    
    for key in ref_file.GetListOfKeys():
        obj = key.ReadObj()
        # Histograms
        if obj.IsA().InheritsFrom('TH1'):
            if verbose: print ' + TH1:', obj.GetName()
            new_file.cd()
            if isMC and not hasLumiWeights: obj.Scale(LUMI*XS/totalEntries)
            obj.Write()
        # Tree
        elif obj.IsA().InheritsFrom('TTree'):
            continue
        # Directories
        elif obj.IsFolder():
            subdir = obj.GetName()
            if verbose: print ' \ Directory', subdir, ':'
            new_file.mkdir(subdir)
            new_file.cd(subdir)
            for subkey in ref_file.GetDirectory(subdir).GetListOfKeys():
                subobj = subkey.ReadObj()
                if subobj.IsA().InheritsFrom('TH1'):
                    if verbose: print '   + TH1:', subobj.GetName()
                    new_file.cd(subdir)
                    if isMC and not hasLumiWeights: subobj.Scale(LUMI*XS/totalEntries)
                    subobj.Write()
            new_file.cd('..')
        

    # Variables declaration
    #dummy = array('f', [1.0]) #example variable
    mTb_massless = array('f', [1.0]) #example variable
    mTb_mass = array('f', [1.0]) #example variable

    min_DeltaR = array('f', [1.0]) 
    max_DeltaR = array('f', [1.0])
    minDphiJet1BJet = array('f', [1.0])
    minDphiJet2BJet = array('f', [1.0])
    topdRMass = array('f', [1.0]) 
    WdRMass = array('f', [1.0]) 
    Dphi_Wb = array('f', [1.0])
    Dphi_topb = array('f', [1.0])
    Dphi_WMET = array('f', [1.0])
    Dphi_topMET = array('f', [1.0])
    pt_bjet = array('f', [1.0]) 
    DeltaR_bb = array('f', [1.0])
    cosTheta_bb = array('f', [1.0])
    HT3 = array('f', [1.0])

    stitchWeight = array('f', [1.0])
    eventWeightLumi = array('f', [1.0])  # global event weight with lumi


    nev = old_tree.GetEntriesFast()
    new_file.cd()
    
    #we are not going to copy the entire old tree
    tname = "Ftree"
    new_tree = TTree(tname,tname)
    if copytree:
        tname = "tree"
        new_tree = old_tree.CopyTree("")

    #define new branches you want to add here
    #!!!Attention: if run twice, this will create a new branch with the same name as the old one
    mTb_massBranch = new_tree.Branch('mTb_mass', mTb_mass, 'mTb_mass/F')
    mTb_masslessBranch = new_tree.Branch('mTb_massless', mTb_massless, 'mTb_massless/F')

    minDphiJet1BJetBranch = new_tree.Branch('minDphiJet1BJet', minDphiJet1BJet, 'minDphiJet1BJet/F')
    minDphiJet2BJetBranch = new_tree.Branch('minDphiJet2BJet', minDphiJet2BJet, 'minDphiJet2BJet/F')
    min_DeltaRBranch = new_tree.Branch('min_DeltaR', min_DeltaR, 'min_DeltaR/F')
    max_DeltaRBranch = new_tree.Branch('max_DeltaR', max_DeltaR, 'max_DeltaR/F')
    topdRMassBranch = new_tree.Branch('topdRMass', topdRMass, 'topdRMass/F')
    WdRMassBranch = new_tree.Branch('WdRMass', WdRMass, 'WdRMass/F')
    Dphi_WbBranch = new_tree.Branch('Dphi_Wb', Dphi_Wb, 'Dphi_Wb/F')
    Dphi_topbBranch = new_tree.Branch('Dphi_topb', Dphi_topb, 'Dphi_topb/F')
    Dphi_WMETBranch = new_tree.Branch('Dphi_WMET', Dphi_WMET, 'Dphi_WMET/F')
    Dphi_topMETBranch = new_tree.Branch('Dphi_topMET', Dphi_topMET, 'Dphi_topMET/F')
    pt_bjetBranch = new_tree.Branch('pt_bjet', pt_bjet, 'pt_bjet/F')
    DeltaR_bbBranch = new_tree.Branch('DeltaR_bb', DeltaR_bb, 'DeltaR_bb/F')
    cosTheta_bbBranch = new_tree.Branch('cosTheta_bb', cosTheta_bb, 'cosTheta_bb/F')
    HT3Branch = new_tree.Branch('HT3', HT3, 'HT3/F')

    if not (hasLumiWeights): #only do lumi branches if they don't exist already
        stitchWeightBranch = new_tree.Branch('stitchWeight', stitchWeight, 'stitchWeight/F')
        eventWeightLumiBranch = new_tree.Branch('eventWeightLumi', eventWeightLumi, 'eventWeightLumi/F')

    
    
    # looping over events
    for event in range(0, old_tree.GetEntries()):
        if verbose and (event%10000==0 or event==nev-1): print ' = TTree:', old_tree.GetName(), 'events:', nev, '\t', int(100*float(event+1)/float(nev)), '%\r',
        old_tree.GetEntry(event)

        #calculate new variable and fill
        #dummy[0] = 1
        ##CS implemtentation, not very elegant but it should work
        csv = [old_tree.Jet1_csv, old_tree.Jet2_csv, old_tree.Jet3_csv, old_tree.Jet4_csv]
        id_csv = csv.index(max(csv))
        id2_csv = heapq.nlargest(2, xrange(len(csv)), key=csv.__getitem__)[1]

        jets = [(old_tree.Jet1_pt, old_tree.Jet1_eta, old_tree.Jet1_phi, old_tree.Jet1_E),
                (old_tree.Jet2_pt, old_tree.Jet2_eta, old_tree.Jet2_phi, old_tree.Jet2_E),
                (old_tree.Jet3_pt, old_tree.Jet3_eta, old_tree.Jet3_phi, old_tree.Jet3_E),
                (old_tree.Jet4_pt, old_tree.Jet4_eta, old_tree.Jet4_phi, old_tree.Jet4_E),]
        #        print jets
        #        print jets[id_csv]

        minDphiJet1BJet[0]=Return_minDphiJetBJet(jets,id_csv)[0]
        minDphiJet2BJet[0]=Return_minDphiJetBJet(jets,id_csv)[1]

        topdRMass[0] = Return_Top(jets,id_csv,old_tree.MET_phi)[0]
        WdRMass[0] = Return_Top(jets,id_csv,old_tree.MET_phi)[1]
        
        Dphi_Wb[0] = Return_Top(jets,id_csv,old_tree.MET_phi)[2]
        Dphi_topb[0] = Return_Top(jets,id_csv,old_tree.MET_phi)[3]
        Dphi_WMET[0] = Return_Top(jets,id_csv,old_tree.MET_phi)[4]
        Dphi_topMET[0] = Return_Top(jets,id_csv,old_tree.MET_phi)[5]

        jets_EtaPhi = [(old_tree.Jet1_eta, old_tree.Jet1_phi),
                       (old_tree.Jet2_eta, old_tree.Jet2_phi),
                       (old_tree.Jet3_eta, old_tree.Jet3_phi),
                       (old_tree.Jet4_eta, old_tree.Jet4_phi),
                       (old_tree.Jet5_eta, old_tree.Jet5_phi),
                       (old_tree.Jet6_eta, old_tree.Jet6_phi),]

        min_DeltaR[0] = Return_DeltaR(jets_EtaPhi,1)[0]
        max_DeltaR[0] = Return_DeltaR(jets_EtaPhi,2)[1]

        HT3[0] = 0        
        for i, j in enumerate(jets):
            if (i!=0 and i!=1): 
               if (j[0]>0): 
                   HT3[0] += j[0] 

        if (csv[id_csv]>=0 and csv[id2_csv]>=0):
            DeltaR_bb[0] = math.sqrt( math.pow((jets_EtaPhi[id_csv][0]-jets_EtaPhi[id2_csv][0]),2) + math.pow((jets_EtaPhi[id_csv][1]-jets_EtaPhi[id2_csv][1]),2) )
            cosTheta_bb[0] = math.fabs(math.tanh((jets_EtaPhi[id_csv][1]-jets_EtaPhi[id2_csv][1])/2))
        else:
            DeltaR_bb[0] = -9
            cosTheta_bb[0] = -9

        if max(csv) >= 0:
            mTbs = Return_mTbs(jets[id_csv], old_tree.MET_pt, old_tree.MET_phi)

            mTb_mass[0] = mTbs['mTb_mass']
            mTb_massless[0] = mTbs['mTb_massless']
            pt_bjet[0] = jets[id_csv][0]
        else:
            mTb_mass[0] =  mTb_massless[0]= -99
            pt_bjet[0] = -9

        if not (hasLumiWeights):
            eventWeightLumi[0] = stitchWeight[0] = 1
        if isMC:
            # MC stitching
            if sample=='DYJetsToLL' or sample=='WJetsToLNu' or sample=='W1JetsToLNu' or sample=='W2JetsToLNu' or sample=='W3JetsToLNu' or sample=='W4JetsToLNu':
                if old_tree.LheHT > 70.: stitchWeight[0] = 0.
            if not (hasLumiWeights):
                eventWeightLumi[0] = old_tree.eventWeight * old_tree.bTagWeight * old_tree.TopWeight
                eventWeightLumi[0] *= LUMI*XS/totalEntries

            #fill the new branches of the tree
            if not (hasLumiWeights):
                if copytree:
                    stitchWeightBranch.Fill()
                    eventWeightLumiBranch.Fill()

        if copytree:
            #dummyBranch.Fill()
            
            mTb_massBranch.Fill()
            mTb_masslessBranch.Fill()
            min_DeltaRBranch.Fill()
            max_DeltaRBranch.Fill()
            minDphiJet1BJetBranch.Fill()
            minDphiJet2BJetBranch.Fill()
            topdRMassBranch.Fill()
            WdRMassBranch.Fill()
            Dphi_WbBranch.Fill()
            Dphi_topbBranch.Fill()
            Dphi_WMETBranch.Fill()
            Dphi_topMETBranch.Fill()
            pt_bjetBranch.Fill()
            DeltaR_bbBranch.Fill()
            cosTheta_bbBranch.Fill()
            HT3Branch.Fill()

        else:
            new_tree.Fill()

    new_file.cd()
    new_tree.Write(tname,TObject.kOverwrite)
    if verbose: print ' '
    new_file.Close() 


if __name__ == "__main__":
    
    if not treefile.replace(ANALYSIS+".", "").replace(".root", "") in xsection.keys(): exit
    processFile(treefile, verboseout)
    
