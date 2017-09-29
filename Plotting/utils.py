import os, multiprocessing
import copy
import math
from array import array
from ROOT import ROOT, gROOT, gStyle, gRandom, TSystemDirectory
from ROOT import TFile, TChain, TTree, TCut, TH1, TH1F, TH2F, THStack, TGraph, TGraphAsymmErrors
from ROOT import TStyle, TCanvas, TPad
from ROOT import TLegend, TLatex, TText, TLine, TBox, TMath

#from DMPD.Heppy.tools.samples import *
#from DMPD.Heppy.tools.variables import *
#from DMPD.Heppy.tools.selections import *
#from DMPD.Heppy.samples.Spring15.xSections import *

#triggerSF = {'HLT_BIT_HLT_Mu45_eta2p1_v' : 0.94, 'HLT_BIT_HLT_Mu50_v' : 0.94, 'HLT_BIT_HLT_IsoMu27_v' : 1.002, 'HLT_BIT_HLT_Ele27_WP85_Gsf_v' : 0.9995, 'HLT_BIT_HLT_Ele105_CaloIdVT_GsfTrkIdT_v' : 0.97, 'HLT_BIT_HLT_PFMET170_NoiseCleaned_v' : 0.9697}


##################
# ANALYSIS UTILS #
##################

#def project(var, cut, reg, weight, samples, pd, ntupledir):
#    # Create dict
#    file = {}
#    tree = {}
#    hist = {}
#    
#    ### Create and fill MC histograms ###
#    for i, s in enumerate(samples):
#        if "HIST" in cut: # Histogram written to file
#            tmphist = file[ss].Get(var)
#        else: # Project from tree
#            tree[s] = TChain(reg)
#            for j, ss in enumerate(sample[s]['files']):
#                if not 'data' in s or ('data' in s and ss in pd):
#                    tree[s].Add(ntupledir + ss + ".root")
#            if variable[var]['nbins']>0: hist[s] = TH1F(s, ";"+variable[var]['title'], variable[var]['nbins'], variable[var]['min'], variable[var]['max']) # Init histogram
#            else: hist[s] = TH1F(s, ";"+variable[var]['title'], len(variable[var]['bins'])-1, array('f', variable[var]['bins']))
#            hist[s].Sumw2()
#            tmpcut = cut
#            if not 'data' in s:
#                if s.endswith('_0b'): tmpcut += " && nBJets==0"
#                elif s.endswith('_1b'): tmpcut += " && nBJets==1"
#                elif s.endswith('_2b'): tmpcut += " && nBJets>=2"
#                if s.endswith('_0l'): tmpcut += " && genNl==0"
#                elif s.endswith('_1l'): tmpcut += " && genNl==1"
#                elif s.endswith('_2l'): tmpcut += " && genNl>=2"
#            cutstring = "("+weight+")" + ("*("+tmpcut+")" if len(tmpcut)>0 else "")
#            tree[s].Project(s, var, cutstring)
#            hist[s].SetOption("%s" % tree[s].GetTree().GetEntriesFast())
#            hist[s].Scale(sample[s]['weight'] if hist[s].Integral() >= 0 else 0)

#        hist[s].SetFillColor(sample[s]['fillcolor'])
#        hist[s].SetFillStyle(sample[s]['fillstyle'])
#        hist[s].SetLineColor(sample[s]['linecolor'])
#        hist[s].SetLineStyle(sample[s]['linestyle'])
#    
#    return hist


