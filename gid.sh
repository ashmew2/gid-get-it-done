# Ultimate Todo Man
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

GID_TODO_FILE="${HOME}/mytodo.gid"
BACKUP_FLAG=1
BACKUP_PREFIX=""
BACKUP_SUFFIX=".bck"
REVIEW_THRESHOLD=20
#CYAN='\033[0;36m'
#NC='\033[0m'

# Default timeout if no arg to tda --timer
DEFAULT_TIMEOUT=1

# Function for reviewing the todo list
function tdre {

    # Be safe (not pregnant)
    cp -a ${GID_TODO_FILE} ${GID_TODO_FILE}.$(date +%s)

    IMP_FILE=${GID_TODO_FILE}.i.tmp
    OTHER_FILE=${GID_TODO_FILE}.o.tmp
    NEW_FILE=${GID_TODO_FILE}.new.tmp

    touch $IMP_FILE
    touch $OTHER_FILE
    touch $NEW_FILE

    # rm -f the old files as well. It has a .tmp extension, we expect the user to understand.
    IFS=$'\t\n'; todo_file=( $(cat $GID_TODO_FILE) ); unset IFS

    for item in "${todo_file[@]}"; do

        echo $item

        impbool=y
        read -n 1 -p "Important? [ [yY]: Yes; [dD]: Delete; [Any other key]: No ]: " impbool

        if [[ "$impbool" == "y" || "$impbool" == "Y" ]]; then
            echo "$item" >> $IMP_FILE
        elif [[ "$impbool" != "d" && "$impbool" != "D" ]]; then
            echo "$item" >> $OTHER_FILE
        fi

        echo
        echo
    done

    cat $IMP_FILE $OTHER_FILE >> $GID_TODO_FILE.new.tmp

    # Show the diff so that user can undo major screw ups if [requ|des]ired
    diff -u $GID_TODO_FILE $NEW_FILE
    > $GID_TODO_FILE
    cat $NEW_FILE > $GID_TODO_FILE

    # Wind up
    rm -f $IMP_FILE
    rm -f $OTHER_FILE
    rm -f $NEW_FILE

    # TODO - Delete the backup here? Probably leave it around for $AGE..
}

function tda {

    # make sure we have some args.
    if [[ $# -eq 0 ]]; then
        #echo -e "${CYAN}No task mentioned to be added. (Ex: tda *task to be added*)${NC}"
        return 0
    fi

    # Handle args

    # TODO - Do we need a proper timing service?
    # TODO - Enable script execution after timer expires.

    [[ $1 = --timer ]]  &&  {

        if [[ $2 == "" ]]; then
            timeout=$DEFAULT_TIMEOUT
        else
            timeout=$2
        fi

#        sleep $timeout && echo "Timer Expired for $timeout second(s)." &

        echo $timeout >> /tmp/timer
        echo date >> /tmp/timer
#        echo "Timer Expired for $timeout second(s)." >> /tmp/timer
        kill -USR1 $(pgrep gidserver)

        # >> $(ls -l /proc/$$/fd/1 | awk '{print $NF}') &
        return 0
    }

    [[ $1 = --file ]] && {
        echo $GID_TODO_FILE
        return 0
    }

    [[ $1 = --show ]] && {
        num=0
        while read -r; do
            echo -e "====\n#${num}:\n===="
            echo $REPLY
            num=$(expr $num + 1)
        done < $GID_TODO_FILE | less
        return 0
    }

    [[ $1 = --search ]] && {
        SEARCH_USAGE='USAGE: tda --search word1 [word2] [word3] ...'

        [[ -z $2 ]] && echo "$SEARCH_USAGE" && return 1

        searchstring=''
        for i in $*; do
            searchstring="${searchstring}|${i}"
        done

        searchstring=${searchstring#|--search|}

        grep --color=always -E -i $searchstring $GID_TODO_FILE

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
        echo -e "\ttda --usage : Print this message and exit"
        echo -e "\ttda --file  : Print the full path to todo file"
        echo -e "\ttda --search word1 [word2] ... : Search for all words in TODO file"
        echo -e "\ttda --timer [seconds]: Sleep for numseconds and notify parent terminal."
        echo -e "\ttdr         : Reload the gid.sh script"
        echo -e "\ttdre        : Review and sort your todo list"
        echo -e "\t--------------"

        return 0
    }

    echo "$@" >> $GID_TODO_FILE

    # Honor the REVIEW_THRESHOLD
    num_items=$(wc -l $GID_TODO_FILE | awk '{print $1}')

    if [[ $num_items -ge $REVIEW_THRESHOLD ]]; then
        echo "[Warning] $num_items in TODO. Advice: tdre"
    fi

    if [[ $BACKUP_FLAG -eq 1 ]]; then
        echo "$@" >> ${BACKUP_PREFIX}${GID_TODO_FILE}${BACKUP_SUFFIX}
    fi

    # Mess with the user.
    if [[ "$1" =~ "tda" ]]; then
        echo "$(whoami) $(whoami) ;-)"
    fi
}

#TODO - fix handling of special symbols in input
#TODO - Decide if we need a tda-clean function as well to clean up old files etc or not.
#TODO - Add network sync to keep safe with buffering.
#TODO - Fix the naming scheme of all binaries, maybe tdreload is better than tdr as we can have tdremove and tdedit as well in that case
#TODO - Add a tda --expire <DATE-TIME> to set a reminder which expires in the future. --timer and --expire (or renamed) should have a common core.
#TODO - Follow code conventions, reeplace all $x -> ${x} (and consistent) (use regex)
#TODO - define multiple levels of THRESHOLD REVIEW advice to help the user control abuse of this todo man
