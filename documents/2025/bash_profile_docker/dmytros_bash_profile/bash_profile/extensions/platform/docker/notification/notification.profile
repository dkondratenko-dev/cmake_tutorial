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