#def draw(hist, channel, data, back, sign, snorm=1, lumi=-1, ratio=0, log=False):
#    # If not present, create BkgSum
#    if not 'BkgSum' in hist.keys():
#        hist['BkgSum'] = hist['data_obs'].Clone("BkgSum") if 'data_obs' in hist else hist[back[0]].Clone("BkgSum")
#        hist['BkgSum'].Reset("MICES")
#        for i, s in enumerate(back): hist['BkgSum'].Add(hist[s])
#    hist['BkgSum'].SetMarkerStyle(0)
#    
#    # Set Poisson error bars
#    #if len(data) > 0: hist['data_obs'].SetBinErrorOption(1) # doesn't work
#    
#    alpha = 1 - 0.6827
#    hist['data_obs'].SetBinErrorOption(TH1.kPoisson)
#    data_graph = TGraphAsymmErrors(hist['data_obs'].GetNbinsX())
#    data_graph.SetMarkerStyle(hist['data_obs'].GetMarkerStyle())
#    data_graph.SetMarkerSize(hist['data_obs'].GetMarkerSize())
#    res_graph = data_graph.Clone()
#    for i in range(hist['data_obs'].GetNbinsX()):
#        N = hist['data_obs'].GetBinContent(i+1)
#        B = hist['BkgSum'].GetBinContent(i+1)
#        L =  0 if N==0 else ROOT.Math.gamma_quantile(alpha/2,N,1.)
#        U =  ROOT.Math.gamma_quantile_c(alpha/2,N+1,1)
#        data_graph.SetPoint(i, hist['data_obs'].GetXaxis().GetBinCenter(i+1), N if not N==0 else -1.e99)
#        data_graph.SetPointError(i, hist['data_obs'].GetXaxis().GetBinWidth(i+1)/2., hist['data_obs'].GetXaxis().GetBinWidth(i+1)/2., N-L, U-N)
#        res_graph.SetPoint(i, hist['data_obs'].GetXaxis().GetBinCenter(i+1), N/B if not B==0 and not N==0 else -1.e99)
#        res_graph.SetPointError(i, hist['data_obs'].GetXaxis().GetBinWidth(i+1)/2., hist['data_obs'].GetXaxis().GetBinWidth(i+1)/2., (N-L)/B if not B==0 else -1.e99, (U-N)/B if not B==0 else -1.e99)
#    
#    
#    # Create stack
#    bkg = THStack("Bkg", ";"+hist['BkgSum'].GetXaxis().GetTitle()+";Events")
#    for i, s in enumerate(back): bkg.Add(hist[s])
#    
#    # Legend
#    n = len([x for x in data+back+['BkgSum']+sign if sample[x]['plot']])
#    leg = TLegend(0.7, 0.9-0.05*n, 0.95, 0.9)
#    leg.SetBorderSize(0)
#    leg.SetFillStyle(0) #1001
#    leg.SetFillColor(0)
#    if len(data) > 0:
#        leg.AddEntry(hist[data[0]], sample[data[0]]['label'], "pl")
#    for i, s in reversed(list(enumerate(['BkgSum']+back))):
#        leg.AddEntry(hist[s], sample[s]['label'], "f")
#    for i, s in enumerate(sign):
#        if sample[s]['plot']: leg.AddEntry(hist[s], sample[s]['label'].replace("m_{#Chi}=1 GeV", ""), "fl")
#    
#    
#    # --- Display ---
#    c1 = TCanvas("c1", hist.values()[0].GetXaxis().GetTitle(), 800, 800 if ratio else 600)
#    
#    if ratio:
#        c1.Divide(1, 2)
#        setTopPad(c1.GetPad(1), ratio)
#        setBotPad(c1.GetPad(2), ratio)
#    c1.cd(1)
#    c1.GetPad(bool(ratio)).SetTopMargin(0.06)
#    c1.GetPad(bool(ratio)).SetRightMargin(0.05)
#    c1.GetPad(bool(ratio)).SetTicks(1, 1)
#    if log:
#        c1.GetPad(bool(ratio)).SetLogy()
#        
#    # Draw
#    bkg.Draw("HIST") # stack
#    hist['BkgSum'].Draw("SAME, E2") # sum of bkg
#    #if len(data) > 0: hist['data_obs'].Draw("SAME, PE") # data
#    data_graph.Draw("SAME, PE")
#    for i, s in enumerate(sign):
#        if sample[s]['plot']:
#            hist[s].DrawNormalized("SAME, HIST", hist[s].Integral()*snorm) # signals
#    
#    bkg.GetYaxis().SetTitleOffset(bkg.GetYaxis().GetTitleOffset()*1.075)
#    bkg.SetMaximum((2. if log else 1.2)*max(bkg.GetMaximum(), hist['data_obs'].GetBinContent(hist['data_obs'].GetMaximumBin())+hist['data_obs'].GetBinError(hist['data_obs'].GetMaximumBin())))
#    bkg.SetMinimum(max(min(hist['BkgSum'].GetBinContent(hist['BkgSum'].GetMinimumBin()), hist['data_obs'].GetMinimum()), 5.e-1)  if log else 0.)
#    if log:
#        bkg.GetYaxis().SetNoExponent(bkg.GetMaximum() < 1.e4)
#        bkg.GetYaxis().SetMoreLogLabels(True)
#    
#    #if log: bkg.SetMinimum(1)
#    leg.Draw()
#    drawCMS(lumi, "Preliminary")
#    drawRegion(channel)
#    drawAnalysis(channel)
#    
#    #if nm1 and not cutValue is None: drawCut(cutValue, bkg.GetMinimum(), bkg.GetMaximum()) #FIXME
#    if len(sign) > 0:
#        if channel.startswith('X') and len(sign)>0: drawNorm(0.9-0.04*(n+1), "#sigma(X) #times B(X #rightarrow VH) = %.1f pb" % snorm)
#        #elif "SR" in channel: drawNorm(0.9-0.04*(n+1), "DM+bb/tt, scaled by %.0f" % snorm, "m_{#chi}=1 GeV, scalar mediator")
#        elif "SR" in channel: drawNorm(0.9-0.04*(n+1), "DM+bb/tt, m_{#chi}=1 GeV", "scalar mediator")
#    
#    setHistStyle(bkg, 1.2 if ratio else 1.1)
#    setHistStyle(hist['BkgSum'], 1.2 if ratio else 1.1)
#       
#    if ratio:
#        c1.cd(2)
#        err = hist['BkgSum'].Clone("BkgErr;")
#        err.SetTitle("")
#        err.GetYaxis().SetTitle("Data / Bkg")
#        for i in range(1, err.GetNbinsX()+1):
#            err.SetBinContent(i, 1)
#            if hist['BkgSum'].GetBinContent(i) > 0:
#                err.SetBinError(i, hist['BkgSum'].GetBinError(i)/hist['BkgSum'].GetBinContent(i))
#        setBotStyle(err)
#        errLine = err.Clone("errLine")
#        errLine.SetLineWidth(1)
#        errLine.SetFillStyle(0)
#        res = hist['data_obs'].Clone("Residues")
#        for i in range(0, res.GetNbinsX()+1):
#            if hist['BkgSum'].GetBinContent(i) > 0: 
#                res.SetBinContent(i, res.GetBinContent(i)/hist['BkgSum'].GetBinContent(i))
#                res.SetBinError(i, res.GetBinError(i)/hist['BkgSum'].GetBinContent(i))
#        setBotStyle(res)
#        #err.GetXaxis().SetLabelOffset(err.GetXaxis().GetLabelOffset()*5)
#        #err.GetXaxis().SetTitleOffset(err.GetXaxis().GetTitleOffset()*2)
#        err.Draw("E2")
#        errLine.Draw("SAME, HIST")
#        if len(data) > 0:
#            #res.Draw("SAME, PE0")
#            res_graph.Draw("SAME, PE0")
##            if len(err.GetXaxis().GetBinLabel(1))==0: # Bin labels: not a ordinary plot
##                drawRatio(hist['data_obs'], hist['BkgSum'])
##                drawKolmogorov(hist['data_obs'], hist['BkgSum'])
#    
#    c1.Update()
#    
#    # return list of objects created by the draw() function
#    return [c1, bkg, leg, err, errLine, res, data_graph, res_graph]




