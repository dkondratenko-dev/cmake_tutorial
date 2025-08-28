bp_print_welcome_message ${BASH_SOURCE[0]}

dev_commit_and_push_the_changes()
{
    git_add_all

    sleep 3

    meld .

    branchName=$(git rev-parse --abbrev-ref HEAD)
    issueID=${branchName##*_}

    if  [[ ! $issueID == APRICOT-* ]] || [[ ! $issueID == orange-* ]];
    then
        issueID=""
    fi

    local commitMsgFile="/tmp/commit_msg.txt"

    if [ -f $commitMsgFile ]; then
        rm $commitMsgFile
    fi

    echo "ec:" >> $commitMsgFile
    echo "" >> $commitMsgFile
    echo "Issue-ID: $issueID" >> $commitMsgFile

    modifiedTime=$(date -r $commitMsgFile)

    gedit $commitMsgFile

    lastModifiedTime=$(date -r $commitMsgFile)

    while [ "$lastModifiedTime" == "$modifiedTime" ]
        do
            echo "Wating for file to be modified: l:$lastModifiedTime , m:$modifiedTime"
            sleep 1
            lastModifiedTime=$(date -r $commitMsgFile)

            # TODO : break when gedit pid is gone $!
        done

    echo "Commit msg:"
    cat $commitMsgFile

    sleep 1

    commitMsg=$(cat $commitMsgFile)
    git commit -m "$commitMsg"

    echo "Commited with message:"
    echo "$commitMsg"

    git push -u origin $branchName

    git_log_show_last_commit

    rm $commitMsgFile

    git_status
}

dev_test_commit_and_push_the_changes()
{
    run_docker_command "make_code_format && clean_build && make_format_check && run_all_unit_tests"

    hlp_get_command_call_string_result

    # Exit due to build failed
    if [ $HLP_COMMAND_CALL_RESULT != 0 ]; then
        echo
        echo "$HLP_COMMAND_CALL_STRING_RESULT : dev_test_commit_and_push_the_changes"
        echo
        return 1
    fi

    dev_commit_and_push_the_changes
}
