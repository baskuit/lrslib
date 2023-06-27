cmake_minimum_required(VERSION 3.0)
project(lrsnash)

set(CMAKE_C_STANDARD 99)
set (CMAKE_CXX_STANDARD 20)

set (lrsnashliblong_sources
src/lib-long.c
src/lrsnashlib.c
src/lrslib.c
src/lrslong.c
src/lrsdriver.c
)

set (lrsnashlibgmp_sources
src/lib-gmp.c
src/lrsnashlib.c
src/lrslib.c
src/lrsgmp.c
src/lrsdriver.c
)

add_library(lrsnashliblong ${lrsnashliblong_sources})
add_library(lrsnashlibgmp ${lrsnashlibgmp_sources})

target_include_directories(lrsnashliblong INTERFACE .)
target_include_directories(lrsnashlibgmp INTERFACE .)

find_library(GMP_LIBRARY gmp)
target_link_libraries(lrsnashlibgmp INTERFACE ${GMP_LIBRARY})

find_library(GMPXX_LIBRARY gmpxx)
target_link_libraries(lrsnashlibgmp INTERFACE ${GMPXX_LIBRARY})

# use the flag below to set data type for lrsnash.
# top is 128 bit arithmetic, bottom is mpz_t from the gmp library
target_compile_definitions(lrsnashliblong PUBLIC B128 LRSLONG SAFE LRS_QUIET)
target_compile_definitions(lrsnashlibgmp PUBLIC GMP LRS_QUIET)

# Executables

add_executable(main main.cc)
target_link_libraries(main lrsnashliblong)
target_link_libraries(main lrsnashlibgmp)

add_executable(main-long main-long.cc)
target_link_libraries(main-long lrsnashliblong)

add_executable(main-gmp1 main-gmp1.cc)
target_link_libraries(main-gmp1 lrsnashlibgmp)

add_executable(main-gmp2 main-gmp2.cc)
target_link_libraries(main-gmp2 lrsnashlibgmp)

add_executable(lrsnash lrsnash.c)
target_link_libraries(lrsnash lrsnashlibgmp)