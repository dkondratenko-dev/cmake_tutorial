# Enable verbose for debugging
#set -v

BP_CURRENT_PROFILE_TYPE=$BP_PROFILE_TYPE_DESKTOP

setup_variables()
{
    source $BP_CONFIG_FULL_PATH
    bp_print_welcome_message ${BASH_SOURCE[0]}
}

print_welcome_message()
{
    echo "Started any.profile($BP_CURRENT_PROFILE_TYPE) v.$BP_VERSION"
}

run_profile()
{
    pm_source_recursive extensions/platform/desktop

    pm_import_profile_recursive universal
    pm_import_profile_recursive platform/desktop

    print_welcome_message

unset -f setup_variables
unset -f print_welcome_message
unset -f run_profile
}
