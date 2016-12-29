#!/bin/bash
set -x
# Push a file from a connected Android device.

ANDROID_FILE_PATH="/storage/sdcard1/mytodo.mp4"
#ANDROID_FILE_PATH="/storage/emulated/todo.txt"
#ANDROID_FILE_PATH="/storage/sdcard1/efs/imei/bt.txt"

#Hint: Use file manager on android, long press and details for file name.
#If you dont have a details button, install a brain...err...a proper file manager.
LOCAL_FILE_PATH="/tmp/todo.txt.tmp"

adb start-server
adb wait-for-device
adb push -p $LOCAL_FILE_PATH $ANDROID_FILE_PATH

[[ $? -eq 0 ]] && echo "File push Successful." && exit 0

[[ $? -ne 0 ]] && echo "File push Failed." && exit 1
