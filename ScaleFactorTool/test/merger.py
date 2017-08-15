#! /usr/bin/env python

import os, sys, getopt
import copy, math
from array import array
from ROOT import gROOT, gStyle, gRandom
from ROOT import TFile, TChain, TTree, TCut, TH1F, TH2F, THStack, TGraph, TGaxis
from ROOT import TStyle, TCanvas, TPad, TLegend, TLatex, TText

from samples import sample
from utils import *

gROOT.SetBatch(True)
gStyle.SetOptStat(0)
gStyle.SetOptTitle(0)
gStyle.SetPadTopMargin(0.06)
gStyle.SetPadRightMargin(0.05)

#gStyle.SetErrorX(1)
NTUPLEDIR   = "../../Ntuple/VH/"
RATIO       = 4

back = ["DYJetsToLL_HT"]
sign = ["XZH_M800", "XZH_M1000", "XZH_M1200", "XZH_M1400", "XZH_M1800", "XZH_M2000", "XZH_M2500", "XZH_M3000", "XZH_M3500", "XZH_M4000", "XZH_M4500"]
#sign = ["XZZ_M2000", "XZZ_M2500", "XZZ_M3000", "XZZ_M3500", "XZZ_M4000", "XZZ_M4500"]
sigp = ['XZh_M1000', 'XZh_M1400', 'XZh_M2000', 'XZh_M2500', 'XZh_M3000', 'XZh_M3500', 'XZh_M4000', 'XZh_M4500']


def weight(files, name):

    for f in files:
        inFile1 = TFile(f[0], "READ")
        inFile1.cd()
        h1 = inFile1.Get



def merge(files, target):

    hist = {}
    
    inFile = TFile(files[0][0], "READ")
    inFile.cd()
    for key in inFile.GetListOfKeys():
        obj = key.ReadObj()
        # Copy and rescale histograms
        if obj.IsA().InheritsFrom("TH1"):
            hist[obj.GetName()] = weight(files, obj.GetName())
        elif obj.IsA().InheritsFrom("TTree"):
            print "Skip tree", obj.GetName()
            # do nothing
        # Directories
        elif obj.IsFolder():
            subdir = obj.GetName()
            print "In directory", subdir
            inFile.cd(subdir)
            for subkey in inFile.GetDirectory(subdir).GetListOfKeys():
                subobj = subkey.ReadObj()
                if subobj.IsA().InheritsFrom("TH1"):
                    if 'Eff' in subdir: effh.append(subobj.GetName())
                    elif 'Gen' in subdir: gens.append(subobj.GetName())
                    elif not 'Counter' in subdir: plotHist([subobj])
            inFile.cd("..")
    inFile.Close()
    
    print "Now plotting efficiency histograms", effh

    for j, h in enumerate(effh):
        name = h.split("_", 1)[0].replace("Eff", "") #if not 'HEEP_E' in h else h.replace("Eff", "")
        num = TH1F()
        den = TH1F()
        files = {}
        for i, s in enumerate(sign):
            files[s] = TFile(NTUPLEDIR + sample[s]['files'][0] + ".root", "READ")
            files[s].cd()
            tnum = files[s].Get("Eff/"+h)
            tden = files[s].Get("Leptons/"+name)
            tnum.Multiply(tden)
            if i==0:
                num = tnum.Clone("Numerator")
                den = tden.Clone("Denominator")
            else:
                num.Add(tnum)
                den.Add(tden)
            #files[s].Close()
        num.Divide(den)
        num.SetName(h)
        plotHist([num])
    
    
    print "Now plotting gen signal histograms", effh

    for j, h in enumerate(gens):
        files = {}
        sigh = []
        for i, s in enumerate(sigp):
            files[s] = TFile(NTUPLEDIR + sample[s]['files'][0] + ".root", "READ")
            files[s].cd()
            sigh.append(files[s].Get("Gen/"+h))
            sigh[-1].SetTitle(s)
        plotHist(sigh)
            

