cmake_minimum_required(VERSION 3.19)
project(HuffmanAlgorithm)
set(CMAKE_CXX_STANDARD 20)
find_package(Boost REQUIRED )

add_executable(${PROJECT_NAME}_main main.cpp)
target_include_directories(${PROJECT_NAME}_main PUBLIC ${Boost_INCLUDE_DIRS})
set_target_properties(${PROJECT_NAME}_main PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")