bp_print_welcome_message ${BASH_SOURCE[0]}

docker_cleanup_disk_space()
{
    docker system prune
}

docker_remove_all_images()
{
    docker system prune -a
}

docker_stop_all_containers()
{
    docker kill $(docker ps -q)
}
