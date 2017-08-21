#! /usr/bin/env python

import os, multiprocessing, math
from array import array
from ROOT import TFile, TH1, TF1, TH3F, TLorentzVector


from xsections import xsection

import optparse
usage = 'usage: %prog [options]'
parser = optparse.OptionParser(usage)
parser.add_option('-i', '--input', action='store', type='string', dest='origin', default='./AnalysisOutput/')
parser.add_option('-o', '--output', action='store', type='string', dest='target', default='../../Ntuple/AZh/')
parser.add_option('-f', '--filter', action='store', type='string', dest='filter', default='')
parser.add_option('-p', '--parallelize', action='store_true', dest='parallelize', default=True)
parser.add_option('-s', '--stitch', action='store_true', dest='stitch', default=False)
parser.add_option('-v', '--verbose', action='store_true', dest='verbose', default=False)

(options, args) = parser.parse_args()

origin      = options.origin
target      = options.target
filterset   = options.filter
parallelize = options.parallelize
stitch      = options.stitch
verboseout  = options.verbose

LUMI    = 35867.
ANALYSIS = "AZhAnalysis"

nEvents = {}
binsHT = [0, 100, 200, 400, 600, 800, 1200, 2500, 4000]
binsV_pt = [0, 40, 100, 200, 400]
binsNj = [0, 1, 2, 3, 4]
binsNb = [0] #1, 2

if not os.path.exists(origin):
    print 'Origin directory', origin, 'does not exist, aborting...'
    exit()
if not os.path.exists(target):
    print 'Target directory', target,'does not exist, aborting...'
    exit()


##############################

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
    
    # Open stitching file
    stitch_file = TFile("stitching.root", 'READ')
    stitch_num = {}
    if sample.startswith('DY'):
        for i in range(len(binsNb)): stitch_num[i] = stitch_file.Get("nEvents_%s_nB%d" % (sample, i))
        stitch_den = stitch_file.Get('nEvents')

    # Cross section
    XS = xsection[sample]['xsec']*xsection[sample]['kfactor']*xsection[sample]['br']
    Leq = LUMI*XS/totalEntries if totalEntries > 0 else 0.
    
    print sample, ": Leq =", (Leq if isMC else "Data")
    
    # Variables declaration
    stitchWeight = array('f', [1.0])
    eventWeightLumi = array('f', [1.0])  # global event weight with lumi
    
    # Looping over file content
    for key in ref_file.GetListOfKeys():
        obj = key.ReadObj()
        # Histograms
        if obj.IsA().InheritsFrom('TH1'):
            if verbose: print ' + TH1:', obj.GetName()
            new_file.cd()
            if isMC: obj.Scale(LUMI*XS/totalEntries)
            obj.Write()
        # Tree
        elif obj.IsA().InheritsFrom('TTree'):
            nev = obj.GetEntriesFast()
            new_file.cd()
            new_tree = obj.CopyTree("")
            # New branches
            stitchWeightBranch = new_tree.Branch('stitchWeight', stitchWeight, 'stitchWeight/F')
            eventWeightLumiBranch = new_tree.Branch('eventWeightLumi', eventWeightLumi, 'eventWeightLumi/F')

            # looping over events
            for event in range(0, obj.GetEntries()):
                if verbose and (event%10000==0 or event==nev-1): print ' = TTree:', obj.GetName(), 'events:', nev, '\t', int(100*float(event+1)/float(nev)), '%\r',
                #print '.',#*int(20*float(event)/float(nev)),#printProgressBar(event, nev)
                obj.GetEntry(event)
                # Initialize
                eventWeightLumi[0] = stitchWeight[0] = 1.
                # Weights
                if isMC:
                    # MC stitching
                    if sample.startswith('DY') and 'LL' in sample and not 'BJets' in sample:
                        nB = min(int(obj.LheNb), binsNb[-1])
                        bin = stitch_den.FindBin(min(obj.LheHT, binsHT[-1]-1), min(obj.LheV_pt, binsV_pt[-1]-1), min(obj.LheNj, binsNj[-1]))
                        nS = stitch_num[nB].GetBinContent(bin)
                        nT = stitch_den.GetBinContent(bin)
                        stitchWeight[0] = nS / nT
                    
                    # if sample=='DYJetsToLL' or sample=='WJetsToLNu':
                    #     if obj.LheHT > 100.: stitchWeight[0] = 0.
                    #     else: stitchWeight[0] = 0.843
                    eventWeightLumi[0] = obj.eventWeight * obj.bTagWeight
                    eventWeightLumi[0] *= LUMI*XS/totalEntries
                    
                # Fill the branches
                stitchWeightBranch.Fill()
                eventWeightLumiBranch.Fill()

            new_file.cd()
            new_tree.Write()
            if verbose: print ' '
        
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
                    if isMC: subobj.Scale(LUMI*XS/totalEntries)
                    subobj.Write()
            new_file.cd('..')
        
    new_file.Close() 



def preStitching():
    print "  Running pre-stitching"

    for d in os.listdir(origin):
        sample = d.replace(ANALYSIS+".", "").replace(".root", "")
        if not '.root' in d: continue
        #d = d.replace(ANALYSIS+".", "").replace(".root", "")
        if not sample.startswith('DY'): continue
        if not 'LL' in sample: continue
        if 'BJets' in sample: continue
        #if sample.startswith('DYBBJets'): continue
        #if 'Zpt' in sample: continue
        
        nEvents[sample] = {}
        for i in range(len(binsNb)):
            nEvents[sample][i] = TH3F("nEvents_%s_nB%d" % (sample, i), ";HT;Z pT;n partons", len(binsHT)-1, array('f', binsHT), len(binsV_pt)-1, array('f', binsV_pt) , len(binsNj)-1, array('f', binsNj) )
            nEvents[sample][i].SetDirectory(0)

        ref_file = TFile(origin + '/' + d, 'READ')
        obj = ref_file.Get("tree")
        for event in range(0, obj.GetEntries()):
            obj.GetEntry(event)
            nB = min(int(obj.LheNb), binsNb[-1])
            nEvents[sample][nB].Fill(min(obj.LheHT, binsHT[-1]-1), min(obj.LheV_pt, binsV_pt[-1]-1), min(obj.LheNj, binsNj[-1]), 1. if obj.eventWeight>0 else -1.)
        print "    Done for", d.replace(ANALYSIS+".", "").replace(".root", "")

    total = nEvents[nEvents.keys()[0]][i].Clone("nEvents")
    total.Reset("MICES")
    #total.SetDirectory(0)
    for name, h in nEvents.iteritems():
        for i in range(len(binsNb)): total.Add(h[i])

    outFile = TFile("stitching.root", "RECREATE")
    outFile.cd()
    total.Write()
    for name, h in nEvents.iteritems():
        for i in range(len(binsNb)): h[i].Write()
    outFile.Close()

    print "  Pre-stitching complete"



jobs = []

if stitch: preStitching()

else:
    for d in os.listdir(origin):
        if not '.root' in d: continue
        if not d.replace(ANALYSIS+".", "").replace(".root", "") in xsection.keys(): continue
        if len(filterset)>0 and not filterset in d: continue
        if not parallelize:
            processFile(d, verboseout)
        else:
            p = multiprocessing.Process(target=processFile, args=(d,verboseout,))
            jobs.append(p)
            p.start()
        #exit()
    
#print '\nDone.'

