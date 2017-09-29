#! /usr/bin/env python

import os, sys, getopt, multiprocessing
import copy
import math
from array import array
from ROOT import gROOT, TFile, TH1F

import optparse
usage = "usage: %prog [options]"
parser = optparse.OptionParser(usage)
parser.add_option("-f", "--fileName", action="store", type="string", dest="fileName", default="")
parser.add_option("-r", "--remove", action="store", type="string", default="", dest="catList")
parser.add_option("-c", "--cutcount", action="store_true", default=False, dest="isCutAndCount")
parser.add_option("-o", "--override", action="store_true", default=False, dest="override")
parser.add_option("-v", "--verbose", action="store_true", default=False, dest="verbose")
(options, args) = parser.parse_args()

fileName = options.fileName
catList = options.catList
isShape = not options.isCutAndCount
isOverride = options.override
verbose = options.verbose

jobs = []



categories = []
back = []
sign = []

#CMS_scale_j, CMS_res_j, CMS_eff_b, CMS_scale_q2, CMS_scale_met, CMS_scale_pu, CMS_eff_l, lumi_8TeV, pdf_gg, pdf_qg, pdf_qq, CMS_norm_ZJets

#shape = ['CMS_eff_b', 'CMS_scale_pu' 'CMS_eff_trigger', 'CMS_eff_e', 'CMS_eff_m', 'QCDscale', 'EWKscale'] #, 'pdf', 'CMS_scale_met_jes', 'CMS_scale_met_jer', 'CMS_scale_met_m', 'CMS_scale_met_e', 'CMS_scale_met_t', 'CMS_scale_met_u', 'CMS_scale_met_g',
shape = []

norm = {
#    "QCDscale_Z_ACCEPT" : {"DYJets" : 1.03,},
#    "QCDscale_W_ACCEPT" : {"WJets": 1.03,},
#    "CMS_norm_VH" : {"VH" : 1.15,},
#    "CMS_norm_VV" : {"VV" : 1.15,},
#    "CMS_norm_ST" : {"ST" : 1.15,},
#    "CMS_norm_QCD" : {"QCD" : 1.50,},
#    'CMS_scale_j'  : {"VH" : 1.030, "VV" : 1.030, "ST" : 1.030, "QCD" : 1.030, "DM" : 1.030},
#    'CMS_res_j'  : {"VH" : 1.010, "VV" : 1.010, "ST" : 1.010, "QCD" : 1.010, "DM" : 1.010},
#    "CMS_eff_t" : {"VH" : 1.030, "VV" : 1.030, "ST" : 1.030, "QCD" : 1.030,},
    "lumi_13TeV" : {"VH" : 1.025, "VV" : 1.025, "ST" : 1.025, "QCD" : 1.025, "DM" : 1.025},
}


freenorm = {
#    "CMS_norm_DYJetsToNuNu_HT" : 4,
#    "CMS_norm_DYJetsToLL_HT" : 4,
#    "CMS_norm_WJetsToLNu_HT" : 4,
#    "CMS_norm_TTbarSL" : 4,
}

# name rateParam : [[list of Bkg], [list of categories], value]
rateparam = {
    'rateTopAH' : {'AH0lSR' : 'TTbarSL', 'AH0lCR' : 'TTbarSL', 'AH1eWR' : 'TTbarSL', 'AH1mWR' : 'TTbarSL', 'AH2eZR' : 'TTbarSL', 'AH2mZR' : 'TTbarSL', 'AH1eTR' : 'TTbarSL', 'AH1mTR' : 'TTbarSL'},
    'rateZjetsAH' : {'AH0lSR' : 'DYJetsToNuNu_HT', 'AH0lCR' : 'DYJetsToNuNu_HT', 'AH1eWR' : 'DYJetsToNuNu_HT', 'AH1mWR' : 'DYJetsToNuNu_HT', 'AH2eZR' : 'DYJetsToLL_HT', 'AH2mZR' : 'DYJetsToLL_HT', 'AH1eTR' : 'DYJetsToNuNu_HT', 'AH1mTR' : 'DYJetsToNuNu_HT'},
    'rateWjetsAH' : {'AH0lSR' : 'WJetsToLNu_HT', 'AH0lCR' : 'WJetsToLNu_HT', 'AH1eWR' : 'WJetsToLNu_HT', 'AH1mWR' : 'WJetsToLNu_HT', 'AH2eZR' : 'WJetsToLNu_HT', 'AH2mZR' : 'WJetsToLNu_HT', 'AH1eTR' : 'WJetsToLNu_HT', 'AH1mTR' : 'WJetsToLNu_HT'},
    'rateTopSL' : {'SL1eSR' : 'TTbarSL', 'SL1mSR' : 'TTbarSL', 'SL1eWR' : 'TTbarSL', 'SL1mWR' : 'TTbarSL', 'SL1e1mTR' : 'TTbarSL'},
    'rateWjetsSL' : {'SL1eSR' : 'WJetsToLNu_HT', 'SL1mSR' : 'WJetsToLNu_HT', 'SL1eWR' : 'WJetsToLNu_HT', 'SL1mWR' : 'WJetsToLNu_HT', 'SL1e1mTR' : 'WJetsToLNu_HT'},
}


