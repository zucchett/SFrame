#! /usr/bin/env python

import os, sys, getopt
import copy
import time
import math
from array import array
from ROOT import gROOT, gRandom
from ROOT import TFile, TTree, TCut, TH1F, TH2F, TGraph, TGraph2D, TGraphErrors, TGraphAsymmErrors
from ROOT import TStyle, TCanvas, TPad
from ROOT import TLegend, TLatex, TText, TColor

from utils import *

# Combine output
# 0 - Observed Limit
# 1 - Expected  2.5%
# 2 - Expected 16.0%
# 3 - Expected 50.0%
# 4 - Expected 84.0%
# 5 - Expected 97.5%
# 6 - Significance
# 7 - p-value
# 8 - Best fit r
# 9 - Best fit r down
#10 - Best fit r up

import optparse
usage = "usage: %prog [options]"
parser = optparse.OptionParser(usage)
parser.add_option("-c", "--category", action="store", type="string", dest="category", default="")
parser.add_option("-s", "--signal", action="store", type="string", dest="signal", default="")
parser.add_option("-m", "--mediator", action="store", type="string", dest="mediator", default="SC")
parser.add_option("-j", "--bjets", action="store", type="string", dest="bjets", default="1b")
parser.add_option("-a", "--all", action="store_true", default=False, dest="all")
parser.add_option("-b", "--bash", action="store_true", default=False, dest="bash")
parser.add_option("-B", "--blind", action="store_true", default=True, dest="blind")
parser.add_option("-N", "--name", action="store", type="string", default="", dest="name")
(options, args) = parser.parse_args()
if options.bash: gROOT.SetBatch(True)
gStyle.SetOptStat(0)

LUMI        = 35867

if options.category=="":
    print "Specify category: AH, SL, ALL"
    sys.exit(2)

if options.signal=="":
    print "Specify signal: ttDM, tDM"
    sys.exit(2)


signals = [10, 20, 50, 100, 200, 300, 500]

try: os.stat('plotsLimit_'+options.name)
except: os.mkdir('plotsLimit_'+options.name)

def fillValues(filename):
    val = {}
    mass = []
    for i, s in enumerate(signals):
        try:
            file = open(filename % s, 'r')
            if file==None:
                print "Signal", filename % s, "does not exist"
                continue
            val[s] = file.read().splitlines()
            if len(val[s]) <= 1:
                signals.remove(s)
                print "Signal", filename % s, "has no values"
                continue
            for i, f in enumerate(val[s]): val[s][i] = float(val[s][i])
            if 'fullCls' in filename: val[1:5]=sorted(val[1:5])
            if not s in mass: mass.append(s)
        except:
            print "File", filename % s, "does not exist"
    return mass, val





