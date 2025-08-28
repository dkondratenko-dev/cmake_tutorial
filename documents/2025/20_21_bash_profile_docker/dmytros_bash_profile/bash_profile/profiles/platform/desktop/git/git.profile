bp_print_welcome_message ${BASH_SOURCE[0]}

git_status()                      # - Do: git status / git branch
{
    echo ">>> git status:"
    git status "$@"

    echo ">>> git branch:"
    git branch
}

git_checkout()                      # - Do: git checkout and git status
{
    git checkout "$@"

    git_status
}

git_add_all()                     # - Do: git add -A / git_status
{
    git add -A && git_status
}

git_add_modified()                # - Do: git add -u / git_status
{
    git add -u && git_status
}

git_add_and_commit()              # - Add tracked changes, do: git commit / git status
{
    git_add_modified

    sleep 2

    git commit "$@"

    git_status
}

git_add_and_commit_and_push()     # - Add tracked changes, do: git commit / git push / git status
{
    git_add_modified

    sleep 2

    git commit "$@"

    git push

    git_log_show_last_commit

    git_status
}

git_commit_amend()                # - Add tracked changes, do: git commit --amend / git push --force / git status
{
    git add -u
    git status
    sleep 2
    git commit --amend --no-edit
    git push --force

    echo "
    >>> Commit has been completed. Last commit:
    "

    git_log_show_last_commit

    git_status

    echo
}

git_reset_hard()                  # - Do: git reset --hard / git status
{
    git reset --hard "$@"
    git_status
}

git_reset_soft()                  # - Do: git reset --soft / git status
{
    git reset --soft "$@"
    git_status
}

git_pull_and_status()             # - Do: git pull / git status
{
    git pull "$@"
    git_status
}

git_push_and_status()             # - Do: git push / git status
{
    git push "$@"
    git_status
}

git_log_short()                   # - Shows last 10 commits with a one line message option
{
    local linesCount=$1

    if [[ -z "$linesCount" ]]; then
        linesCount=10
    fi

    git log --oneline -$linesCount
}

git_log_show_last_commit()        # - Shows last commit
{
    git log -1
}

git_revert_file()
{
    git reset HEAD "$@"

    git checkout "$@"

    git_status
}

git_info()
{
    printf ">>> Origin URL: "
    git config --get remote.origin.url

    printf "\n>>> Additional info:\n"
    git remote show origin
}

git_undo_add()
{
    git reset $@
    git_status
}

git_undo_add_all_files()
{
    git reset HEAD -- .
    git_status
}

git_log_for_yocto_notes()
{
    local revision=$1

    if [ ! -z $revision ]; then
        # get only first 6 digits
        revision=$(cut -c 1-6 <<< $revision)

        # Get formatted git log
        git log --pretty=format:"%h %B" | grep -B 100000 $revision > tmp_whole.log.txt

        # Extract commit messages into separate file
        egrep '[a-z0-9]+ ' tmp_whole.log.txt > message.log.txt

        # Remove last line, to remove the commit message for passed $revision
        head -n-1 message.log.txt > message.log1.txt

        # Get jira tickets in separate file
        egrep 'Issue-ID:' tmp_whole.log.txt > jira_tickets.log.txt

        # Cut off 'Issue-ID: ' substring
        sed -i -E "s/Issue-ID: ([a-zA-Z0-9\S]+)/\1/g" jira_tickets.log.txt

        # Remove duplicates
        sort jira_tickets.log.txt | uniq > jira.log.txt

        # Add commits + comments into result file
        cat message.log1.txt > out.txt
        # Add new line
        echo "" >> out.txt
        # Add 'Issue-ID: ' line + replace new lines with  ", " string by sed from jira ticketsm so it will be one line (APRICOT-1234, APRICOT-12345, ...)
        # and append to the result file. The output line will be: Issue-ID: APRICOT-1234, APRICOT-12345, ...
        echo "Issue-ID: $(sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/, /g' jira.log.txt)" >> out.txt

        # Remove unneeded files
        rm tmp_whole.log.txt message.log.txt message.log1.txt jira_tickets.log.txt jira.log.txt

        # Print output, so it can be copied
        cat out.txt
        return 0
    fi

    git log --pretty=format:"%h %B"
    return 0
}

git_remove_remote_branches()
{
    if [ "$#" -lt 2 ]; then
        echo "Usage: git_remove_remote remoteName branchName"
        echo "Ex: remote branch name: remotes/origin/testFeature"
        echo "$git_remove_remote origin testFeature"
        return 1
    fi

    # origin
    local remoteName=$1
    local branches=( "$@" )

    for (( i=1; i<${#branches[@]}; i++ ));
    do
        local branchName=${branches[$i]}
        echo "Removing remote branch: $remoteName/$branchName"
        git push -d $remoteName $branchName
    done

    return $?
}

git_rebase_with_latest_main_branch()
{
    local mainBranchName=$1

    if [ -z $mainBranchName ]; then
        echo ">>> Main branch name can be provided as function argument. Default is: master"
        mainBranchName="master"
    fi

    local currentBranch=$(git rev-parse --abbrev-ref HEAD)

    # update master
    git checkout $mainBranchName
    git pull upstream $mainBranchName

    # currentBranch -> rebase master
    git checkout $currentBranch
    git rebase $mainBranchName
}

git_pull_upstream()
{
    local currentBranch=$(git rev-parse --abbrev-ref HEAD)
    git pull upstream $currentBranch

    git_status
    git_log_show_last_commit
}

  # Add git completion to aliases
if [ -f /usr/share/bash-completion/completions/git ]; then
  __git_complete gck _git_checkout
  __git_complete gpl _git_pull
  __git_complete grh _git_reset
  __git_complete grs _git_reset
  __git_complete gbd _git_branch
  __git_complete git_rebase_with_latest_main_branch _git_branch
fi
