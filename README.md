# C++ Development Examples and Documentation

This repository contains various examples and documentation related to C++ development, focusing on CMake, testing with Google Test, memory leak detection using tools like AddressSanitizer and Valgrind, and multi-target debugging.

## Table of contents

- [C++ Development Examples and Documentation](#c-development-examples-and-documentation)
  - [Table of contents](#table-of-contents)
  - [Repository Structure](#repository-structure)
    - [CMake Cheat Sheet](#cmake-cheat-sheet)
    - [CMake Examples](#cmake-examples)
    - [Documents](#documents)
      - [Dependency Injection Documentation](#dependency-injection-documentation)
      - [Memory Leaks Documentation](#memory-leaks-documentation)
      - [Agenda](#agenda)
      - [Online Tools and AI Prompts](#online-tools-and-ai-prompts)
      - [Performance Profiling](#performance-profiling)
    - [Multi-Target Debugging](#multi-target-debugging)
  - [Topics Covered](#topics-covered)
    - [Memory Leak Detection](#memory-leak-detection)
    - [Multi-Target Debugging](#multi-target-debugging-1)
    - [Dependency Injection](#dependency-injection)
    - [Testing with Google Test](#testing-with-google-test)
  - [Getting Started](#getting-started)


## Repository Structure

This section provides a brief overview of the main directories in this repository to help you navigate.

### [CMake Cheat Sheet](documents/2025/cmake_cheat_sheet.md)

- **A quick reference guide for CMake commands and configurations.**

### [CMake Examples](cmake-examples)

A collection of examples demonstrating how to use CMake for various C++ project setups, including basic builds, multi-file projects, using external libraries, and integrating testing frameworks like Google Test.

### [Documents](documents)

Contains various documentation, notes, and resources related to the project topics.

#### [Dependency Injection Documentation](documents/2025/07_deps_interface_injection)

- Documents explaining the differences between Dependency Injection (DI) and Dependency Inversion Principle (DIP).
- Guides on interface injection techniques.

#### [Memory Leaks Documentation](documents/2025/08_09_memory_leaks)

Detailed documents on understanding, detecting, and preventing memory leaks using tools such as AddressSanitizer (ASan) and Valgrind. Includes notes on process monitoring.

#### [Agenda](documents/2025/agenda)

- Contains agendas for various meetings and sessions.

#### [Online Tools and AI Prompts](documents/2025/online_tools_ai_promts)

- Comparisons and approaches involving AI tools and prompts.
- Mind maps and related documentation for AI prompts.

#### [Performance Profiling](documents/2025/performance_profiling)

- Documentation and images related to performance profiling techniques and tools.

### [Multi-Target Debugging](multi-target-debugging)

Examples and configurations illustrating how to set up and perform debugging for applications involving multiple processes or targets, such as client-server architectures.

## Topics Covered

### Memory Leak Detection

Explore practical examples and documentation on detecting and fixing memory leaks in C++ applications.

- **[08 Memory Leak ASan](cmake-examples/08-memory-leak-asan)**: Examples tailored for detection using AddressSanitizer.
- **[09 Continuous Memory Leak](cmake-examples/09-top-continious-memory-leak)**: Examples focusing on scenarios with continuous memory growth, often detectable by monitoring tools like `top`.
- Related Documentation: [Memory Leaks Documentation](documents/2025/08_09_memory_leaks)

### Multi-Target Debugging

Learn how to debug applications with multiple targets or processes.

- **[Multi-Target Debugging Examples](multi-target-debugging)**: Examples and configurations for debugging multi-process applications.

### Dependency Injection

Examples and documentation on implementing dependency injection in C++ projects to improve modularity and testability.

- **[Dependency Injection Examples](cmake-examples/07-gtest-dependency-injection)**: Examples demonstrating dependency injection techniques in C++.

### Testing with Google Test

Examples and guides on integrating and using Google Test for unit testing in C++ projects.

- **[Google Test Examples](cmake-examples/06-google_test)**: Examples of setting up and writing tests using Google Test.

## Getting Started

To get started with any of the examples or documentation, simply navigate to the relevant directory and explore the contents. Each directory contains its own set of examples, documentation, or resources to help you understand and implement the concepts covered.
