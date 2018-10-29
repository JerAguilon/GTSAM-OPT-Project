find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories (${SimpleParserFull_SOURCE_DIR}/src
                     ${Boost_INCLUDE_DIRS}
                     )
add_definitions (-DBOOST_TEST_DYN_LINK)

file(GLOB_RECURSE SOURCES "*.cc")
add_executable (Test ${SOURCES})

target_link_libraries (Test
                       simple-parser
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )
add_test (NAME ALL_UNIT_TESTS COMMAND Test)
