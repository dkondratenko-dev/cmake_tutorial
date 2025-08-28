bp_print_welcome_message ${BASH_SOURCE[0]}

scrp()                            # - source profile
{
    . $BP_BASH_PROFILE_DIR/runner.profile $BP_CURRENT_PROFILE_TYPE
}

srcp()                            # - source profile
{
    scrp
}

profiles()                        # - shows a list of loaded profiles
{
    pm_print_imported_profiles
}