def limit(channel, signal):
    multF = 1. # in fb
    filename = "./limitOutput_"+options.name + "/" + signal + "_MChi1_MPhi%d_scalar"+options.bjets+"_"+ channel + "_AsymptoticLimits_grepOutput.txt"
    if(options.mediator=='SC'):
        filename = "./limitOutput_"+options.name + "/" + signal + "_MChi1_MPhi%d_scalar"+options.bjets+"_"+ channel + "_AsymptoticLimits_grepOutput.txt"
    elif(options.mediator=='PS'):
        filename = "./limitOutput_"+options.name + "/" + signal + "_MChi1_MPhi%d_pseudo"+options.bjets+"_"+ channel + "_AsymptoticLimits_grepOutput.txt"
    else:
        print 'WRONG mediator type'
    mass, val = fillValues(filename)
    
    Obs0s = TGraph()
    Exp0s = TGraph()
    Exp1s = TGraphAsymmErrors()
    Exp2s = TGraphAsymmErrors()
    Sign = TGraph()
    pVal = TGraph()
    Best = TGraphAsymmErrors()
    
    for i, m in enumerate(mass):
        if not m in val:
            print "Key Error:", m, "not in value map"
            continue
        
        n = Exp0s.GetN()
        Obs0s.SetPoint(n, m, val[m][0]*multF)
        Exp0s.SetPoint(n, m, val[m][3]*multF)
        Exp1s.SetPoint(n, m, val[m][3]*multF)
        Exp1s.SetPointError(n, 0., 0., val[m][3]*multF-val[m][2]*multF, val[m][4]*multF-val[m][3]*multF)
        Exp2s.SetPoint(n, m, val[m][3]*multF)
        Exp2s.SetPointError(n, 0., 0., val[m][3]*multF-val[m][1]*multF, val[m][5]*multF-val[m][3]*multF)
        #Sign.SetPoint(n, m, val[m][6])
        #pVal.SetPoint(n, m, val[m][7])
        #Best.SetPoint(n, m, val[m][8])
        #Best.SetPointError(m, 0., 0., abs(val[m][9]), abs(val[m][10]))
   
    Exp2s.SetLineWidth(2)
    Exp2s.SetLineStyle(1)
    Obs0s.SetLineWidth(3)
    Obs0s.SetMarkerStyle(0)
    Obs0s.SetLineColor(1)
    Exp0s.SetLineStyle(2)
    Exp0s.SetLineWidth(3)
    Exp1s.SetFillColor(417) #kGreen+1
    Exp1s.SetLineColor(417) #kGreen+1
    Exp2s.SetFillColor(800) #kOrange
    Exp2s.SetLineColor(800) #kOrange
    Exp2s.GetXaxis().SetTitle("m_{#phi} (GeV)")
    Exp2s.GetXaxis().SetTitleSize(Exp2s.GetXaxis().GetTitleSize()*1.25)
    Exp2s.GetXaxis().SetNoExponent(True)
    Exp2s.GetXaxis().SetMoreLogLabels(True)
    Exp2s.GetYaxis().SetTitle("#sigma/#sigma_{th}")
    Exp2s.GetYaxis().SetTitleOffset(1.5)
    Exp2s.GetYaxis().SetNoExponent(True)
    Exp2s.GetYaxis().SetMoreLogLabels()

    Sign.SetLineWidth(2)
    Sign.SetLineColor(629)
    Sign.GetXaxis().SetTitle("m_{#phi} (GeV)")
    Sign.GetXaxis().SetTitleSize(Sign.GetXaxis().GetTitleSize()*1.1)
    Sign.GetYaxis().SetTitle("Significance")
    
    pVal.SetLineWidth(2)
    pVal.SetLineColor(629)
    pVal.GetXaxis().SetTitle("m_{#phi} (GeV)")
    pVal.GetXaxis().SetTitleSize(pVal.GetXaxis().GetTitleSize()*1.1)
    pVal.GetYaxis().SetTitle("local p-Value")
    
    Best.SetLineWidth(2)
    Best.SetLineColor(629)
    Best.SetFillColor(629)
    Best.SetFillStyle(3003)
    Best.GetXaxis().SetTitle("m_{#phi} (GeV)")
    Best.GetXaxis().SetTitleSize(Best.GetXaxis().GetTitleSize()*1.1)
    Best.GetYaxis().SetTitle("Best Fit (pb)")
    
    
    c1 = TCanvas("c1", "Exclusion Limits", 800, 600)
    c1.cd()
    #SetPad(c1.GetPad(0))
    c1.GetPad(0).SetTopMargin(0.06)
    c1.GetPad(0).SetRightMargin(0.05)
    c1.GetPad(0).SetTicks(1, 1)
    #c1.GetPad(0).SetGridx()
    #c1.GetPad(0).SetGridy()
    c1.GetPad(0).SetLogx()
    c1.GetPad(0).SetLogy()
    Exp2s.Draw("A3")
    Exp1s.Draw("SAME, 3")
    Exp0s.Draw("SAME, L")
    if not options.blind: Obs0s.Draw("SAME, L")
    #Theory[0].Draw("SAME, L")
    #Theory[1].Draw("SAME, L")
    #setHistStyle(Exp2s)
    Exp2s.GetXaxis().SetTitleSize(0.045)
    Exp2s.GetXaxis().SetMoreLogLabels(True)
    Exp2s.GetXaxis().SetNoExponent(True)
    Exp2s.GetYaxis().SetTitleSize(0.04)
    Exp2s.GetXaxis().SetLabelSize(0.04)
    Exp2s.GetYaxis().SetLabelSize(0.04)
    Exp2s.GetXaxis().SetTitleOffset(1)
    Exp2s.GetYaxis().SetTitleOffset(1.25)
    Exp2s.GetYaxis().SetMoreLogLabels(True)
    Exp2s.GetYaxis().SetNoExponent(True)
    Exp2s.GetYaxis().SetRangeUser(0.1, 1000.)
    #else: Exp2s.GetYaxis().SetRangeUser(0.1, 1.e2)
    Exp2s.GetXaxis().SetRangeUser(mass[0], mass[-1])
    drawAnalysis("tDM")
    drawRegion(channel, True)
    drawCMS(LUMI, "Preliminary")
    
    if True:
        if(options.mediator=='SC'):
            massT, valT = fillValues("./limitOutput_"+options.name + "/" + signal.replace('tttDM', 'tDM') + "_MChi1_MPhi%d_scalar"+options.bjets+"_"+ channel + "_AsymptoticLimits_grepOutput.txt")
        elif(options.mediator=='PS'):
            massT, valT = fillValues("./limitOutput_"+options.name + "/" + signal.replace('tttDM', 'tDM') + "_MChi1_MPhi%d_pseudo"+options.bjets+"_"+ channel + "_AsymptoticLimits_grepOutput.txt")
        ExpT, ObsT = TGraphAsymmErrors(), TGraphAsymmErrors()
        for i, m in enumerate(massT):
            if not m in val: continue
            ExpT.SetPoint(ExpT.GetN(), m, valT[m][3]*multF)
            ObsT.SetPoint(ObsT.GetN(), m, valT[m][0]*multF)
        ExpT.SetLineWidth(3)
        ExpT.SetLineColor(602) #602
        ExpT.SetLineStyle(5)
        ObsT.SetLineWidth(3)
        ObsT.SetLineColor(602)
        ExpT.SetMarkerStyle(21)
        ObsT.SetMarkerStyle(22)
        ExpT.SetMarkerColor(602)
        ObsT.SetMarkerColor(602)
        ExpT.Draw("SAME, PC")
        #if not options.blind: ObsT.Draw("SAME, P")
        
        if(options.mediator=='SC'):
            massTTT, valTTT = fillValues("./limitOutput_"+options.name + "/" + signal.replace('tttDM', 'ttDM') + "_MChi1_MPhi%d_scalar"+options.bjets+"_"+ channel + "_AsymptoticLimits_grepOutput.txt")
        elif(options.mediator=='PS'):
            massTTT, valTTT = fillValues("./limitOutput_"+options.name + "/" + signal.replace('tttDM', 'ttDM') + "_MChi1_MPhi%d_pseudo"+options.bjets+"_" + channel + "_AsymptoticLimits_grepOutput.txt")

        ExpTTT, ObsTTT = TGraphAsymmErrors(), TGraphAsymmErrors()
        for i, m in enumerate(massTTT):
            if not m in val: continue
            ExpTTT.SetPoint(ExpTTT.GetN(), m, valTTT[m][3]*multF)
            ObsTTT.SetPoint(ObsTTT.GetN(), m, valTTT[m][0]*multF)
        ExpTTT.SetLineWidth(3)
        ExpTTT.SetLineColor(634) #602
        ExpTTT.SetLineStyle(5)
        ObsTTT.SetLineWidth(3)
        ObsTTT.SetLineColor(634)
        ExpTTT.SetMarkerStyle(21)
        ObsTTT.SetMarkerStyle(22)
        ExpTTT.SetMarkerColor(634)
        ObsTTT.SetMarkerColor(634)
        ExpTTT.Draw("SAME, PC")
        #if not options.blind: ObsTTT.Draw("SAME, P")
    
    # legend
    top = 0.9
    nitems = 4+2
    
    leg = TLegend(0.55, top-nitems*0.3/5., 0.95, top)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0) #1001
    leg.SetFillColor(0)
    leg.SetHeader("95% CL limits")
    leg.AddEntry(Obs0s,  "Observed", "l")
    leg.AddEntry(Exp0s,  "Expected (t+DM, tt+DM)", "l")
    leg.AddEntry(Exp1s, "#pm 1 s. d.", "f")
    leg.AddEntry(Exp2s, "#pm 2 s. d.", "f")
    if True:
        leg.AddEntry(ExpT,  "Expected (t+DM)", "p")
        leg.AddEntry(ExpTTT,  "Expected (tt+DM)", "p")
        
    leg.Draw()
    c1.GetPad(0).RedrawAxis()
    c1.GetPad(0).Update()
    if gROOT.IsBatch():
        c1.Print("plotsLimit_"+options.name+"/Exclusion_"+channel+"_"+options.mediator+"_"+options.bjets+".png")
        c1.Print("plotsLimit_"+options.name+"/Exclusion_"+channel+"_"+options.mediator+"_"+options.bjets+".pdf")

    if not gROOT.IsBatch(): raw_input("Press Enter to continue...")
    
