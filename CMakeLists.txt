cmake_minimum_required(VERSION 3.0)
project(lrsnash)

set (lrsnashlib_sources
    src/lib.c
    src/solve.c
    src/lrslib.c
    src/lrsgmp.c
    src/lrsdriver.c
)

add_library(lrsnashlib ${lrsnashlib_sources})

find_library(GMP_LIBRARY gmp)
target_link_libraries(lrsnashlib INTERFACE ${GMP_LIBRARY})

target_compile_definitions(lrsnashlib PUBLIC GMP LRS_QUIET SURSKIT MUTE)

# Executables

# add_executable(lrsnash src/lrsnash.c)
# target_link_libraries(lrsnash lrsnashlib)