def datacard(cat, signal):
    
    if verbose: print "  Starting datacard for category", cat, "and mass", signal, "..."
    
#    hist = {}
#    syst = {}
#    nbin = {}
#    name = {}
#    inFile = TFile("rootfiles/"+cat+".root", "READ")
#    inFile.cd()
#    for c in categories:
#        hist[c] = {}
#        syst[c] = {}
#        nbin[c] = inFile.Get(c+"/data_obs").GetNbinsX() if isShape else 1
#        hist[c]["data_obs"] = inFile.Get(c+"/data_obs")
#        #print "  category", c, "has", nbin[c], "bins"
#        for i, s in enumerate(back + sign):
#            hist[c][s] = inFile.Get(c+"/"+s)
#            #print c, s, hist[c][s].Integral()
#            #if 'CR' in c: continue
#            syst[c][s] = {}
#            for i, h in enumerate(shape):
#                base = c+"/Sys_"+s+"/"+h
#                #print c, s, h, base, inFile.Get(base).Integral(), inFile.Get(base+"Up").Integral(), inFile.Get(base+"Down").Integral()
#                syst[c][s][h] = [inFile.Get(base), inFile.Get(base+"Up"), inFile.Get(base+"Down")]
#                #print c, s, h, syst[c][s][h][0].Integral(), syst[c][s][h][1].Integral(), syst[c][s][h][-1].Integral()
#    
#    if verbose: print "  Histagrams read, producing header..."
    #exit()
    
    ### Header, Categories and Processes ###
    space = " "*50
    hline = "-"*100 + "\n"
    
    card =  "imax *   number of channels\n"
    card += "jmax *   number of backgrounds\n"
    card += "kmax *   number of nuisance parameters\n"
    card += hline
    card += "bin        " + space + ("%-25s" % cat) + "\n"
    card += "observation" + space + ("%-25.0f" % -1.) + "\n"
    card += hline
    card += "shapes * * " + space + "rootfiles/$CHANNEL.root          $PROCESS          $SYSTEMATIC/$PROCESS\n"
    card += hline
    card += "bin        " + space
    for i, s in enumerate([signal] + back):
        card += "%-25s" % cat
    card += "\n"
    card += "process    " + space
    for i, s in enumerate([signal] + back):
        card += "%-25s" % s
    card += "\n"
    card += "process    " + space
    for i, s in enumerate([signal] + back):
        card += "%-25d" % i
    card += "\n"
    card += "rate       " + space
    for i, s in enumerate([signal] + back):
        card += "%-25.6f" % getNumber(cat, s) #-1.
    card += "\n"
    card += hline
    
    if verbose: print "  Header done, now shape uncertainties..."
    
    ### Systematic Uncertainties ###
    if verbose: print "  MC statistics..."
    # MC statistics
    #card += "* autoMCStats 0.1\n"
    
    # Shape
    if isShape:
        for i, h in enumerate(shape):
            card += "%-50s shape     " % h
            for i, s in enumerate([signal] + back):
