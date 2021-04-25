cmake_minimum_required(VERSION 3.19)
project(HuffmanAlgorithm)
set(CMAKE_CXX_STANDARD 20)
find_package(Boost REQUIRED)
set(REQUIRED_LIBS Core)
set(REQUIRED_LIBS_QUALIFIED
        Qt5::Core
        )
set(QT_VERSION 5)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS ${REQUIRED_LIBS} REQUIRED)
if (EXISTS "$ENV{CLP_DIR}/googletest")
    set(GTEST_DIR "$ENV{CLP_DIR}/googletest")
    add_subdirectory(${GTEST_DIR} GTESTBUILD)
endif ()

set(${PROJECT_NAME}_INCLUDE_DIRS impl)

if (GTEST_DIR)
    add_executable(${PROJECT_NAME}_test tests.cpp)
    target_include_directories(${PROJECT_NAME}_test PUBLIC ${${PROJECT_NAME}_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME}_test PRIVATE
            gtest_main ${REQUIRED_LIBS_QUALIFIED})
    set_target_properties(${PROJECT_NAME}_test PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
endif ()