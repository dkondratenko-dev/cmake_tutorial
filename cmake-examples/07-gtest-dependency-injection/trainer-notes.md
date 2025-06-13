Run tests:
ctest --verbose

or

cmake --build . --target run_tests


Troubleshooting:

1. /home/dkondratenko/Documents/uw/Femi/cmake_tutorial-main/cmake-examples/07-gtest-dependency-injection/src/**app/user_processor.h:6:10**: fatal error: infra/ilogger.h: No such file or directory
    6 | #include "infra/ilogger.h" // Include the ILogger interface
      |          ^~~~~~~~~~~~~~~~~
Solution:
GTestMockDIProject/src/**app/CMakeLists.txt:**

target_include_directories(ApplicationLibrary PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

Replace with:

target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} **${CMAKE_SOURCE_DIR}/src**)

--------------------------------------------------------------------------------------------------------------------------------------------

2. Linking issue:

[ 77%] Linking CXX executable my_application
/usr/bin/ld: cannot find -lInfraLib: No such file or directory
collect2: error: ld returned 1 exit status
gmake[2]: *** [src/CMakeFiles/my_application.dir/build.make:99: src/my_application] Error 1
gmake[1]: *** [CMakeFiles/Makefile2:398: src/CMakeFiles/my_application.dir/all] Error 2
gmake: *** [Makefile:146: all] Error 2

Solution:

I renamed InfraLib to InfrastructureLibrary, but I haven't updated all the CMake files that use it.
Search for InfraLib through the project

