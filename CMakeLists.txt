cmake_minimum_required(VERSION 3.20)
project(APT_INFO)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_CPR_TESTS OFF)
set(CMAKE_USE_OPENSSL OFF)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}})

#SFML SECTION
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)


add_executable(APT_INFO main.cpp APT_control.h apt_realtime_status_parcing.h sounds.h)
add_executable(Probe probe.cpp)

#CPR SECTION
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 21f42cf882d0b7e5ae9e3434574fc47e187728de) # The commit hash for 1.8.0. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)
target_link_libraries(APT_INFO PRIVATE cpr::cpr sfml-audio sfml-graphics sfml-network sfml-system sfml-window)
target_link_libraries(Probe PRIVATE cpr::cpr sfml-audio sfml-graphics sfml-network sfml-system sfml-window)


#SFML SECTION
find_package(SFML COMPONENTS REQUIRED audio graphics network system window )
include_directories(C:/SFML/include)




