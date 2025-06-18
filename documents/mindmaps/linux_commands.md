```mermaid
mindmap
  root((**USEFUL LINUX COMMANDS**))

    **FILESYSTEM**
      .
            .
                **ls**: list files
                **cd**: change directory
                **mkdir**: create directory

                **rm**: remove files/dirs
                **cp**: copy files
                **mv**: move/rename files

                **touch**: create empty file
                **cat**: view file content

    **MONITORING**
      .
            .
                **top**: system processes
                **htop**: better top
                **free**: memory usage
                **df**: disk space
                **du**: dir disk usage

    **LOGS**
      .
            .
                **journalctl**: systemd logs
                **dmesg**: kernel logs
                **tail -f**: live log view
                **less**: view file content

    **USERS & PERMISSIONS**
      .
            .
                **whoami**: current user
                **su**: switch user
                **useradd**: add user
                **userdel**: remove user
                **chmod**: change permissions
                **chown**: change ownership
                **sudo**: elevated privileges

    **NETWORKING**
      .
            .
                **ping**: connectivity test
                **ip**: network tool
                **ifconfig**: interface config
                **curl**: transfer data
                **netstat**: network stats

    **PACKAGE MANAGEMENT**
      .
            .
                **apt**: Debian manager
                **dpkg**: Debian tool
                **yum**: RedHat manager
                **rpm**: RedHat tool

    **PROCESS MANAGEMENT**
      .
            .
                **ps**: process status
                **kill**: terminate process
                **bg**: background process
                **fg**: foreground process