#def saveHist(hist, channel, analysis, addStat=False):
#    
#    #if not 'XZh' in channel or not 'monoB' in channel: return True
#    # Blind
#    #if options.blind: hist['data_obs'] = hist['BkgSum'].Clone("data_obs")
#    
#    # Re-normalize signals
#    #for s in hist.keys():
#        #if 'bbDM' in s or 'ttDM' in s:
#            #print "scaling", s, "by", xsections[sample[s]['files'][0][:-3]]
#            #hist[s].Scale( 1./xsections[sample[s]['files'][0][:-3]] )
#        #if 'bbDM' in s:
#            #hist[s].Scale( kfactors4F5F[sample[s]['files'][0][:-3]] )
#    
#    # Normalize to lumi
#    #for s in hist.keys():
#    #    hist[s].Scale(3000/LUMI)
#    
#    # Fix name
#    #hist['data_obs'].SetName("data_obs")
#    
#    # Merge Z->ll and Z->nunu together
#    if 'DYJetsToNuNu_HT' in hist:
#        if 'DYJetsToLL_HT' in hist:
#            hist['DYJets_HT'] = hist['DYJetsToLL_HT'].Clone('DYJets_HT')
#            hist['DYJets_HT'].Add(hist['DYJetsToNuNu_HT'])
#            #hist['DYJetsToLL_HT'].SetName("DYJets_HT")
#        elif 'DYJetsToLL' in hist:
#            hist['DYJets'] = hist['DYJetsToLL'].Clone('DYJets')
#            hist['DYJets'].Add(hist['DYJetsToNuNu'])
#            #hist['DYJetsToLL'].SetName("DYJets")
#        #hist.pop('DYJetsToNuNu_HT')
#        
#    
#    # Merge Others
#    #hist['Others'] = copy.deepcopy( hist['BkgSum'] )
#    #hist['Others'].SetFillColor(920)
#    #hist['Others'].SetName("Others")
#    #hist['Others'].Reset()
#    #for i, s in enumerate(othr):
#    #    hist['Others'].Add( hist[s] )
#    
#    # Sanity check
##    smax = max(hist, key=lambda x: hist[x].Integral())
##    for i in range(hist[smax].GetNbinsX()):#FIXME
##    for s in hist.keys():
##        if not hist[s].GetBinContent(i+1)>0.: hist[s].SetBinContent(i+1, 1.e-6) # Sanity check
#    
#    nbins = hist['data_obs'].GetNbinsX()
#    #CMS_stat_Z0b_bin57
#    
#    outFile = TFile("rootfiles/"+analysis+"Hist.root", "UPDATE")
#    outFile.cd()
#    if outFile.GetDirectory(channel): outFile.rmdir(channel)
#    outFile.mkdir(channel)
#    outFile.cd(channel)
#    #hist['data_obs'].Write("data_obs")
#    #hist['Others'].Write()
#    for s in hist.keys():
#        #if not 'DYJetsToLL' in s and not 'DYJetsToNuNu' in s: 
#        hist[s].Write()
#    outFile.cd("..")
#    # Statistical MC uncertainty
#    if addStat:
#        for s in hist.keys():
#            if 'data' in s: continue
#            dirname = channel+"/Sys_"+s
#            if outFile.GetDirectory(dirname): outFile.rmdir(dirname)
#            outFile.mkdir(dirname)
#            outFile.cd(dirname)
#            for k in range(1, nbins+1):
#                sysname = "CMS_stat_%s_%s_bin%d" % (channel, s, k)
#                histUp = hist[s].Clone(sysname+"Up")
#                histUp.SetBinContent(k, histUp.GetBinContent(k) + histUp.GetBinError(k))
#                histUp.Write()
#                histDown = hist[s].Clone(sysname+"Down")
#                histDown.SetBinContent(k, max(histDown.GetBinContent(k) - histDown.GetBinError(k), 1.e-6))
#                histDown.Write()
#            outFile.cd("..")
#    
#    
#    outFile.Close()
#    print "Histograms saved in file rootfiles/"+analysis+"Hist.root"

