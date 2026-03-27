
find_package(SQLite3 REQUIRED)


# Get the output path/binary.so from unaccent_lower target to UNACCENT_EXT_PATH
get_target_property(UNACCENT_EXT_PATH unaccent_lower LIBRARY_OUTPUT_NAME)
if(NOT UNACCENT_EXT_PATH)
    set(UNACCENT_EXT_PATH "${CMAKE_BINARY_DIR}/libunaccent_lower.so")
endif()

add_executable(sqlite_unaccent_lower_tests test.cpp test_sqlite_integration.cpp)
target_compile_definitions(sqlite_unaccent_lower_tests PRIVATE UNACCENT_EXT_PATH="${UNACCENT_EXT_PATH}")
target_include_directories(sqlite_unaccent_lower_tests SYSTEM PRIVATE ${SQLite3_INCLUDE_DIRS})
target_link_libraries(sqlite_unaccent_lower_tests sqlite_unaccent_lower_lib GTest::GTest GTest::Main SQLite::SQLite3)
gtest_discover_tests(sqlite_unaccent_lower_tests)
