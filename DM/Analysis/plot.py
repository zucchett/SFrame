#! /usr/bin/env python

import os, multiprocessing
import copy
import math
from array import array
from ROOT import ROOT, gROOT, gStyle, gRandom, TSystemDirectory, gErrorIgnoreLevel
from ROOT import TFile, TChain, TTree, TCut, TH1F, TH2F, THStack, TGraph, TGraphErrors, TRandom3
from ROOT import TStyle, TCanvas, TPad
from ROOT import TLegend, TLatex, TText, TLine, TF1, TFormula
import numpy as np
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
parser.add_option("", "--saveplots", action="store_true", default=False, dest="saveplots")
parser.add_option("-m", "--mode", action="store", type="string", dest="mode", default="shape")
parser.add_option("-N", "--name", action="store", type="string", dest="name", default="test")
(options, args) = parser.parse_args()
if options.bash: gROOT.SetBatch(True)

########## SETTINGS ##########
gErrorIgnoreLevel = 2001
gStyle.SetOptStat(0)
gROOT.LoadMacro('functions.C')

NTUPLEDIR   = "/mnt/t3nfs01/data01/shome/dpinna/SFrame/CMSSW_8_0_26_patch1/src/SFrame/DM/Analysis_Ntuples_sys/"
SIGNAL      = 1 # Signal magnification factor
RATIO       = 4 # 0: No ratio plot; !=0: ratio between the top and bottom pads
NORM        = options.norm
PARALLELIZE = False
BLIND       = False
LUMI        = 35867

########## SAMPLES ##########
data = ["data_obs"]
#back = ["DYJetsToLL"]
back = ["VV", "ST", "TTbarSL", "WJetsToLNu_HT", "DYJetsToNuNu_HT", "DYJetsToLL_HT", "QCD"]
sign = [
'ttDM_MChi1_MPhi10_scalar', 'ttDM_MChi1_MPhi20_scalar', 'ttDM_MChi1_MPhi50_scalar', 'ttDM_MChi1_MPhi100_scalar', 'ttDM_MChi1_MPhi200_scalar', 'ttDM_MChi1_MPhi300_scalar', 'ttDM_MChi1_MPhi500_scalar', 'tDM_MChi1_MPhi10_scalar', 'tDM_MChi1_MPhi20_scalar', 'tDM_MChi1_MPhi50_scalar', 'tDM_MChi1_MPhi100_scalar', 'tDM_MChi1_MPhi200_scalar', 'tDM_MChi1_MPhi300_scalar', 'tDM_MChi1_MPhi500_scalar', 'tttDM_MChi1_MPhi10_scalar', 'tttDM_MChi1_MPhi20_scalar', 'tttDM_MChi1_MPhi50_scalar', 'tttDM_MChi1_MPhi100_scalar', 'tttDM_MChi1_MPhi200_scalar', 'tttDM_MChi1_MPhi300_scalar', 'tttDM_MChi1_MPhi500_scalar','ttDM_MChi1_MPhi10_pseudo', 'ttDM_MChi1_MPhi20_pseudo', 'ttDM_MChi1_MPhi50_pseudo', 'ttDM_MChi1_MPhi100_pseudo', 'ttDM_MChi1_MPhi200_pseudo', 'ttDM_MChi1_MPhi300_pseudo', 'ttDM_MChi1_MPhi500_pseudo', 'tDM_MChi1_MPhi10_pseudo', 'tDM_MChi1_MPhi20_pseudo', 'tDM_MChi1_MPhi50_pseudo', 'tDM_MChi1_MPhi100_pseudo', 'tDM_MChi1_MPhi200_pseudo', 'tDM_MChi1_MPhi300_pseudo', 'tDM_MChi1_MPhi500_pseudo', 'tttDM_MChi1_MPhi10_pseudo', 'tttDM_MChi1_MPhi20_pseudo', 'tttDM_MChi1_MPhi50_pseudo', 'tttDM_MChi1_MPhi100_pseudo', 'tttDM_MChi1_MPhi200_pseudo', 'tttDM_MChi1_MPhi300_pseudo', 'tttDM_MChi1_MPhi500_pseudo']