#                    if i>0: card += ("%-20.1f" % 1)
#                    else: card += "%-25s" % "-"
                if getNumber(cat, s, h+'Down') > 0.: card += ("%-25.0f" % 1)
                else: card += "%-25s" % "-"
            card += "\n"
    
    # Cut&count++
    else:
        for i, h in enumerate(shape):
            card += "%-50s lnN       " % h
            for c in categories:
                for b in range(nbin[c] if not isShape else 1):
                    for i, s in enumerate([signal] + back): #
                        #print c, s, h, syst[c][s][h][1].GetBinContent(b+1)
                        if syst[c][s][h][0].GetBinContent(b+1) != 0:
                            up = max(min(syst[c][s][h][1].GetBinContent(b+1)/syst[c][s][h][0].GetBinContent(b+1), 2), 0.5)
                            down = max(min(syst[c][s][h][-1].GetBinContent(b+1)/syst[c][s][h][0].GetBinContent(b+1), 2), 0.5)
                            string = "%.3f/%.3f" % (up, down)
                        else:
                            string = "-"
                        card += "%-25s" % string
            card += "\n"
    
    if verbose: print "  Shapes done, now normalization..."
    
    # Normalization
    for k in sorted(norm.keys()):
        card += "%-50s lnN       " % k
        for i, s in enumerate([signal] + back):
            issyst = False
            for n, nn in norm[k].iteritems():
                if n in s and norm[k][n]>0:
                    card += ("%-25.3f" % norm[k][n])
                    issyst = True
            if not issyst: card += "%-25s" % "-"
        card += "\n"
    
    if verbose: print "  Normalization done, now minor backgrounds..."
    
    # Free backgrounds
    for k in sorted(freenorm.keys()):
        card += "%-50s lnU       " % k
        for i, s in enumerate([signal] + back):
            if s in k: card += ("%-25.3f" % freenorm[k])
            else: card += "%-25s" % "-"
        card += "\n"
    
    if verbose: print "  MC statistics..."
    # MC statistics
#    card += "* autoMCStats 0.1\n"
#    if isShape:
#        for c1 in categories:
#            for i1, s1 in enumerate(back): #signal + 
#                for i in range(1, nbin[c1]+1):
#                    card += "%-50s shape     " % ("CMS_stat_%s_%s_bin%d" % (c1, s1, i))
#                    for c2 in categories:
#                        for i2, s2 in enumerate(signal + back):
#                            if s1==s2 and c1==c2: card += ("%-25.0f" % 1)
#                            else: card += ("%-25s" % '-')
#                    card += "\n"

    if verbose: print "  Rate params..."
    for p, m in rateparam.iteritems():
        if cat in m:
            card += "%-25s%-20s%-20s\t%-20s          1.\n" % (p, 'rateParam', cat, m[cat], )

    if verbose: print "  Done. Writing to file..."
    # Close rootfile
    #inFile.Close()
    
    # Write to file
    if not os.path.exists("datacards/"):
        print "Error: directory", "datacards/ does not exist, aborting..."
        exit()
    
    dataname = signal #if len(signal)==1 else signal[0].replace('bb', 'hf')
    
    outname = "datacards/" + signal + '_' + cat + ".txt"
    cardfile = open(outname, 'w')
    cardfile.write(card)
    cardfile.close()
    print "Datacard for ", signal, "saved in", outname
    
    # Avoid closing the file, directly remove the TFile from internal list
    # https://root.cern.ch/phpBB3/viewtopic.php?t=14450
    #gROOT.GetListOfFiles().Remove(inFile)


def getNumber(cat, s, syst=''):
    f = TFile("rootfiles/"+cat+".root", "READ")
    h = f.Get((syst+'/' if len(syst)>0 else '')+s)
    if h==None: return -1
    n = h.Integral()
    f.Close()
    #print "In category", cat, "background", s, "has", n, "events"
    return n


def fillLists():
    # List files and fill categories
    for c in os.listdir('rootfiles/'): categories.append(c.replace('.root', ''))
    
    # Read file and histograms
    inFile = TFile("rootfiles/"+categories[0]+".root", "READ")
    inFile.cd()
    for key in inFile.GetListOfKeys():
        obj = key.ReadObj()
        if obj.IsA().InheritsFrom("TH1"):
            name = obj.GetName()
            if 'DM' in name: sign.append( name )
            elif not "data_obs" in name and not "BkgSum" in name: back.append(name)
        # Categories (directories)
        if obj.IsFolder():
            subdir = obj.GetName()
            subdir = subdir.replace('Up', '').replace('Down', '')
            if not subdir in shape: shape.append(subdir)
    inFile.Close()
        

fillLists()

print "Categories  :", categories
print "Signal      :", sign
print "Backgrounds :", back
print "Shape unc   :", shape


for cat in categories:
    for s in sign:
        datacard(cat, s)

# python datacard.py -f rootfiles/DM.root -o