###################################################

#def applyStyle(hist):
#    for s, h in hist.iteritems():
#        if s=="Data" or s=="data_obs":
#            h.SetMarkerStyle(20)
#            h.SetMarkerSize(1.25)
#        elif s=="BkgSum":
#            h.SetFillStyle(3003)
#            h.SetFillColor(1)
#        else:
#            h.SetFillColor(sample[s]['fillcolor'])
#            h.SetFillStyle(sample[s]['fillstyle'])
#            h.SetLineColor(sample[s]['linecolor'])
#            h.SetLineWidth(sample[s]['linewidth'])
#            h.SetLineStyle(sample[s]['linestyle'])



## Merge Z->ll and Z->nunu together
#def mergeDY(hist):
#    origin = [x for x in hist if 'DYJetsToLL' in x]
#    delete = [x.replace('DYJetsToLL', 'DYJetsToNuNu') for x in origin]
#    for i, o in enumerate(origin):
#        hist[origin[i]].Add(hist[delete[i]])
#        hist[origin[i]].SetName(o.replace('ToLL', ''))
#        #print "SETNAME", o, hist[origin[i]].GetName()
#        hist.pop(delete[i])
#    
##    if 'DYJetsToNuNu_HT' in hist: ori = 'DYJetsToNuNu_HT'
##    
##        if 'DYJetsToLL_HT' in hist:
##            hist['DYJetsToLL_HT'].Add(hist['DYJetsToNuNu_HT'])
##            hist['DYJetsToLL_HT'].SetName("DYJets_HT")
##        elif 'DYJetsToLL' in hist:
##            hist['DYJetsToLL'].Add(hist['DYJetsToNuNu_HT'])
##            hist['DYJetsToLL'].SetName("DYJets")
##        hist.pop('DYJetsToNuNu_HT')
#    return hist



def printTable(hist, sign=[]):
    samples = [x for x in hist.keys() if not 'data' in x and not 'BkgSum' in x and not x in sign]
    print "Sample                  Events          Entries         %"
    print "-"*60
    for i, s in enumerate(['data_obs']+samples+['BkgSum']):
        if i==1 or i==len(samples)+1: print "-"*60
        print "%-20s" % s, "\t%-10.2f" % hist[s].Integral(), "\t%-10.0f" % (hist[s].GetEntries()-2), "\t%-10.2f" % (100.*hist[s].Integral()/hist['BkgSum'].Integral()) if hist['BkgSum'].Integral() > 0 else 0
    print "-"*60
    for i, s in enumerate(sign):
        #if not sample[s]['plot']: continue
        print "%-20s" % s, "\t%-10.2f" % hist[s].Integral(), "\t%-10.0f" % (hist[s].GetEntries()-2), "\t%-10.2f" % 0.
    print "-"*60




