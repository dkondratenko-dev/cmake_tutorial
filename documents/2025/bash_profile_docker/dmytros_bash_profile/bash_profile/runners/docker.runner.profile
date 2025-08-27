# Setup for Docker variables
# Enable verbose for debugging
#set -v

BP_CURRENT_PROFILE_TYPE=$BP_PROFILE_TYPE_DOCKER

setup_variables()
{
    local gitBranch=$(git branch --show-current)
    echo "Git branch: $gitBranch"

    local currentDir=$(pwd)
    local thisProfileDir=$(dirname ${BASH_SOURCE[0]})

    cd $thisProfileDir
    cd ..

    BP_BASH_PROFILE_DIR=$(pwd)
    BP_PROFILE_MANAGER_DIR=$BP_BASH_PROFILE_DIR/profile_manager
    BP_PROJECT_DIR=$MOUNT_POINT_NAME/your_project_name
    BP_PROJECT_BUILD_DIR=$MOUNT_POINT_NAME/ec_docker_build/$gitBranch
    BP_QUICK_PROFILE_DIR=$BP_PROJECT_DIR/.vscode/bp/quick_profile
    BP_QUICK_PROFILE=$BP_QUICK_PROFILE_DIR/quick.profile
    BP_SHOW_WELCOME_MESSAGE=1

    export BP_BASH_PROFILE_DIR=$BP_BASH_PROFILE_DIR
    export BP_PROFILE_MANAGER_DIR=$BP_PROFILE_MANAGER_DIR
    export BP_PROJECT_DIR=$BP_PROJECT_DIR
    export BP_PROJECT_BUILD_DIR=$BP_PROJECT_BUILD_DIR
    export BP_QUICK_PROFILE_DIR=$BP_QUICK_PROFILE_DIR
    export BP_QUICK_PROFILE=$BP_QUICK_PROFILE
    export BP_SHOW_WELCOME_MESSAGE=$BP_SHOW_WELCOME_MESSAGE

    echo "---------------------------------------------------------------------------------------------------"
    echo "░▒▓▓ Docker Profile Environment Variables:"
    echo "BP_BASH_PROFILE_DIR:$BP_BASH_PROFILE_DIR"
    echo "BP_PROFILE_MANAGER_DIR:$BP_PROFILE_MANAGER_DIR"
    echo "BP_PROJECT_DIR:$BP_PROJECT_DIR"
    echo "BP_PROJECT_BUILD_DIR:$BP_PROJECT_BUILD_DIR"
    echo "BP_QUICK_PROFILE:$BP_QUICK_PROFILE"
    echo "BP_SHOW_WELCOME_MESSAGE:$BP_SHOW_WELCOME_MESSAGE"
    echo "---------------------------------------------------------------------------------------------------"

    cd $currentDir

    bp_print_welcome_message ${BASH_SOURCE[0]}
}

print_welcome_message()
{
    echo "
**************************************************************

Welcome to any.profile! The purpose of this .profile is to
simplify using this environment."

pm_show_help_menu

echo ">>> Started any.profile($BP_CURRENT_PROFILE_TYPE) v.$BP_VERSION! Enjoy the coding :)

**************************************************************
"
}

run_profile()
{
    pm_import_profile_recursive universal "! -name system"
    pm_import_profile_recursive platform/docker

    # Load quick profile
    mkdir -p $BP_QUICK_PROFILE_DIR
    touch $BP_QUICK_PROFILE
    source $BP_QUICK_PROFILE

    print_welcome_message

unset -f setup_variables
unset -f print_welcome_message
unset -f run_profile
}
