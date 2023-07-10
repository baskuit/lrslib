cmake_minimum_required(VERSION 3.0)
project(lrsnash)

set (lrsnashlib_sources
    src/lrsnashlib.c
    src/lrslib.c
    src/lrsgmp.c
    src/lrsdriver.c
)

set (lrsnashlibsurskit_sources
    src/solve.c
    src/lrslib.c
    src/lrsgmp.c
    src/lrsdriver.c
)

set (lrsnashlibsurskitlong_sources
    src/solve.c
    src/lrslib.c
    src/lrslong.c
    src/lrsdriver.c
)


add_library(lrsnashlib    ${lrsnashlib_sources})
add_library(lrsnashlibsurskit ${lrsnashlibsurskit_sources})
add_library(lrsnashlibsurskitlong ${lrsnashlibsurskitlong_sources})

find_library(GMP_LIBRARY gmp)
target_link_libraries(lrsnashlib INTERFACE ${GMP_LIBRARY})
target_link_libraries(lrsnashlibsurskit INTERFACE ${GMP_LIBRARY})
target_link_libraries(lrsnashlibsurskitlong INTERFACE ${GMP_LIBRARY})

target_compile_definitions(lrsnashlib PUBLIC GMP LRS_QUIET)
target_compile_definitions(lrsnashlibsurskit PUBLIC GMP LRS_QUIET SURSKIT)
target_compile_definitions(lrsnashlibsurskitlong PUBLIC LRSLONG B128 LRS_QUIET)

# Executables

add_executable(lrsnash src/lrsnash.c)
target_link_libraries(lrsnash lrsnashlib)

add_executable(lrsnashsurskit src/lrsnashsurskit.c)
target_link_libraries(lrsnashsurskit lrsnashlibsurskit)

add_executable(lrsnashsurskitlong src/lrsnash.c)
target_link_libraries(lrsnashsurskitlong lrsnashlibsurskitlong)