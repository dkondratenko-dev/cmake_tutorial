bp_print_welcome_message ${BASH_SOURCE[0]}

uml_convert()
{
    local currentDir=$(dirname ${BASH_SOURCE[0]})
    python3 $currentDir/py/cpp_to_uml.py $@
}

cpp_to_uml()
{
    local rootDir=$1
    local currentDir=$(dirname ${BASH_SOURCE[0]})
    local converterPY=$currentDir/py/cpp_to_uml.py

    local subDirList=$(find $rootDir -type d)

    for dir in $subDirList
    do
        python3 $converterPY $dir
    done
}
