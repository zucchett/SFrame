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

