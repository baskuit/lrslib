#include "lrsdriver.h"
#include "lrslib.h"
#include "lrsnashlib.h"

#ifndef LIBLRSNASH
#define LIBLRSNASH

#ifdef __cplusplus
extern "C"
{
#endif

    extern void solve(int rows, int cols, int *row, int *col);

#ifdef __cplusplus
}
#endif

#endif // LIBLRSNASH