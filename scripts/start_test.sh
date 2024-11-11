#!/bin/bash

# First, ensure CFS_TOPDIR environment variable is defined
if [ -z ${CFS_TOPDIR} ]
then
    echo "CFS_TOPDIR is not defined. Try sourcing setvars.sh before running."
    exit 1
fi


echo "Test with 10 messages (default)"
$HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=4 # Stop lingering test
$HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=6 --uint32=100
$HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=3

# echo "Test with 50 messages (sleeping 3 seconds..)"; sleep 3
# $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=4 # Stop lingering test
# $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=6 --uint32=50
# sleep 1
# $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=3

# echo "Test with 200 messages (sleeping 3 seconds..)"; sleep 3
# $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=4 # Stop lingering test
# $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=6 --uint32=200
# sleep 1
# $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=3

# echo "Test with 500 messages (sleeping 3 seconds..)"; sleep 3
# $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=4 # Stop lingering test
# $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=6 --uint32=500
# sleep 1
# $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=3

# echo "Test with 950 messages (sleeping 3 seconds..)"; sleep 3
# $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=4 # Stop lingering test
# $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=6 --uint32=950
# sleep 1
# $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1898 --endian=LE --cmdcode=3