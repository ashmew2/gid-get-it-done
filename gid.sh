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
# $ tda --show
# 0. Get grocery
# 1. Code a game
# 2. work hard

GID_TODO_FILE="${HOME}/mytodo.gid"
ALARMTONE=$HOME/gid/alarmtone.mp3
BACKUP_FLAG=1
BACKUP_PREFIX=""
BACKUP_SUFFIX=".bck"
REVIEW_THRESHOLD=20
CYAN='\033[0;36m'
#NC='\033[0m'

# Function for reviewing the todo list
# Most likely going away in favour of --edit
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
        echo -e "ERROR: No arguments given."
        tda --usage
        return 1
    fi

    # Handle args
    case $1 in
        --alarm)
            if [[ $# -lt 2 ]]; then
                echo "[ERROR]: Usage: tda --alarm <timeout seconds>"
                return 1
            fi

            term=$(which $(ps -e --forest $$ | grep '^\s*'$$ -B 1 | head -n 1 | awk '{print $NF}'))
            case $(basename $term) in

                sakura) $term -e "/bin/bash -c 'sleep $2; mpg123 $ALARMTONE'" 2>/dev/null & ;;

                *) if which xterm 2>/dev/null >&2; then
                       $(which xterm) -e "/bin/bash -c 'sleep $2; mpg123 $ALARMTONE'" 2>/dev/null &
                   else
                       echo "ERROR: Failed to detect terminal to set alarm"
                   fi
                   ;;
            esac
            return 0
            ;;

        --show)
            num=0
            while read -r; do
                [[ -z $REPLY ]] && continue
                echo "${num}. $REPLY"
                num=$(expr $num + 1)
            done < $GID_TODO_FILE
            return 0
            ;;

        --search)
            SEARCH_USAGE='USAGE: tda --search word1 [word2] [word3] ...'

            [[ -z $2 ]] && echo "$SEARCH_USAGE" && return 1

            searchstring=''
            for i in $*; do
                searchstring="${searchstring}|${i}"
            done

            searchstring=${searchstring#|--search|}

            grep --color=always -E -i $searchstring $GID_TODO_FILE

            return 0
            ;;

        --edit)
            cp $GID_TODO_FILE $GID_TODO_FILE.edited.$(date +%YYYYMMDD)
            $EDITOR $GID_TODO_FILE
            return 0
            ;;

        --help|--usage)
            echo -e "Example usage:"
            echo
            echo -e "\t$ tda Conquer the world"
            echo -e "\t$ tda Fix stack corruption in TCP"
            echo -e "\t$ tda Book tickets for GnR tour"
            echo
            printf "%-30s %s\n" "tda --show" "Show your todo list"
            printf "%-30s %s\n" "tda --edit" "Edit the file directly using EDITOR env var. (Use caution)"
            printf "%-30s %s\n" "tda --search word" "Search for word in task list"
            printf "%-30s %s\n" "tda --alarm seconds" "Ring alarm after [seconds]."
            printf "%-30s %s\n" "tdr"  "Reload the gid.sh script"
            printf "%-30s %s\n" "tdre" "Review your todo list"
            printf "%-30s %s\n" "tda --help" "Show this help and exit."
            printf "%-30s %s\n" "tda --usage" "Same as --help."
            echo
            echo "Report bugs at github.com/ashmew2/gid-get-it-done"
            return 0
            ;;
    esac

    # Add item to our todo file.
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

#TODO - Follow code conventions, reeplace all $x -> ${x} (and consistent) (use regex)
#TODO - define multiple levels of THRESHOLD REVIEW advice to help the user control abuse of this todo man
#TODO - fix handling of special symbols in input
#TODO - Add network sync to keep safe with buffering.
