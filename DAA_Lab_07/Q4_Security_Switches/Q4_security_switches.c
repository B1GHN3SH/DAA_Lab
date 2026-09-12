/*
 * Q4. Security switches
 * ----------------------
 * n switches numbered 1 (leftmost) .. n (rightmost). Rules:
 *   (i)   switch n (rightmost) may be toggled freely.
 *   (ii)  switch i (i < n) may be toggled only if switch i+1 is ON and
 *         all switches to the right of i+1 are OFF.
 *   (iii) one toggle per move.
 * All switches start ON; turn them all OFF in the minimum number of
 * moves.
 *
 * ALGORITHM
 * ---------
 * Only switch n (rightmost) may be toggled unconditionally; every
 * other switch i needs switch i+1 ON with everything to the right of
 * i+1 OFF before it can be toggled. This is the same legality
 * structure as the classical "Chinese Rings" (Baguenaudier) puzzle,
 * and it obeys the recurrence, in terms of T(n) = minimum moves to
 * clear n switches that start all ON:
 *
 *      T(0) = 0,  T(1) = 1
 *      T(n) = 2*T(n-1) + (n mod 2)   for n >= 1   ... (*)
 *      (add 1 only when n is odd)
 *
 * whose closed form is
 *
 *      T(n) = floor( 2^(n+1) / 3 )
 *
 * (exactly (2^(n+1)-1)/3 for odd n, (2^(n+1)-2)/3 for even n).
 * This grows like 2^n, i.e. it is exponential, but with a smaller
 * constant than the naive "double every step" (2^n - 1) guess for
 * ordinary Tower-of-Hanoi -- an easy trap, since turning switch n off
 * only needs the (n-1)-switch prefix cleared once (not twice), so the
 * "+1" in (*) is not unconditional the way Hanoi's is.
 *
 * We VERIFY the closed form by exhaustive breadth-first search over
 * the state space {0,1}^n (state = ON/OFF pattern of all n switches)
 * for small n, finding the TRUE shortest path from "all ON" to
 * "all OFF" under the legality rules, and comparing it against
 * floor(2^(n+1)/3).
 *
 * COMPLEXITY
 * ----------
 *  Formula:                         O(1) per query (with fast pow),
 *                                     O(n) to print the actual n moves.
 *  BFS validation (used for n<=16
 *  only, to confirm optimality):     O(2^n * n) time, O(2^n) space
 *  (2^n states, O(n) legal moves generated per state)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Return 1 if switch idx (1-indexed, idx from 1 to n) can be toggled
 * in state (bit (i-1) = 1 means switch i is ON), for n total switches. */
static int can_toggle(int state, int n, int idx) {
    if (idx == n) return 1; /* rightmost: always allowed */
    /* switch idx+1 must be ON, and all switches to its right (idx+2..n) must be OFF */
    if (!((state >> idx) & 1)) return 0; /* switch idx+1 is bit index idx */
    for (int j = idx + 2; j <= n; j++)
        if ((state >> (j - 1)) & 1) return 0;
    return 1;
}

static long bfs_min_moves(int n, int print_path) {
    int size = 1 << n;
    int *dist = malloc(sizeof(int) * size);
    int *parent = malloc(sizeof(int) * size);
    int *movedIdx = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) { dist[i] = -1; parent[i] = -1; }
    int *queue = malloc(sizeof(int) * size);
    int qh = 0, qt = 0;
    int start = size - 1; /* all ON */
    int target = 0;        /* all OFF */
    dist[start] = 0;
    queue[qt++] = start;
    while (qh < qt) {
        int s = queue[qh++];
        if (s == target) break;
        for (int idx = 1; idx <= n; idx++) {
            if (can_toggle(s, n, idx)) {
                int ns = s ^ (1 << (idx - 1));
                if (dist[ns] == -1) {
                    dist[ns] = dist[s] + 1;
                    parent[ns] = s;
                    movedIdx[ns] = idx;
                    queue[qt++] = ns;
                }
            }
        }
    }
    long result = dist[target];

    if (print_path && result >= 0) {
        int *path = malloc(sizeof(int) * (result + 1));
        int cur = target, k = (int)result;
        while (cur != start) { path[k--] = movedIdx[cur]; cur = parent[cur]; }
        printf("  Shortest legal move sequence (switch toggled at each step):\n   ");
        for (int i = 1; i <= result; i++) printf(" %d", path[i]);
        printf("\n");
        free(path);
    }

    free(dist); free(parent); free(movedIdx); free(queue);
    return result;
}

static long long pow2ll(int e) { long long r = 1; while (e-- > 0) r *= 2; return r; }

/* T(n) = floor(2^(n+1)/3), computed via the DP T(n)=2*T(n-1)+(n mod 2)
 * (avoids relying purely on the closed form, doubles as an
 * independent check). */
static long long switches_moves_dp(int n) {
    long long *T = malloc((n + 1) * sizeof(long long));
    T[0] = 0;
    for (int i = 1; i <= n; i++) T[i] = 2 * T[i - 1] + (i % 2);
    long long r = T[n];
    free(T);
    return r;
}

int main(int argc, char *argv[]) {
    int n = 5;
    if (argc > 1) n = atoi(argv[1]);
    else {
        printf("Enter number of switches n: ");
        if (scanf("%d", &n) != 1) return 1;
    }
    if (n <= 0) { printf("n must be positive\n"); return 1; }

    long long closedForm = (pow2ll(n + 1)) / 3;
    long long dpForm = switches_moves_dp(n);
    printf("Closed form: minimum moves = floor(2^(n+1)/3) = %lld\n", closedForm);
    printf("DP recurrence T(n)=2*T(n-1)+(n mod 2) gives      = %lld\n", dpForm);

    if (n <= 20) {
        long bf = bfs_min_moves(n, n <= 6);
        printf("BFS (exhaustive shortest path) verified minimum = %ld  (%s)\n",
               bf, (bf == closedForm) ? "MATCHES formula" : "MISMATCH");
    } else {
        printf("(BFS validation skipped for n>20; state space 2^n too large)\n");
    }

    printf("\nTable for n = 1..15:\n");
    for (int i = 1; i <= 15; i++)
        printf("  n=%2d  moves=floor(2^(n+1)/3)=%6lld\n", i, (pow2ll(i + 1)) / 3);

    return 0;
}
