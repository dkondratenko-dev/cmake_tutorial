Goal - I am actually learning C++ by myself but I lack knowledge of "dev environment". So most help will focus on environment.
(Targeting end of September 2025 to finish training - some time estimates are shown below)

Training pattern

Before training
	- Training documents
During training
	- Overview 10%
	- Why is it useful? How to use? Where to use? When to use? 20%
	- Hello World code example 40%
	- Coding practice 30%

VSCode(June/July)

- [ ] CMake (2 weeks)
	- [x] CMake/Google Testing
		- [x] Test instrumenting with chrono and rdtsc 
	- [ ] CMakePresets - introduction
- [ ] Debugging (3 weeks)	
	- [ ] Profiling (1 week)
		- [x] Valgrind
		- [ ] gprof
		- [ ] perf
		- [ ] If I have 10 chained method calls, 9 good, one bad, how can I find the bad?
	- [ ] Analyzing memory problems (2 weeks)
		- [x] AddressSanitizer? - compile with -fsanitize=address
		- [ ] GDB? - run program with gdb ./my_program and use info, memory, or watch commands to monitor memory
	- [x] Static code analysis
- [ ] Memory and architecture
	- [x] VTable
	- [x] vbase offset table
	- [x] Manual memory management (object lifetime, custom allocator)
        - [x] Sizeof class with virtual functions. Why? sizeof(A) give 1byte for struct A {}
        - [x] Structural alignment
	- [x] Cache locality, branch prediction
	- [ ] Play around with cache friendly data structures
	- [ ] Inline assembly (basic level)
	- [ ] Memory ordering
- [ ] Systems and network Programming
	- [ ] Wireshark basics
	- [ ] TCP/UDP, sockets
	- [ ] Network latency test
	- [ ] Multicast
	- [ ] Compile w/flags
	- [ ] Paging
	- [ ] Cache (OS/CPU)
	- [ ] Scheduling, CPU affinity
	- [ ] NUMA (Non-Uniform Memory Access)
	- [ ] Kernel bypass
- [ ] VSCode in Linux (2 weeks)
	- [ ] Debugging code running on a remote Linux server	
	- [ ] Debug a remote Docker container
	- [ ] Add simple steps with tasks.json file 
		- [ ] pre-build task to run before build - check dependency, setup environment
		- [ ] post build task to run after build - run tests, package artifacts, deply to unix server?(xcopy, run script, etc)
- [ ] AOP in c++ -- Aspect Oriented Programming (AOP) is not popular in c++. Only Java and C#

Docker(August - start to middle) 

- [ ] Docker with VSCode/c++ 
- [ ] Create simple docker container - windows
- [ ] Create simple docker container - Linux
- [ ] Debug docker container
- [ ] Debug remote docker container

Git/Github (August - middle to end)

