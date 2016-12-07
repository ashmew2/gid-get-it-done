# Ultimate Todo Man
# Add more functions like viewing
# Add network sync to keep safe with buffering.
# author: ashmew2
#
# To use from bash:
#  $ . gid.sh

# Automate loading of gid
# This REDIRECTION BELOW is >> (APPEND).
#
# echo "source ${HOME}/gid/gid.sh" >> ~/.bashrc

# Usage:
# tda <list of stuff>

# Example:
# $ tda Get grocery
# $ tda Code a game
# $ tda work hard
#
# $ cat todo.gid
# 0. Get grocery
# 1. Code a game
# 2. work hard

COUNT=0
FILENAME="${HOME}/mytodo.gid"
BACKUP_FLAG=1
BACKUP_PREFIX=""
BACKUP_SUFFIX=".bck"

function tda {
    # Handle args

    [[ $1 = --show ]] && {
	cat -n $FILENAME
	return
    }

    [[ $1 = --usage ]] && {
	echo -e "\tExample usage:"
	echo -e "\t--------------"
	echo -e "\t\t$ tda Go to the grocery store at 13:00"
	echo -e "\t\t$ tda Book a table for my date."
	echo -e "\t\t$ tda fix memory corruption in the network stack"
	echo -e "\t\t$ tda Conquer the world!"
	echo -e "\t--------------"
	echo -e "\ttda --show  : Show your todo list"
	echo -e "\ttda --usage : Print this message and exit"
	return
    }

    [[ $1 == --reload ]] && {

	return
    }

  echo "${COUNT}. $@" >> $FILENAME


  if [ $BACKUP_FLAG -eq 1 ]; then
    echo "${COUNT}. $@" >> ${BACKUP_PREFIX}${FILENAME}${BACKUP_SUFFIX}
  fi

  echo "L${COUNT} inserted."
  COUNT=$(expr $COUNT + 1)


}

#TODO - fix handling of special symbols in input
#TODO - gid-reload script (Call it gr) to reload from gid.sh
