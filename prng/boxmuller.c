#include <math.h>

#include "mtwister.h"


double genrand_normal_distributed_double(void) {
    // Uses Box-Muller Transform, to get normal distributed random numbers from uniform distributed ones
    // 0xffffffff == 2 ** 32 -1 => press numbers into [0, 1] range
    double u1;
    const double u2 = (double) genrand_int32() / 0xffffffff;

    do {
        u1 = (double)genrand_int32() / 0xffffffff;
    } while (u1 == 0); // log is undefined for u1 so cant have that


    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}
