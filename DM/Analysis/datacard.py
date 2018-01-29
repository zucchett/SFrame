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
parser.add_option("-N", "--name", action="store", type="string", dest="name", default="test")
(options, args) = parser.parse_args()

fileName = options.fileName
catList = options.catList
isShape = not options.isCutAndCount
isOverride = options.override
verbose = options.verbose

jobs = []

greenShape = ['QCDscale_fac', 'QCDscale_ren', 'EWKscale_Z', 'EWKscale_W', 'CMS_eff_b', 'CMS_scale_pu', 'CMS_scale_top', 'CMS_eff_trigger', 'CMS_eff_e', 'CMS_eff_m']

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


rateparam = {
    'rateTopAH' : 'TTbarSL',
    'rateZjetsAH' : 'DYJetsToNuNu_HT',
    'rateWjetsAH' : 'WJetsToLNu_HT',
    'rateTopSL' : 'TTbarSL',
    'rateWjetsSL' : 'WJetsToLNu_HT',
}


def datacard(cat, sign):
    
    if verbose: print "  Starting datacard for category", cat, "and mass", sign, "..."
    
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
    card += "shapes * * " + space + "rootfiles_"+options.name+"/$CHANNEL.root          $PROCESS          $SYSTEMATIC/$PROCESS\n"
    card += hline
    card += "bin        " + space
    for i, s in enumerate([sign] + back):
        card += "%-25s" % cat
    card += "\n"
    card += "process    " + space
    for i, s in enumerate([sign] + back):
        card += "%-25s" % s
    card += "\n"
    card += "process    " + space
    for i, s in enumerate([sign] + back):
        card += "%-25d" % i
    card += "\n"
    card += "rate       " + space
    for i, s in enumerate([sign] + back):
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
            for i, s in enumerate([sign] + back):
#                    if i>0: card += ("%-20.1f" % 1)
#                    else: card += "%-25s" % "-"
                if not checkShape(cat, s, h): card += ("%-25.0f" % 1)
                else: card += "%-25s" % "-"
            card += "\n"
    
    # Cut&count++
    else:
        for i, h in enumerate(shape):
            card += "%-50s lnN       " % h
            for c in categories:
                for b in range(nbin[c] if not isShape else 1):
                    for i, s in enumerate([sign] + back): #
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
        for i, s in enumerate([sign] + back):
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
        if any([True for x in back if x in k]):
            card += "%-50s lnU       " % k
            for i, s in enumerate([sign] + back):
                if s in k: card += ("%-25.3f" % freenorm[k])
                else: card += "%-25s" % "-"
            card += "\n"
    

    if verbose: print "  Rate params..."
    for p, m in rateparam.iteritems():
        if ('AH' in cat and 'AH' in p) or ('SL' in cat and 'SL' in p):
            card += "%-25s%-20s%-20s\t%-20s          1.   [0.5,1.5]\n" % (p, 'rateParam', cat, m, )
    
    if verbose: print "  MC statistics..."
    # MC statistics
    card += "* autoMCStats 0.1\n"
    
    if verbose: print "  Done. Writing to file..."
    # Close rootfile
    #inFile.Close()
    
    # Write to file
    try: os.stat("datacards_"+options.name) 
    except: os.mkdir("datacards_"+options.name)
    
    outname = "datacards_"+options.name+"/" + sign + '_' + cat + ".txt"
    cardfile = open(outname, 'w')
    cardfile.write(card)
    cardfile.close()
    print "Datacard for ", sign, "saved in", outname
    
    # Avoid closing the file, directly remove the TFile from internal list
    # https://root.cern.ch/phpBB3/viewtopic.php?t=14450
    #gROOT.GetListOfFiles().Remove(inFile)


def getNumber(cat, s, syst=''):
    f = TFile("rootfiles_"+options.name+"/"+cat+".root", "READ")
    h = f.Get((syst+'/' if len(syst)>0 else '')+s)
    if h==None: return -1
    n = h.Integral()
    f.Close()
    #print "In category", cat, "background", s, "has", n, "events"
    return n

def checkShape(cat, s, syst=''):
    f = TFile("rootfiles_"+options.name+"/"+cat+".root", "READ")
    h = f.Get(s)
    hUp = f.Get(syst+'Up'+'/'+s)
    hDown = f.Get(syst+'Down'+'/'+s)
    if h==None or hUp==None or hDown==None: return False
    isSame = True
    if h.Integral()<1.e-4 or hUp.Integral()<1.e-4 or hDown.Integral()<1.e-4: return True
    for i in range(h.GetNbinsX()):
        if not h.GetBinContent(i+1) > 0.: continue
        if abs(hUp.GetBinContent(i+1)-h.GetBinContent(i+1))/h.GetBinContent(i+1) > 1.e-2 or abs(h.GetBinContent(i+1)-hDown.GetBinContent(i+1))/h.GetBinContent(i+1) > 1.e-2:
            isSame = False
            break
    f.Close()
    #if not isSame: print "systematic", syst,"for sample", s, "in cat", cat, "has been degraded"
    return isSame

def fillLists():
    # List files and fill categories
    for c in os.listdir('rootfiles_'+options.name+'/'):
        if c.endswith('root'):
            categories.append(c.replace('.root', ''))
    
    # Read file and histograms
    inFile = TFile("rootfiles_"+options.name+"/"+categories[0]+".root", "READ")
    inFile.cd()
    for key in inFile.GetListOfKeys():
        obj = key.ReadObj()
        if obj.IsA().InheritsFrom("TH1"):
            name = obj.GetName()
            i = 0
            if 'DM' in name:
                sign.append( name )
                i = i +1
            if i > 0 : break
            elif not "data_obs" in name and not "BkgSum" in name: back.append(name)
        # Categories (directories)
        if obj.IsFolder():
            subdir = obj.GetName()
            subdir = subdir.replace('Up', '').replace('Down', '')
            if not subdir in greenShape: continue
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

