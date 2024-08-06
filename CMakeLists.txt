cmake_minimum_required(VERSION 3.26)
project(DSAGroup68)

set(CMAKE_CXX_STANDARD 17)

# Find the CURL package
find_package(CURL REQUIRED)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# Include the CURL headers
include_directories(${CURL_INCLUDE_DIRS})

add_executable(Project3Group68 main.cpp
        gui.h
        gui.cpp
        bridges-cxx-3.4.4-arm64-apple-darwin23.5.0/include/Bridges.h
        bridges-cxx-3.4.4-arm64-apple-darwin23.5.0/include/DataSource.h)

# Link the CURL library
target_link_libraries(Project3Group68 ${CURL_LIBRARIES})

target_link_libraries(Project3Group68 sfml-system sfml-window sfml-graphics sfml-audio sfml-network)


