cmake_minimum_required(VERSION 3.21.1)

project(PCB)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
find_package(Qt5 REQUIRED COMPONENTS Quick Core Qml)

include_directories(source/Scheduler)
include_directories(source/Model)
file(GLOB_RECURSE SRCS RELATIVE  ${CMAKE_CURRENT_SOURCE_DIR} LINK_DIRECTORIES false
     ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/source/.h
    )
LIST(APPEND SRCS source/qml/qml.qrc)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
)


