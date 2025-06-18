# Monitoring a single process

Monitoring a single process is a common task for troubleshooting, resource management, or ensuring application uptime. The approach varies slightly depending on your operating system (Linux or Windows) and the level of detail you need.

Here's a breakdown of how to monitor a single process, along with common tools and techniques:

## On Linux

Linux offers a variety of command-line tools for process monitoring, from quick snapshots to continuous, detailed views.

### 1. `top` command

`top` is a real-time process viewer. To monitor a specific process, you'll need its Process ID (PID).

* **Find the PID:**

    ```bash
    pgrep <process_name>
    ```

    (e.g., `pgrep firefox`)
    or

    ```bash
    ps aux | grep <process_name>
    ```

    (e.g., `ps aux | grep nginx`)

* **Monitor with `top`:**

    ```bash
    top -p <PID>
    ```

    (e.g., `top -p 12345`)

    This will show only the details for the specified PID, refreshing periodically.

### 2. `htop` command (enhanced `top`)

`htop` is a more user-friendly and interactive version of `top`. If you don't have it, install it using your distribution's package manager (e.g., `sudo apt-get install htop` on Debian/Ubuntu, `sudo yum install htop` on CentOS/RHEL).

* **Monitor with `htop`:**

    ```bash
    htop -p <PID>
    ```

    (e.g., `htop -p 12345`)
    You can also run `htop` and then press `F4` to filter by process name or `F3` to search.

### 3. `ps` command (process status)

`ps` provides a snapshot of processes. It's not real-time, but it's excellent for getting specific information about a process.

* **Get detailed info for a PID:**

    ```bash
    ps -p <PID> -o %cpu,%mem,vsz,rss,cmd
    ```

    (This example shows CPU usage, memory usage, virtual size, resident set size, and the command.)

* **Get CPU usage by process name:**

    ```bash
    ps -C <process_name> -o %cpu
    ```

    (e.g., `ps -C rcu_sched -o %cpu`)

### 4. `pidstat` command (part of `sysstat` package)

`pidstat` is designed for per-process monitoring and provides more granular statistics than `top` or `ps`. You might need to install the `sysstat` package.

* **Monitor a specific PID continuously:**

    ```bash
    pidstat -p <PID> 1
    ```

    (This monitors PID every 1 second. Press Ctrl+C to stop.)

* **Monitor with specific metrics (e.g., CPU, memory, I/O):**

    ```bash
    pidstat -u -r -d -p <PID> 1
    ```

    (`-u` for CPU, `-r` for memory, `-d` for I/O)

### 5. `lsof` (list open files)

While not a direct performance monitor, `lsof` can show you all the files a specific process has open, including network connections. This is useful for debugging network issues or understanding what a process is interacting with.

* **List open files for a PID:**

    ```bash
    lsof -p <PID>
    ```

### 6. Scripting with `bash`

For continuous logging or custom alerts, you can combine these commands within a bash script.

**Example: Simple CPU/Memory logger for a process**

```bash
#!/bin/bash

PROCESS_NAME="your_process_name" # Or use PID directly: PID="12345"
INTERVAL=5 # Seconds between checks
LOG_FILE="process_monitor_${PROCESS_NAME}.log"

echo "Timestamp,PID,CPU_Usage,Memory_Usage,Command" > "$LOG_FILE"

while true; do
    # Get PID if monitoring by name
    if [ -z "$PID" ]; then
        PID=$(pgrep -o "$PROCESS_NAME")
        if [ -z "$PID" ]; then
            echo "$(date +%F_%T): Process $PROCESS_NAME not found. Retrying in $INTERVAL seconds."
            sleep "$INTERVAL"
            continue
        fi
    fi

    # Get CPU and Memory usage
    STATS=$(ps -p "$PID" -o %cpu,%mem --no-headers)
    CPU_USAGE=$(echo "$STATS" | awk '{print $1}')
    MEM_USAGE=$(echo "$STATS" | awk '{print $2}')
    CMD=$(ps -p "$PID" -o cmd --no-headers)

    TIMESTAMP=$(date +%F_%T)

    echo "${TIMESTAMP},${PID},${CPU_USAGE},${MEM_USAGE},${CMD}" >> "$LOG_FILE"
    sleep "$INTERVAL"
done
```

