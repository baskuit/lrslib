cmake_minimum_required(VERSION 3.0)
project(lrsnash1)

set(CMAKE_C_STANDARD 99)

set (SOURCES
src/lrsnash.c 
src/lrsnashlib.c 
src/lrslib.c 
src/lrslong.c 
src/lrsdriver.c
)

add_executable(lrsnash1 ${SOURCES})
target_compile_definitions(lrsnash1 PRIVATE LRSLONG SAFE)