def addSystematics():
    # Read Histograms
    hist = []
    histFile = TFile("rootfiles/monoX.root", "READ")
    for k, s in enumerate(sample):
        hist.append(histFile.Get(s))
        print hist[k].GetName(), hist[k].Integral() 
    hist.append(histFile.Get("AllBkg"))
    
    # Read systematics
    ns = 10
    sysUp = []
    sysDown = []
    sysFile = TFile("rootfiles/systematics.root", "READ")
    for k in range(ns):
        up = sysFile.Get("Sys%dUp" % k)
        down = sysFile.Get("Sys%dDown" % k)
        sysUp.append(up)
        sysDown.append(down)
    #sysFile.Close()
    
    
    outFile = TFile("rootfiles/monoX.root", "RECREATE")
    outFile.cd()
    hist[0].SetName("data_obs")
    hist[0].Write("data_obs")
    for i, s in list(enumerate(sample[1:])):
        name = hist[i+1].GetName()
        hist[i+1].Write()
        outFile.mkdir("Sys"+name)
        outFile.cd("Sys"+name)
        for k in range(10):
            up = hist[i+1].Clone(name+"_Sys%dUp" % k)
            up.Multiply(sysUp[k])
            up.Write()
            down = hist[i+1].Clone(name+"_Sys%dDown" % k)
            down.Multiply(sysDown[k])
            down.Write()
        outFile.cd()
    hist[len(hist)-1].Write("AllBkg")
    outFile.Close()


def convertHistToGraph(hist):
    graph = TGraphAsymmErrors(hist.GetNbinsX())
    for i in range(hist.GetNbinsX()):
        graph.SetPoint(i, hist.GetXaxis().GetBinCenter(i), hist.GetBinContent(i))
        graph.SetPointError(i, hist.GetXaxis().GetBinWidth(i)/2., hist.GetXaxis().GetBinWidth(i)/2., hist.GetBinError(i), hist.GetBinError(i))
    graph.SetLineWidth(hist.GetLineWidth())
    graph.SetLineStyle(hist.GetLineStyle())
    graph.SetLineColor(hist.GetLineColor())
    graph.SetMarkerSize(hist.GetMarkerSize())
    graph.SetMarkerStyle(hist.GetMarkerStyle())
    graph.SetMarkerColor(hist.GetMarkerColor())
    graph.SetFillStyle(hist.GetFillStyle())
    graph.SetFillColor(hist.GetFillColor())
    return graph

##################
### DRAW UTILS ###
##################

def getChannel(channel):
    text = ""
    #
    if 'Electrons' in channel: return "2e, 2 jets"
    elif 'Muons' in channel: return "2#mu, 2 jets"
    elif 'MET' in channel: return "0l, 2 jets"
    isAH = ('hp' in channel or 'lp' in channel or 'qqqq' in channel)
    #
    if 'ah' in channel: return "All categories"
    if 'VHsl' in channel: return "0l, 1l, 2l categories"
    if 'WHsl' in channel: return "1l categories"
    if 'ZHsl' in channel: return "0l, 2l categories"
    if 'sum' in channel: return "Total"
    # leptons
    if 'ee' in channel or '2e' in channel: text += "2e"
    elif 'e' in channel or '1e' in channel: text += "1e"
    if 'mm' in channel or '2m' in channel: text += "2#mu"
    elif 'm' in channel or '1m' in channel: text += "1#mu"
    if 'll' in channel or '2l' in channel: text += "2l"
    elif 'ln' in channel or '2l' in channel: text += "1l"
    if 'nn' in channel or '0l' in channel: text += "0l"
    #if 'qq' in channel or 'hp' in channel or 'lp' in channel: text += "all-had"
    if 'sb' in channel: text += "sidebands"
    elif 'zr' in channel: text += "Z mass"
    elif 'wr' in channel: text += "W mass"
    elif 'vr' in channel: text += "V b tag"
    # purity
    if 'hp' in channel: text += ", high purity"
    elif 'lp' in channel: text += ", low purity"
    elif 'ap' in channel: text += ", no purity"
    # b-tag
    if 'bbbb' in channel or '4b' in channel: text += ", tight b tag" if isAH else ", 4 b tag"
    elif 'bbb' in channel or '3b' in channel: text += ", loose b tag" if isAH else ", 3 b tag"
    elif 'bb' in channel or '2b' in channel: text += ", tight b tag" if isAH else ", 2 b tag"
    elif 'b' in channel or '1b' in channel: text += ", loose b tag" if isAH else ", 1 b tag"
    # region
    if 'SR' in channel: text += ", signal region"
    elif 'SB' in channel: text += ", sidebands region"
    elif 'TR' in channel: text += ", top control region"
    elif 'WR' in channel: text += ", W control region"
    elif 'ZR' in channel: text += ", Z control region"
    elif 'MC' in channel: text += ", simulation"
    elif 'Inc' in channel: text += ", inclusive region"
    
    return text

