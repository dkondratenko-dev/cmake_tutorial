hlp_file_name=$(basename ${BASH_SOURCE[0]})

hlp_exit_on_error()
{
    local HLP_COMMAND_CALL_RESULT=$?
    local MSG=$1

    if [ -z "$MSG" ]; then
        MSG="Operation failed. Exiting"
    fi

    if [ $HLP_COMMAND_CALL_RESULT != 0 ]; then
        echo
        echo "MSG: $MSG"
        echo
        # Exit will close terminal
        # exit $HLP_COMMAND_CALL_RESULT
        # sadfasdfs
        return $HLP_COMMAND_CALL_RESULT
    fi
}

hlp_get_result()
{
    HLP_COMMAND_CALL_RESULT=$?
}

hlp_get_command_call_string_result()
{
    HLP_COMMAND_CALL_RESULT=$?
    HLP_COMMAND_CALL_STRING_RESULT="PASSED"

    if [ $HLP_COMMAND_CALL_RESULT != 0 ]; then
        HLP_COMMAND_CALL_STRING_RESULT="FAILED"
    fi

    return $HLP_COMMAND_CALL_RESULT
}

# function to set terminal title
hlp_set_tab_title()
{
  title=$1
  (printf "\e]2;$title\a" &)
  #PROMPT_COMMAND='echo -en "\033]0; $title \a"'
}

hlp_list_profile_commands()
{
    local filePath=$1
    if [ ! -f $filePath ]; then
        echo "$hlp_file_name:$LINENO " "Can't list profile commands. File does not exists: $filePath"
        return 1
    fi

    grep -E '^[[:space:]]*([[:alnum:]_]+[[:space:]]*\(\)|alias[[:space:]]+[[:alnum:]_]+)' $filePath | sed -e 's/^/    /'
}

hlp_check_function_exists()
{
    # Assign to a variable to hide command output
    local t=$(type -t $1)

    hlp_get_result

    echo "Check cmd: $1, Result: $HLP_COMMAND_CALL_RESULT, t: $t"

    return $HLP_COMMAND_CALL_RESULT
}
