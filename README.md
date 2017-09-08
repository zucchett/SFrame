# SFrame Repository

## Step-by-step guide to installation

### Step 0: ROOT and CMSSW enviroments

Set up the ROOT environment. The easiest way to do that is to run the `cmsenv` commend in a previously installed CMSSW release (now using CMSSW_8_0_26_patch1).

### Step 1: Download and setup SFrame

Clone the git repository. Navigate to your work directory, preferably not in a CMSSW release, and run
```bash
git clone https://github.com/zucchett/SFrame
```
and enter the `SFrame` directory.
Then, setup the SFrame environment. In the `SFrame` directory, run the `setup.sh` script:
```bash
cd SFrame/SFrame/
source setup.sh
cd ..
```

### Step 2: Compile packages

Before running the code, it is necessary to compile the packages. Enter every directory, and run the command
```bash
make
```
Start from the `SFrame` directory first, and then move to the others.
Remember that, in case you perform significant modifications to SFrame like adding new variables, adding new methods to classes, adding new packages, it's often useful to clear compiled objects with the `make distclean` command, and then recompile with `make`.



## How to run the code

There are two running modes, locally or with the batch system. The former is used for debugging, checking the output before moving to mass production, and running on small files. The batch mode is used to run on large amount of files such as data and backgrounds. This mode curretly works only on the UZH cluster.

### Local mode

Enter your package, and compile it with the `make` command.
Then, set up your configuration xml file. The configuration files are located in the `config` directory, and include the list of packages to be loaded by SFrame, the package parameters, the verbosity level, and the ntuple file to be read.
Once the configuration file is set, and points to a readable rootfile, the code can be run with the command:
```bash
sframe_main config/Configfile.xml
```
where `Configfile.xml` is the name of the desired configuration file.


### Batch mode

Before running the batch mode, please make sure that the code runs and produce meaningful results. Do not run in batch mode for testing, as it will create a lot of processes.
In order to acces files not stored on the tier3, make sure to create a valid grid proxy with the command:
```bash
voms-proxy-init --voms cms
```

The batch jobs are divided in several blocks, to allow some flexibility and run on a subset of the samples (for instance, you may want to run on data alone without processing MC). Each block has a `submitBlock.py` file, which constains approximately the same information provided by the `.xml` configuration files.
Submit the desired block with the command:
```bash
python -u submitSFrame.py -j submitBlock.py --nosandbox --useEnvironment --keepTemp &> logBlock.txt &
```
where `Block` replaces the block name. The commands to run all blocks in once is in the `submitAll.sh` file. Handle with care!

You can check the status of each block with the `tail -f logBlock.txt` command.

Once the logfiles end with `Tschoe!` line, the jobs have finished. The temporary outputs can be found in the `AnalysisTemp` directory, and the merged outputs in the `AnalysisOutput` directory. If the latter is empty, and the jobs have finished, checking the temporary files and logs is a good strating point to understand what has gone wrong.



### Post-processing

The post-processing script `addWeight.py` is also included. The simplest post-processing step consists in adding a new variable to the tree containing the weight relatove to each single event, by accounting for the equivalent luminosity and the cross section of the sample.
The cross sections are taken from the `xsection.py` file, which is included by `addWeight.py`. Control histograms are rescaled as well. If needed, the post-processing step may be extended to perform more complicate operations, such as calculating multivariate discriminants.
The script accepts as argument a filter list with the `-f filterlist` where a string to filter input files may be also provided. The output directory should be also specified with the `-o` parameter, or the default direcotry should already exist.
By default, the post-processing is performed in parallel between different samples, to expolit multi-core machines. The post-processing can be lauched with the command:
```bash
python addWeight.py
```
