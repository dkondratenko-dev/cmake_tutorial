#!/bin/bash

BP_BASH_PROFILE_DIR=""
BP_PROJECT_DIR=""
BP_PROFILE_MANAGER_DIR=""

get_emaster_dir()
{
    current_dir=$(pwd)

    exec_file_path=$0

    base_filename=$(basename -- "$exec_file_path")
    rpath=$(realpath $base_filename)

    if [ $rpath != $exec_file_path ]; then
        #echo "Exec file path: $exec_file_path"
        #echo "Would be replaced with real path: $rpath"
        exec_file_path=$rpath
    fi

    if [ ! -e "$exec_file_path" ]; then
    case $exec_file_path in
        (*/*) exit 1;;
        (*) exec_file_path=$(command -v -- "$exec_file_path") || exit;;
    esac
    fi

    BP_BASH_PROFILE_DIR=$(
    cd -P -- "$(dirname -- "$exec_file_path")" && pwd -P
    ) || exit
    exec_file_path=$BP_BASH_PROFILE_DIR/$(basename -- "$exec_file_path") || exit

    cd $BP_BASH_PROFILE_DIR
    cd ../..
    BP_PROJECT_DIR=$(pwd)

    cd $current_dir
}

source_profile_manager()
{
    BP_PROFILE_MANAGER_DIR=$BP_BASH_PROFILE_DIR/profile_manager
    source $BP_PROFILE_MANAGER_DIR/profile_manager.profile

    pm_source_profile desktop
}

main()
{
    get_emaster_dir

    echo "BP_BASH_PROFILE_DIR : $BP_BASH_PROFILE_DIR"
    echo "BP_PROJECT_DIR      : $BP_PROJECT_DIR"

    source_profile_manager

    pm_setup_profile $BP_PROFILE_TYPE_DESKTOP
}

main