def drawAnalysis(s, center=False):
    text = ""
    if s.startswith('X'):
        if 'ah' in s or 'qq' in s or 'hp' in s or 'lp' in s or 'ap' in s or 'sb' in s:
            if "ZH" in s: text = "X #rightarrow ZH #rightarrow q#bar{q}b#bar{b}"
            elif "WH" in s: text = "X #rightarrow WH #rightarrow q#bar{q}b#bar{b}"
            else: text = "X #rightarrow VH #rightarrow q#bar{q}b#bar{b}"
        elif 'mm' in s or 'ee' in s or 'mn' in s or 'en' in s or 'nn' in s: text = "X #rightarrow VH #rightarrow (#nu#nu,l#nu,ll)b#bar{b}"
        elif "ZH" in s: text = "Z' #rightarrow ZH #rightarrow (#nu#nu,ll)bb"
        elif "WH" in s: text = "W' #rightarrow WH #rightarrow l#nu bb"
        elif "sl" in s: text = "X #rightarrow VH #rightarrow (#nu#nu,l#nu,ll)bb"
    elif "AZh" in s: text = "A #rightarrow Zh #rightarrow (#nu#nu,ll)bb"
    elif "DM" in s:
        text = "t+DM"
        if 'AH' in s: text += "   (hadronic channel)"
        if 'SL' in s: text += "   (semileptonic channel)"
    else: return True
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextSize(0.04)
    latex.SetTextFont(42)
    #latex.SetTextAlign(33)
    latex.DrawLatex(0.15 if not center else 0.25, 0.95, text)

def drawRegion(channel, left=False):
    #if not channel.startswith('X'): return False
    text = getChannel(channel)
    
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextFont(72) #52
    latex.SetTextSize(0.04)
    if left: latex.DrawLatex(0.15, 0.75, text)
    else:
        latex.SetTextAlign(22)
        latex.DrawLatex(0.5, 0.85, text)


def drawCMS(lumi, text, onTop=False):
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextSize(0.04)
    latex.SetTextColor(1)
    latex.SetTextFont(42)
    latex.SetTextAlign(33)
    if (type(lumi) is float or type(lumi) is int):
        if float(lumi) > 0: latex.DrawLatex(0.95, 0.985, "%.1f fb^{-1}  (13 TeV)" % (float(lumi)/1000.))
        else: latex.DrawLatex(0.95, 0.985, "(13 TeV)")
    elif type(lumi) is str: latex.DrawLatex(0.95, 0.985, "%s  (13 TeV)" % lumi)
    if not onTop: latex.SetTextAlign(11)
    latex.SetTextFont(62)
    latex.SetTextSize(0.05 if len(text)>0 else 0.06)
    if not onTop: latex.DrawLatex(0.15, 0.87 if len(text)>0 else 0.84, "CMS")
    else: latex.DrawLatex(0.20, 0.99, "CMS")
    latex.SetTextSize(0.04)
    latex.SetTextFont(52)
    if not onTop: latex.DrawLatex(0.15, 0.83, text)
    else: latex.DrawLatex(0.4, 0.98, text)
#    latex.SetTextSize(0.04)
#    latex.SetTextFont(62)
#    latex.SetTextAlign(13)
#    latex.DrawLatex(0.45, 0.98, "DM monojet")

def drawOverflow():
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextAngle(90)
    latex.SetTextSize(0.03)
    #latex.SetTextColor(1)
    latex.SetTextFont(42)
    #latex.SetTextAlign(33)
    latex.DrawLatex(0.97, 0.02, "Overflow")

def drawBox(x1, y1, x2, y2, t=""):
    box = TBox(x1, y1, x2, y2)
    box.SetFillColor(1)
    box.SetFillStyle(3004)
    box.Draw()
    if not t=="":
        text = TLatex()
        text.SetTextColor(1)
        text.SetTextFont(42)
        text.SetTextAlign(23)
        text.SetTextSize(0.04)
        text.DrawLatex((x1+x2)/2., y2/1.15, t)
        text.Draw()
    return box

