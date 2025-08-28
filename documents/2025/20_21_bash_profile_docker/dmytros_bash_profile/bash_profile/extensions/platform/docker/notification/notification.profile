#Custom audio commands for use in notification during build
#Used originally for very long builds, like 30mins - audio builds free a developer to work on something else while waiting for audio notifications
#Docker version is because Ubuntu audio files may not be available in docker

hlp_file_name=$(basename ${BASH_SOURCE[0]})

ntfy_play_bell()
{
    echo -e "\a"
}

ntfy_build_complete()
{
    ntfy_play_bell
}

ntfy_build_failed()
{
    ntfy_play_bell
    sleep 1
    ntfy_play_bell
}
