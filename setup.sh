#!/bin/bash

sudo dmesg --clear

if [[ "$(lsmod | grep ldd)" ]]; then
	sudo rmmod ldd
fi

sudo insmod ldd.ko
if [[ $? -eq 0 ]]; then 
	echo loaded ldd successfully
fi
