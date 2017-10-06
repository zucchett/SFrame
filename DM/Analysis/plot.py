#! /usr/bin/env python

import os, multiprocessing
import copy
import math
from array import array
from ROOT import ROOT, gROOT, gStyle, gRandom, TSystemDirectory
from ROOT import TFile, TChain, TTree, TCut, TH1F, TH2F, THStack, TGraph, TGraphErrors
from ROOT import TStyle, TCanvas, TPad
from ROOT import TLegend, TLatex, TText, TLine

from samples import sample
from variables import variable
from selections import selection
from utils import *

########## SETTINGS ##########

import optparse
usage = "usage: %prog [options]"
parser = optparse.OptionParser(usage)
parser.add_option("-v", "--variable", action="store", type="string", dest="variable", default="")
parser.add_option("-c", "--cut", action="store", type="string", dest="cut", default="")
parser.add_option("-n", "--norm", action="store_true", default=False, dest="norm")
parser.add_option("-e", "--eff", action="store_true", default=False, dest="efficiency")
parser.add_option("-s", "--sign", action="store_true", default=False, dest="signal")
parser.add_option("-a", "--all", action="store_true", default=False, dest="all")
parser.add_option("-b", "--bash", action="store_true", default=False, dest="bash")
parser.add_option("-B", "--blind", action="store_true", default=False, dest="blind")
parser.add_option("-f", "--file", action="store", type="string", dest="file", default="")
parser.add_option("-l", "--limit", action="store_true", default=False, dest="limit")
(options, args) = parser.parse_args()
if options.bash: gROOT.SetBatch(True)

########## SETTINGS ##########

gStyle.SetOptStat(0)

NTUPLEDIR   = "/scratch/zucchett/Ntuple/DM/"
SIGNAL      = 1 # Signal magnification factor
RATIO       = 4 # 0: No ratio plot; !=0: ratio between the top and bottom pads
NORM        = options.norm
PARALLELIZE = False
BLIND       = True
LUMI        = 35867

########## SAMPLES ##########
data = ["data_obs"]
back = ["VV", "ST", "TTbarSL", "WJetsToLNu_HT", "DYJetsToNuNu_HT", "DYJetsToLL_HT", "QCD"]
sign = ['tDM_MChi1_MPhi100', 'tDM_MChi1_MPhi300']
#sign = []
########## ######## ##########

########## ######## ##########

jobs = []

