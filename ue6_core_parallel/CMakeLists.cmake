cmake_minimum_required(VERSION 3.24)
project(UnrealEngine6_Core VERSION 6.0.0 LANGUAGES CXX)

# Enforce bleeding-edge C++ standards for 2028 architecture
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Gather Core Files
set(UE6_CORE_SOURCES
    src/memory/NeuralAssetCompressor.cpp
    src/scheduling/FiberTaskDispatcher.cpp
    src/MainLoop_UE6.cpp
)

# Build the executable
add_executable(UE6_Parallel_Core ${UE6_CORE_SOURCES})

# Aggressive compiler optimizations for lock-free parallel execution
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Configuring GCC/Clang with aggressive LTO and Fast Math...")
    target_compile_options(UE6_Parallel_Core PRIVATE 
        -O3 
        -march=native 
        -flto 
        -ffast-math 
        -pthread
    )
    target_link_libraries(UE6_Parallel_Core PRIVATE pthread)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(STATUS "Configuring MSVC for Multithreaded DLL and AVX extensions...")
    target_compile_options(UE6_Parallel_Core PRIVATE 
        /O2 
        /GL 
        /arch:AVX2 
        /std:c++20
    )
endif()

# Output message
message(STATUS "=======================================================")
message(STATUS " UE6 Asynchronous Core Build Configured Successfully")
message(STATUS "=======================================================")
