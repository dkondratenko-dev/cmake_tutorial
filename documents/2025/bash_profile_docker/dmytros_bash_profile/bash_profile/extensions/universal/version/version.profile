vrsn_get()
{
    local fileName=${BASH_SOURCE[0]}
    local dir=$(dirname $fileName)

    local versionFilePath=$dir/version.txt

    local profileVersion=$(cat $versionFilePath)
    BP_VERSION=$profileVersion

    bp_print_welcome_message ${BASH_SOURCE[0]} "v. $BP_VERSION"

    unset -f vrsn_get
}

vrsn_get
