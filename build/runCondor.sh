#!/bin/bash

source /cvmfs/sft.cern.ch/lcg/contrib/gcc/8.3.0/x86_64-centos7/setup.sh
source /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/bin/thisroot.sh
source /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/bin/geant4.sh

/afs/cern.ch/work/v/vyeroshe/TL/tl-stereo_plus/build/TLStereo vis_file.mac data_$1.root
