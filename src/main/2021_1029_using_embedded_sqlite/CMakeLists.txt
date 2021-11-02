
# HZH's SQLiteUtils

cmake_minimum_required(VERSION 3.10)

set(prj_name SqliteTest_HZH_2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -pthread")



project(${prj_name})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../sqlite_20211019/sqlite_20211019/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cpp)


# target_src:
    # aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}  target_src)
    set(target_src  "${target_src};${CMAKE_CURRENT_SOURCE_DIR}/SQLiteUtils_HZH.cpp")

    aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../sqlite_20211019/sqlite_20211019/src  target_src)

    # aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/../cpp  target_src)
    # set(target_src  "${target_src};${CMAKE_CURRENT_SOURCE_DIR}/../cpp/WaveSignalStruct.h")

message(STATUS  "target_src: ${target_src}")


# add_executable(${prj_name} ${target_src})
# target_link_libraries(${prj_name} dl pthread)

add_executable("main_HZH"  "${target_src};${CMAKE_CURRENT_SOURCE_DIR}/main_HZH.cpp")
target_link_libraries("main_HZH" dl pthread)

add_executable("main_2_HZH"  "${target_src};${CMAKE_CURRENT_SOURCE_DIR}/main_2_HZH.cpp")
target_link_libraries("main_2_HZH" dl pthread)
