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

int i_cmp(const void *const _a, const void *const _b) {
    const int32_t *const a = _a, *const b = _b;
    return (*a > *b) - (*a < *b);
}

_Static_assert(sizeof(float) == sizeof(int32_t), "");

static void sort_floats_as_ints(float *const f, const size_t n) {

    if (n < 2) return;

    int32_t *const a = (int32_t *)f;
    qsort(a, n, sizeof(float), i_cmp);

    if (*a >= 0) return;
    ssize_t l = 0, r = n - 1, last_neg;

    if (a[r] >= 0) {
        last_neg = 0;
        while (l < r) {
            ssize_t m = (l + r) / 2;
            if (a[m] < 0)
                last_neg = m, l = m + 1;
            else
                r = m - 1;
        }
        l = 0, r = last_neg;
    }

    if (unlikely(a[l] == a[r])) return;

    for (int32_t t; l < r; l++, r--)
        t = a[l], a[l] = a[r], a[r] = t;

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
    sort_floats_as_ints(B, N);
    clock_gettime(CLOCK_REALTIME, &___t2);

    _(memcmp(A, B, sizeof(A)))

    printf(
        "\n"
        "\tas floats: %8zu ms\n"
        "\tas   ints: %8zu ms\n"
        "\n"
    , ms, (size_t)TIME_DIFF_MS);

    return 0;
}
