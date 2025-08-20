# Enable verbose for debugging
#set -v

BP_CURRENT_PROFILE_TYPE=$1

setup_global_variables()
{
    # Get bash profile folder path
    local this_profile_file_path=${BASH_SOURCE[0]}
    BP_BASH_PROFILE_DIR=$(dirname "$this_profile_file_path")
    BP_PROFILE_MANAGER_DIR=$BP_BASH_PROFILE_DIR/profile_manager

    # To get profile types
    source $BP_PROFILE_MANAGER_DIR/profile_manager.defines
}

source_profile_manager()
{
    source $BP_PROFILE_MANAGER_DIR/profile_manager.profile
}

source_desktop_profile()
{
    source $BP_BASH_PROFILE_DIR/runners/desktop.runner.profile
}

source_docker_profile()
{
    source $BP_BASH_PROFILE_DIR/runners/docker.runner.profile
}

main()
{
    if [ -z "$BP_CURRENT_PROFILE_TYPE" ]; then
        echo "Can't run the profile. Profile type is not set. Please pass the profile type to run eg. "
        echo "runner.profile \"DESKTOP|DOCKER|etc\""
        echo "See profile_manager.defines for available profile types"
        return 1
    fi

    setup_global_variables

    if [ "$BP_CURRENT_PROFILE_TYPE" = "$BP_PROFILE_TYPE_DESKTOP" ]; then
        source_desktop_profile
    elif [ "$BP_CURRENT_PROFILE_TYPE" = "$BP_PROFILE_TYPE_DOCKER" ]; then
        source_docker_profile
    fi

    setup_variables       # This call will depend on profile type

    source_profile_manager

    run_profile           # This call will depend on profile type

unset -f source_profile_manager
unset -f source_desktop_profile
unset -f source_docker_profile
unset -f setup_global_variables
unset -f main
}

main