def plot(var, cut, norm=False, nm1=False):
    ### Preliminary Operations ###
    fileRead = os.path.exists(options.file)
    treeRead = not any(x==cut for x in ['0l', '1e', '1m', '2e', '2m', '1e1m', 'Gen', 'Trigger']) # Read from tree
    channel = cut
    plotdir = cut
    plotname = var
    weight = "eventWeightLumi" #*(2.2/35.9)
    isBlind = BLIND and 'SR' in channel
    showSignal = True#('SR' in channel)
    cutSplit = cut.split()
    for s in cutSplit:
        if s in selection.keys():
            plotdir = s
            cut  = cut.replace(s, selection[s])
    #if treeRead and cut in selection: cut  = cut.replace(cut, selection[cut])
    
    # Determine Primary Dataset
    pd = []
    print cut
    if any(w in cut for w in ['1l', '1m', '2m', 'isWtoMN', 'isZtoMM', 'isTtoEM']): pd += [x for x in sample['data_obs']['files'] if 'SingleMuon' in x]
    if any(w in cut for w in ['1l', '1e', '2e', 'isWtoEN', 'isZtoEE']): pd += [x for x in sample['data_obs']['files'] if 'SingleElectron' in x]
    if any(w in cut for w in ['0l', 'isZtoNN']): pd += [x for x in sample['data_obs']['files'] if 'MET' in x]
    if len(pd)==0: raw_input("Warning: Primary Dataset not recognized, continue?")
    
    print "Plotting from", ("tree" if treeRead else "file"), var, "in", channel, "channel with:"
    print "  dataset:", pd
    print "  cut    :", cut
    print "  cut    :", weight
    
    ### Create and fill MC histograms ###
    # Create dict
    file = {}
    tree = {}
    hist = {}
    
    ### Create and fill MC histograms ###
    for i, s in enumerate(data+back+sign):
        if fileRead:
            fileName = options.file if not s=='data_obs' else "rootfiles/"+channel+".root"
            histName = "shapes_fit_b/"+channel+"/"+s if not s=='data_obs' else s
            file[s] = TFile(fileName, "READ")
            tmphist = file[s].Get(histName)
            if tmphist==None:
                tmphist = hist[back[0]].Clone(s)
                tmphist.Reset("MICES")
                print "Histogram", histName, "not found in file", fileName
            if s=='data_obs': hist[s] = tmphist
            else:
                hist[s] = hist['data_obs'].Clone(s)
                #hist[s].Reset("MICES")
                hist[s].SetMarkerSize(0)
                for i in range(tmphist.GetNbinsX()+1): hist[s].SetBinContent(i+1, tmphist.GetBinContent(i+1))
        elif treeRead: # Project from tree
            tree[s] = TChain("tree")
            for j, ss in enumerate(sample[s]['files']):
                if not 'data' in s or ('data' in s and ss in pd):
                    tree[s].Add(NTUPLEDIR + ss + ".root")
            if variable[var]['nbins']>0: hist[s] = TH1F(s, ";"+variable[var]['title']+";Events;"+('log' if variable[var]['log'] else ''), variable[var]['nbins'], variable[var]['min'], variable[var]['max'])
            else: hist[s] = TH1F(s, ";"+variable[var]['title']+";Events;"+('log' if variable[var]['log'] else ''), len(variable[var]['bins'])-1, array('f', variable[var]['bins']))
            hist[s].Sumw2()
            cutstring = "("+weight+")" + ("*("+cut+")" if len(cut)>0 else "")
            if '-' in s: cutstring = cutstring.replace(cut, cut + "&& nBQuarks==" + s.split('-')[1][0])
            tree[s].Project(s, var, cutstring)
            if not tree[s].GetTree()==None: hist[s].SetOption("%s" % tree[s].GetTree().GetEntriesFast())
        else: # Histogram written to file
            for j, ss in enumerate(sample[s]['files']):
                if not 'data' in s or ('data' in s and ss in pd):
                    file[ss] = TFile(NTUPLEDIR + ss + ".root", "R")
                    if file[ss].IsZombie():
                        print "WARNING: file", NTUPLEDIR + ss + ".root", "does not exist"
                        continue
                    tmphist = file[ss].Get(cut+"/"+var)
                    if tmphist==None: continue
                    if not s in hist.keys(): hist[s] = tmphist
                    else: hist[s].Add(tmphist)
        if hist[s].Integral() < 0: hist[s].Scale(0)
        hist[s].SetFillColor(sample[s]['fillcolor'])
        hist[s].SetFillStyle(sample[s]['fillstyle'])
        hist[s].SetLineColor(sample[s]['linecolor'])
        hist[s].SetLineStyle(sample[s]['linestyle'])
    
#    if treeRead:
#        for i, s in enumerate([x for x in back if 'DY' in x]):
#            hist[s].Scale(0.903)
    
    hist['BkgSum'] = hist[back[0]].Clone("BkgSum")
    hist['BkgSum'].Reset("MICES")
    for i, s in enumerate(back): hist['BkgSum'].Add(hist[s], 1)
    if fileRead:
        #hist['BkgSum'] = file[back[0]].Get("shapes_fit_b/"+channel+"/"+"total_background")
        tmphist = file[back[0]].Get("shapes_prefit/"+channel+"/"+"total_background")
        hist['PreFit'] = hist['BkgSum'].Clone("PreFit")
        for i in range(tmphist.GetNbinsX()+1): hist['PreFit'].SetBinContent(i+1, tmphist.GetBinContent(i+1))
        hist['PreFit'].SetLineStyle(2)
        hist['PreFit'].SetLineColor(923)
        hist['PreFit'].SetLineWidth(3)
        hist['PreFit'].SetFillStyle(0)
    hist['BkgSum'].SetFillStyle(3003)
    hist['BkgSum'].SetFillColor(1)
    

    # Create data and Bkg sum histograms
