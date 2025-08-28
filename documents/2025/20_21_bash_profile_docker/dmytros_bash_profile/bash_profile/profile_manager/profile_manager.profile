#Engine for all of the profile functionality - Called from runner.profile
#Switching between platforms, loading and managing help functionaltiy etc
#Manages the recursive loading function 

# Enable verbose for debugging
# set -x
# set -o functrace

bp_print_welcome_message ${BASH_SOURCE[0]}

source $BP_PROFILE_MANAGER_DIR/profile_manager.profile.aliases

# Print FILE:LINE before the message
# echo "$pm_file_name:$LINENO "  "Test message"
pm_file_name=$(basename ${BASH_SOURCE[0]})

# Stores a list of added profiles
pm_imported_profiles_array=()

# Import\source a profile from /profiles folder. Eg. "pm_import_profile mttf" will load
# /profiles/mttf/mttf.profile
pm_import_profile()
{
    local profileName=$1

    pm_source_profile $profileName

    # If profile sourced successfully , add it to array of profiles
    if [[ $? -eq 0 ]]; then
        pm_imported_profiles_array+=( "$profileName" )
    fi
}

pm_source_profile()
{
    # Check argument is not empty
    local profileName=$1
    if [ -z $profileName ]; then
        echo "$pm_file_name:$LINENO " "Please pass a profile name. Ex: pm_source_profile git"
        return 1
    fi

    # Get profile name. eg. platform/docker/build will use build as a name
    profileBaseName=$(basename $profileName)

    # Source profile platform/desktop/archieve
    local profileFilePath=$BP_BASH_PROFILE_DIR/profiles/$profileName/$profileBaseName.profile
    if [ ! -f $profileFilePath ]; then
        #echo "$pm_file_name:$LINENO " "Profile does not exists: $profileFilePath"
        return 1
    fi

    source $profileFilePath

    # Source aliases if exists
    local aliases=$profileFilePath.aliases
    if [ -f $aliases ]; then
        source $aliases
    fi

    return 0
}

pm_setup_tools_dirs(){
    echo "Please setup tools directories. If the tool is not available or you are not using it, you can hit enter to leave the path empty"

    local dlt_app_name="dlt_viewer"
    echo "Finding $dlt_app_name..."
    find / -type f -name $dlt_app_name 2>/dev/null
    read -p "Set $dlt_app_name directory: " dlt_viwer_dir

    local mttf_app_name="mttf-client"
    echo "Finding $mttf_app_name..."
    find / -type f -name $mttf_app_name 2>/dev/null
    read -p "Set $mttf_app_name directory: " mttf_client_dir

    TOOL_DLT_VIWER_DIR=$dlt_viwer_dir
    TOOL_MTTF_CLIENT_DIR=$mttf_client_dir
}

pm_setup_emaster_dirs(){
    BP_PROJECT_DIR=$1
}

pm_print_profile_variables()
{
    echo "Printing profile variable..."
    echo

    echo "BP_PROJECT_DIR is set to          : $BP_PROJECT_DIR"
    echo "BP_BASH_PROFILE_DIR is set to     : $BP_BASH_PROFILE_DIR"
    echo "BP_PROFILE_MANAGER_DIR is set to  : $BP_PROFILE_MANAGER_DIR"

    echo "TOOL_DLT_VIWER_DIR is set to      : $TOOL_DLT_VIWER_DIR"
    echo "TOOL_MTTF_CLIENT_DIR is set to    : $TOOL_MTTF_CLIENT_DIR"
}

pm_write_config()
{
    echo "Writing config file: $BP_CONFIG_FULL_PATH"

    # write emaster dirs
    echo "BP_PROJECT_DIR=\"$BP_PROJECT_DIR\"" >> $BP_CONFIG_FULL_PATH
    echo "BP_BASH_PROFILE_DIR=\"$BP_BASH_PROFILE_DIR\"" >> $BP_CONFIG_FULL_PATH
    echo "BP_PROFILE_MANAGER_DIR=\"$BP_PROFILE_MANAGER_DIR\"" >> $BP_CONFIG_FULL_PATH

    # write tools dirs
    echo "TOOL_DLT_VIWER_DIR=\"$TOOL_DLT_VIWER_DIR\"" >> $BP_CONFIG_FULL_PATH
    echo "TOOL_MTTF_CLIENT_DIR=\"$TOOL_MTTF_CLIENT_DIR\"" >> $BP_CONFIG_FULL_PATH

    # Other options
    echo "BP_SHOW_WELCOME_MESSAGE=1" >> $BP_CONFIG_FULL_PATH
}

pm_create_new_config(){
    echo "Finding your_project_name directories on this system:"
    find / -type d -name "your_project_name" 2>/dev/null | grep -v -e Trash -e '/opt/'

    read -p "Set your_project_name source dir: " emaster_src_dir

    pm_setup_emaster_dirs $emaster_src_dir

    pm_setup_tools_dirs

    pm_write_config
}

pm_source_config()
{
    source $BP_CONFIG_FULL_PATH

    pm_print_profile_variables
}

