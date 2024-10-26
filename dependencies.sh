#!/bin/bash

sudo apt update
sudo apt upgrade
sudo apt install -y build-essential linux-headers-$(uname -r) kmod