########## ######## ##########

########## ######## ##########

jobs = []

def plot(var, cut, norm=False, nm1=False):
    ### Preliminary Operations ###
    fileRead = os.path.exists(options.file)
    treeRead = not any(x==cut for x in ['0l', '1e', '1m', '2e', '2m', '1e1m', 'Gen', 'Trigger'])#(var in variable.keys()) # Read from tree
    binLow = ""
    binHigh = ""
    binName = ""
    if "binned" in cut:
        binLow = cut[cut.find("LowVal")+6:cut.find("HighVal")-1]
        binHigh = cut[cut.find("HighVal")+7:]
        binName = "bin_"+binLow+"_"+binHigh
        cut = cut[:cut.find("binned")]
    
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
            if not binLow == "":
                cut = cut + " && " + var + " > " + binLow + " && " + var + " < " + binHigh
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
            fileName = options.file if not s=='data_obs' else "rootfiles_"+options.name+"/"+channel+binName+".root"
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
            if not binLow == "":
                hist[s] = TH1F(s, ";"+variable[var]['title']+";Events;"+('log' if variable[var]['log'] else ''), 1, float(binLow), float(binHigh))
            elif binLow == "" and  variable[var]['nbins']>0: 
                hist[s] = TH1F(s, ";"+variable[var]['title']+";Events;"+('log' if variable[var]['log'] else ''), variable[var]['nbins'], variable[var]['min'], variable[var]['max'])
            else: 
                hist[s] = TH1F(s, ";"+variable[var]['title']+";Events;"+('log' if variable[var]['log'] else ''), len(variable[var]['bins'])-1, array('f', variable[var]['bins']))
            hist[s].Sumw2()
            redFactorString = ""
            redFactorValue = ""
            if isBlind and 'data' in s:
                redFactorString = " && Entry$ % 15 == 0"
            if isBlind and 'data' not in s:
                redFactorValue = " / 15"
            cutstring = "("+weight+redFactorValue+")" + ("*("+cut+redFactorString+")" if len(cut)>0 else "")
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
        #if 'WJetsToLNu' in s and 'SL' in channel and 'WR' in channel: hist[s].Scale(1.30)
        #if 'TTbar' in s and 'SL' in channel and 'TR' in channel: hist[s].Scale(0.91)
    
    
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
        for i, s in enumerate(sign):
            hist[s].Scale(hist['BkgSum'].Integral()/hist[s].Integral())
#        for i, s in enumerate(back):
#            hist[s].SetFillStyle(3005)
#            hist[s].SetLineWidth(2)
#        #for i, s in enumerate(sign):
#        #    hist[s].SetFillStyle(0)
#        if not var=="Events":
#            sfnorm = hist[data[0]].Integral()/hist['BkgSum'].Integral()
#            print "Applying SF:", sfnorm
#            for i, s in enumerate(back+['BkgSum']): hist[s].Scale(sfnorm)
    
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
    #data_graph.Draw("SAME, PE")
    if 'PreFit' in hist: hist['PreFit'].Draw("SAME, HIST")
    if showSignal:
        for i, s in enumerate(sign):
            if sample[s]['plot']: hist[s].Draw("SAME, HIST") # signals Normalized, hist[s].Integral()*sample[s]['weight']
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
        #err.GetXaxis().SetLabelOffset(err.GetXaxis().GetLabelOffset()*5)
        #err.GetXaxis().SetTitleOffset(err.GetXaxis().GetTitleOffset()*2)
        err.Draw("E2")
        if 'PreFit' in hist:
            respre = hist['PreFit'].Clone("ResiduesPreFit")
            respre.Divide(hist['BkgSum'])
            respre.Draw("SAME, HIST")
        errLine.Draw("SAME, HIST")
        if not isBlind and len(data) > 0:
            res.Draw("SAME, PE0")
            #res_graph.Draw("SAME, PE0")
            if len(err.GetXaxis().GetBinLabel(1))==0: # Bin labels: not a ordinary plot
                drawRatio(hist['data_obs'], hist['BkgSum'])
                drawStat(hist['data_obs'], hist['BkgSum'])
    
    c1.Update()
        
    if gROOT.IsBatch() and options.saveplots: # and (treeRead and channel in selection.keys()):
        if not os.path.exists("plots_"+options.name+"/"+plotdir): os.makedirs("plots_"+options.name+"/"+plotdir)
        c1.Print("plots_"+options.name+"/"+plotdir+"/"+plotname+binName+".png")
        c1.Print("plots_"+options.name+"/"+plotdir+"/"+plotname+binName+".pdf")
    
    # Print table
    printTable(hist, sign)
    
    if not gROOT.IsBatch(): raw_input("Press Enter to continue...")

    if gROOT.IsBatch() and not fileRead and (var == 'MET_pt' or (channel.startswith('SL') and var == 'MET_sign') or (channel.endswith('ZR') and var == 'FakeMET_pt')):
        saveHist(hist, channel+binName)

    
