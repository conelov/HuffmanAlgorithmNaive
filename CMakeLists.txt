cmake_minimum_required(VERSION 3.19)
project(HuffmanAlgorithm)
set(CMAKE_CXX_STANDARD 20)
find_package(Boost REQUIRED)
set(REQUIRED_LIBS Core)
set(REQUIRED_LIBS_QUALIFIED
        Qt5::Core
        #        Boost
        )
set(QT_VERSION 5)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS ${REQUIRED_LIBS} REQUIRED)
if (EXISTS "$ENV{CLP_DIR}/googletest")
    set(GTEST_DIR "$ENV{CLP_DIR}/googletest")
    add_subdirectory(${GTEST_DIR} GTESTBUILD)
endif ()

add_library(${PROJECT_NAME} STATIC
        impl/HuffmanAlgorithm.hpp impl/HuffmanAlgorithm.cpp
        )
target_link_libraries(${PROJECT_NAME} PUBLIC
        ${REQUIRED_LIBS_QUALIFIED}
        )
target_include_directories(${PROJECT_NAME} PUBLIC
        impl
        )
set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

if (GTEST_DIR)
    add_executable(${PROJECT_NAME}_test tests.cpp)
    target_link_libraries(${PROJECT_NAME}_test PRIVATE
            gtest_main ${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME}_test PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
endif ()