def compareHist(variables, obj):
    
    num = {}
    den = {}
    files = {}
    for i, s in enumerate(sign):
        for j, ss in enumerate(sample[s]['files']):
            files[ss] = TFile(NTUPLEDIR + ss + ".root", "READ")
    
    for variable in variables:
        var = variable[0]
        denname = var.split("_")[0]
        for i, f in files.iteritems():
#            tnum = f.Get("Muons/"+var)
#            tden = f.Get("Muons/"+denname)
            tnum = f.Get(obj+"/"+var)
            tden = f.Get(obj+"/"+denname)
            if tnum==None or tden==None: continue
            if not var in num.keys():
                num[var] = tnum.Clone("Numerator")
                den[var] = tden.Clone("Denominator")
            else:
                num[var].Add(tnum)
                den[var].Add(tden)
            #files[s].Close()
        num[var].Divide(den[var])
        num[var].SetName("Multi_"+var)
        num[var].SetTitle(variable[1])
        num[var].GetYaxis().SetRangeUser(0., 1.)
        #print num[var].Integral()
    
    h = num.values()
    h.sort(key=lambda x: x.Integral(), reverse=True)
    plotHist(h)



def plotHist(h):
    colors = [633, 416+1, 862, 800, 921, 922, 1]
    n = len(h)
    
    c1 = TCanvas("c1", "Signals", 800, 600)
    c1.cd()
    #c1.GetPad(1).SetPadTopMargin(0.06)
    #c1.GetPad(1).SetPadRightMargin(0.05)

    c1.GetPad(0).SetTicky(2)
    
    for i in range(n):
        col = sample[h[i].GetTitle()]['linecolor'] if n > len(colors) else colors[i]
        h[i].SetMarkerColor(col)
        h[i].SetLineColor(col)
    
    if "Counter" in h[0].GetName():
        h[0].GetYaxis().SetRangeUser(0., -1)
    else:
        for i in range(n):
            h[i].SetMarkerStyle(20)
            #h[i].SetMarkerSize(1)
            h[i].SetLineWidth(2)
    
    ymax = 0.
    for i in range(n):
        if h[i].GetMaximum() > ymax: ymax = h[i].GetMaximum()
    h[0].SetMaximum(ymax*1.2)
    if h[0].Integral()/h[0].GetNbinsX() < 1.:
        #h[0].GetYaxis().SetTitle("Efficiency")
        h[0].GetYaxis().SetRangeUser(0., 1.)
    
    style = "HIST" if "Counter" in h[0].GetName() or "Barrel" in h[0].GetName() or "Endcap" in h[0].GetName() or h[0].GetName().startswith("Gen") else "PE1"
    h[0].Draw(style)
    for i in range(1, n): h[i].Draw("SAME, "+style)
    
    leg = TLegend(0.50, 0.55-0.05*n, 0.85, 0.55)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0) #1001
    leg.SetFillColor(0)
    for i in range(n):
        if h[i].GetTitle()!="":
            if h[i].GetTitle() in sample.keys(): leg.AddEntry(h[i], sample[h[i].GetTitle()]['label'], "pl")
            else: leg.AddEntry(h[i], h[i].GetTitle(), "pl")
    leg.Draw()
    drawCMS(-1, "Simulation", True)
    c1.Print("plots/Leptons/" + h[-1].GetName() + ".png")
    c1.Print("plots/Leptons/" + h[-1].GetName() + ".pdf")
    #raw_input("Press Enter to continue...")
    c1.Close()
    return True




merge([["IsoEfficienciesAndSF_BCDEF.root", (5.9+2.6+4.4+4.1+3.2)/(5.9+2.6+4.4+4.1+3.2+7.7+8.9)], ["IsoEfficienciesAndSF_GH.root", (7.7+8.9)/(5.9+2.6+4.4+4.1+3.2+7.7+8.9)]], target = "MuonIso_2016.root")


