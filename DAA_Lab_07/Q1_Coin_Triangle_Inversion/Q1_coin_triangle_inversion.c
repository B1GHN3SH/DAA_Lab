/*

 * Q1. Invert the coin-triangle
 * ----------------------------
 * A triangle of n rows of closely packed coins (row r, 0-indexed, holds
 * r+1 coins, r = 0..n-1, apex at the top) must be turned upside down by
 * sliding individual coins to new lattice positions, minimizing the
 * number of coins that are moved.
 *
 * ALGORITHM / DERIVATION
 * -----------------------
 * Model each coin by its centre on the triangular lattice using
 * "doubled" integer coordinates so that all neighbours are unit
 * distance apart with integer arithmetic only:
 *      coin (row r, col c), 0 <= c <= r  ->  (X, Y) = (2c - r, -r)
 *
 * A coin that already sits at a lattice point which is ALSO occupied in
 * the inverted (180-degree rotated) picture does not need to move.
 * Hence:
 *      minimum moves = (total coins) - (maximum possible overlap
 *                        between the original triangle and a
 *                        180-degree rotated copy of it, over all
 *                        placements of the rotated copy on the same
 *                        lattice)
 *
 * It is a classical result (see e.g. the "reverse the triangle" coin
 * puzzle) that this maximum overlap is achieved by keeping the largest
 * possible central sub-triangle fixed, and the resulting minimum number
 * of moves for an n-row triangle is simply
 *
 *      M(n) = floor( T(n) / 3 ),   where T(n) = n(n+1)/2 is the total
 *                                   number of coins (a triangular number)
 *
 * i.e. divide the number of coins by 3 and drop the remainder.
 *  n = 2 (3 coins)  -> M = 1
 *  n = 3 (6 coins)  -> M = 2
 *  n = 4 (10 coins) -> M = 3   (the classical "10-coin" puzzle)
 *  n = 5 (15 coins) -> M = 5
 *  n = 6 (21 coins) -> M = 7
 *
 * This program:
 *   1. Computes M(n) directly from the closed-form formula  -> O(1)
 *   2. VALIDATES the formula for small n by brute force: it builds the
 *      coordinate set of the upward triangle, tries every integer
 *      translation of the 180-degree-rotated copy within a bounded
 *      window, measures the overlap, and confirms
 *              T(n) - maxOverlap == M(n)
 *
 * COMPLEXITY
 * ----------
 *  Formula evaluation:               O(1)
 *  Brute-force validation for a
 *  given n (used only to double
 *  check correctness for small n):
 *      #coins           T(n) = O(n^2)
 *      #translations tried    O(n^2)   (a (4n+1) x (4n+1) window)
 *      cost per translation    O(n^2)  (linear scan / membership test)
 *      total                   O(n^6)  -- fine for the small n (<= 12)
 *                                         used only for validation; the
 *                                         actual answer for any n is
 *                                         produced in O(1) by the formula.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct { int x, y; } Point;

/* Build the coordinate list of an n-row upward triangle. Returns count. */
static int build_triangle(int n, Point *pts) {
    int k = 0;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c <= r; c++) {
            pts[k].x = 2 * c - r;
            pts[k].y = -r;
            k++;
        }
    }
    return k;
}

static int contains(Point *pts, int cnt, int x, int y) {
    for (int i = 0; i < cnt; i++)
        if (pts[i].x == x && pts[i].y == y) return 1;
    return 0;
}

/* Formula: closed form minimum number of moves */
static long formula_moves(long n) {
    long T = n * (n + 1) / 2;
    return T / 3;
}

/* Brute force validation for small n: maximise overlap of the triangle
 * with a 180-degree-rotated (point reflected) copy of itself over all
 * integer translations in a bounded window, then report
 * total - maxOverlap, which should equal formula_moves(n).            */
static long brute_force_moves(int n) {
    int T = n * (n + 1) / 2;
    Point *up = malloc(sizeof(Point) * T);
    build_triangle(n, up);

    int best = 0;
    int W = 2 * n + 2; /* search window radius */
    for (int dx = -W; dx <= W; dx++) {
        for (int dy = -W; dy <= W; dy++) {
            int overlap = 0;
            for (int i = 0; i < T; i++) {
                /* 180-degree rotation of coin i is (-x,-y); then shift by (dx,dy) */
                int rx = -up[i].x + dx;
                int ry = -up[i].y + dy;
                if (contains(up, T, rx, ry)) overlap++;
            }
            if (overlap > best) best = overlap;
        }
    }
    free(up);
    return T - best;
}

int main(int argc, char *argv[]) {
    int n;
    if (argc > 1) {
        n = atoi(argv[1]);
    } else {
        printf("Enter number of rows n of the coin-triangle: ");
        if (scanf("%d", &n) != 1) return 1;
    }
    if (n <= 0) { printf("n must be positive.\n"); return 1; }

    long T = (long)n * (n + 1) / 2;
    long m = formula_moves(n);
    printf("Triangle rows            : %d\n", n);
    printf("Total coins T(n)         : %ld\n", T);
    printf("Minimum moves M(n)=T/3   : %ld\n", m);

    if (n <= 10) {
        long bf = brute_force_moves(n);
        printf("Brute-force validation   : %ld  (%s)\n",
               bf, bf == m ? "MATCHES formula" : "MISMATCH");
    } else {
        printf("(brute-force skipped for n > 10; grows too fast, formula still O(1))\n");
    }

    printf("\nTable of M(n) for n = 1..12:\n");
    for (int i = 1; i <= 12; i++) {
        printf("  n=%2d  coins=%3ld  moves=%3ld\n", i, (long)i*(i+1)/2, formula_moves(i));
    }
    return 0;
}