pm_add_runner_to_bash_rc()
{
    local bash_rc="$HOME/.bashrc"

    echo  >> $bash_rc
    echo "source $BP_BASH_PROFILE_DIR/runner.profile $BP_PROFILE_TYPE_DESKTOP" >> $bash_rc
}

pm_setup_profile()
{

    local profile_type=$1

    # Remove config if already exist
    if [ -f "$BP_CONFIG_FULL_PATH" ]; then
        rm $BP_CONFIG_FULL_PATH
    fi

    pm_create_new_config

    pm_source_config

    pm_add_runner_to_bash_rc
}

pm_show_help_menu_for_specific_profile()
{
    local profileName=$1
    if [ -z $profileName ]; then
        echo "$pm_file_name:$LINENO " "Please pass a profile name."
        return 1
    fi

    # Get profile name. eg. platform/docker/build will use build as a name
    profileBaseName=$(basename $profileName)


    local profileFilePath=$BP_BASH_PROFILE_DIR/profiles/$profileName/$profileBaseName.profile

    if [ ! -f $profileFilePath ]; then
        echo "$pm_file_name:$LINENO " "Profile does not exists: $profileFilePath"
        return 1
    fi

    # Get commands from profile
    local profile_hlp=$(hlp_list_profile_commands $profileFilePath)

    # Get profile aliases if exists
    local aliases=$profileFilePath.aliases
    local aliases_profile_hlp=""
    if [ -f $aliases ]; then

        aliases_profile_hlp=$(hlp_list_profile_commands $aliases)

        if [  ${#aliases_profile_hlp} -eq 0 ]; then
            # Profile doesn't have any commands\aliases. Skip it \ Exit ...
            if [  ${#profile_hlp} -eq 0 ]; then
                return 1;
            fi
        fi
    fi

    # Print profile commands/aliases
    echo "Help menu for \"$profileName\" profile:"

    # Print commands if available
    if [  ${#profile_hlp} -ne 0 ]; then
        echo "  Commands:"
        printf '%s\n' "${profile_hlp[@]}"
    fi

    if [  ${#aliases_profile_hlp} -ne 0 ]; then
        echo
        echo "  Aliases:"
        hlp_list_profile_commands $aliases
    fi

    return 0;
}

pm_show_help_menu()
{
    # To make it work: "h git" will show the help menu for git profile only
    local specific_profile=$1
    if [ ! -z $specific_profile ]; then

        # Check if array contains value
        if [[ " ${pm_imported_profiles_array[*]} " =~ " ${specific_profile} " ]]; then
            pm_show_help_menu_for_specific_profile $specific_profile
            return 0
        fi

        for profile in ${pm_imported_profiles_array[@]}; do
            if [[ $(basename $profile) == $specific_profile ]]; then
                pm_show_help_menu_for_specific_profile $profile
            fi
        done

        return 0
    fi

    echo "
Here is the list of commands you can use:"

    echo

    # Iterate through all added profiles and print the help menu for them
    for p in ${pm_imported_profiles_array[@]}; do
        if pm_show_help_menu_for_specific_profile $p; then
            # print a new line if command executed successfully
            echo
        fi
    done

    echo "Other commands:

    h,help                            # - prints this menu
    h generic|git|mttf                # - prints help for specified profile name
"
}

pm_print_imported_profiles()
{
    for p in ${pm_imported_profiles_array[@]}; do
        echo $p
    done
}

# Usage:
#       arg1 - should be a root dir inside $BP_BASH_PROFILE_DIR to search
#       arg2 - passed into find function
# Ex:
#       pm_source_recursive extensions/universal "! -name system"
#           Will source all the profiles under "$BP_BASH_PROFILE_DIR/extensions/universal/"
#           except system
pm_source_recursive()
{
    local rootDir=$1

    if [ -z "$rootDir" ]; then
        echo "Pass the directory to start search"
        return 1
    fi

    local bp_root=$BP_BASH_PROFILE_DIR/$rootDir

    if [ ! -d $bp_root ]; then
        echo "Directory doesn't exists: $bp_root"
        return 1
    fi

    local files=($(find $bp_root -type f ! -name "*.txt" $2))

    for file in ${files[@]}; do
        source $file
    done

    return 0
}

# Usage:
#       arg1 - should be a root dir inside $BP_BASH_PROFILE_DIR/profiles to search
#       arg2 - passed into find function
# Ex:
#       pm_import_profile_recursive universal "! -name system"
#           Will import all the profiles under "$BP_BASH_PROFILE_DIR/profiles/universal"
#           except system
pm_import_profile_recursive()
{
    local rootDir=$1

    if [ -z "$rootDir" ]; then
        echo "Pass the directory to start search"
        return 1
    fi

    local profilesDir=$BP_BASH_PROFILE_DIR/profiles
    local bp_root=$profilesDir/$rootDir

    if [ ! -d $bp_root ]; then
        echo "Directory doesn't exists: $bp_root"
        return 1
    fi

    local pervDir=$(pwd)

    cd $profilesDir
    local dirs=($(find $rootDir/* -type d $2))

    cd $pervDir

    for profileKey in ${dirs[@]}; do
        pm_import_profile $profileKey 
    done
}

pm_source_recursive extensions/universal