## On Windows

Windows offers its own set of tools, primarily GUI-based.

### 1. Task Manager

The quickest way to get a real-time overview.

* **Open Task Manager:** Press `Ctrl+Shift+Esc` or right-click on the taskbar and select "Task Manager."
* **Go to the "Details" tab:** Here you'll see a list of all running processes with their PIDs.
* **Sort by name or PID:** Find your desired process.
* **Right-click and select "Go to details" (if you were in "Processes" tab) or "Properties" or "End task"** to manage it. You can see CPU, Memory, Disk, and Network usage. You can also "Analyze wait chain" to see if a process is waiting on another.

### 2. Resource Monitor (`resmon.exe`)

Resource Monitor provides a more detailed, real-time view of system resources, including per-process statistics.

* **Open Resource Monitor:** Type `resmon` in the Start menu search bar and press Enter.
* **Select the desired tab:**
  * **CPU:** Shows CPU usage for individual processes. You can check the box next to a process to filter the graphs and modules below to only show data for that process.
  * **Memory:** Shows memory usage.
  * **Disk:** Shows disk activity.
  * **Network:** Shows network activity.
* **Filter by process:** In each tab, you can easily select a specific process to see its resource consumption in detail.

### 3. Performance Monitor (`perfmon.msc`)

Performance Monitor is a powerful tool for collecting and viewing performance data over time. It's more complex but highly customizable for long-term monitoring and creating performance logs.

* **Open Performance Monitor:** Type `perfmon.msc` in the Start menu search bar and press Enter.
* **Navigate to "Monitoring Tools" -> "Performance Monitor."**
* **Click the green '+' sign** to "Add Counters."
* **Select "Process"** from the available counters.
* **Choose specific performance counters** (e.g., `% Processor Time`, `Private Bytes`, `IO Data Bytes/sec`).
* **Select the specific instance of the process** you want to monitor from the "Instances of selected object" list.
* **Click "Add >>" and then "OK."**
    You'll see a real-time graph of the selected metrics for your specific process. You can also create "Data Collector Sets" to log this data to a file for later analysis.

### 4. Process Monitor (`procmon.exe`) - Sysinternals Suite

Process Monitor is an advanced utility from Microsoft Sysinternals that shows real-time file system, Registry, and process/thread activity. It's incredibly powerful for troubleshooting specific process behavior.

* **Download:** Get it from the official Microsoft Sysinternals website.
* **Run `procmon.exe`:** It will immediately start capturing events.
* **Set Filters:** This is crucial, as `procmon` generates a lot of data. Go to "Filter" -> "Filter..." (or `Ctrl+L`).
  * Add a filter: `Process Name` `is` `<your_process_name.exe>` `Include`
  * You can also filter by PID, Path, Operation, etc.
* **Start/Stop Capture:** Use the magnifying glass icon or `Ctrl+E` to start/stop capturing events.
* **Analyze events:** Observe the detailed activity (file access, registry changes, process/thread creation, network connections) of your chosen process.

## General Tips for Monitoring a Single Process

* **Identify the Process (PID or Name):** This is the first step for most monitoring tools. PIDs are unique for a running instance, but process names are more consistent if the PID changes (e.g., after a restart).
* **Define Your Goal:** What exactly are you trying to monitor? CPU usage? Memory leaks? Disk I/O? Network activity? Specific file access? This will guide your tool choice.
* **Real-time vs. Historical Data:** Do you need a live view, or do you want to collect data over time for analysis?
* **Alerting:** For critical processes, consider setting up alerts if certain thresholds are breached (e.g., high CPU for too long, excessive memory usage). Many enterprise monitoring solutions (like Nagios, Zabbix, Datadog) can do this, as can custom scripts.
* **Logging:** If you need historical data, ensure your chosen method logs the information to a file.
* **Overhead:** Be mindful of the monitoring tool's own resource consumption, especially for very fine-grained or continuous logging.

# Additional resources

## Monitoring tools

<https://sematext.com/blog/ubuntu-monitoring-tools/>