#    print "p1s[",
#    for i in range(Exp0s.GetN()):
#        print Exp0s.GetY()[i]+Exp1s.GetErrorYhigh(i), ",",
#    print "],"
#    print "m1s[",
#    for i in range(Exp0s.GetN()):
#        print Exp0s.GetY()[i]-Exp1s.GetErrorYlow(i), ",",
#    print "],"
#    print "[",
#    for i in range(Exp0s.GetN()):
#        print Exp0s.GetY()[i], ",",
#    print "]"
    
    return
    
    # ---------- Significance ----------
    c2 = TCanvas("c2", "Significance", 800, 600)
    c2.cd()
    c2.GetPad(0).SetTopMargin(0.06)
    c2.GetPad(0).SetRightMargin(0.05)
    c2.GetPad(0).SetTicks(1, 1)
    c2.GetPad(0).SetGridx()
    c2.GetPad(0).SetGridy()
    Sign.GetYaxis().SetRangeUser(0., 5.)
    Sign.Draw("AL3")
    drawCMS(LUMI, "Preliminary")
    drawAnalysis(channel[1:3])
    if gROOT.IsBatch():
        c2.Print("plotsLimit_"+options.name+"/Significance/"+channel+"_"+options.mediator+"_"+options.bjets+".png")
        c2.Print("plotsLimit_"+options.name+"/Significance/"+channel+"_"+options.mediator+"_"+options.bjets+".pdf")