def drawLine(x1, y1, x2, y2):
    line = TLine(x1, y1, x2, y2)
    line.SetLineStyle(2)
    line.SetLineWidth(2)
    line.Draw()
    return line

def drawText(x, y, t, isNDC=False):
    text = TLatex()
    text.SetTextColor(1)
    text.SetTextFont(42)
    text.SetTextAlign(23)
    text.SetTextSize(0.04)
    if isNDC: text.DrawLatexNDC(x, y, t)
    else: text.DrawLatex(x, y, t)
    text.Draw()
    return text


def drawMass(m):
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextColor(1)
    latex.SetTextFont(42)
    latex.SetTextAlign(22)
    latex.SetTextSize(0.04)
    latex.DrawLatex(0.75, 0.85, "m_{X} = %.0f GeV" % m)

def drawChi2(chi2, ndf=1, prob=False):
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextColor(1)
    latex.SetTextFont(62)
    latex.SetTextSize(0.08)
    if prob: latex.DrawLatex(0.65, 0.85, "#chi^{2}/ndf = %.1f/%.0f     p-value = %.2f" % (chi2, ndf, TMath.Prob(chi2, ndf)))
    elif ndf==1: latex.DrawLatex(0.75, 0.85, "#chi^{2} = %.3f" % chi2)
    else: latex.DrawLatex(0.75, 0.85, "#chi^{2}/ndf = %.1f/%.0f" % (chi2, ndf))

def drawKS(data, bkg):
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextColor(1)
    latex.SetTextFont(62)
    latex.SetTextSize(0.08)
    latex.DrawLatex(0.65, 0.85, "K-S p-value = %.3f" % (data.KolmogorovTest(bkg)))

def drawMediator(med):
    text = "Pseudoscalar" if 'ps' in med else "Scalar"
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextSize(0.035)
    latex.SetTextFont(42)
    latex.DrawLatex(0.15, 0.70, text+" mediator") #, g_{#Phi q#bar{q}} Yukawa
    latex.DrawLatex(0.15, 0.65, "g_{q}=1,   m_{#chi}=1 GeV")


def drawNorm(y, text, secondline=""):
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextColor(1)
    latex.SetTextFont(42)
    latex.SetTextSize(0.032)
    latex.DrawLatex(0.71, y, text)
    if len(secondline) > 0: latex.DrawLatex(0.71, y-0.045, secondline)
    
def drawRatio(data, bkg):
    errData = array('d', [1.0])
    errBkg = array('d', [1.0])
    intData = data.IntegralAndError(1, data.GetNbinsX(), errData)
    intBkg = bkg.IntegralAndError(1, bkg.GetNbinsX(), errBkg)
    ratio = intData / intBkg if intBkg!=0 else 0.
    error = math.hypot(errData[0]*ratio/intData,  errBkg[0]*ratio/intBkg) if intData>0 and intBkg>0 else 0
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextColor(1)
    latex.SetTextFont(62)
    latex.SetTextSize(0.08)
    latex.DrawLatex(0.25, 0.85, "Data/Bkg = %.3f #pm %.3f" % (ratio, error))
    print "  Ratio:\t%.3f +- %.3f" % (ratio, error)
    #return [ratio, error]


def drawStat(data, bkg):
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextColor(1)
    latex.SetTextFont(62)
    latex.SetTextSize(0.08)
#    chi2 = min(data.Chi2Test(bkg, "UW,CHI2/NDF"), data.Chi2Test(bkg, "WW,CHI2/NDF"))
    chi2 = data.Chi2Test(bkg, "UW,CHI2/NDF")
#    for i in range(bkg.GetNbinsX()):
#        if bkg.GetBinContent(i+1) < 1.e-6: bkg.SetBinError(i+1, 0.)
    latex.DrawLatex(0.55, 0.85, "#chi^{2}/ndf = %.2f,   K-S = %.3f" % (chi2, data.KolmogorovTest(bkg)))

def drawCut(cut, ymin, ymax):
    line = TLine()
    line.SetLineWidth(2)
    line.SetLineStyle(7)
    line.SetLineColor(1)
    line.PaintLineNDC(cut, ymin, cut, ymax)

