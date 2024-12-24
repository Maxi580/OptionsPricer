#include <stdint.h>

#define STANDARD_RANDOM_SEED 5489UL
#define N 624 // Size of State
#define M 397
#define MATRIX_A 0x9908b0dfUL
#define UPPER_MASK 0x80000000UL
#define LOWER_MASK 0x7fffffffUL
#define TEMPERING_MASK_A 0x9d2c5680
#define TEMPERING_MASK_B 0xefc60000

static uint32_t state[N];
static int ctr = N + 1;

void init_genrand(const uint32_t seed) {
    // First element is seed, go through entire state and calc new number based on the number before
    state[0] = seed;
    for (ctr = 1; ctr < N; ctr++) {
        state[ctr] = (1812433253UL * (state[ctr-1] ^ (state[ctr-1] >> 30)) + ctr);
    }
}

static void twist(void) {
    // Refreshes the state and resets ctr
    int i;
    uint32_t y;
    static const uint32_t mag01[2] = {0x0UL, MATRIX_A};

    for (i = 0; i < N - M; i++) {
        y = (state[i] & UPPER_MASK) | (state[i+1] & LOWER_MASK);
        state[i] = state[i+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    for (; i < N-1; i++) {
        y = (state[i] & UPPER_MASK) | (state[i+1] & LOWER_MASK);
        state[i] = state[i+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    y = (state[N-1] & UPPER_MASK) | (state[0] & LOWER_MASK);
    state[N-1] = state[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

    ctr = 0;
}

uint32_t genrand_int32(void) {
    uint32_t y;

    if (ctr >= N) {
        // If ctr is bigger than N (size of state), we have used all numbers and need to
        if (ctr == N+1) {
            // Initialize if First time
            init_genrand(STANDARD_RANDOM_SEED);
        }
        // Refresh state
        twist();
    }

    y = state[ctr++];

    y ^= (y >> 11);
    y ^= (y << 7) & TEMPERING_MASK_A;
    y ^= (y << 15) & TEMPERING_MASK_B;
    y ^= (y >> 18);

    return y;
}
