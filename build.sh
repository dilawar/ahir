#!/bin/bash

set -x
set -a
export AHIR_RELEASE=$(pwd)/release
export BOOST_VERSION=boost_1_58_0
echo "setting boost version" to $BOOST_VERSION
echo "Making vhdl release."
cd vhdl/release
. cat.sh
. catForAsic.sh
. catForAsicFpga.sh
cd -
echo "Running scons to build tools."
cd v2
source build_bashrc
scons
cd -
echo "Copying files to release area."
make -f ReleaseMakefile