- [ ] Using git from command line - fetch, pull, push, merge, rebase, cherry-pick, reset
- [ ] [https://learngitbranching.js.org/](https://learngitbranching.js.org/)

C++ packages (1 week)

- [ ] Creating C++ package 
	- [ ] with vcpkg  
	- [ ] with Conan2

Minor issues (1 day)

- [ ] How to fix VSCode SonarCube issue?
- [ ] Why is ECS pattern good for games but not good for financial market analysis? - with C++ examples

Leetcode (The rest of September)
 - [ ] I will learn this on my own, but I need you to play the role of an interviewer and ask random questions to test my skills
-----
CMake tools links
  - [ ] CMake Tools quick start - [https://code.visualstudio.com/docs/cpp/CMake-linux](https://code.visualstudio.com/docs/cpp/CMake-linux)
  - [ ] Configure and build a project with CMake Presets - [https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/cmake-presets.md](https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/cmake-presets.md)
  - [ ] Configure a project with kits and variants - [https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/how-to.md\#configure-a-project](https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/how-to.md#configure-a-project)
  - [ ] Build a project with kits and variants - [https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/how-to.md\#build-a-project](https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/how-to.md#build-a-project)
  - [ ] Configure and build a project using tasks - [https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/tasks.md](https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/tasks.md)
  - [ ] Debug a project - [https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/how-to.md\#debug-a-project](https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/how-to.md#debug-a-project)
  - [ ] Configure CMake Tools settings - [https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/cmake-settings.md](https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/cmake-settings.md)
  - [ ] How to - [https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/how-to.md](https://github.com/maxmitti/vscode-cmake-tools/blob/HEAD/docs/how-to.md)
  - [ ] FAQ
-----
Leetcode - practice
  - [ ] Arrays & Hashing (Prefix Sum, Two Pointers, Sliding window)
      - [ ] Vector - Remove element
      - [ ] Vector - Find Max Min
      - [ ] Vector - Find Longest String
      - [ ] Vector - Remove Duplicates\_SortedArray
      - [ ] Vector - Max Profit
      - [ ] Vector - Rotated
      - [ ]  Vector - Maximum Sub Array
      - [ ] H-Sliding Window-Minimum Window Substring
      - [ ] M-Greedy Kadane's-Gas Station
      - [ ] M-Greedy Kadane's-Jump Game
      - [ ] M-Greedy Kadane's-Jump Game II
      - [ ] M-Greedy Kadane's-Maximum Subarray (already covered)
      - [ ] M-Greedy Kadane's-Partition Labels
      - [ ] M-Hashing-Encode and Decode strings
      - [ ] M-Hashing-Valid Sudoku
      - [ ] M-Sliding Window-Permutation in a string
      - [ ] HasPairwithSum.cs 
      - [ ] Note: Two types of sliding windows - fixed and variable
  - [ ] Stacks and queues
      - [ ] Stack-Push for stack using vector
      - [ ] Stack-Pop for stack using vector
      - [ ] Stack-Reverse string
      - [ ] Stack-Parentheses balanced
      - [ ] Stack-Sort stack
      - [ ] Queue-Enqueue for queue using stack
      - [ ] Queue-Dequeue for queue using stack
      - [ ] Leetcode232 Queue using stacks
      - [ ] M-Stack-Car Fleet
      - [ ] M-Stack-Daily Temperature
      - [ ] M-Stack-Generate Parentheses
  - [ ] HashTables
      - [ ] HT.FindDuplicates
      - [ ] HT.FirstNonRepeatingCharacter
      - [ ] HT.GroupAnagrams
      - [ ] HT.ItemInCommon
      - [ ] HT.SubarraySum
      - [ ] HT.TwoSum
      - [ ] Set.FindPairs
      - [ ] Set.HasUniqueChars
      - [ ] Set.LongestConsecutiveSequence
      - [ ] Set.RemoveDuplicates
      - [ ] Subarray with given sum
      - [ ] SubarraySum-Continuous
  - [ ] Basic Sorts
      - [ ] Leetcode 2164
  - [ ] Merge Sort
      - [ ] Leetcode 87 Merge Two Sorted LL
  - [ ] Binary Search
      - [ ] M-Binary Search
      - [ ] M-Search a 2D Matrix
      - [ ] M-Koko Eating Bananas
      - [ ] M-Find Minimum in Rotated Sorted Arrays
      - [ ] M-Search in Rotated Sorted Arrays
      - [ ] M-Time Based Key Value Store
      - [ ] M-Median of Two Sorted Arrays
  - [ ] LinkedList
      - [ ] Find Middle node
      - [ ] Has Loop
      - [ ] Find Kth node from end
      - [ ] Partition list
      - [ ] Remove Duplicates
      - [ ] Binary to decimal
      - [ ] Reverse between
      - [ ] M-LRU Cache
      - [ ] Reverse linked list 2
      - [ ] Swap nodes
  - [ ] DLL
      - [ ] Swap first and last
      - [ ] Reverse
      - [ ] Palindrome
      - [ ] Swap nodes in pairs
  - [ ] Trees
      - [ ] E-Invert Binary Trees
      - [ ] E-Maximum Depth of Binary Tree
      - [ ] E-Diameter of Binary Tree
      - [ ] E-Balanced Binary Tree
      - [ ] M-Binary Tree Level Order Traversal
      - [ ] M-Binary Tree Right Side View
      - [ ] M-Validate BST
      - [ ] M-Kth smallest Element in a BST
      - [ ] M-Construct Binary Tree from Preorder and Inorder Traversal
      - [ ] H-Binary Tree Maximum Path Sum
      - [ ] Walls and Gates
  - [ ] Tries
      - [ ] M-Implement Trie Prefix Tree
      - [ ] M-Design Add and Search words data structure
  - [ ] Heap/Priority Queue
      - [ ] E-Kth Largest Element in a Stream
      - [ ] M-K Closest Points to Origin
      - [ ] M-Kth Largest Element in an Array
      - [ ] M-Task Scheduler
      - [ ] M-Design Twitter
      - [ ] Load Balancer (caching with hash map)
      - [ ] Load Sharer
  - [ ] Backtracking (prerequisites are Tree Maze, Subsets, Combinations, Permutations)
      - [ ] M-Subsets
      - [ ] M-Combination Sum
      - [ ] M-Combination Sum II
      - [ ] M-Permutations
  - [ ] Graph
      - [ ] M-Number of Islands
      - [ ] M-Max Area of Island
      - [ ] M-Clone Graph
      - [ ] M-Walls and Gates
      - [ ] M-Rotting Oranges
      - [ ] M-Course Schedule
      - [ ] M-Course Schedule II
      - [ ] M-Number of Connected Components in an Undirected Graph
      - [ ] M-Network Delay Time
      - [ ] M-Open the lock
  - [ ] Intervals
      - [ ] E-Meeting Rooms
      - [ ] M-Insert Intervals
      - [ ] M-Merge Intervals
      - [ ] M-Non Overlapping Intervals
      - [ ] M-Meeting Rooms II
  - [ ] Dynamic Programming (1-D)
      - [ ] M-House Robber
  - [ ] Bit Manipulation
      - [ ] E-Counting Bits
      - [ ] E-Reverse Bits
      - [ ] M-Sum of Two Integers
      - [ ] M-Reverse Integer
