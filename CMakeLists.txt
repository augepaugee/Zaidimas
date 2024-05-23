cmake_minimum_required(VERSION 3.27)
project(grajus)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.1)
FetchContent_MakeAvailable(SFML)

set(CMAKE_CXX_STANDARD 17)

add_executable(grajus main.cpp
)

target_link_libraries(grajus sfml-graphics sfml-audio)