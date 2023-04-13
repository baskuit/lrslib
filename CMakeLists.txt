cmake_minimum_required(VERSION 3.0)
project(lrsnash1)

set(CMAKE_C_STANDARD 99)

add_executable(lrsnash1 lrsnash.c lrsnashlib.c lrslib.c lrslong.c lrsdriver.c)
target_compile_definitions(lrsnash1 PRIVATE LRSLONG SAFE)
target_include_directories(lrsnash1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})