bp_print_welcome_message ${BASH_SOURCE[0]}

cpp_create_new_class()
{
    local className=$1
    local currentDir=$(pwd)

    echo ">> currentDir: $currentDir"

    # Get bash profile folder path
    local this_profile_file_path=${BASH_SOURCE[0]}
    THIS_PROFILE_DIR=$(dirname "$this_profile_file_path")

    python $THIS_PROFILE_DIR/py/classCreator.py $currentDir $className
}
