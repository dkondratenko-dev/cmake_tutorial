bp_print_welcome_message ${BASH_SOURCE[0]}

ungz()
{
    local fileName=$1
    local exctracted=$(basename -s .gz $fileName)
    pv $fileName | gzip -d -c > $exctracted
}
