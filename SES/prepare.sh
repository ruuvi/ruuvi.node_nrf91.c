 #! /bin/bash

DIRECTORY1=LTE_DEBUG/spm/zephyr/misc/generated/syscalls_links/
DIRECTORY2=LTE_DEBUG/zephyr/misc/generated/syscalls_links/
DIRECTORY3=LTE/spm/zephyr/misc/generated/syscalls_links/
DIRECTORY4=LTE/zephyr/misc/generated/syscalls_links/

DIRECTORY5=NBIOT_DEBUG/spm/zephyr/misc/generated/syscalls_links/
DIRECTORY6=NBIOT_DEBUG/zephyr/misc/generated/syscalls_links/
DIRECTORY7=NBIOT/spm/zephyr/misc/generated/syscalls_links/
DIRECTORY8=NBIOT/zephyr/misc/generated/syscalls_links/

DEFAULT_USER=ruuviUser
DEV_USER=$1
TEMP="s/${DEFAULT_USER}/${DEV_USER}/g"

if [ $# -eq 0 ]
  then
    echo "No username provided. Please provide the username used in /home/USERNAME"
else
    # Below will replace ruuviUser with developers name in all files
    find . \( ! -regex '.*/\..*' \) -type f | xargs sed -i "${TEMP}"

    # As symlink are not seen as standard files the below will replace ruuviUser with developers name in symlinks found at
    # LTE_DEBUG/spm/zephyr/misc/generated/syscalls_links/
    while read -r line
        do
            echo $line
            CUR_LINK_PATH="$(readlink "$line")"
            NEW_LINK_PATH="$CUR_LINK_PATH"  
            NEW_LINK_PATH="${NEW_LINK_PATH/"$DEFAULT_USER"/"$DEV_USER"}"
            rm "$line"
            ln -s "$NEW_LINK_PATH" "$line"
        done <<< $(find "$DIRECTORY1" -type l)
    
    while read -r line
        do
            echo $line
            CUR_LINK_PATH="$(readlink "$line")"
            NEW_LINK_PATH="$CUR_LINK_PATH"  
            NEW_LINK_PATH="${NEW_LINK_PATH/"$DEFAULT_USER"/"$DEV_USER"}"
            rm "$line"
            ln -s "$NEW_LINK_PATH" "$line"
        done <<< $(find "$DIRECTORY2" -type l)
    
    while read -r line
        do
            echo $line
            CUR_LINK_PATH="$(readlink "$line")"
            NEW_LINK_PATH="$CUR_LINK_PATH"  
            NEW_LINK_PATH="${NEW_LINK_PATH/"$DEFAULT_USER"/"$DEV_USER"}"
            rm "$line"
            ln -s "$NEW_LINK_PATH" "$line"
        done <<< $(find "$DIRECTORY3" -type l)
    
    while read -r line
        do
            echo $line
            CUR_LINK_PATH="$(readlink "$line")"
            NEW_LINK_PATH="$CUR_LINK_PATH"  
            NEW_LINK_PATH="${NEW_LINK_PATH/"$DEFAULT_USER"/"$DEV_USER"}"
            rm "$line"
            ln -s "$NEW_LINK_PATH" "$line"
        done <<< $(find "$DIRECTORY4" -type l)

    while read -r line
        do
            echo $line
            CUR_LINK_PATH="$(readlink "$line")"
            NEW_LINK_PATH="$CUR_LINK_PATH"  
            NEW_LINK_PATH="${NEW_LINK_PATH/"$DEFAULT_USER"/"$DEV_USER"}"
            rm "$line"
            ln -s "$NEW_LINK_PATH" "$line"
        done <<< $(find "$DIRECTORY5" -type l)
    
    while read -r line
        do
            echo $line
            CUR_LINK_PATH="$(readlink "$line")"
            NEW_LINK_PATH="$CUR_LINK_PATH"  
            NEW_LINK_PATH="${NEW_LINK_PATH/"$DEFAULT_USER"/"$DEV_USER"}"
            rm "$line"
            ln -s "$NEW_LINK_PATH" "$line"
        done <<< $(find "$DIRECTORY6" -type l)
    
    while read -r line
        do
            echo $line
            CUR_LINK_PATH="$(readlink "$line")"
            NEW_LINK_PATH="$CUR_LINK_PATH"  
            NEW_LINK_PATH="${NEW_LINK_PATH/"$DEFAULT_USER"/"$DEV_USER"}"
            rm "$line"
            ln -s "$NEW_LINK_PATH" "$line"
        done <<< $(find "$DIRECTORY7" -type l)
    
    while read -r line
        do
            echo $line
            CUR_LINK_PATH="$(readlink "$line")"
            NEW_LINK_PATH="$CUR_LINK_PATH"  
            NEW_LINK_PATH="${NEW_LINK_PATH/"$DEFAULT_USER"/"$DEV_USER"}"
            rm "$line"
            ln -s "$NEW_LINK_PATH" "$line"
        done <<< $(find "$DIRECTORY8" -type l)
fi
