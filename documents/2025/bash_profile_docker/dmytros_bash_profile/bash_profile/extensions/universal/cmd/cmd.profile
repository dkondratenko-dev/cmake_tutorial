bp_print_welcome_message ${BASH_SOURCE[0]}

# Check is function exists before run
# If functions is not exists, it will not be executed
# Use for cross-platform checks before using a function
cmd_crossplatform_run()
{
    local cmd=$1
    shift

    hlp_check_function_exists $cmd

    if [[ $HLP_COMMAND_CALL_RESULT == 0 ]]; then

        if [ "$#" -ne 0 ]; then
            echo "Runing command with parameters: $cmd $@"
            eval $cmd "\"$@\""
        else
            echo "Runing command without parameters: $cmd"
            eval $cmd
        fi
    else
        echo "BP: Command $cmd not found"
    fi
}
