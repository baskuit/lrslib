cmake_minimum_required(VERSION 3.0)
project(lrsnash)

set(CMAKE_C_STANDARD 99)
set (CMAKE_CXX_STANDARD 20)

set (lrsnashlib128_sources
src/lib-128.c
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

add_library(lrsnashlib128 ${lrsnashlib128_sources})
add_library(lrsnashlibgmp ${lrsnashlibgmp_sources})

find_library(GMP_LIBRARY gmp)
target_link_libraries(lrsnashlibgmp INTERFACE ${GMP_LIBRARY})

# use the flag below to set data type for lrsnash.
# top is 128 bit arithmetic, bottom is mpz_t from the gmp library
target_compile_definitions(lrsnashlib128 PUBLIC B128 LRSLONG SAFE)
target_compile_definitions(lrsnashlibgmp PUBLIC GMP)

add_executable(main-gmp main-gmp.cc)
target_link_libraries(main-gmp lrsnashlibgmp)

add_executable(lrsnash lrsnash.c)
target_link_libraries(lrsnash lrsnashlibgmp)