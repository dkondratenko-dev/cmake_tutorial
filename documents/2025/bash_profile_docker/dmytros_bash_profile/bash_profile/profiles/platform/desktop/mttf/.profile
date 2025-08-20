profilePath=${BASH_SOURCE[0]}
profileAliasesPath=${profilePath}_aliases

# Source aliases
. $profileAliasesPath

echo ""

cat <<EOF
.profile has been loadaded

To show all commands, enter 'help,h' command

To unload the profile, enter 'unload_profile' command

EOF

help()
{
    echo "Aliases:"
    cat $profileAliasesPath

    echo ""

    echo "Functions: "
    declare -F | cut -f3 -d" "
}

unload_profile()
{
    unset `declare -F | cut -f3 -d" "`
    unalias -a

    rm $profileAliasesPath
    rm $profilePath

    echo "Profile has been unloaded!"
}

# ----------------------------------- team_name - master -----------------------------------

emaster_stop()
{
    systemctl stop your_project_name
}

emaster_start()
{
    systemctl start your_project_name
}

emaster_restart()
{
    systemctl restart your_project_name
}

emaster_is_running()
{
    ps aux | grep [your]_project-master
}

emaster_kill()
{
    kill -9 $(ps aux | grep [your]_project-master | awk '{ print $1 }')
}

emaster_cp_from_home_to_usr_bin()
{
    cp your_project_name /usr/bin/your_project_name
    chown team_name:team_name /usr/bin/your_project_name
}

emaster_install_audio_data()
{
    mkdir -p /usr/share/your_project_name-data/audio/
    cp something.* /usr/share/your_project_name-data/audio/
    chown -R team_name:team_name /usr/share/your_project_name-data
    ls -lah /usr/share/your_project_name-data/audio/
}

# ----------------------------------- DASH CAMERA -----------------------------------

dashcam_name_stop()
{
    systemctl stop dashcam_name
}

dashcam_name_start()
{
    systemctl start dashcam_name
}

dashcam_name_restart()
{
    systemctl restart dashcam_name
}

dashcam_name_is_running()
{
    ps aux | grep [dash]cam
}

dashcam_name_kill()
{
    kill -9 $(ps aux | grep [dash]cam | awk '{ print $1 }')
}

# ----------------------------------- CAMERA APPLICATION -----------------------------------

camera_app_name_stop()
{
    systemctl stop camera_app_name
}

camera_app_name_start()
{
    systemctl start camera_app_name
}

camera_app_name_restart()
{
    systemctl restart camera_app_name
}

camera_app_name_is_running()
{
    ps aux | grep [came]ra-app
}

camera_app_name_kill()
{
    kill -9 $(ps aux | grep [came]ra-app | awk '{ print $1 }')
}
