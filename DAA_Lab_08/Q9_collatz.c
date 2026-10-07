/* Q9: Collatz analysis. Modular design:
 *   collatz_next()   one step with unsigned 64-bit overflow guard
 *   trajectory()     builds full path in a malloc/realloc-grown array (pointers)
 *   analyse_range()  for every n in [a,b]: steps to reach 1, peak value; reports the
 *                    longest chain and highest peak without storing paths.
 * Usage: ./collatz            (built-in demo)   |  ./collatz n [a b]
 * Cost: O(L) per start value (L = stopping time); O((b-a+1) * avg L) for a range. */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned long long u64;

static int collatz_next(u64 n, u64 *out) {
    if (n % 2 == 0) { *out = n / 2; return 1; }
    if (n > (ULLONG_MAX - 1) / 3) return 0;          /* 3n+1 would overflow */
    *out = 3 * n + 1; return 1;
}
static u64 *trajectory(u64 n, size_t *len, int *overflow) {
    size_t cap = 16; u64 *t = malloc(cap * sizeof(u64)); *len = 0; *overflow = 0;
    t[(*len)++] = n;
    while (n != 1) {
        if (!collatz_next(n, &n)) { *overflow = 1; break; }
        if (*len == cap) { cap *= 2; t = realloc(t, cap * sizeof(u64)); }
        t[(*len)++] = n;
    }
    return t;
}
static void report_single(u64 n) {
    size_t len; int ov; u64 *t = trajectory(n, &len, &ov), peak = 0;
    for (size_t i = 0; i < len; i++) if (t[i] > peak) peak = t[i];
    printf("n=%llu: steps=%zu, peak=%llu%s\n  path: ", n, len - 1, peak, ov ? "  [OVERFLOW STOP]" : "");
    for (size_t i = 0; i < len && i < 30; i++) printf("%llu%s", t[i], i + 1 < len ? " -> " : "");
    if (len > 30) printf("... -> 1");
    printf("\n"); free(t);
}
static void analyse_range(u64 a, u64 b) {
    u64 bestN = a, bestSteps = 0, peakN = a, peakVal = 0, sum = 0; int ovs = 0;
    for (u64 n = a; n <= b; n++) {
        u64 x = n, steps = 0, peak = n; int ok = 1;
        while (x != 1) { if (!collatz_next(x, &x)) { ok = 0; ovs++; break; } steps++; if (x > peak) peak = x; }
        if (!ok) continue;
        sum += steps;
        if (steps > bestSteps) { bestSteps = steps; bestN = n; }
        if (peak > peakVal) { peakVal = peak; peakN = n; }
    }
    printf("[%llu,%llu]: longest chain n=%llu (%llu steps); highest peak %llu from n=%llu; avg steps %.2f; overflows=%d\n",
           a, b, bestN, bestSteps, peakVal, peakN, (double)sum / (b - a + 1), ovs);
}
int main(int argc, char **argv) {
    if (argc >= 2) {
        u64 n = strtoull(argv[1], 0, 10);
        if (n < 1) { fprintf(stderr, "n must be >= 1\n"); return 1; }
        report_single(n);
        if (argc >= 4) analyse_range(strtoull(argv[2], 0, 10), strtoull(argv[3], 0, 10));
        return 0;
    }
    report_single(27);              /* expect 111 steps, peak 9232 */
    report_single(1);
    analyse_range(1, 10);           /* expect n=9, 19 steps */
    analyse_range(1, 100000);       /* expect n=77031, 350 steps */
    return 0;
}