########## ######## ##########

def addSys(var, cut, sys):
    binLow = ""
    binHigh = ""
    binName = ""
    if "binned" in cut:
        binLow = cut[cut.find("LowVal")+6:cut.find("HighVal")-1]
        binHigh = cut[cut.find("HighVal")+7:]
        binName = "bin_"+binLow+"_"+binHigh
        cut = cut[:cut.find("binned")]

    channel = cut
    weight = "eventWeightLumi" #+ ("*stitchWeight" if any([x for x in back if x.endswith('b')]) else "")
    cut  = selection[cut]
    if not binLow == "":
        cut = cut + " && " + var + " > " + binLow + " && " + var + " < " + binHigh

    weightUp = weightDown = weight
    varUp = varDown = var
    cutUp = cutDown = cut

    # Systematics
    if sys=='CMS_scale_j':
        if var!="MET_sign": varUp = var.replace('pt', 'ptScaleUp')
        else: varUp = var.replace('sign', 'signScaleUp')
        if var!="MET_sign": varDown = var.replace('pt', 'ptScaleDown')
        else: varDown = var.replace('sign', 'signScaleDown')

        cutUp = cut.replace('MET_pt', 'MET_ptScaleUp')
        cutUp = cutUp.replace('Jets', 'JetsScaleUp')
        cutUp = cutUp.replace('12', '12ScaleUp')
        cutUp = cutUp.replace('mT>', 'mTScaleUp>')
        cutUp = cutUp.replace('mT2', 'mT2ScaleUp')

        cutDown = cut.replace('MET_pt', 'MET_ptScaleDown')
        cutDown = cutDown.replace('Jets', 'JetsScaleDown')
        cutDown = cutDown.replace('12', '12ScaleDown')
        cutDown = cutDown.replace('mT>', 'mTScaleDown>')
        cutDown = cutDown.replace('mT2', 'mT2ScaleDown')
    elif sys=='CMS_res_j':
        if var!="MET_sign": varUp = var.replace('pt', 'ptResUp')
        else: varUp = var.replace('sign', 'signResUp')
        if var!="MET_sign": varDown = var.replace('pt', 'ptResDown')
        else: varDown = var.replace('sign', 'signResDown')

        cutUp = cut.replace('MET_pt', 'MET_ptResUp')
        cutUp = cutUp.replace('Jets', 'JetsResUp')
        cutUp = cutUp.replace('12', '12ResUp')
        cutUp = cutUp.replace('mT>', 'mTResUp>')
        cutUp = cutUp.replace('mT2', 'mT2ResUp')
        cutDown = cut.replace('MET_pt', 'MET_ptResDown')
        cutDown = cutDown.replace('Jets', 'JetsResDown')
        cutDown = cutDown.replace('12', '12ResDown')
        cutDown = cutDown.replace('mT>', 'mTResDown>')
        cutDown = cutDown.replace('mT2', 'mT2ResDown')

    elif sys=='CMS_WqcdWeightRen': weightUp += "*WqcdWeightRenUp/WqcdWeight"; weightDown += "*WqcdWeightRenDown/WqcdWeight"
    elif sys=='CMS_WqcdWeightFac': weightUp += "*WqcdWeightFacUp/WqcdWeight"; weightDown += "*WqcdWeightFacDown/WqcdWeight"
    elif sys=='CMS_ZqcdWeightRen': weightUp += "*ZqcdWeightRenUp/ZqcdWeight"; weightDown += "*ZqcdWeightRenDown/ZqcdWeight"
    elif sys=='CMS_ZqcdWeightFac': weightUp += "*ZqcdWeightFacUp/ZqcdWeight"; weightDown += "*ZqcdWeightFacDown/ZqcdWeight"
    elif sys=='CMS_WewkWeight': weightUp += "/WewkWeight"; weightDown += ""
    elif sys=='CMS_ZewkWeight': weightUp += "/ZewkWeight"; weightDown += ""

    elif sys=='CMS_pdf': weightUp += "*PDFWeightUp/eventWeight"; weightDown += "*PDFWeightDown/eventWeight"
    elif sys=='CMS_HF': weightUp += "*1.20"; weightDown += "*0.8"

    elif sys=='CMS_eff_b': weightUp += "*bTagWeightUp/bTagWeight"; weightDown += "*bTagWeightDown/bTagWeight"
    elif sys=='CMS_scale_pu': weightUp += "*puWeightUp/puWeight"; weightDown += "*puWeightDown/puWeight"
    elif sys=='CMS_scale_top': weightUp += "/TopWeight"; weightDown += ""
    elif sys=='CMS_eff_trigger': weightUp += "*triggerWeightUp/triggerWeight"; weightDown += "*triggerWeightDown/triggerWeight"
    elif sys=='CMS_eff_e' and '2e' in cut or '1e' in channel: weightUp += "*leptonWeightUp/leptonWeight"; weightDown += "*leptonWeightDown/leptonW\
eight"
    elif sys=='CMS_eff_m' and '2m' in cut or '1m' in channel: weightUp += "*leptonWeightUp/leptonWeight"; weightDown += "*leptonWeightDown/leptonW\
eight"
    elif sys=='QCDscale_ren': weightUp += "*QCDRenWeightUp"; weightDown += "*QCDRenWeightDown"
    elif sys=='QCDscale_fac': weightUp += "*QCDFacWeightUp"; weightDown += "*QCDFacWeightDown"
    # elif sys=='EWKscale_Z': weightDown += "/ZewkWeight"                                                                                          
    # elif sys=='EWKscale_W': weightDown += "/WewkWeight"                                                                                          
    else:
        print "Systematic", sys, "not applicable or not recognized."
    ### Create and fill MC histograms ###
    file = {}
    tree = {}
    hist = {}
    histUp = {}
    histDown = {}
    isBlind = BLIND and 'SR' in channel
    for i, s in enumerate(back+sign):
        tree[s] = TChain("tree")
        for j, ss in enumerate(sample[s]['files']): tree[s].Add(NTUPLEDIR + ss + ".root")
        if not binLow == "":
            hist[s] = TH1F(s, ";"+variable[var]['title']+";Events;"+('log' if variable[var]['log'] else ''), 1, float(binLow), float(binHigh))
        elif binLow == "" and variable[var]['nbins']>0:
            hist[s] = TH1F(s, ";"+variable[var]['title']+";Events;"+('log' if variable[var]['log'] else ''), variable[var]['nbins'], variable[var]['min'], variable[var]['max'])
        else:
            hist[s] = TH1F(s, ";"+variable[var]['title'], len(variable[var]['bins'])-1, array('f', variable[var]['bins']))
        hist[s].Sumw2()
        histUp[s] = hist[s].Clone(s+'Up')
        histDown[s] = hist[s].Clone(s+'Down')
        redFactorString = ""
        redFactorValue = ""
        if isBlind and 'data' not in s:
            redFactorValue = " / 15"
        cutstring = ("*("+cut+")" if len(cut)>0 else "")
        cutstringUp   = ("*("+cutUp+")" if len(cut)>0 else "")
        cutstringDown = ("*("+cutDown+")" if len(cut)>0 else "")
        if '-' in s: cutstring = cutstring.replace(cut, cut + "&& nBQuarks==" + s.split('-')[1][0])
        tree[s].Project(s, var, "("+weight+redFactorValue+")" + cutstring)
 
        if 'HF' not in sys or 'QCDscale' not in sys :
            tree[s].Project(s+'Up', varUp, "("+weightUp+redFactorValue+")" + cutstring)
            tree[s].Project(s+'Down', varDown, "("+weightDown+redFactorValue+")" + cutstring)

        if 'HF' in sys:
            if s.startswith('WJ') or s.startswith('ZJ') or s.startswith('DYJets'):
                tree[s].Project(s+'Up', varUp, "("+weightUp+redFactorValue+")" + cutstringUp)
                tree[s].Project(s+'Down', varDown, "("+weightDown+redFactorValue+")" + cutstringDown)
            else:
                tree[s].Project(s+'Up', varUp, "("+weight+redFactorValue+")" + cutstringUp)
                tree[s].Project(s+'Down', varDown, "("+weight+redFactorValue+")" + cutstringDown)

        if 'QCDscale' in sys:
            if s.startswith('WJ') or s.startswith('ZJ') or s.startswith('DYJets'):
                tree[s].Project(s+'Up', varUp, "("+weight+redFactorValue+")" + cutstringUp)
                tree[s].Project(s+'Down', varDown, "("+weight+redFactorValue+")" + cutstringDown)
            else:
                tree[s].Project(s+'Up', varUp, "("+weightUp+redFactorValue+")" + cutstringUp)
                tree[s].Project(s+'Down', varDown, "("+weightDown+redFactorValue+")" + cutstringDown)


        hist[s].Scale(sample[s]['weight'] if hist[s].Integral() >= 0 else 0)
        hist[s].SetLineWidth(2)
        histUp[s].SetLineWidth(2)
        histDown[s].SetLineWidth(2)
        hist[s].SetLineColor(1)
        histUp[s].SetLineColor(629)
        histDown[s].SetLineColor(602)
    
    # Rescale normalization for QCD scales FIXME
    if 'QCDscale' in sys:
        for s in back+sign:#['TTbar', 'TTbarSL', 'ST']:
            if s in hist and histUp[s].Integral() > 0. and histDown[s].Integral() > 0.:
                histUp[s].Scale(hist[s].Integral()/histUp[s].Integral())
                histDown[s].Scale(hist[s].Integral()/histDown[s].Integral())

    hist['BkgSum'] = hist[back[0]].Clone("BkgSum")
    hist['BkgSum'].Reset()
    histUp['BkgSum'] = hist['BkgSum'].Clone("BkgSumUp")
    histUp['BkgSum'].SetLineColor(629)
    histUp['BkgSum'].Reset()
    histDown['BkgSum'] = hist['BkgSum'].Clone("BkgSumDown")
    histDown['BkgSum'].SetLineColor(602)
    histDown['BkgSum'].Reset()

    for i, s in enumerate(back):
        hist['BkgSum'].Add(hist[s], 1)
        histUp['BkgSum'].Add(histUp[s], 1)
        histDown['BkgSum'].Add(histDown[s], 1)
    
    for i, s in enumerate(back+sign+['BkgSum']):
        addOverflow(hist[s], False)
        addOverflow(histUp[s], False)
        addOverflow(histDown[s], False)
    
    c1 = TCanvas("c1", "Signals", 800, 600)
    c1.cd()
    gStyle.SetOptStat(0)
    gStyle.SetOptTitle(0)

    if RATIO:
        c1.Divide(1, 2)
        setTopPad(c1.GetPad(1), RATIO)
        setBotPad(c1.GetPad(2), RATIO)

    c1.cd(1)
    c1.GetPad(bool(RATIO)).SetTopMargin(0.06)
    c1.GetPad(bool(RATIO)).SetRightMargin(0.06)
    c1.GetPad(bool(RATIO)).SetTicks(1, 1)
    c1.GetPad(bool(RATIO)).SetLogy()

    histUp['BkgSum'].SetMaximum(histUp['BkgSum'].GetMaximum()*5)
    histUp['BkgSum'].Draw("HIST")
    histDown['BkgSum'].Draw("SAME, HIST")
    hist['BkgSum'].Draw("SAME, HIST")
    drawCMS(-1, "Simulation", False)

    setHistStyle(histUp['BkgSum'], 1.2 if RATIO else 1.1)

    if RATIO:
        c1.cd(2)
        errUp = histUp['BkgSum'].Clone("BkgUp;")
        errUp.Add(hist['BkgSum'],-1)
        errUp.Divide(hist['BkgSum'])
        errUp.SetTitle("")
        errUp.GetYaxis().SetTitle("#frac{shifted-central}{central}")
        errUp.GetYaxis().SetNdivisions(503)
        setBotStyle(errUp)
        errUp.GetYaxis().SetRangeUser(-0.3,0.3)
        errUp.Draw("HIST")

        errDown = histDown['BkgSum'].Clone("BkgDown;")
        errDown.Add(hist['BkgSum'],-1)
        errDown.Divide(hist['BkgSum'])
        errDown.Draw("SAME, HIST")

        f1 = TF1("myfunc","[0]",-100000,10000);
        f1.SetLineColor(1)
        f1.SetLineStyle(7)
        f1.SetLineWidth(1)
        f1.SetParameter(0,0);
        f1.Draw("same")

    leg = TLegend(0.65, 0.80, 0.95, 0.80)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0) #1001
    leg.SetHeader(sys.replace('CMS', '').replace('_', ' '))
    leg.AddEntry(histUp['BkgSum'], "Up", "l")
    leg.AddEntry(hist['BkgSum'], "Central", "l")
    leg.AddEntry(histDown['BkgSum'], "Down", "l")
    leg.SetY1(0.75-leg.GetNRows()*0.045)
    c1.cd(1)
    leg.Draw()

    if options.saveplots:
        if not os.path.exists("plotsSys_"+options.name+"/"+channel+binName): os.makedirs("plotsSys_"+options.name+"/"+channel+binName)
        c1.Print("plotsSys_"+options.name+"/"+channel+binName+"/"+sys+".png")
        c1.Print("plotsSys_"+options.name+"/"+channel+binName+"/"+sys+".pdf")
    
    for i, s in enumerate(back+sign):
        c2 = TCanvas(s+"canvas", "Signals", 800, 600)
        c2.cd()
        gStyle.SetOptStat(0)
        gStyle.SetOptTitle(0)
        c2.GetPad(0).SetTopMargin(0.06)
        c2.GetPad(0).SetRightMargin(0.06)
        c2.GetPad(0).SetTicky(2)
        c2.GetPad(0).SetLogy()
        histUp[s].SetMaximum(histUp[s].GetMaximum()*5)
        histUp[s].Draw("HIST")
        histDown[s].Draw("SAME, HIST")
        hist[s].Draw("SAME, HIST")
        drawCMS(-1, "Simulation", False)
        if options.saveplots:
            c2.Print("plotsSys_"+options.name+"/"+channel+binName+"/"+sys+"_"+s+".png")
            c2.Print("plotsSys_"+options.name+"/"+channel+binName+"/"+sys+"_"+s+".pdf")

    saveHist(histUp, channel+binName, sys+'Up')
    saveHist(histDown, channel+binName, sys+'Down')

    print "Added systematic", sys, "to channel", channel


