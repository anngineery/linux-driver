#!/bin/bash

install_dev=true
install_test=true

while getopts "m:" arg
do
	case $arg in
		m) mode=$OPTARG
		   if [[ $mode == "dev" ]]; then 
			   install_test=false
			   echo only install dev packages
	           elif [[ $mode == "test" ]]; then 
			   install_dev=false
			   echo only install test packages
	           fi
		   ;;
		*) echo "please specify a mode (all | dev | test)";;
	esac
done

declare -a PKG_NAMES=(
	"linux-headers-$(uname -r)"
	"kmod"
	"build-essential"
	)

sudo apt update &>/dev/null
sudo apt -y upgrade &>/dev/null

if [[ $install_dev = true ]]; then
	for i in "${PKG_NAMES[@]}"
	do
		echo checking for "$i"
		if [[ -z "$(dpkg -s "$i" | grep -o 'install ok installed')" ]]; then
			echo Installing: "$i"
			sudo apt install "$i"
		fi
	done
fi

if [[ $install_test = true ]]; then
	echo checking for pip
	if [[ -z "$(command -v pip)" ]]; then
		echo Installing: pip 
		sudo apt install python3-pip
	fi

	echo checking for pytest
	if [[ -z "$(pip list | grep pytest)" ]]; then
		echo Installing: pytest
		sudo apt install python3-pytest
	fi
fi
