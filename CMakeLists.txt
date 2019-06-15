cmake_minimum_required(VERSION 2.8)
project(TS_Parse_master)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-std=c++11)

include_directories(include)
include_directories(src)

add_executable(TS_Parse_master
        include/analysis.h
        include/pat.h
        include/pes.h
        include/pmt.h
        include/section.h
        include/tsheader.h
        src/analysis.cpp
        src/pat.cpp
        src/pes.cpp
        src/pmt.cpp
        src/section.cpp
        src/ts_parse.cpp
        src/tsheader.cpp include/PacketLength.h src/PacketLength.cpp include/PacketInfo.h src/PacketInfo.cpp include/FileSava.h src/FileSave.cpp)
