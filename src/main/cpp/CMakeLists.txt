cmake_minimum_required(VERSION 3.10)
project(WaveformRetrieve)

set(CMAKE_CXX_STANDARD 14)

set(JAVA_HOME /opt/jdk1.8.0_301)

find_package(JNI REQUIRED)
#include_directories(${JNI_INCLUDE_DIRS})

add_executable(WaveformRetrieve main.cpp SQLiteUtils.cpp SQLiteUtils.h HBaseUtils.cpp HBaseUtils.h WaveSignalStruct.h)

include_directories(
        ${JAVA_HOME}/include
        ${JAVA_HOME}/include/linux
)


target_link_libraries(WaveformRetrieve sqlite3)
#target_link_libraries(WaveformRetrieve
#        ${JAVA_HOME}/include
#        ${JAVA_HOME}/include/linux
#)

target_link_libraries(WaveformRetrieve ${JNI_LIBRARIES})

#add_library(WaveformRetrieve SHARED )