#    c2.Print("plotsLimit/Significance/"+channel+suffix+".root")
#    c2.Print("plotsLimit/Significance/"+channel+suffix+".C")

    # ---------- p-Value ----------
    c3 = TCanvas("c3", "p-Value", 800, 600)
    c3.cd()
    c3.GetPad(0).SetTopMargin(0.06)
    c3.GetPad(0).SetRightMargin(0.05)
    c3.GetPad(0).SetTicks(1, 1)
    c3.GetPad(0).SetGridx()
    c3.GetPad(0).SetGridy()
    c3.GetPad(0).SetLogy()
    pVal.Draw("AL3")
    pVal.GetYaxis().SetRangeUser(2.e-7, 0.5)
    
    ci = [1., 0.317310508, 0.045500264, 0.002699796, 0.00006334, 0.000000573303, 0.000000001973]
    line = TLine()
    line.SetLineColor(922)
    line.SetLineStyle(7)
    text = TLatex()
    text.SetTextColor(922)
    text.SetTextSize(0.025)
    text.SetTextAlign(12)
    for i in range(1, len(ci)-1):
        line.DrawLine(pVal.GetXaxis().GetXmin(), ci[i]/2, pVal.GetXaxis().GetXmax(), ci[i]/2);
        text.DrawLatex(pVal.GetXaxis().GetXmax()*1.01, ci[i]/2, "%d #sigma" % i);
    
    drawCMS(LUMI, "Preliminary")
    drawAnalysis(channel[1:3])
    if gROOT.IsBatch():
        c3.Print("plotsLimit_"+options.name+"/pValue/"+channel+suffix+"_"+options.mediator+"_"+options.bjets+".png")
        c3.Print("plotsLimit_"+options.name+"/pValue/"+channel+suffix+"_"+options.mediator+"_"+options.bjets+".pdf")
#    c3.Print("plotsLimit/pValue/"+channel+suffix+".root")
#    c3.Print("plotsLimit/pValue/"+channel+suffix+".C")

    # --------- Best Fit ----------
    c4 = TCanvas("c4", "Best Fit", 800, 600)
    c4.cd()
    c4.GetPad(0).SetTopMargin(0.06)
    c4.GetPad(0).SetRightMargin(0.05)
    c4.GetPad(0).SetTicks(1, 1)
    c4.GetPad(0).SetGridx()
    c4.GetPad(0).SetGridy()
    Best.Draw("AL3")
    drawCMS(LUMI, "Preliminary")
    drawAnalysis(channel[1:3])
    if gROOT.IsBatch():
        c4.Print("plotsLimit_"+options.name+"/BestFit/"+channel+suffix+"_"+options.mediator+"_"+options.bjets+".png")
        c4.Print("plotsLimit_"+options.name+"/BestFit/"+channel+suffix+"_"+options.mediator+"_"+options.bjets+".pdf")
#    c4.Print("plotsLimit/BestFit/"+channel+suffix+".root")
#    c4.Print("plotsLimit/BestFit/"+channel+suffix+".C")
    
    if not gROOT.IsBatch(): raw_input("Press Enter to continue...")



if options.all:
    limit("tttDM")
else:
    limit(options.category, options.signal)