########## ######## ##########

def saveHist(hist, channel, directory='', addStat=False):
    
    # Blind
#    if BLIND and 'data_obs' in hist and 'SR' in channel:
#        rando = TRandom3()
#        hist['data_obs'].Reset()
#        hist['data_obs'].SetMarkerStyle(21)
#        for i in range(hist['data_obs'].GetNbinsX()):
        #this wiggles the pseudodata with a Poison 
        #-> different from Asimov dataset (-t -1 option), will give different obs/expected limits but thats ok
#            hist['data_obs'].SetBinContent(i+1, rando.Poisson( hist['BkgSum'].GetBinContent(i+1) ))
#            hist['data_obs'].SetBinContent(i+1, hist['BkgSum'].GetBinContent(i+1) )

    # Sanity check
#    smax = max(hist, key=lambda x: hist[x].Integral())
#    for s in hist.keys():
#        for i in range(hist[s].GetNbinsX()):
#            if not hist[s].GetBinContent(i+1)>0.: hist[s].SetBinContent(i+1, 1.e-4) # Sanity check
#            if math.isnan(hist[s].GetBinContent(i+1)) or math.isinf(hist[s].GetBinContent(i+1)): print "WARNING: in channel", channel, "bkg", s, "bin", i+1, "is nan or inf"
#            #print "checking", s, i, hist[s].GetBinContent(i+1)
    
    outFile = TFile("rootfiles_"+options.name+"/"+channel+".root", "RECREATE" if len(directory)==0 else "UPDATE")
    outFile.cd()
    if len(directory) > 0:
        if not outFile.GetDirectory(directory): outFile.mkdir(directory)
        outFile.cd(directory)

    for s in sorted(hist.keys()):
        hist[s].Write(hist[s].GetName().replace('Up', '').replace('Down', ''))

    #outFile.cd("..")
    # Statistical MC uncertainty
    if addStat:
        nbins = hist['data_obs'].GetNbinsX()
        #CMS_stat_Z0b_bin57
        for s in sorted(hist.keys()):
            if 'data' in s: continue
            dirname = channel+"/Sys_"+s
            #if outFile.GetDirectory(dirname): outFile.rmdir(dirname)
            outFile.mkdir(dirname)
            outFile.cd(dirname)
            for k in range(1, nbins+1):
                sysname = "CMS_stat_%s_%s_bin%d" % (channel, s, k)
                histUp = hist[s].Clone(sysname+"Up")
                histUp.SetBinContent(k, histUp.GetBinContent(k) + histUp.GetBinError(k))
                histUp.Write()
                histDown = hist[s].Clone(sysname+"Down")
                histDown.SetBinContent(k, max(histDown.GetBinContent(k) - histDown.GetBinError(k), 1.e-6))
                histDown.Write()
            outFile.cd("..")
    
    outFile.Close()
    print "Histograms saved in file rootfiles_"+options.name+"/"+channel+".root"

    


