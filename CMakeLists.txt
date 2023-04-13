cmake_minimum_required(VERSION 3.0)
project(lrsnash)

set(CMAKE_C_STANDARD 99)
set (CMAKE_CXX_STANDARD 20)

set (lrsnashlib_sources
src/lrsnashlib.c 
src/lrslib.c 
src/lrslong.c 
src/lrsdriver.c
)

add_library(lrsnashlib ${lrsnashlib_sources})

add_executable(main main.cc)
target_link_libraries(main lrsnashlib)

add_executable(lrsnash lrsnash.c)
target_link_libraries(lrsnash lrsnashlib)