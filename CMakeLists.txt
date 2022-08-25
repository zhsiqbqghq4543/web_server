cmake_minimum_required(VERSION 3.0.0)

project(web_server)

include_directories(${CMAKE_SOURCE_DIR}/include)

aux_source_directory(${CMAKE_SOURCE_DIR}/source  SRC)

add_executable(web_server main.cc ${SRC})

TARGET_LINK_LIBRARIES(web_server pthread)