cmake_minimum_required(VERSION 3.4.3)
project(SimpleProject)

find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Now build our tools
add_executable(simple-parser main.cc KaleidoscopeJIT.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti")

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
target_link_libraries(simple-parser ${LLVM_LIBS} ${LLVM_AVAILABLE_LIBS})