def setHistStyle(hist, r=1.1):
    hist.GetXaxis().SetTitleSize(hist.GetXaxis().GetTitleSize()*r*r)
    hist.GetYaxis().SetTitleSize(hist.GetYaxis().GetTitleSize()*r*r)
    hist.GetXaxis().SetLabelSize(hist.GetXaxis().GetLabelSize()*r)
    hist.GetYaxis().SetLabelSize(hist.GetYaxis().GetLabelSize()*r)
    hist.GetXaxis().SetLabelOffset(hist.GetXaxis().GetLabelOffset()*r*r*r*r*r*r)
    hist.GetXaxis().SetTitleOffset(hist.GetXaxis().GetTitleOffset()*r)
    hist.GetYaxis().SetTitleOffset(hist.GetYaxis().GetTitleOffset())
    if hist.GetXaxis().GetTitle().find("GeV") != -1: # and not hist.GetXaxis().IsVariableBinSize()
        div = (hist.GetXaxis().GetXmax() - hist.GetXaxis().GetXmin()) / hist.GetXaxis().GetNbins()
        hist.GetYaxis().SetTitle("Events / %.1f GeV" % div)

def addOverflow(hist, addUnder=True):
    if not hasattr(hist, "GetNbinsX"): return
    n = hist.GetNbinsX()
    hist.SetBinContent(n, hist.GetBinContent(n) + hist.GetBinContent(n+1))
    hist.SetBinError(n, math.sqrt( hist.GetBinError(n)**2 + hist.GetBinError(n+1)**2 ) )
    #hist.SetBinContent(n+1, 0.) # For some reason, this doubles the number of bins sometimes
    #hist.SetBinError(n+1, 0.)
    if addUnder:
        hist.SetBinContent(1, hist.GetBinContent(0) + hist.GetBinContent(1))
        hist.SetBinError(1, math.sqrt( hist.GetBinError(0)**2 + hist.GetBinError(1)**2 ) )
        hist.SetBinContent(0, 0.)
        hist.SetBinError(0, 0.)

def setTopPad(TopPad, r=4):
    TopPad.SetPad("TopPad", "", 0., 1./r, 1.0, 1.0, 0, -1, 0)
    TopPad.SetTopMargin(0.24/r)
    TopPad.SetBottomMargin(2*0.04/r)
    TopPad.SetRightMargin(0.05)
    TopPad.SetTicks(1, 1)

def setBotPad(BotPad, r=4):
    BotPad.SetPad("BotPad", "", 0., 0., 1.0, 1./r, 0, -1, 0)
    BotPad.SetTopMargin(0.5*r/100.)
    BotPad.SetBottomMargin(r/10.)
    BotPad.SetRightMargin(0.05)
    BotPad.SetTicks(1, 1)

def setBotStyle(h, r=4, fixRange=True):
    h.GetXaxis().SetLabelSize(h.GetXaxis().GetLabelSize()*(r-1));
    h.GetXaxis().SetLabelOffset(h.GetXaxis().GetLabelOffset()*(r-1));
    h.GetXaxis().SetTitleSize(h.GetXaxis().GetTitleSize()*(r-1));
    h.GetYaxis().SetLabelSize(h.GetYaxis().GetLabelSize()*(r-1));
    h.GetYaxis().SetNdivisions(505);
    h.GetYaxis().SetTitleSize(h.GetYaxis().GetTitleSize()*(r-1));
    h.GetYaxis().SetTitleOffset(h.GetYaxis().GetTitleOffset()/(r-1));
    if fixRange:
        h.GetYaxis().SetRangeUser(0., 2.)
        for i in range(1, h.GetNbinsX()+1):
            if h.GetBinContent(i)<1.e-6:
                h.SetBinContent(i, -1.e-6)
    
def drawCut(hist):
    #drawCut(80, 140, 0., hist['BkgSum'].GetMaximum())
    line1 = TLine(80, 0, 80, hist['BkgSum'].GetMaximum())
    line1.SetLineWidth(2)
    line1.SetLineStyle(7)
    line1.SetLineColor(1)
    line1.Draw()
    
    line2 = TLine(140, 0, 140, hist['BkgSum'].GetMaximum())
    line2.SetLineWidth(2)
    line2.SetLineStyle(7)
    line2.SetLineColor(1)
    line2.Draw()
    
    line1 = TLine(0.841, 0, 0.841, 15)
    line1.SetLineWidth(2)
    line1.SetLineStyle(7)
    line1.SetLineColor(1)
    line1.Draw()
    
    line1 = TLine(100, 0, 100, hist['BkgSum'].GetMaximum())
    line1.SetLineWidth(2)
    line1.SetLineStyle(7)
    line1.SetLineColor(1)
    line1.Draw()


