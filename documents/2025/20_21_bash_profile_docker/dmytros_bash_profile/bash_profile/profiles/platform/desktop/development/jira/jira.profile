bp_print_welcome_message ${BASH_SOURCE[0]}

# Source build.profile.impl
source ${BASH_SOURCE[0]}.impl

jira_create_subtasks()
{
    local sprintVersion=$1

    if [ -z "$sprintVersion" ]; then
        read -p "Enter sprintVersion: " sprintVersion
    fi

    _impl_jira_create_subtasks "Update SSA" "Update SSA according to the code changes" $sprintVersion
    _impl_jira_create_subtasks "Update SDD" "Update SDD according to the code changes" $sprintVersion
    _impl_jira_create_subtasks "Add unit-tests" "Code changed must be coevered with the unit-tests" $sprintVersion
    _impl_jira_create_subtasks "Add component-tests" "Code changed must be coevered with the component-tests" $sprintVersion
    _impl_jira_create_subtasks "Perform manual test" "Apllication with corresponding changes has to be manually tested" $sprintVersion
    _impl_jira_create_subtasks "Add integration-test" "Code changed must be coevered with the integration-tests" $sprintVersion
}

jira_generate_release_notes_links()
{
    local currentDir=$(dirname ${BASH_SOURCE[0]})
    $currentDir/generate_relase_notes_links.sh
}
