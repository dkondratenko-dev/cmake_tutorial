# C++ Networking and Systems Programming

## Part 1: Networking Fundamentals (60 minutes)

### TCP/UDP and Sockets

**Overview:**
TCP and UDP are the two main protocols for network communication. Sockets are the programming interface that allows applications to send and receive data over the network.

**TCP (Transmission Control Protocol):**

- Reliable, connection-oriented protocol
- Guarantees data delivery and order
- Like making a phone call - you establish connection first, with sockets
- Examples: Web browsing (HTTP), email, file transfers
- Data is chunked into multiple packets. Packets have sequence numbers which are used in reordering them after arrival.

**UDP (User Datagram Protocol):**

- Unreliable, connectionless protocol
- Faster but no guarantee of delivery
- Like sending postcards - you send and hope they arrive
- Examples: Video streaming, online games, DNS lookups

**Sockets:**
Think of sockets as "telephone endpoints" for network communication. In C++, you create a socket, connect to another computer, and then read/write data just like reading/writing to a file.

**Why useful?**

- TCP: When you need reliable data transfer (banking, file downloads)
- UDP: When speed matters more than reliability (live video, gaming)

**When to use?**

- TCP: Web servers, databases, file transfers
- UDP: Real-time applications, broadcasting, gaming

---

The seven layers of the OSI model are:

Physical Layer (Layer 1): Deals with the physical transmission of raw bit streams over a physical medium. (e.g., cables, connectors, voltages).

Data Link Layer (Layer 2): Provides reliable data transfer between adjacent nodes, handles error detection and correction, and manages access to the physical medium. (e.g., MAC addresses, Ethernet).

Network Layer (Layer 3): Responsible for logical addressing (IP addresses) and routing packets across different networks. (e.g., IP, routers).

Transport Layer (Layer 4): Provides end-to-end communication, segmenting data, and ensuring reliable delivery. (e.g., TCP, UDP).

Session Layer (Layer 5): Establishes, manages, and terminates connections (sessions) between applications. (e.g., NetBIOS, RPC).

Presentation Layer (Layer 6): Translates data between the application layer and the network format, handling data encryption, decryption, and compression. (e.g., JPEG, MPEG, ASCII).

Application Layer (Layer 7): Provides network services directly to end-user applications. (e.g., HTTP, FTP, SMTP).

---


### Wireshark Basics

**Overview:**
Wireshark is a network protocol analyzer - it captures and displays network traffic in real-time. Think of it as a "network microscope" that lets you see exactly what data is flowing through your network.

**Why useful?**

- Debug network problems
- Analyze application performance
- Security analysis
- Learning how protocols work

**How it looks:**
Wireshark shows a three-panel interface:

- Top panel: List of captured packets
- Middle panel: Detailed breakdown of selected packet
- Bottom panel: Raw hexadecimal data

**Where to use?**

- Development and testing environments
- Network troubleshooting
- Performance optimization
- Security investigations

**When to use?**

- When network communication isn't working as expected
- When you need to understand protocol behavior
- When optimizing network performance

---

### Network Latency Testing

**Overview:**
Network latency is the time it takes for data to travel from source to destination. Testing latency helps identify performance bottlenecks in network applications.

**Why useful?**

- Identifies slow network connections
- Helps optimize application performance
- Critical for real-time applications

**Common tools:**

- `ping` - measures round-trip time to a host
- `traceroute` - shows path and latency to each hop
- Custom applications with timestamps

**Where to use?**

- High-frequency trading systems
- Online gaming
- Video conferencing
- Real-time data feeds

**When to test?**

- Before deploying latency-sensitive applications
- When users report slow performance
- During network optimization

---

### Multicast

**Overview:**
Multicast allows one sender to transmit data to multiple receivers simultaneously. Instead of sending individual copies to each recipient, you send one copy to a multicast group.

**Why useful?**

- Efficient bandwidth usage
- Reduces network congestion
- Perfect for broadcasting information to many clients

**How it works:**
Think of it like a radio station - the station broadcasts once, and anyone tuned to that frequency receives the signal.

**Where to use?**

- Financial market data feeds
- Video streaming to multiple viewers
- Software updates distribution
- Online gaming (game state updates)

**When to use?**

- When you need to send the same data to many recipients
- When bandwidth efficiency is important
- When real-time updates are needed for multiple clients

---

## Part 2: System Programming Concepts (30 minutes)

### Compile with Flags

**Overview:**
Compiler flags are options you pass to the compiler to control how your code is compiled. They affect optimization, debugging information, warnings, and standards compliance.

**Common flags:**

- `-O2` or `-O3`: Optimization levels
- `-g`: Include debugging information
- `-Wall`: Enable all warnings
- `-std=c++17`: Use C++17 standard

**Why useful?**

