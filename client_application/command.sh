#!/bin/bash

if [ $# -ne 2 ]
then
    echo "$# arguments entered"
    echo "Enter in $0 <ip> <port_no> form"
    exit 1
fi

ip_addr=$1
port_no=$2

echo "Connecting to server"

nc $ip_addr $port_no

echo "Enter command below"
while true
do
    
done