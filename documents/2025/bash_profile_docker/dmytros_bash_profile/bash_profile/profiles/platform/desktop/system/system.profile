bp_print_welcome_message ${BASH_SOURCE[0]}

system_clean_disk_space()
{
    hlp_set_tab_title "Cleanup disk space"

    # request sudo
    sudo echo

    printf "\n>>> Clenaing up disk space used by Docker\n"
    docker_cleanup_disk_space

    printf "\n>>> Clenaing up Trash folder\n"
    rm_dir_quick $HOME/.local/share/Trash/*

    printf "\n>>> Clenaing up ~/.cache folder\n"
    rm_dir_quick $HOME/.cache/dlt_viewer vscode-cpptools mozilla google-chrome tracker

    printf "\n>>> Clenaing up: /var/log/journal\n"
    du -csh /var/log/journal
    journalctl --disk-usage
    sudo journalctl --vacuum-time=2d

    printf "\n>>> Removing downloaded packages\n"
    sudo du -csh /var/cache/apt  2> /dev/null
    sudo apt-get clean

    printf "\n>>> Removing unnecessary packages\n"
    sudo apt-get autoremove


    # Remove old kernels:
    #sudo apt-get autoremove --purge

    printf "\n>>> Check the /var/tmp/ folder usage:\n"
    sudo du -sh /var/tmp/ 2> /dev/null

    printf "\n>>> See the bigges installed packages:\n"
    dpigs -H

    system_run_du
}

system_run_du()
{
    sudo baobab
}

track()
{
    $HOME/Development/bash_scripts/track.sh $@
}

track_work()
{
    track "work"
}

download_aria()
{
    local url=$1
    local threadsCnt=$(nproc)
    local destDir=$(pwd)
aria2c \
--certificate="path/to/swf.crt" \
--private-key="path/to/swf.key" \
-c -x $threadsCnt -s $threadsCnt -d $destDir $url
}

show_syslog()
{
    sudo vi /var/log/syslog
}

fix_auth_window()
{
    dbus-send --type=method_call --dest=org.gnome.Shell /org/gnome/Shell org.gnome.Shell.Eval string:"global.reexec_self()"
}
