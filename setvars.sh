#!/bin/bash

export CFS_TOPDIR=$PWD

# Ensure CFS_TOPDIR is set properly; else attempt to use git command to set it
if ! [ -d ${CFS_TOPDIR}/scripts ]
then
   CFS_TOPDIR=$(git rev-parse --show-toplevel)
fi

echo "CFS_TOPDIR=${CFS_TOPDIR}"

cp cfe/cmake/Makefile.sample Makefile > /dev/null 2>&1
cp -r cfe/cmake/sample_defs sample_defs > /dev/null 2>&1

# Variables
export CPU1_DIR="${CFS_TOPDIR}/build/exe/cpu1"
export HOST_DIR="${CFS_TOPDIR}/build/exe/host"

# Aliases
alias build-cfs="cd ${CFS_TOPDIR}; make distclean; make SIMULATION=native prep; make; make install"
alias run-cfs="cd ${CPU1_DIR}; ./core-cpu1"
alias repo="cd ${CFS_TOPDIR}"