- Optimize performance for production
- Enable debugging for development
- Catch potential bugs with warnings
- Control language features

**Example:**

```bash
g++ -std=c++17 -O2 -Wall -g myprogram.cpp -o myprogram
```

---

### Paging

**Overview:**
Paging is how the operating system manages memory by dividing it into fixed-size blocks called pages. When programs need more memory than physically available, pages are swapped to disk.

**Why useful?**

- Allows programs to use more memory than physically available
- Provides memory protection between processes
- Enables virtual memory

**Where it matters:**

- Large applications that use lots of memory
- Systems with limited RAM
- Performance-critical applications

**When to care:**

- When your application uses large amounts of memory
- When you notice performance degradation due to swapping
- When optimizing memory usage

---

### Cache (OS/CPU)

**Overview:**
Caches are fast, small memory storage areas that keep frequently accessed data close to the processor. There are multiple levels (L1, L2, L3) with different speeds and sizes.

**Why useful?**

- Dramatically improves performance
- Reduces memory access latency
- Automatically managed by hardware

**How it affects programming:**

- Data locality matters - keep related data together
- Sequential access is faster than random access
- Cache-friendly algorithms perform much better

**Where to optimize:**

- High-performance computing
- Real-time systems
- Data-intensive applications

---

### Scheduling and CPU Affinity

**Overview:**
CPU scheduling determines which process runs on which CPU core and when. CPU affinity allows you to bind processes or threads to specific CPU cores.

**Why useful?**

- Better performance for multi-threaded applications
- Reduces cache misses when threads stay on same core
- Predictable performance for real-time applications

**When to use CPU affinity:**

- High-performance computing applications
- Real-time systems
- Applications with specific performance requirements

**Where it matters:**

- Trading systems
- Scientific computing
- Game engines

---

### NUMA (Non-Uniform Memory Access)

**Overview:**
NUMA is a computer memory design where memory access time depends on memory location relative to the processor. Some memory is "closer" (faster) to each CPU.

**Why it exists:**

- Allows scaling to many CPU cores
- Each CPU has fast access to local memory
- Shared access to remote memory (slower)

**Where it matters:**

- Multi-socket servers
- High-performance computing
- Large multi-threaded applications

**When to care:**

- When running on NUMA systems (most modern servers)
- When performance is critical
- When scaling across many CPU cores

---

### Kernel Bypass

**Overview:**
Kernel bypass techniques allow applications to communicate directly with network hardware, skipping the operating system kernel for better performance.

**Why useful?**

- Ultra-low latency network communication
- Higher throughput
- Reduced CPU overhead

**Technologies:**

- DPDK (Data Plane Development Kit)
- User-space networking
- SR-IOV

**Where used:**

- High-frequency trading
- Network function virtualization
- High-performance networking appliances

**When to consider:**

- When microsecond latency matters
- When standard networking is too slow
- When you need maximum network performance

About SR-IOV: In virtualization, single root input/output virtualization (SR-IOV) is a specification that allows the isolation of PCI Express resources for manageability and performance reasons.

A single physical PCI Express bus can be shared in a virtual environment using the SR-IOV specification.[1][2] The SR-IOV offers different virtual functions to different virtual components (e.g. network adapter) on a physical server machine. SR-IOV uses physical and virtual functions to control or configure PCIe devices. Physical functions have the ability to move data in and out of the device while virtual functions are lightweight PCIe functions that support data flowing but also have a restricted set of configuration resources. The virtual or physical functions available to the hypervisor or guest operating system depend on the PCIe device.[3]

The SR-IOV allows different virtual machines (VMs) in a virtual environment to share a single PCI Express hardware interface. In contrast, MR-IOV allows I/O PCI Express to share resources among different VMs on different physical machines.

---

## Part 3: Practical Examples Overview (30 minutes)

The following examples demonstrate inter-process communication and networking concepts using Boost libraries. Each example builds upon previous concepts and shows practical application of C++ networking and system programming.

### Example 1: Boost.Interprocess with Shared Memory

Demonstrates basic inter-process communication using shared memory segments.

### Example 2: Boost.Interprocess with Synchronization

Shows how to safely share data structures between processes using mutexes.

### Example 3: Boost.Asio Async Networking

Illustrates asynchronous network communication between processes on different machines.

---

## Key Takeaways

1. **Networking**: TCP for reliability, UDP for speed, sockets for programming interface
2. **Tools**: Wireshark for analysis, latency testing for performance
3. **System concepts**: Understanding how OS manages memory, CPU, and processes
4. **Performance**: Cache locality, CPU affinity, and kernel bypass for optimization
5. **Practical skills**: Boost libraries provide powerful tools for C++ system programming

This overview provides the foundation for deeper exploration into each topic as needed for specific projects and requirements.
