cmake_minimum_required(VERSION 3.12)
project(sqlite_unnacent_lower C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_EXTENSIONS ON)

option(ENABLE_TESTS "Enable tests" ON)
option(ENABLE_SANITIZERS "Enable sanitizers" OFF)

# add FORTIFY_SOURCE=3 to all targets
add_compile_options(-D_FORTIFY_SOURCE=3)

add_subdirectory(vendor/utf8proc)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    message(STATUS "Setting G++ flags")
    add_compile_options(-Wall -Wextra -Wformat-security -Wconversion -Wsign-conversion  -Wno-gnu -Wno-gnu-statement-expression)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    message(STATUS "Setting MSVC flags")
    add_compile_options(/W4 /WX)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    message(STATUS "Setting Clang flags")
    add_compile_options(-Weverything -Wno-unsafe-buffer-usage-in-libc-call -Wno-missing-variable-declarations -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-global-constructors -Wno-exit-time-destructors -Wno-weak-vtables -Wno-documentation -Wno-documentation-unknown-command)
endif()

if(ENABLE_TESTS)
    include(CTest)
    enable_testing()
    find_package(GTest)

    if(GTest_FOUND)
        include(GoogleTest)
    endif()
endif()

if(ENABLE_SANITIZERS)
    add_compile_options(-fno-omit-frame-pointer -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=pointer-compare -fsanitize=pointer-overflow -fsanitize=pointer-subtract -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -g)
    add_link_options(-fno-omit-frame-pointer -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=pointer-compare -fsanitize=pointer-overflow -fsanitize=pointer-subtract -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -g)

    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        message(STATUS "Setting Clang sanitize flags")
        add_compile_options(-fsanitize=implicit-conversion -fsanitize=unsigned-integer-overflow)
        add_link_options(-fsanitize=implicit-conversion -fsanitize=unsigned-integer-overflow)
    endif()
endif()

# Add include directory
include_directories(include)

# Function to filter out excluded files
function(filter_out excluded output)
    set(result "")
    foreach(file ${${output}})
        if(NOT "${file}" IN_LIST ${excluded})
            list(APPEND result ${file})
        endif()
    endforeach()
    set(${output} "${result}" PARENT_SCOPE)
endfunction()

# Add source files
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Exclude specific files
set(EXCLUDE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
filter_out(EXCLUDE_FILES SOURCES)

message(STATUS "Sources: ${SOURCES}")

add_library(sqlite_unnacent_lower_lib STATIC ${SOURCES})
target_link_libraries(sqlite_unnacent_lower_lib PUBLIC utf8proc)

# Add executable
add_library(unaccent_lower SHARED "src/main.cpp")
target_link_libraries(unaccent_lower sqlite_unnacent_lower_lib utf8proc)

if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()
