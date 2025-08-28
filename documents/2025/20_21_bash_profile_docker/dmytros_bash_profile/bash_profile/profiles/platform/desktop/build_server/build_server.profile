bp_print_welcome_message ${BASH_SOURCE[0]}

# Source build.profile.impl
# source ${BASH_SOURCE[0]}.impl

ssh_to_build_server()
{
    hlp_set_tab_title "Build server"

    ssh 'user_name@0.0.0.0'
}

ssh_to_build_server_and_source_bp()
{
    hlp_set_tab_title "SCP: Build server"

    ssh -t user_name@0.0.0.0 'source /NewDisk/user_name/bp/setup_bp.sh;cd_project; bash -l'
}

scp_from_build_server()
{
    hlp_set_tab_title "SCP: Build server"

    scp -r user_name@0.0.0.0:~/$@ .
}

scp_from_build_server_shared()
{
    hlp_set_tab_title "SCP: Build server"

    scp -r user_name@0.0.0.0:~/SharedFolder/$@ .
    unzip $@
}

scp_to_build_server_shared()
{
    hlp_set_tab_title "SCP: Build server"

    scp -r "$@" user_name@0.0.0.0:~/SharedFolder/
}