#    if options.blind or 'SR' in channel:
#        hist['data_obs'] = hist['BkgSum'].Clone("data_obs")
#        hist['data_obs'].Reset("MICES")
    # Set histogram style
    hist[data[0]].SetMarkerStyle(20)
    hist[data[0]].SetMarkerSize(1.25)
    
    for i, s in enumerate(data+back+sign+['BkgSum']): addOverflow(hist[s], False) # Add overflow
    for i, s in enumerate(sign): hist[s].SetLineWidth(3)
    #for i, s in enumerate(sign): sample[s]['plot'] = True#sample[s]['plot'] and s.startswith(channel[:2])
    
    
    if norm:
        for i, s in enumerate(back):
            hist[s].SetFillStyle(3005)
            hist[s].SetLineWidth(2)
        #for i, s in enumerate(sign):
        #    hist[s].SetFillStyle(0)
        if not var=="Events":
            sfnorm = hist[data[0]].Integral()/hist['BkgSum'].Integral()
            print "Applying SF:", sfnorm
            for i, s in enumerate(back+['BkgSum']): hist[s].Scale(sfnorm)
        if BLIND and var.endswith("Mass"):
            for i, s in enumerate(data+back+['BkgSum']):
                first, last = hist[s].FindBin(65), hist[s].FindBin(135)
                for j in range(first, last): hist[s].SetBinContent(j, -1.e-4)
        if BLIND and var.endswith("Tau21"):
            for i, s in enumerate(data):
                first, last = hist[s].FindBin(0), hist[s].FindBin(0.6)
                for j in range(first, last): hist[s].SetBinContent(j, -1.e-4)
    
    # Create stack
    bkg = THStack("Bkg", ";"+hist['BkgSum'].GetXaxis().GetTitle()+";Events")
    for i, s in enumerate(back): bkg.Add(hist[s])
    
    # Legend
    leg = TLegend(0.65, 0.6, 0.95, 0.9)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0) #1001
    leg.SetFillColor(0)
    if len(data) > 0:
        leg.AddEntry(hist[data[0]], sample[data[0]]['label'], "pe")
    for i, s in reversed(list(enumerate(['BkgSum']+back))):
        leg.AddEntry(hist[s], sample[s]['label'], "f")
    if 'PreFit' in hist: leg.AddEntry(hist['PreFit'], sample['PreFit']['label'], "l")
    if showSignal:
        for i, s in enumerate(sign):
            if sample[s]['plot']: leg.AddEntry(hist[s], sample[s]['label'], "fl")
        
    leg.SetY1(0.9-leg.GetNRows()*0.05)
    
    
    # --- Display ---
    c1 = TCanvas("c1", hist.values()[0].GetXaxis().GetTitle(), 800, 800 if RATIO else 600)
    
    if RATIO:
        c1.Divide(1, 2)
        setTopPad(c1.GetPad(1), RATIO)
        setBotPad(c1.GetPad(2), RATIO)
    c1.cd(1)
    c1.GetPad(bool(RATIO)).SetTopMargin(0.06)
    c1.GetPad(bool(RATIO)).SetRightMargin(0.05)
    c1.GetPad(bool(RATIO)).SetTicks(1, 1)
    
    log = ("log" in hist['BkgSum'].GetZaxis().GetTitle())
    if log: c1.GetPad(bool(RATIO)).SetLogy()
        
    # Draw
    bkg.Draw("HIST") # stack
    hist['BkgSum'].Draw("SAME, E2") # sum of bkg
    if not isBlind and len(data) > 0: hist[data[0]].Draw("SAME, PE") # data
    if 'PreFit' in hist: hist['PreFit'].Draw("SAME, HIST")
    if showSignal:
        for i, s in enumerate(sign):
            if sample[s]['plot']: hist[s].Draw("SAME, HIST")
    bkg.GetYaxis().SetTitleOffset(bkg.GetYaxis().GetTitleOffset()*1.075)
    bkg.SetMaximum((5. if log else 1.25)*max(bkg.GetMaximum(), hist[data[0]].GetBinContent(hist[data[0]].GetMaximumBin())+hist[data[0]].GetBinError(hist[data[0]].GetMaximumBin())))
    if len(sign) > 0 and bkg.GetMaximum() < max(hist[sign[0]].GetMaximum(), hist[sign[-1]].GetMaximum()): bkg.SetMaximum(max(hist[sign[0]].GetMaximum(), hist[sign[-1]].GetMaximum())*1.25)
    bkg.SetMinimum(max(min(hist['BkgSum'].GetBinContent(hist['BkgSum'].GetMinimumBin()), hist[data[0]].GetMinimum()), 5.e-1)  if log else 0.)
    if log:
        bkg.GetYaxis().SetNoExponent(bkg.GetMaximum() < 1.e4)
        bkg.GetYaxis().SetMoreLogLabels(True)
    
    leg.Draw()
    drawCMS(LUMI, "Preliminary")
    drawRegion(channel, True)
    drawAnalysis("DM"+channel[:2])
    drawOverflow()
    
    setHistStyle(bkg, 1.2 if RATIO else 1.1)
    setHistStyle(hist['BkgSum'], 1.2 if RATIO else 1.1)
       
    if RATIO:
        c1.cd(2)
        err = hist['BkgSum'].Clone("BkgErr;")
        err.SetTitle("")
        err.GetYaxis().SetTitle("Data / Bkg")
        for i in range(1, err.GetNbinsX()+1):
            err.SetBinContent(i, 1)
            if hist['BkgSum'].GetBinContent(i) > 0:
                err.SetBinError(i, hist['BkgSum'].GetBinError(i)/hist['BkgSum'].GetBinContent(i))
        setBotStyle(err)
        errLine = err.Clone("errLine")
        errLine.SetLineWidth(1)
        errLine.SetFillStyle(0)
        res = hist[data[0]].Clone("Residues")
        for i in range(0, res.GetNbinsX()+1):
            if hist['BkgSum'].GetBinContent(i) > 0: 
                res.SetBinContent(i, res.GetBinContent(i)/hist['BkgSum'].GetBinContent(i))
                res.SetBinError(i, res.GetBinError(i)/hist['BkgSum'].GetBinContent(i))
        setBotStyle(res)
        err.Draw("E2")
        if 'PreFit' in hist:
            respre = hist['PreFit'].Clone("ResiduesPreFit")
            respre.Divide(hist['BkgSum'])
            respre.Draw("SAME, HIST")
        errLine.Draw("SAME, HIST")
        if not isBlind and len(data) > 0:
            res.Draw("SAME, PE0")
            if len(err.GetXaxis().GetBinLabel(1))==0: # Bin labels: not a ordinary plot
                drawRatio(hist['data_obs'], hist['BkgSum'])
                drawStat(hist['data_obs'], hist['BkgSum'])
    
    c1.Update()
        
    if gROOT.IsBatch():
        if not os.path.exists("plots/"+plotdir): os.makedirs("plots/"+plotdir)
        c1.Print("plots/"+plotdir+"/"+plotname+".png")
        c1.Print("plots/"+plotdir+"/"+plotname+".pdf")
    
    # Print table
    printTable(hist, sign)
    
    if not gROOT.IsBatch(): raw_input("Press Enter to continue...")


