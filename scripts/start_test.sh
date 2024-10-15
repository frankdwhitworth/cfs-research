#!/bin/bash

# First, ensure CFS_TOPDIR environment variable is defined
if [ -z ${CFS_TOPDIR} ]
then
    echo "CFS_TOPDIR is not defined. Try sourcing setvars.sh before running."
    exit 1
fi


$HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=3