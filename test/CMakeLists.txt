find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
find_package (LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${SimpleProject_SOURCE_DIR})

include_directories(
    ${SimpleParserFull_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS}
)
add_definitions (-DBOOST_TEST_DYN_LINK)

file(GLOB_RECURSE SOURCES "*.cc")
add_executable (Test ${SOURCES})

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "r-dynamic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -g")

llvm_map_components_to_libnames(
    LLVM_LIBS
    Core
    ExecutionEngine
    Object
    OrcJIT
    Support
    native
)

target_link_libraries (Test
                       simple-parser
                       ${LLVM_LIBS}
                       ${LLVM_AVAILABLE_LIBS}
                       ${CMAKE_LIBRARY_LINK_C_FLAGS}
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )
add_test (NAME ALL_UNIT_TESTS COMMAND Test)
