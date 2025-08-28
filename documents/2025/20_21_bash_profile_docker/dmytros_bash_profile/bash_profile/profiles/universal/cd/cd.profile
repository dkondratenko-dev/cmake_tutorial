bp_print_welcome_message ${BASH_SOURCE[0]}

source ${BASH_SOURCE[0]}.impl

cd_project()                      # - cd into emaster project directory. From any path
{
    cdimpl_cd $BP_PROJECT_DIR "$@"

    cmd_crossplatform_run git_status
}

cd_bash_profile()                 # - cd into bash profile directory
{
    cdimpl_cd $BP_BASH_PROFILE_DIR

    cmd_crossplatform_run git_status
    cmd_crossplatform_run hlp_set_tab_title "BASH Profile"
}

cd_build_bin()                    # - cd into build/bin directory
{
    # Set default build dir if empty
    if [ -z "$BP_PROJECT_BUILD_DIR" ]; then
        BP_PROJECT_BUILD_DIR=$BP_PROJECT_DIR/build
    fi

    cdimpl_cd $BP_PROJECT_BUILD_DIR

    cmd_crossplatform_run hlp_set_tab_title "Build binaries"
}
