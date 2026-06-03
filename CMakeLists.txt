cmake_minimum_required(VERSION 3.20)

project(CodeForInterview LANGUAGES CXX)

add_executable(CodeForInterview
    main.cpp
)

target_compile_features(CodeForInterview PRIVATE cxx_std_17)

target_include_directories(CodeForInterview PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MSVC)
    target_compile_options(CodeForInterview PRIVATE /utf-8)
endif()
