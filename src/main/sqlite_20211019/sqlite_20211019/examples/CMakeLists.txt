cmake_minimum_required(VERSION 3.10)
set(prj_name SqliteTest)
project(${prj_name})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -pthread")


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} target_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../src target_src)
add_executable(${prj_name} ${target_src})
target_link_libraries(${prj_name} dl)
