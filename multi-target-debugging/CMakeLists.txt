cmake_minimum_required(VERSION 3.10)
project(MultiTargetExample CXX) # Changed project name for clarity and specified CXX language

# Set build type to Debug. This automatically adds -g (debug info) and -O0 (no optimization) for GCC.
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")


# Define executables with their correct paths
add_executable(server server/src/server.cpp)
add_executable(client client/src/client.cpp)

# Set C++ standard to C++17 for both targets
target_compile_features(server PUBLIC cxx_std_17)
target_compile_features(client PUBLIC cxx_std_17)

# --- ВАЖЛИВО: Видаліть або закоментуйте цей блок ---
# if(WIN32)
#     set_target_properties(server PROPERTIES
#         COMPILE_FLAGS "/Zi"  # ЦЕ ДЛЯ MSVC - ВИДАЛИТИ
#         LINK_FLAGS "/DEBUG"  # ЦЕ ДЛЯ MSVC - ВИДАЛИТИ
#     )
#     set_target_properties(client PROPERTIES
#         COMPILE_FLAGS "/Zi"  # ЦЕ ДЛЯ MSVC - ВИДАЛИТИ
#         LINK_FLAGS "/DEBUG"  # ЦЕ ДЛЯ MSVC - ВИДАЛИТИ
#     )
# endif()
# --- Кінець блоку для видалення ---