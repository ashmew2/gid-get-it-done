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


function tda-review {

    # Be safe (not pregnant)
    cp -a ${FILENAME} ${FILENAME}.$(date +%s)

    IMP_FILE=${FILENAME}.i.tmp
    OTHER_FILE=${FILENAME}.o.tmp
    NEW_FILE=${FILENAME}.new.tmp

    touch $IMP_FILE
    touch $OTHER_FILE
    touch $NEW_FILE

    # rm -f the old files as well. It has a .tmp extension, we expect the user to understand.
    IFS=$'\t\n'; todo_file=( $(cat $FILENAME) ); unset IFS

    for item in "${todo_file[@]}"; do

	echo $item

	impbool=y
	read -p "Important? [ Everything except [yY] is a NO ]: " impbool

	# Stupid shit
	if [[ "$impbool" == "y" || "$impbool" == "Y" ]]; then
	    echo "$item" >> $IMP_FILE
	else
	    echo "$item" >> $OTHER_FILE
	fi
    done
    cat $IMP_FILE $OTHER_FILE >> $FILENAME.new.tmp

    # Show the diff so that user can undo major screw ups if [requ|des]ired
    diff -u $FILENAME $NEW_FILE
    > $FILENAME
    cat $NEW_FILE > $FILENAME

    # Wind up
    rm -f $IMP_FILE
    rm -f $OTHER_FILE
    rm -f $NEW_FILE

    # TODO: Delete the backup here? Probably leave it around for $AGE..
}

function tda {
    # Handle args

    [[ $1 = --show ]] && {
	cat -n $FILENAME
	return 0
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
	echo -e "\ttdr         : Reload the gid.sh script"
	echo -e "\ttda --usage : Print this message and exit"
	echo -e "\ttda-review  : Review and sort your todo list"
	echo -e "\t--------------"

	return 0
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
#TODO - Decide if we need a tda-clean function as well to clean up old files etc or not.
#TODO - Change $FILENAME to some other var name
