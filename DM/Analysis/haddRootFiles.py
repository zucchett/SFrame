import sys, os, glob,re

import re
import optparse
usage = "usage: %prog [options]"
parser = optparse.OptionParser(usage)
parser.add_option("-N", "--name", action="store", type="string", dest="name", default="test")
parser.add_option("", "--rm", action="store_true", default=False, dest="rm")
(options, args) = parser.parse_args()
if __name__ == "__main__":

    dirlist = glob.glob("rootfiles_"+options.name+"/*root")
    print dirlist
    counter = 0
    files = []
    filesForRM = []
    for d in dirlist:
        n = len(re.findall('_', os.path.basename(d)))
        print "occurences",n
        if "_" not in os.path.basename(d):
            print d
            files.append(d.replace('.root',''))
        elif 'bin' in os.path.basename(d) and n ==2:
            files.append(d.replace('.root',''))

    files = set(files)
    print "Hadding all the systematics files, will also remove them if option --rm is specified"
    for f in files:
        try: os.stat("rootfiles_"+options.name+"/subFiles")
        except: os.mkdir("rootfiles_"+options.name+"/subFiles")
        cmd = "mv "+ f + "* rootfiles_"+options.name+"/subFiles/."
        print cmd
        os.system(cmd)
        cmd = "hadd -f " + f + ".root " + "rootfiles_"+options.name+"/subFiles/"+os.path.basename(f) + "*" 
        print cmd
        os.system(cmd)

    if options.rm:
        print "Removing subfiles"
        cmd = "rm -r rootfiles_"+ options.name+"/subFiles/"
        print cmd
        os.system(cmd)



#        cmd = "hadd -f " + sample + "_combined.root " + sample + "*" 
        
