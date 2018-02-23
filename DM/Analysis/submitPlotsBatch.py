#!/usr/bin/env python
import os, glob, sys
from ROOT import *


import re
import optparse
usage = "usage: %prog [options]"
parser = optparse.OptionParser(usage)
parser.add_option("-N", "--name", action="store", type="string", dest="name", default="test")
parser.add_option("-S", "--sr", action="store", type="string", dest="sr", default="ALL")
parser.add_option("-m", "--mode", action="store", type="string", dest="mode", default="shape")

(options, args) = parser.parse_args()


def createJobs(s, jobs):
    cmd = 'python plot.py -l --batch -N '+options.name+' --sys "'+s+'" -m "'+options.mode+'" \n'
    jobs.write(cmd)
    return 1

def submitJobs(jobList, nchunks):
    print 'Reading joblist'
    jobListName = jobList
    subCmd = 'qsub -t 1-%s -o rootfiles_%s/logs/ psibatch_runner.sh %s' %(nchunks,options.name,jobListName)
    print 'Going to submit', nchunks, 'jobs with', subCmd
    os.system(subCmd)

    return 1
if __name__ == "__main__":
    try: os.stat("rootfiles_"+options.name) 
    except: os.mkdir("rootfiles_"+options.name)
    try: os.stat("rootfiles_"+options.name+"/logs") 
    except: os.mkdir("rootfiles_"+options.name+"/logs")

    #first empty entry runs the default stuff
    sys = ['','CMS_scale_j','CMS_res_j','CMS_WqcdWeightRen','CMS_WqcdWeightFac','CMS_ZqcdWeightRen','CMS_ZqcdWeightFac','CMS_WewkWeight','CMS_pdf','CMS_HF','CMS_eff_b', 'CMS_scale_pu', 'CMS_scale_top', 'CMS_eff_trigger', 'CMS_eff_e', 'CMS_eff_m', 'QCDscale_ren', 'QCDscale_fac']
    jobList = 'joblist_rootfiles'+options.name+'.txt'
    jobs = open(jobList, 'w') 
    chunks = 0
    for s in sys:
        createJobs(s,jobs)
        chunks = chunks+1



    submit = raw_input("Do you also want to submit "+str(chunks)+" jobs to the batch system? [y/n] ")
    if submit == 'y' or submit=='Y': 
        print "submitting jobs"
        submitJobs(jobList, chunks)
    else:
        print "didn't submit any jobs"
    jobs.close()
        


    
