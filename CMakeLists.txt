cmake_minimum_required(VERSION 3.4.3)
project(SimpleProject C CXX)

find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${SimpleProject_SOURCE_DIR})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")


include_directories(ast kaleidoscope lexer logger parser utils)

file(GLOB_RECURSE SOURCES "*.cc")
set(SOURCE_FILES main.cc ${SOURCES})
add_executable(simple-parser ${SOURCES})

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "r-dynamic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -g")


# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(
    LLVM_LIBS
    Core
    ExecutionEngine
    Object
    OrcJIT
    Support
    native
)

message(STATUS "LLVM_LIBS = ${LLVM_LIBS}")

# Link against LLVM libraries
target_link_libraries(
    simple-parser
    ${LLVM_LIBS}
    ${LLVM_AVAILABLE_LIBS}
    ${CMAKE_LIBRARY_LINK_C_FLAGS}
)
