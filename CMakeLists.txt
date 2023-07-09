cmake_minimum_required(VERSION 3.0)
project(lrsnash)

# set(CMAKE_C_STANDARD 99)
# set (CMAKE_CXX_STANDARD 20)

set (lrsnashlibgmp_sources
src/lrsnashlib.c
src/lrslib.c
src/lrsgmp.c
src/lrsdriver.c
)

add_library(lrsnashlibgmp ${lrsnashlibgmp_sources})

# target_include_directories(lrsnashlibgmp INTERFACE .)

find_library(GMP_LIBRARY gmp)
target_link_libraries(lrsnashlibgmp INTERFACE ${GMP_LIBRARY})

# find_library(GMPXX_LIBRARY gmpxx)
# target_link_libraries(lrsnashlibgmp INTERFACE ${GMPXX_LIBRARY})

target_compile_definitions(lrsnashlibgmp PUBLIC GMP LRS_QUIET)

# Executables

add_executable(lrsnash src/lrsnash.c)
target_link_libraries(lrsnash lrsnashlibgmp)