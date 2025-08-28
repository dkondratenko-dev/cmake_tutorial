bp_print_welcome_message ${BASH_SOURCE[0]}

BP_QA_AUTOMATION_QEMU_SDK_DIR="$HOME/Downloads/SDK_installers/qemu/nuc"
BP_QA_AUTOMATION_REPO_DIR="$HOME/Development/trunk/apricot/QA_automation/your_project_name"
BP_QA_AUTOMATION_DIR="$BP_QA_AUTOMATION_REPO_DIR/test/integration"

qa_scp_integration_tests_to_qemu()
{
    cd $BP_QA_AUTOMATION_DIR
    scp_to_qemu_home src/
}

qa_start_qemu()
{
    cd $BP_QA_AUTOMATION_REPO_DIR/scripts/qemu
    ./run_qemu_with_pulseaudio.sh $BP_QA_AUTOMATION_QEMU_SDK_DIR
}
