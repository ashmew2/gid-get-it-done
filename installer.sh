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

## TODO: Check with grep if gid.sh is already sourced.

## Update .bashrc to auto load on shell startup
echo "" >> $BASHRC
echo "## Automatically added by gid installer at $(date)" >> $BASHRC
echo "source $installed_file" >> $BASHRC

## Load the $installed_file to enable gid
source $installed_file

## Tell the user what to do by running tda --usage
echo "Installation Finished. No errors."
echo -e "The todo file is at $installed_file\n\n"

tda --usage

echo -e "\ntda will be available in all new shells. Enjoy."
## GoodBye!
