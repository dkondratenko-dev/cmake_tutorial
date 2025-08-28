bp_print_welcome_message ${BASH_SOURCE[0]}

rm_dir_quick()
{
    local useSudo=0
    local useSudoArg=$1

    if [ "--sudo" = "$1" ] || [ "-s" = "$1" ]; then
        useSudo=1
        shift;
    fi

    local dirToDelete="$@"
    local tmpDir="/tmp/dir_to_delete/"

    if [ ${#dirToDelete[@]} -eq 0 ] || [ -z "$dirToDelete" ]; then
        echo "Please pass a directory(ies) to delete"
        return 1
    fi

    mkdir -p $tmpDir

    for dir in $dirToDelete; do

        echo "Removing dir: $dir"

        local startTime=$(date +%s)

        if [ $useSudo ]; then
            sudo rsync -a --delete $tmpDir $dir
            sudo rm -rf $dir
        else
            rsync -a --delete $tmpDir $dir
            rm -rf $dir
        fi

        hlp_get_command_call_string_result

        local endTime=$(date +%s)
        local elapsed=$endTime-$startTime

        if [ $HLP_COMMAND_CALL_RESULT != 0 ]; then
            # Failed to delete dir
            echo "$HLP_COMMAND_CALL_STRING_RESULT to delete dir: $dir"
        else
            printf "Directory has been removed. Elapsed time: "
            dt_hu_time $elapsed
        fi
    done

    rm -rf $tmpDir
}

# sudo_cmd()
# {
#     local cmd=$1
#     sudo bash -c "$(declare -f $cmd); $@"
# }
