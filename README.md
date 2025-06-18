# C++ Development Examples and Documentation

This repository contains various examples and documentation related to C++ development, focusing on CMake, testing with Google Test, memory leak detection using tools like AddressSanitizer and Valgrind, and multi-target debugging.

## Table of Contents

- [C++ Development Examples and Documentation](#c-development-examples-and-documentation)
  - [Table of Contents](#table-of-contents)
  - [Repository Structure](#repository-structure)
    - [CMake Examples](#cmake-examples)
    - [Documents](#documents)
    - [Mind Maps](#mind-maps)
    - [Multi-Target Debugging](#multi-target-debugging)
  - [Topics Covered](#topics-covered)
    - [Memory Leak Detection](#memory-leak-detection)
    - [Multi-Target Debugging](#multi-target-debugging-1)
    - [Dependency Injection](#dependency-injection)
    - [Testing with Google Test](#testing-with-google-test)
  - [Getting Started](#getting-started)

## Repository Structure

This section provides a brief overview of the main directories in this repository to help you navigate.

### CMake Examples

A collection of examples demonstrating how to use CMake for various C++ project setups, including basic builds, multi-file projects, using external libraries, and integrating testing frameworks like Google Test.

- **[01-intro-cmake](cmake-examples/01-intro-cmake)**: Introduction to CMake with recommended literature.
- **[02-basic-hello](cmake-examples/02-basic-hello)**: Basic "Hello World" example.
- **[03-simple-subdirs](cmake-examples/03-simple-subdirs)**: Example with simple subdirectories.
- **[04-multifile-project](cmake-examples/04-multifile-project)**: Multi-file project example.
- **[05-using-libraries](cmake-examples/05-using-libraries)**: Example of using external libraries.
- **[06-google_test](cmake-examples/06-google_test)**: Example of using Google Test for unit testing.
- **[07-gtest-dependency-injection](cmake-examples/07-gtest-dependency-injection)**: Example demonstrating dependency injection techniques in C++.
- **[08-memory-leak-asan](cmake-examples/08-memory-leak-asan)**: Example tailored for detection using AddressSanitizer.
- **[09-top-continuous-memory-leak](cmake-examples/09-top-continuous-memory-leak)**: Example focusing on continuous memory growth scenarios.

### Documents

Contains various documentation, notes, and resources related to the project topics.

- **[CMake Cheat Sheet](documents/2025/cmake_cheat_sheet.md)**: A quick reference guide for CMake commands and configurations.
- **[Dependency Injection Documentation](documents/2025/07_deps_interface_injection)**: Documents explaining the differences between Dependency Injection (DI) and Dependency Inversion Principle (DIP).
- **[Memory Leaks Documentation](documents/2025/08_09_memory_leaks)**: Detailed documents on understanding, detecting, and preventing memory leaks.
- **[Agenda](documents/2025/agenda)**: Contains agendas for various meetings and sessions.
- **[Online Tools and AI Prompts](documents/2025/online_tools_ai_promts)**: Comparisons and approaches involving AI tools and prompts.
- **[Performance Profiling](documents/2025/performance_profiling)**: Documentation and images related to performance profiling techniques and tools.

### Mind Maps

- **[Linux Mind Map](documents/mindmaps/linux/linux.jpg)**: A mind map related to Linux commands and structure.
- **[Linux Commands](documents/mindmaps/linux/linux_commands.md)**: Documentation of Linux commands.

### Multi-Target Debugging

Examples and configurations illustrating how to set up and perform debugging for applications involving multiple processes or targets, such as client-server architectures.

- **[Multi-Target Debugging Examples](multi-target-debugging)**: Examples and configurations for debugging multi-process applications.

## Topics Covered

### Memory Leak Detection

Explore practical examples and documentation on detecting and fixing memory leaks in C++ applications.

- **Examples:**
  - [08 Memory Leak ASan](cmake-examples/08-memory-leak-asan)
  - [09 Continuous Memory Leak](cmake-examples/09-top-continuous-memory-leak)

### Multi-Target Debugging

Learn how to debug applications with multiple targets or processes.

- **Examples:**
  - [Multi-Target Debugging Examples](multi-target-debugging)

### Dependency Injection

Examples and documentation on implementing dependency injection in C++ projects to improve modularity and testability.

- **Examples:**
  - [Dependency Injection Examples](cmake-examples/07-gtest-dependency-injection)

### Testing with Google Test

Examples and guides on integrating and using Google Test for unit testing in C++ projects.

- **Examples:**
  - [Google Test Examples](cmake-examples/06-google_test)

## Getting Started

To get started with any of the examples or documentation, simply navigate to the relevant directory and explore the contents. Each directory contains its own set of examples, documentation, or resources to help you understand and implement the concepts covered.
