#!/bin/bash

# First, ensure CFS_TOPDIR environment variable is defined
if [ -z ${CFS_TOPDIR} ]
then
    echo "CFS_TOPDIR is not defined. Try sourcing setvars.sh before running."
    exit 1
fi

send_blink_noargs()
{
    $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1882 --endian=LE --cmdcode=$1
}

send_blink_specblinker()
{
    $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1882 --endian=LE --cmdcode=$1 --uint16=$2 --uint16=99
}

send_blink_swapblinkers()
{
    $HOST_DIR/cmdUtil --host="127.0.0.1" --port=1234 --pktid=0x1882 --endian=LE --cmdcode=11 --uint16=$1 --uint16=$2
}


# send_blink_noargs 0 # Blinker NOOP
# send_blink_noargs 4 # Reset all blinkeres
# send_blink_specblinker 5 5 # Reset specific blinker
# send_blink_specblinker 6 5 # Report specific blinker
# send_blink_specblinker 7 5 # Enable specific blinker
# send_blink_specblinker 8 5 # Disable specific blinker
# send_blink_noargs 9 # Enable ALL blinkers
# send_blink_noargs 10 # Disable ALL blinkers
# send_blink_swapblinkers 5 3 # Swap two blinkers' counts

send_blink_noargs 0 # Blinker NOOP
send_blink_noargs 10 # Disable ALL blinkers
send_blink_noargs 4 # Reset all blinkeres
sleep .5
send_blink_specblinker 6 5 # Report specific blinker
sleep .5
# Let's get blinker 5 to count some
send_blink_specblinker 7 5 # Enable specific blinker
echo Sleeping for 5 seconds for blinks... 
sleep 5
send_blink_specblinker 8 5 # Disable specific blinker
send_blink_specblinker 6 5 # Report specific blinker
# Let's get blinker 3 to count some
send_blink_specblinker 7 3 # Enable specific blinker
echo Sleeping for 2 seconds for blinks... 
sleep 2
send_blink_specblinker 8 3 # Disable specific blinker
send_blink_specblinker 6 3 # Report specific blinker
sleep .5
send_blink_swapblinkers 5 3 # Swap two blinkers' counts
send_blink_specblinker 6 5 # Report specific blinker
send_blink_specblinker 6 3 # Report specific blinker
