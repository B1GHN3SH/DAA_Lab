/*
 * Q3. Reve's puzzle: n disks, 4 pegs
 * -----------------------------------
 * Classic Tower of Hanoi with an EXTRA peg (4 pegs total: source,
 * two auxiliary pegs, destination). With 8 disks the optimal solution
 * uses exactly 33 moves; we generalize with the Frame-Stewart algorithm.
 *
 * FRAME-STEWART ALGORITHM
 * ------------------------
 * To move n disks using 4 pegs:
 *   1. Choose a split point k (1 <= k <= n).
 *   2. Move the top k disks from source to a spare peg, using ALL 4
 *      pegs (recursively, this is again a 4-peg sub-problem)  -> FS(k)
 *   3. Move the remaining n-k (larger) disks from source to
 *      destination using only 3 pegs (the 4th peg is occupied by the
 *      k smaller disks and cannot be used) -> classical Hanoi, 2^(n-k)-1
 *   4. Move the k disks from the spare peg to destination using all
 *      4 pegs again (recursively) -> FS(k)
 *
 *   FS(n) = min over k in [1..n] of  2*FS(k) + (2^(n-k) - 1)
 *   FS(0) = 0
 *
 * This recurrence (the Frame-Stewart numbers) is CONJECTURED optimal
 * for all n and has been PROVEN optimal for 4 pegs (Bousch, 2014).
 * For n = 8 it gives FS(8) = 33, matching "Reve's puzzle".
 *
 * The optimal k for n disks is the one satisfying
 *      k(k+1)/2 >= n   i.e.  k = smallest integer with a triangular
 * number T(k) = k(k+1)/2 no smaller than n; this is the well-known
 * closed-form shortcut, but the program finds k by DP / direct search
 * to make no assumption and to double check.
 *
 * COMPLEXITY
 * ----------
 * DP over n and, for each n, a scan over k in [1..n]        -> O(n^2)
 * (each candidate cost O(1) with pre-computed 2^(n-k))
 * Space: O(n) for the FS[] table (plus O(n) for split[] choices)
 * Move generation / printing (for small n): O(2^ceil(n/2)) moves are
 * produced for the pure-Hanoi sub-calls, dominated by the final
 * 3-peg leg.
 */

#include <stdio.h>
#include <stdlib.h>

static long long pow2(int e) { long long r = 1; while (e-- > 0) r *= 2; return r; }

/* Classical 3-peg Hanoi move-printer (used for the "big disks" leg) */
static long moveCount3peg = 0;
static void hanoi3(int n, char from, char via, char to, int print) {
    if (n == 0) return;
    hanoi3(n - 1, from, to, via, print);
    moveCount3peg++;
    if (print) printf("  Move disk %d: %c -> %c   [3-peg leg]\n", n, from, to);
    hanoi3(n - 1, via, from, to, print);
}

static long movePrintCount = 0;
/* 4-peg Frame-Stewart move-printer.
 * pegs: p[0]=source p[1]=aux1 p[2]=aux2 p[3]=dest (labels only) */
static void frameStewart(int n, char src, char a1, char a2, char dst,
                          int *splitK, int print) {
    if (n == 0) return;
    if (n == 1) {
        movePrintCount++;
        if (print) printf("  Move disk 1: %c -> %c   [4-peg]\n", src, dst);
        return;
    }
    int k = splitK[n];
    frameStewart(k, src, a2, dst, a1, splitK, print);   /* top k disks: src->a1 using all 4 pegs (a2,dst as spares) */
    hanoi3(n - k, src, a2, dst, print);                  /* n-k big disks: src->dst using 3 pegs (a1 unusable) */
    movePrintCount += 0; /* counted inside hanoi3 via moveCount3peg */
    frameStewart(k, a1, src, a2, dst, splitK, print);    /* k disks: a1->dst using all 4 pegs */
}

int main(int argc, char *argv[]) {
    int N = 8;
    if (argc > 1) N = atoi(argv[1]);
    else {
        printf("Enter number of disks n: ");
        if (scanf("%d", &N) != 1) return 1;
    }
    if (N < 0) { printf("Invalid n\n"); return 1; }

    long long *FS = malloc((N + 1) * sizeof(long long));
    int *split = malloc((N + 1) * sizeof(int));
    FS[0] = 0; split[0] = 0;

    FS[1] = 1; split[1] = 1; /* base case: one disk, one move, no split needed */
    for (int n = 2; n <= N; n++) {
        /* k must be strictly less than n: k disks go to the 4-peg
         * sub-problem, the remaining n-k >= 1 disks use only 3 pegs. */
        long long best = -1; int bestK = 1;
        for (int k = 1; k <= n - 1; k++) {
            long long cost = 2 * FS[k] + (pow2(n - k) - 1);
            if (best == -1 || cost < best) { best = cost; bestK = k; }
        }
        FS[n] = best;
        split[n] = bestK;
    }

    printf("Frame-Stewart minimum move counts, n = 1..%d:\n", N);
    for (int n = 1; n <= N; n++)
        printf("  n=%2d disks -> FS(n)=%4lld moves  (split k=%d)\n", n, FS[n], split[n]);

    printf("\nSpecial case check: n=8 (Reve's puzzle) -> expect 33 moves.\n");
    printf("Computed FS(8) = %lld\n", FS[8 <= N ? 8 : N]);

    if (N <= 10) {
        printf("\nFull move sequence for n=%d disks on 4 pegs (A,B,C,D), A->D:\n", N);
        moveCount3peg = 0; movePrintCount = 0;
        frameStewart(N, 'A', 'B', 'C', 'D', split, 1);
        printf("Total moves printed = %ld (matches FS(%d)=%lld: %s)\n",
               movePrintCount + moveCount3peg, N, FS[N],
               (movePrintCount + moveCount3peg == FS[N]) ? "yes" : "no");
    } else {
        printf("\n(move sequence printing skipped for n>10; grows exponentially)\n");
    }

    free(FS); free(split);
    return 0;
}
