#!/bin/bash

# Pull a file from a connected Android device.

#ANDROID_FILE_PATH="/storage/sdcard/mytodo.txt"
#ANDROID_FILE_PATH="/storage/emulated/todo.txt"
ANDROID_FILE_PATH="/storage/sdcard1/efs/imei/bt.txt"

#Hint: Use file manager on android, long press and details for file name.
#If you dont have a details button, install a brain...err...a proper file manager.
#ANDROID_FILE_PATH="/storage/emulated/todo.txt"
LOCAL_FILE_PATH="/tmp/mytodo.txt.android"

adb start-server
adb wait-for-device
adb pull $ANDROID_FILE_PATH $LOCAL_FILE_PATH

[[ $? -eq 0 ]] && echo "File pull Successful." && exit 0
[[ $? -ne 0 ]] && echo "File pull Failed." && exit 1

