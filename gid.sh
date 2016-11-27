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
  echo "${COUNT}. $@" >> $FILENAME

  if [ $BACKUP_FLAG -eq 1 ]; then
    echo "${COUNT}. $@" >> ${BACKUP_PREFIX}${FILENAME}${BACKUP_SUFFIX}
  fi

  echo "L${COUNT} inserted."
  COUNT=$(expr $COUNT + 1)


}