########## ######## ##########

def plotAll():
    gROOT.SetBatch(True)
    
    hists = {}
    
#    file = TFile(NTUPLEDIR + "DYJetsToLL.root", 'READ')
#    file.cd()
#    # Looping over file content
#    for key in file.GetListOfKeys():
#        obj = key.ReadObj()
#        # Histograms
#        if obj.IsA().InheritsFrom('TH1'): continue
#        #    hists.append(obj.GetName())
#        # Tree
#        elif obj.IsA().InheritsFrom('TTree'): continue
#        # Directories
#        if obj.IsFolder() and obj.GetName() in ['0l', '1e', '1m', '2e', '2m', '1e1m']:
#            subdir = obj.GetName()
#            hists[subdir] = []
#            file.cd(subdir)
#            for subkey in file.GetDirectory(subdir).GetListOfKeys():
#                subobj = subkey.ReadObj()
#                if subobj.IsA().InheritsFrom('TH1'):
#                    hists[subdir].append(subobj.GetName())
#            file.cd('..')
#    file.Close() 
#    
#    for c, l in hists.iteritems():
#        #if not os.path.exists("plots/"+c):
#        #    os.mkdir("plots/"+c)
#        for h in l:
#            plot(h, c)
    
    for r in selection.keys():
        for v in ['Lepton1_pt', 'Lepton1_pfIso', 'Lepton2_pt', 'Lepton2_pfIso', 'Jet1_pt', 'Jet1_csv', 'Jet2_pt', 'Jet2_csv', 'Jet3_pt', 'Jet3_csv', 'Jet4_pt', 'Jet4_csv', 'JetF_pt', 'mZ', 'V_pt', 'mT', 'mT2', 'MinDPhi', 'MinDPhi12', 'MET_pt', 'MET_sign', 'FakeMET_pt', 'nPV', 'nJets', 'nForwardJets', 'nBTagJets', 'nElectrons', 'nMuons', 'nTaus', 'HT', 'ST']:
#            plot(v, r)
            p = multiprocessing.Process(target=plot, args=(v,r,))
            jobs.append(p)
            p.start()
        p.join()


jobs = []

if options.all: plotAll()
elif options.limit: plotLimit()
elif options.signal: plotSignal(options.cut)
elif options.efficiency: plotEfficiency()
else: plot(options.variable, options.cut)

