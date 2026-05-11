cmake_minimum_required(VERSION 3.15)

project(MPES LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_ASAN "Build with AddressSanitizer" OFF)
option(ENABLE_TSAN "Build with ThreadSanitizer" OFF)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
    include/
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

if(ENABLE_TSAN)
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
endif()

find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy;-checks=modernize-*,performance-*,readability-*,bugprone-*)
endif()

if(BUILD_TESTS)
    message(WARNING "The option BUILD_TESTS is enabled but no test have been implemented yet...")
    #enable_testing()
    #add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    message(WARNING "The option BUILD_EXAMPLES is enabled but no examples have been implemented yet...")
    #add_subdirectory(examples)
endif()
