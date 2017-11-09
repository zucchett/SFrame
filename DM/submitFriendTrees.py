#! /usr/bin/env python

import os, multiprocessing, math, glob
from array import array
from ROOT import TFile, TH1, TF1, TLorentzVector, TTree


from xsections import xsection

import optparse
usage = 'usage: %prog [options]'
parser = optparse.OptionParser(usage)
parser.add_option('-i', '--input', action='store', type='string', dest='origin', default='testinsig/')
parser.add_option('-o', '--output', action='store', type='string', dest='target', default='testout/')
parser.add_option('-f', '--filter', action='store', type='string', dest='filter', default='')
parser.add_option('-s', '--single', action='store_true', dest='single', default=False)
parser.add_option('-v', '--verbose', action='store_true', dest='verbose', default=False)
parser.add_option('-c', '--copytree', action='store_true', dest='copytree', default=False)
(options, args) = parser.parse_args()

origin      = options.origin
target      = options.target
filterset   = options.filter
singlecore  = options.single
verboseout  = options.verbose
copytree  = options.copytree

LUMI    = 35867.
ANALYSIS = "DMAnalysis"



if not os.path.exists(origin):
    print 'Origin directory', origin, 'does not exist, aborting'
    exit()
    
if not os.path.exists(target):
    print 'Target directory', target,'does not exist, creating structure'
    try: os.stat(target)
    except: os.mkdir(target)
    try: os.stat(target+'/logs')
    except: os.mkdir(target+'/logs')
    


def createJobs(f , jobs):
    cmd = 'python createFriendTrees.py -i ' + origin + ' -o '+  target + ' -t ' + f + '\n'
    if copytree:
        cmd = 'python createFriendTrees.py -i ' + origin + ' -o '+  target + ' -t ' + f + '-c True \n'
    jobs.write(cmd)
    return 1

def submitJobs(jobList, outfolder,njobs):
    print 'Reading joblist'
    jobListName = jobList
    print jobList
    subCmd = 'qsub -t 1-%s -o %s/logs/ %s %s' %(njobs,outfolder,"psibatch_runner.sh",jobListName)
    print 'Going to submit jobs with', subCmd
    os.system(subCmd)

    return 1


if __name__ == "__main__":

    filelist = glob.glob(origin+'/*root')
    

    jobList = 'joblist_FriendTrees.txt'
    jobs = open(jobList, 'w')
    outs = []
      
    for f in filelist:
        fout = createJobs(os.path.basename(f), jobs)
        outs.append(fout)
    jobs.close()

    print jobList
    print "Have ", len(outs), " jobs to submit with "
    submit = raw_input("Do you also want to submit the jobs to the batch system? [y/n] ")
    # submit = 'y'
    if submit == 'y' or submit=='Y':
        submitJobs(jobList, target, len(outs))
    
