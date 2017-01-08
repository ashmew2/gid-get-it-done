#!/bin/bash
set -x

# Push a file from a connected Android device.

# Hint: Use a file manager on Android, long press and details for file name.
# If you dont have a details button, install a brain...err...a proper file manager.
ANDROID_FILE_PATH="/storage/sdcard1/mytodo.txt"
LOCAL_FILE_PATH="/tmp/todo.txt.tmp"

adb start-server
adb wait-for-device
adb push -p $LOCAL_FILE_PATH $ANDROID_FILE_PATH

[[ $? -eq 0 ]] && echo "File push Successful." && exit 0
[[ $? -ne 0 ]] && echo "File push Failed." && exit 1
