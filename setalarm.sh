#!/bin/bash

# Called from gid.sh only, never separately.
# Validity of $@ is upto the caller.

sleep $1
#zenity alarm expired
mpg123 alarmtone.mp3 2>&1 >/dev/null
