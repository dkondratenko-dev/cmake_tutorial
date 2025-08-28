bp_print_welcome_message ${BASH_SOURCE[0]}

set_tab_title_as_folder_name()    # - Set current folder name as a tab title
{
    hlp_set_tab_title $(basename $(pwd))
}

# Set default tab title to the current folder name
set_tab_title_as_folder_name
