#!/bin/sh
Port=$1
sudo ./pkreceiver_v2 -i eth0 tcp dst port ${Port} or tcp dst port 80
