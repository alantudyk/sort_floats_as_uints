#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define TIME_DIFF_NS   ((___t2.tv_nsec + ___t2.tv_sec * (int64_t)1e9) - \
                        (___t1.tv_nsec + ___t1.tv_sec * (int64_t)1e9))
#define TIME_DIFF_MS   (TIME_DIFF_NS / (int64_t)1e6)
#define TIME_DIFF_MCS  (TIME_DIFF_NS / (int64_t)1e3)

#define unlikely(x) __builtin_expect(!!(x), 0)
#define _(CND) \
    if (unlikely(CND)) { \
        fprintf(stderr, "\n\t🤔, line: %d\n\n", __LINE__); \
        exit(1); \
    }

int f_cmp(const void *const _a, const void *const _b) {
    const float *const a = _a, *const b = _b;
    return (*a > *b) - (*a < *b);
}

int u_cmp(const void *const _a, const void *const _b) {
    const uint32_t *const a = _a, *const b = _b;
    return (*a > *b) - (*a < *b);
}

#define MSB32 ((uint32_t)1 << 31)

static void sort_floats_as_uints(float *const f, const size_t n) {

    uint32_t *const a = (uint32_t *)f;

    for (size_t i = 0; i < n; i++)
        a[i] ^= ((int32_t)a[i] >> 31) | MSB32;

    qsort(a, n, sizeof(float), u_cmp);

    for (size_t i = 0; i < n; i++)
        a[i] ^= ((a[i] >> 31) - 1) | MSB32;
}

#define N (size_t)1e6
static float A[N], B[N];

int main(void) {

    int s = 1;
    for (size_t i = 0; i < N; i++)
        A[i] = B[i] = rand() * s, s = -s;

    {
        volatile int32_t ramp = 1e9; while (--ramp > 0);
    }

    struct timespec ___t1, ___t2;
    clock_gettime(CLOCK_REALTIME, &___t1);
    qsort(A, N, sizeof(float), f_cmp);
    clock_gettime(CLOCK_REALTIME, &___t2);
    const size_t ms = TIME_DIFF_MS;

    clock_gettime(CLOCK_REALTIME, &___t1);
    sort_floats_as_uints(B, N);
    clock_gettime(CLOCK_REALTIME, &___t2);

    _(memcmp(A, B, sizeof(A)))

    printf(
        "\n"
        "\tas floats: %8zu ms\n"
        "\tas  uints: %8zu ms\n"
        "\n"
    , ms, (size_t)TIME_DIFF_MS);

    return 0;
}
