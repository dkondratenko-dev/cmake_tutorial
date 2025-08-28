bp_print_welcome_message ${BASH_SOURCE[0]}

run_docker()                      # - Run Docker with bash profile. From any path
{
    hlp_set_tab_title Docker

    # Check if we can run docker script in current directory, otherwise cd project dir
    # This is used to run the docker from other repo than the on in a project dir
    if [ ! -f run_docker.sh ]; then
        cd $BP_PROJECT_DIR
    fi

    echo ">>> Runing docker in: $(pwd)"

    ./run_docker.sh "$@" --bash_profile "$BP_BASH_PROFILE_DIR/runner.profile $BP_PROFILE_TYPE_DOCKER"
}

run_docker_command()              # - Run passed command inside the Docker
{
    run_docker -e "$@"
}

run_docker_build()                # - Run build command inside the Docker
{
    run_docker_command "build"

    hlp_get_command_call_string_result

    if [ $HLP_COMMAND_CALL_RESULT != 0 ]; then
        ntfy_build_failed
    else
        ntfy_build_complete
    fi
}

run_docker_clean_build()
{
    run_docker_command "clean_build"

    hlp_get_command_call_string_result

    if [ $HLP_COMMAND_CALL_RESULT != 0 ]; then
        ntfy_build_failed
    else
        ntfy_build_complete
    fi
}

run_docker_run_all_unit_tests()
{
    run_docker_command "run_all_unit_tests"
}

run_docker_make_code_format()
{
    run_docker_command "make_code_format"
}
