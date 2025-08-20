bp_print_welcome_message ${BASH_SOURCE[0]}

# Source make.profile.impl
source ${BASH_SOURCE[0]}.impl

make_code_format()                # - make code format
{
    mk_impl_run_command "make codeformat"
}

make_format_check()               # - make code format
{
    mk_impl_run_command "make formatcheck"
}

make_code_coverage()              # - make code coverage
{
    mk_impl_run_command "make coverage"
}

make_code_analysis()              # - make code analysis
{
    local results_xml="cppcheck-result.xml"

    if [ -f "$results_xml" ]; then
        rm $results_xml
    fi

    mk_impl_run_command "make codeanalysis"
}

make_doxygen()
{
    mk_impl_run_command "make doxygen"
}

cmake_list_all_targets()
{
    cmake --build . --target help
}
