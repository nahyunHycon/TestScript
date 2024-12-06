#!/bin/bash

# Load the CAN kernel module
sudo modprobe can
if [ $? -ne 0 ]; then
    echo "Failed to load CAN module"
    exit 1
fi

# Load the VCAN (Virtual CAN) kernel module
sudo modprobe vcan
if [ $? -ne 0 ]; then
    echo "Failed to load VCAN module"
    exit 1
fi

# Create a virtual CAN network interface
sudo ip link add dev vcan0 type vcan
if [ $? -ne 0 ]; then
    echo "Failed to create vcan0 interface"
    exit 1
fi

# Bring up the vcan0 interface
sudo ip link set up vcan0
if [ $? -ne 0 ]; then
    echo "Failed to bring up vcan0 interface"
    exit 1
fi

echo "vcan0 interface successfully set up"