########## ######## ##########

def plotLimit(doBinned = False):
    gROOT.SetBatch(True)
    try: os.stat("rootfiles_"+options.name) 
    except: os.mkdir("rootfiles_"+options.name)
    os.system("rm rootfiles_"+options.name+"/*")

    cat = ["AH0l0fSR", "AH0l1fSR", "AH0l2bSR", "AH1eWR", "AH1mWR", "AH2eZR", "AH2mZR", "AH1eTR", "AH1mTR", "SL1e0fSR", "SL1e1fSR", "SL1m0fSR", "SL1m1fSR", "SL1e2bSR", "SL1m2bSR", "SL1eWR", "SL1mWR", "SL1e1mTR", "SL2eTR", "SL2mTR"]
    #cat = ["AH0l0fSR", "AH0l1fSR", "AH1eWR", "AH1mWR", "AH2eZR", "AH2mZR", "AH1eTR", "AH1mTR", "SL1e0fSR", "SL1e1fSR", "SL1m0fSR", "SL1m1fSR", "SL1eWR", "SL1mWR", "SL1e1mTR"]

    sys = ['CMS_scale_j','CMS_res_j','CMS_WqcdWeightRen','CMS_WqcdWeightFac','CMS_ZqcdWeightRen','CMS_ZqcdWeightFac','CMS_WewkWeight','CMS_pdf','CMS_HF','CMS_eff_b', 'CMS_scale_pu', 'CMS_scale_top', 'CMS_eff_trigger', 'CMS_eff_e', 'CMS_eff_m', 'QCDscale_ren', 'QCDscale_fac']

    for r in cat:
        #fix these variables at some points, its confusing to hack the met significance
        var = 'MET_pt'
        if r.startswith('SL'): var = 'MET_sign' # correct binnning
        if r.endswith('ZR'): var = 'FakeMET_pt'
        
        if doBinned:
            bins = np.array([])
            if 'bins' in variable[var].keys():
                bins = np.array(variable[var]['bins'] )
            else:
                binsize = (variable[var]['max']-variable[var]['min'])/variable[var]['nbins']
                bins = np.arange(variable[var]['min'], variable[var]['max']+binsize, binsize)
                
            bins = np.append(bins, 10000) #add essentially infinite upper bound for the last bin
            
            for i in range(0,len(bins)-1):
                rbin =  r + "binned_LowVal" + str(bins[i]) + "_HighVal" + str(bins[i+1])
                print "rbin:",rbin
            
                p = multiprocessing.Process(target=plot, args=(var, rbin, ))
                jobs.append(p)
                p.start()
    
            for job in jobs:
                job.join()
            del jobs[:]
        else: #shouls dump these pieces in a separate function, but fine for now
            print "running multithreaded"
            #        plot(var, r)
            p = multiprocessing.Process(target=plot, args=(var, r, ))
            jobs.append(p)
            p.start()

    if not doBinned:
        for job in jobs:
            job.join()
        del jobs[:]
        

    print "\n\n@ Main jobs have finished, now running uncertainties\n\n"
    
    doSys = True
    
    if not doSys: return
    
    for s in sys:
        print "\n@ Running uncertainty", s, "\n"
        for r in cat:
            var = 'MET_pt'
            if r.startswith('SL'): var = 'MET_sign' # correct binnning
            if r.endswith('ZR'): var = 'FakeMET_pt'
            
            if doBinned:
                bins = np.array([])
                if 'bins' in variable[var].keys():
                    bins = np.array(variable[var]['bins'] )
                else:
                    binsize = (variable[var]['max']-variable[var]['min'])/variable[var]['nbins']
                    bins = np.arange(variable[var]['min'], variable[var]['max']+binsize, binsize)
                
                bins = np.append(bins, 10000) #add essentially infinite upper bound for the last bin
            
                for i in range(0,len(bins)-1):
                    rbin =  r + "binned_LowVal" + str(bins[i]) + "_HighVal" + str(bins[i+1])
                    print "rbin:",rbin
    #       addSys(var, r, s)
                    p = multiprocessing.Process(target=addSys, args=(var, rbin, s, ))
                    jobs.append(p)
                    p.start()

                for job in jobs:
                    job.join()
                del jobs[:]

            else:
                p = multiprocessing.Process(target=addSys, args=(var, r, s, ))
                jobs.append(p)
                p.start()
            
        if not doBinned:
            for job in jobs:
                job.join()
            del jobs[:]
    print "\n\n@ Uncertainties have finished.\n\n"
    
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
    
    for r in selections.keys():
        for v in ['Lepton1_pt', 'Lepton1_pfIso', 'Lepton2_pt', 'Lepton2_pfIso', 'Jet1_pt', 'Jet1_csv', 'Jet2_pt', 'Jet2_csv', 'Jet3_pt', 'Jet3_csv', 'Jet4_pt', 'Jet4_csv', 'JetF_pt', 'mZ', 'V_pt', 'mT', 'mT2', 'MinDPhi', 'MinDPhi12', 'MET_pt', 'MET_sign', 'FakeMET_pt', 'nPV', 'nJets', 'nForwardJets', 'nBTagJets', 'nElectrons', 'nMuons', 'nTaus', 'HT', 'ST']:
#            plot(v, r)
            p = multiprocessing.Process(target=plot, args=(v,r,))
            jobs.append(p)
            p.start()
        p.join()


jobs = []

if options.all: plotAll()
elif options.limit and options.mode == "shape": plotLimit()
elif options.limit and options.mode == "binned": plotLimit(True)
elif options.signal: plotSignal(options.cut)
elif options.efficiency: plotEfficiency()
else: plot(options.variable, options.cut, options.norm)
#addSys('FakeMET_pt', 'AH2mZR', 'QCDscale_ren')
