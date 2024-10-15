#!/bin/bash

# First, ensure CFS_TOPDIR environment variable is defined
if [ -z ${CFS_TOPDIR} ]
then
    echo "CFS_TOPDIR is not defined. Try sourcing setvars.sh before running."
    exit 1
fi

if [ $# -eq 0 ]; then
    read -p "No argument provided, how many noops?  " num
else
    num=$1
fi
# Let's send x number of noops
for ((i = 0 ; i < $num ; i++)); do
    $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1882 --endian=LE --cmdcode=0
done