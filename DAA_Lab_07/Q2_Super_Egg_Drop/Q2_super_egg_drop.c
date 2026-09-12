/*
 * Q2. Super egg testing experiment (generalized egg-drop problem)
 * -----------------------------------------------------------------
 * Given E identical eggs and an F-storey building, find the minimum
 * number of trial drops that GUARANTEES finding the critical floor
 * (the highest floor from which an egg can be dropped without
 * breaking), in the worst case, using an optimal adaptive strategy.
 *
 * DP FORMULATION (minimize worst-case drops)
 * -------------------------------------------
 * Let dp[e][f] = minimum number of drops needed, in the worst case,
 * to determine the critical floor with e eggs and f floors to test.
 *
 * If we drop an egg from some floor x (1 <= x <= f):
 *    - egg breaks   -> critical floor is below x, we have e-1 eggs and
 *                       x-1 floors left to test :  dp[e-1][x-1]
 *    - egg survives -> critical floor is at or above x, we have e eggs
 *                       and f-x floors left      :  dp[e][f-x]
 * That drop costs 1 + max(dp[e-1][x-1], dp[e][f-x]) in the worst case,
 * and we choose the best x:
 *
 *   dp[e][f] = 0                                   if f == 0
 *   dp[e][f] = f                                   if e == 1  (linear search)
 *   dp[e][f] = min over x in [1..f] of
 *                 1 + max(dp[e-1][x-1], dp[e][f-x])   otherwise
 *
 * This direct recurrence with a linear scan over x gives O(E*F^2).
 *
 * SPEED-UP (used here): for fixed e, dp[e][f-x] is non-increasing in x
 * while dp[e-1][x-1] is non-decreasing in x, so the optimal x can be
 * found with binary search on x for each (e,f), giving O(E*F*log F).
 * We implement the O(E*F*log F) version and also expose the plain
 * O(E*F^2) recurrence in a comment/alternate function for reference.
 *
 * DUAL / EQUIVALENT FORMULATION (used to answer "2 eggs, 100 floors"
 * quickly, and to cross-check the table): let f(d,e) = maximum number
 * of floors that can be resolved with d drops and e eggs.
 *      f(d,e) = f(d-1,e-1) + f(d-1,e) + 1 ,  f(0,e)=0 , f(d,0)=0
 * The answer to "min drops for E eggs, F floors" is the smallest d
 * with f(d,E) >= F. For E=2 this gives the classical closed form
 * d(d+1)/2 >= F  =>  d = ceil((sqrt(8F+1)-1)/2).
 * For E=2, F=100 this yields d = 14, matching dp[2][100] computed below.
 *
 * COMPLEXITY
 * ----------
 *   Binary-search DP used here : Time O(E*F*log F), Space O(E*F)
 *   (can be reduced to O(F) space by rolling on e)
 *   Naive recurrence          : Time O(E*F^2),      Space O(E*F)
 *   f(d,e) cross-check table  : Time O(D*E),         Space O(D*E)
 */

#include <stdio.h>
#include <stdlib.h>

/* dp[e][f] via binary search on the split floor x, O(E*F*log F) */
static long **egg_drop_dp(int E, int F) {
    long **dp = malloc((E + 1) * sizeof(long *));
    for (int e = 0; e <= E; e++) dp[e] = calloc(F + 1, sizeof(long));

    for (int f = 0; f <= F; f++) dp[1][f] = f;      /* 1 egg -> linear search */
    for (int e = 0; e <= E; e++) dp[e][0] = 0;       /* 0 floors -> 0 drops    */

    for (int e = 2; e <= E; e++) {
        for (int f = 1; f <= F; f++) {
            /* breakCase(x)=dp[e-1][x-1] is non-decreasing in x,
             * surviveCase(x)=dp[e][f-x] is non-increasing in x.
             * The worst-case cost max(breakCase,surviveCase) is
             * "unimodal" (first falls, then rises) as x grows, so we
             * binary search for the crossover point instead of
             * scanning all x in [1..f].                              */
            int lo = 1, hi = f;
            while (lo < hi) {
                int x = (lo + hi) / 2;
                long breakCase = dp[e - 1][x - 1];
                long surviveCase = dp[e][f - x];
                if (breakCase < surviveCase) lo = x + 1; else hi = x;
            }
            long b1 = dp[e - 1][lo - 1], s1 = dp[e][f - lo];
            long best = 1 + (b1 > s1 ? b1 : s1);
            if (lo > 1) { /* also check neighbour, crossover can be off by one */
                long b0 = dp[e - 1][lo - 2], s0 = dp[e][f - lo + 1];
                long cand = 1 + (b0 > s0 ? b0 : s0);
                if (cand < best) best = cand;
            }
            dp[e][f] = best;
        }
    }
    return dp;
}

/* Cross-check via maximum-floors-resolvable formulation, O(D*E) */
static int max_floors_min_drops(int E, int F) {
    /* find smallest d such that floors(d,E) >= F */
    long *prevRow = calloc(E + 1, sizeof(long)); /* floors(d-1, e) */
    long *curRow  = calloc(E + 1, sizeof(long));
    int d = 0;
    while (curRow[E] < F) {
        d++;
        curRow[0] = 0;
        for (int e = 1; e <= E; e++)
            curRow[e] = prevRow[e - 1] + prevRow[e] + 1;
        for (int e = 0; e <= E; e++) prevRow[e] = curRow[e];
        if (d > F + 5) break; /* safety */
    }
    free(prevRow); free(curRow);
    return d;
}

int main(int argc, char *argv[]) {
    int E = 2, F = 100;
    if (argc >= 3) { E = atoi(argv[1]); F = atoi(argv[2]); }
    else {
        printf("Enter number of eggs E and number of floors F: ");
        if (scanf("%d %d", &E, &F) != 2) return 1;
    }
    if (E <= 0 || F < 0) { printf("Invalid input.\n"); return 1; }

    long **dp = egg_drop_dp(E, F);
    printf("E = %d eggs, F = %d floors\n", E, F);
    printf("Minimum guaranteed drops (DP table)        : %ld\n", dp[E][F]);
    printf("Minimum guaranteed drops (cross-check)      : %d\n", max_floors_min_drops(E, F));

    printf("\ndp[e][f] table (rows = eggs 1..E, cols = floors 0..F, capped at F=20 for display):\n");
    int show = F < 20 ? F : 20;
    printf("e\\f");
    for (int f = 0; f <= show; f++) printf("%4d", f);
    printf("\n");
    for (int e = 1; e <= E; e++) {
        printf("%3d", e);
        for (int f = 0; f <= show; f++) printf("%4ld", dp[e][f]);
        printf("\n");
    }

    printf("\nSpecial case check: E=2 eggs, F=100 floors -> classic answer is 14 drops.\n");
    long **dp2 = egg_drop_dp(2, 100);
    printf("dp[2][100] = %ld\n", dp2[2][100]);

    for (int e = 0; e <= E; e++) free(dp[e]);
    free(dp);
    for (int e = 0; e <= 2; e++) free(dp2[e]);
    free(dp2);
    return 0;
}
