#!/bin/bash

declare -a PKG_NAMES=(
	"linux-headers-$(uname -r)"
	"kmod"
	"build-essential"
	)

sudo apt update &>/dev/null
sudo apt -y upgrade &>/dev/null

for i in "${PKG_NAMES[@]}"
do
	echo checking for "$i"
	if [[ -z "$(dpkg -s "$i" | grep -o 'install ok installed')" ]]; then
		echo Installing: "$i"
		sudo apt install "$i"
	fi
done

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
