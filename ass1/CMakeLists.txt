cmake_minimum_required(VERSION 3.16)
project(assignment1)
set(CMAKE_CXX_STANDARD 14)
set(LLVM_HOME ~/llvm/llvm-project/build)
set(LLVM_DIR ${LLVM_HOME}/lib/cmake/llvm)
find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

MESSAGE(${LLVM_LIBRARY_DIRS})
MESSAGE(${LLVM_INCLUDE_DIRS})

add_subdirectory(FunctionInfo)
add_subdirectory(LocalOpt)