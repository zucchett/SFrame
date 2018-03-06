#!/usr/bin/env python
import os, glob, sys
from ROOT import *


import re
import optparse
usage = "usage: %prog [options]"
parser = optparse.OptionParser(usage)
parser.add_option("-N", "--name", action="store", type="string", dest="name", default="test")
parser.add_option("-S", "--sr", action="store", type="string", dest="sr", default="ALL")
parser.add_option("-B", "--blind", action="store_true", default=True, dest="blind")
(options, args) = parser.parse_args()


def runCards(f , s):
    cmd = 'combine -M Asymptotic ../' + f + ' -n ' + s
    print cmd
    os.system(cmd)
    return 1

def createJobs(f , s, jobs):
    signame = f[f.find("/t")+1:f.find(".txt")]
    outfile = f.replace("combinedCards","limitOutput").replace(".txt","_AsymptoticLimits_grepOutput.txt")
    if(options.blind):
        cmd =     "combine -M AsymptoticLimits --run blind --datacard " + f + " -m " + s +" -n " + signame + "  | grep -e Observed -e Expected | awk 'BEGIN{print 0.0}{print $NF}' > " + outfile + " \n "
    else:
        cmd =     "combine -M AsymptoticLimits --datacard " + f + " -m " + s +" -n " + signame + "  | grep -e Observed -e Expected | awk '{print $NF}' > " + outfile + " \n "

    print cmd
    jobs.write(cmd)
    return 1

def submitJobs(jobList, nchunks):
    print 'Reading joblist'
    jobListName = jobList
    subCmd = 'qsub -t 1-%s -o limitOutput_%s/logs/ psibatch_runner.sh %s' %(nchunks,options.name,jobListName)
    print 'Going to submit', nchunks, 'jobs with', subCmd
    os.system(subCmd)

    return 1
if __name__ == "__main__":
    
    fileList = glob.glob("combinedCards_"+options.name+"/t*DM*"+options.sr+"*.txt")
    
    try:            
        os.stat('limitOutput_'+options.name) 
    except:
        os.mkdir('limitOutput_'+options.name)

    try:            
        os.stat('limitOutput_'+options.name+'/logs') 
    except:
        os.mkdir('limitOutput_'+options.name+'/logs')
        
        
    
    jobList = 'joblist_limits'+options.name+'.txt'
    jobs = open(jobList, 'w') 
    chunks = 0
    for f in fileList:
        s="100"
        endMphi = -1
        if "pseudo" in f:
            endMphi = f.find("pseudo")-1
        elif "scalar" in f:
            endMphi = f.find("scalar")-1
        s=f[f.find('MPhi')+4:endMphi]
        createJobs(f,s,jobs)
        chunks = chunks+1



    submit = raw_input("Do you also want to submit the jobs to the batch system? [y/n] ")
    if submit == 'y' or submit=='Y': 
        print "submitting jobs"
        submitJobs(jobList, chunks)
    else:
        print "didn't submit any jobs"
    jobs.close()
        


    
