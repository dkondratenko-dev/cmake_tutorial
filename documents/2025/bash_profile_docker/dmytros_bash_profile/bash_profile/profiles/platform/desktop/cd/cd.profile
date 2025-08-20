bp_print_welcome_message ${BASH_SOURCE[0]}


BP_DEVELOPMENT_DIR=$HOME/Development
BP_REPOS_DIR=$BP_DEVELOPMENT_DIR/trunk
BP_META_orange_DIR=$BP_REPOS_DIR/meta-orange/meta-orange

cd_dev()
{
    cdimpl_cd $BP_DEVELOPMENT_DIR
    git_status
}

cd_repos()
{
    cdimpl_cd $BP_REPOS_DIR
}

cd_meta_orange()
{
    cdimpl_cd $BP_META_orange_DIR
}

cd_sdk()
{
    local SDK_DIR=/opt/orange/1.0
    cdimpl_cd $SDK_DIR

    hlp_set_tab_title SDK
}

cd_sdk_installers()
{
    cdimpl_cd $HOME/Downloads/SDK_installers

    hlp_set_tab_title SDK
}

cd_qemu_plus_ui()
{
    cdimpl_cd $HOME/Development/qemu_plus_ui

    hlp_set_tab_title "QEMU plus UI"
}

cd_qa()
{
    cdimpl_cd $BP_PROJECT_DIR/../QA_automation/your_project_name/

    git_status

    hlp_set_tab_title "QA Automation"
}
