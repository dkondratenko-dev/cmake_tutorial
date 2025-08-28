bp_print_welcome_message ${BASH_SOURCE[0]}

# Print FILE:LINE before the message
# echo "$qemu_file_name:$LINENO "  "Test message"
qemu_file_name=$(basename ${BASH_SOURCE[0]})

BP_QEMU_IP="0.0.0.0"

ssh_to_qemu()
{
    hlp_set_tab_title "Qemu SSH"

    ssh-keygen -f "$HOME/.ssh/known_hosts" -R "[$BP_QEMU_IP]"
	ssh -o StrictHostKeyChecking=accept-new root@$BP_QEMU_IP
}

scp_to_qemu_home()
{
    hlp_set_tab_title "scp to QEMU"

	scp -r "$@" root@$BP_QEMU_IP:~/
}

scp_from_qemu_home()
{
    hlp_set_tab_title "scp from QEMU"

    scp -r root@$BP_QEMU_IP:~/$@ .
}

scp_from_qemu()
{
    hlp_set_tab_title "scp from QEMU"

    scp -r root@$BP_QEMU_IP:$@ .
}

qemu_start()
{
    cd_project

    # cd_project will change the tab title
    hlp_set_tab_title "QEMU Instance"

    scripts/qemu/run_qemu_with_pulseaudio.sh $HOME/Downloads/SDK_installers/QEMU
}

qemuui_start()
{
    cd_project

    # cd_project will change the tab title
    hlp_set_tab_title "QEMU Instance"

    # Setup environment
    local yoctoDir="$HOME/Development/qemu_plus_ui/yocto"
    cd $yoctoDir
    source path/sometihng
    sudo env "PATH=$PATH" runqemu-gen-tapdevs `id -u` `id -g` 4 path/usr/bin

    # Set umask
    umask a=rx,u=rwx

    # Run QEMU
    # runqemu wic kvm publicvnc  qemuparams="-monitor telnet:127.0.0.1:1234,server,nowait" "$HOME/Downloads/SDK_installers/QEMU"

    cd_project
    scripts/qemu/run_qemu_with_ui_and_pulseaudio.sh $HOME/Downloads/SDK_installers/QEMU
}

qemu_kill()
{
    kill -9 $(ps aux | grep [qe]mu | awk '{ print $1 }')
    sd kill -9 $(ps aux | grep '[kv]m\|\[nv\]me\|\[qem\]u' | awk '{ print $2 }')
}

qemu_running()
{
    ps aux | grep '[kv]m\|\[nv\]me\|\[qem\]u'
}

qemu_emulate_connect()
{
    cd_project

    hlp_set_tab_title "Qemu SSH"

    scripts/qemu/emulate-connect.sh

    ssh-keygen -f "/home/user_name/.ssh/known_hosts" -R '\"$BP_QEMU_IP\"'

    ssh_to_qemu
}


download_qemu_image()
{
    hlp_set_tab_title "Download QEMU image"

    cd_qemu_plus_ui

    ./download-qemu-artifacts.sh "" $@
}
