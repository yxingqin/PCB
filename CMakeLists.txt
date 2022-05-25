cmake_minimum_required(VERSION 3.21.1)

project(PCB)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#SET(CMAKE_BUILD_TYPE "Release")
find_package(Qt5 REQUIRED COMPONENTS Quick Core Qml)

include_directories(source/Scheduler)
include_directories(source/Model)
include_directories(source/Simulator)
file(GLOB_RECURSE SRCS RELATIVE  ${CMAKE_CURRENT_SOURCE_DIR} LINK_DIRECTORIES false
     ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/.h
    )
LIST(APPEND SRCS source/qml/qml.qrc)


if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_executable(${PROJECT_NAME} )
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    add_executable(${PROJECT_NAME} WIN32)
endif()

target_sources(${PROJECT_NAME} PRIVATE ${SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
)


