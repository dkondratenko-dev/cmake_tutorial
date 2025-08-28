bp_print_welcome_message ${BASH_SOURCE[0]}

# Print FILE:LINE before the message
# echo "$mttf_file_name:$LINENO "  "Test message"
mttf_file_name=$(basename ${BASH_SOURCE[0]})

source ${BASH_SOURCE[0]}.impl

mttf_start()                      # - start mttf client
{
    if [ ! -f $TOOL_MTTF_CLIENT_DIR ]; then
        echo "$mttf_file_name:$LINENO " "MTTF Client does not exists: $TOOL_MTTF_CLIENT_DIR"
        return 1
    fi

    hlp_set_tab_title "MTTF Client"

    local mttf_client_dir=$(dirname "${TOOL_MTTF_CLIENT_DIR}")
    local mttf_client_executable=$(basename "${TOOL_MTTF_CLIENT_DIR}")

    cd $mttf_client_dir
    pwd

    ./$mttf_client_executable &
    echo "MTTF client started."
}

dlt_viewer_start()
{
    if [ ! -f $TOOL_DLT_VIWER_DIR ]; then
        echo "$mttf_file_name:$LINENO " "Dlt viwer does not exists: $TOOL_DLT_VIWER_DIR"
        return 1
    fi

    hlp_set_tab_title "DLT Viewer"

    local dlt_viewer_dir=$(dirname "${TOOL_DLT_VIWER_DIR}")
    local dlt_viewer_executable=$(basename "${TOOL_DLT_VIWER_DIR}")

    cd $dlt_viewer_dir
    pwd

    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$dlt_viewer_dir
    ./$dlt_viewer_executable
}

ssh_to_mttf()
{
    hlp_set_tab_title "MTTF SSH"

    __get_mttf_port

    ssh-keygen -f "$HOME/.ssh/known_hosts" -R "[127.0.0.1]:$MTTF_PORT"
	ssh -o StrictHostKeyChecking=accept-new -p $MTTF_PORT root@127.0.0.1
}

scp_to_mttf_home()
{
    hlp_set_tab_title "MTTF scp -> target"

    __get_mttf_port

    echo "MTTF_PORT:$MTTF_PORT"
    echo "ARGS:$@"

    echo "Measure zipped copy:"

    local startTime=$(date +%s)

    local archieveFile="data_to_send.zip"
    if [ -f $archieveFile ]; then
        rm $archieveFile
    fi

    echo "Removing files on the target device"
    ssh -p $MTTF_PORT root@127.0.0.1  "nohup rm \"$@\" &"

    echo "Archieving..."
    zip -1 -j $archieveFile "$@"

    echo "Copying..."
    scp -P $MTTF_PORT $archieveFile root@127.0.0.1:~/

    echo "Unzipping and removing..."
    #ssh -p 8227 root@127.0.0.1 "unzip -o $archieveFile && rm $archieveFile" &

    ssh -p $MTTF_PORT root@127.0.0.1  "nohup unzip -o $archieveFile && rm $archieveFile &"

    local endTime=$(date +%s)
    local elapsed=$endTime-$startTime

    printf "\nElapsed time: "
    dt_hu_time $elapsed

    rm $archieveFile &

    return 0

    echo "Measure raw copy"
    startTime=$(date +%s)
    scp -P $MTTF_PORT "$@" root@127.0.0.1:~/

    endTime=$(date +%s)
    elapsed=$endTime-$startTime

    printf "\nElapsed time: "
    dt_hu_time $elapsed
}

scp_from_mttf_home()
{
    hlp_set_tab_title "local -> MTTF:~/"

    __get_mttf_port

    scp -P $MTTF_PORT -r root@127.0.0.1:~/$@ .
}


mttf_send_dot_profile()
{
    local currentDir=$(dirname ${BASH_SOURCE[0]})
    local dotProfilePath=$currentDir/.profile
    local dotProfileAliasesPath=$currentDir/.profile_aliases

    scp_to_mttf_home $dotProfilePath $dotProfileAliasesPath
}


mttf_send_emaster()
{
    cd $BP_BINZ_BUILD_DIR/bin

    scp_to_mttf_home your_project_name
}

mttf_reset_port()
{
    __mttf_reset_port
}
