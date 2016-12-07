#!/bin/bash
# Installation script for gid - the no BS Todo manager
# Written by ashmew2 : December of 2016

## Current time: $(date +%YYY-%MM-%DD)

default_install_dir=$HOME/.gid
read -p "Enter the install dir [Press enter for: $default_install_dir]" install_dir
[[ -z $install_dir ]] && install_dir=$default_install_dir

base_dir=.
BASHRC=$HOME/.bashrc
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
    echo "## Automatically added by gid installer at $(date --utc)" >> $BASHRC
    echo "source $installed_file" >> $BASHRC
}

## Check $BASHRC with grep if tdr is defined
grep "alias tdr=" $BASHRC &>/dev/null

[[ $? -ne 0 ]] && {

    ## Set an alias in bashrc to do the source as we can't do it for our parent (why not?)
    echo >> $BASHRC
    echo "## reload todoman with this alias (Added : $(date --utc))" >> $BASHRC
    echo "alias tdr='source $installed_file'" >> $BASHRC
}

## Load the $installed_file to enable gid (For tda --usage later in the script)
source $installed_file

## Tell the user what to do by running tda --usage
echo "Installation Finished. No errors."
echo -e "The todo file is at $installed_file\n\n"

tda --usage
[[ $? -ne 0 ]]
echo -e "\ntda and friends will be in all new shells. Enjoy."
## GoodBye!
