cmake_minimum_required(VERSION 3.0)
project(lrsnash)

# set(CMAKE_C_STANDARD 99)
# set (CMAKE_CXX_STANDARD 20)

set (lrsnashlib_sources
src/lrsnashlib.c
src/lrslib.c
src/lrsgmp.c
src/lrsdriver.c
)

set (lrsnashlibgmp_sources
src/lrsnashlib.c
src/lrslib.c
src/lrsgmp.c
src/lrsdriver.c
)

add_library(lrsnashlib    ${lrsnashlib_sources})
add_library(lrsnashlibgmp ${lrsnashlibgmp_sources})

# target_include_directories(lrsnashlibgmp INTERFACE .)

find_library(GMP_LIBRARY gmp)
target_link_libraries(lrsnashlib INTERFACE ${GMP_LIBRARY})
target_link_libraries(lrsnashlibgmp INTERFACE ${GMP_LIBRARY})

# find_library(GMPXX_LIBRARY gmpxx)
# target_link_libraries(lrsnashlibgmp INTERFACE ${GMPXX_LIBRARY})

target_compile_definitions(lrsnashlib    PUBLIC GMP LRS_QUIET)
target_compile_definitions(lrsnashlibgmp PUBLIC GMP LRS_QUIET SURSKIT)

# Executables

add_executable(lrsnash src/lrsnash.c)
target_link_libraries(lrsnash lrsnashlib)

add_executable(lrsnashgmp src/lrsnashgmp.c)
target_link_libraries(lrsnashgmp lrsnashlibgmp)