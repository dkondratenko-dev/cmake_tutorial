hlp_file_name=$(basename ${BASH_SOURCE[0]})

# display human readable time from seconds
# Converts unix timestamp to days/hours/mins/secs
# usage:
# $ printf "Elapsed time: "
# $ dt_hu_time $elapsed
dt_hu_time()
{
  local inputSeconds=$1
  local DAYS=$((inputSeconds/60/60/24))
  local HOURS=$((inputSeconds/60/60%24))
  local MINUTES=$((inputSeconds/60%60))
  local SECONDS=$((inputSeconds%60))
  (( $DAYS > 0 )) && printf '%d days ' $DAYS
  (( $HOURS > 0 )) && printf '%d hours ' $HOURS
  (( $MINUTES > 0 )) && printf '%d minutes ' $MINUTES
  (( $DAYS > 0 || $HOURS > 0 || $MINUTES > 0 )) && printf 'and '
  printf '%d seconds\n' $SECONDS

  # To print current date time so it can figured out when the
  # command finished it execution in case it was not followed
  echo $(date)
}
