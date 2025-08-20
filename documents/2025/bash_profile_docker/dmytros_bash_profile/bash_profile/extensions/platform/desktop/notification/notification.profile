hlp_file_name=$(basename ${BASH_SOURCE[0]})

ntfy_play_bell()
{
    echo -e "\a"
}

ntfy_say_msg()
{
    spd-say --wait "$@"
}

ntfy_play_sound()
{
  for s in $@; do
    paplay $s
  done
}

ntfy_play_info()
{
  ntfy_play_sound /usr/share/sounds/ubuntu/stereo/dialog-information.ogg
}

ntfy_play_complete()
{
  ntfy_play_sound /usr/share/sounds/freedesktop/stereo/complete.oga
}

ntfy_play_error()
{
  ntfy_play_sound /usr/share/sounds/freedesktop/stereo/suspend-error.oga
}

ntfy_say_build_complete()
{
    ntfy_say_msg "The build, has been completed"
}

ntfy_say_build_failed()
{
    ntfy_say_msg "The build, has been failed"
}

ntfy_build_complete()
{
    ntfy_play_complete
}

ntfy_build_failed()
{
    ntfy_play_error
}
