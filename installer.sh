#!/bin/bash
# Installation script for gid - the no BS Todo manager
# Written by ashmew2 : December of 2016

## Current time: $(date +%YYY-%MM-%DD)

default_install_dir=$HOME/.gid
read -p "Enter the install dir [Press enter for: $default_install_dir]" install_dir
[[ -z $install_dir ]] && install_dir=$default_install_dir

base_dir=$(dirname $BASH_SOURCE)

# BASHRC is set to the relevant file. It is bash_profile for darwin machines.
if [[ $(uname -s) != Linux ]]; then
    BASHRC=$HOME/.bash_profile
    DATE="$(date -u)"
else
    BASHRC=$HOME/.bashrc
    DATE="$(date --utc)"
fi

# The files.
todo_file=$install_dir/mytodo.gid
installed_file=$install_dir/gid.sh

# If the $install_dir does not exist, create it
[[ ! -e $install_dir ]] && mkdir -p $install_dir

# Create the file if it does not exist.
touch $todo_file

cp $base_dir/gid.sh $install_dir/gid.sh

## Set up permissions for the files
chmod 740 $installed_file
chmod 700 $todo_file

## Check $BASHRC with grep if gid is already installed.
grep "## Automatically added by gid installer" $BASHRC &>/dev/null

[[ $? -ne 0 ]] && {

    echo >> $BASHRC
    echo "## Automatically added by gid installer at $DATE" >> $BASHRC
    echo "source $installed_file" >> $BASHRC
}

## Check $BASHRC with grep if tdr is defined
grep "alias tdr=" $BASHRC &>/dev/null

[[ $? -ne 0 ]] && {

    ## Set an alias in $BASHRC to do the source as we can't do it for our parent (why not?)
    echo >> $BASHRC
    echo "## reload todoman with this alias (Added : $DATE)" >> $BASHRC
    echo "alias tdr='source $installed_file'" >> $BASHRC
}

## Load the $installed_file to enable gid (For tda --usage later in the script)
source $installed_file

echo "Installation Finished. No errors."
echo -e "The todo file is at $todo_file\n\n"

## Tell the user what to do by running tda --usage
tda --usage

echo -e "\ntda and friends will be in all new shells. Enjoy."
## GoodBye!
