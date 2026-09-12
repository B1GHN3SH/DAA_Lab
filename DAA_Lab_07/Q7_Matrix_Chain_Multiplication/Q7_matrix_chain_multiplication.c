/*
 * Q7. Matrix Chain Multiplication (MCM)
 * ---------------------------------------
 * Given dimensions p[0..n] for a chain of n matrices A1..An where Ai
 * has dimensions p[i-1] x p[i], find the parenthesization (order of
 * multiplication) that minimizes the total number of scalar
 * multiplications, and report that minimum along with the optimal
 * ordering.
 *
 * DP FORMULATION
 * --------------
 * Let m[i][j] = minimum scalar multiplications to compute the product
 * Ai * A(i+1) * ... * Aj  (1 <= i <= j <= n).
 *
 *   m[i][i] = 0
 *   m[i][j] = min over k in [i, j-1] of
 *                 m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j]
 *
 * The split point k achieving the minimum is stored in s[i][j] so the
 * optimal parenthesization can be reconstructed afterward.
 *
 * We fill the table by increasing CHAIN LENGTH (len = j-i+1 = 2..n),
 * which is the standard bottom-up order guaranteeing that m[i][k] and
 * m[k+1][j] (both of length < len) are already known.
 *
 * COMPLEXITY
 * ----------
 *  Time  : O(n^3)   (O(n^2) subproblems (i,j), each scanned over
 *                     O(n) split points k)
 *  Space : O(n^2)    (tables m[][] and s[][])
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static void print_optimal_parens(int **s, int i, int j, char *prefix) {
    if (i == j) {
        printf("A%d", i);
    } else {
        printf("(");
        print_optimal_parens(s, i, s[i][j], prefix);
        print_optimal_parens(s, s[i][j] + 1, j, prefix);
        printf(")");
    }
}

int main(int argc, char *argv[]) {
    int n;
    long *p;

    if (argc > 1) {
        n = argc - 1;
        p = malloc((n) * sizeof(long));
        for (int i = 1; i < argc; i++) p[i - 1] = atol(argv[i]);
        n = n - 1; /* n matrices need n+1 dimensions */
    } else {
        printf("Enter number of matrices n: ");
        if (scanf("%d", &n) != 1) return 1;
        p = malloc((n + 1) * sizeof(long));
        printf("Enter %d dimensions p[0..%d] (Ai is p[i-1] x p[i]):\n", n + 1, n);
        for (int i = 0; i <= n; i++) scanf("%ld", &p[i]);
    }
    if (n <= 0) { printf("Need at least 1 matrix.\n"); return 1; }

    long **m = malloc((n + 1) * sizeof(long *));
    int **s = malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) {
        m[i] = calloc(n + 1, sizeof(long));
        s[i] = calloc(n + 1, sizeof(int));
    }

    for (int len = 2; len <= n; len++) {
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;
            m[i][j] = LONG_MAX;
            for (int k = i; k < j; k++) {
                long cost = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (cost < m[i][j]) { m[i][j] = cost; s[i][j] = k; }
            }
        }
    }

    printf("\nMatrix dimensions: ");
    for (int i = 0; i <= n; i++) printf("p[%d]=%ld ", i, p[i]);
    printf("\n(A%d is %ldx%ld sized ... A%d is %ldx%ld)\n", 1, p[0], p[1], n, p[n-1], p[n]);

    printf("\nMinimum number of scalar multiplications: %ld\n", m[1][n]);
    printf("Optimal parenthesization: ");
    print_optimal_parens(s, 1, n, "");
    printf("\n");

    printf("\nm[][] table (min cost for subchain Ai..Aj):\n");
    printf("i\\j");
    for (int j = 1; j <= n; j++) printf("%10d", j);
    printf("\n");
    for (int i = 1; i <= n; i++) {
        printf("%3d", i);
        for (int j = 1; j <= n; j++) {
            if (j < i) printf("%10s", "-");
            else printf("%10ld", m[i][j]);
        }
        printf("\n");
    }

    /* Classic textbook check: p = {30,35,15,5,10,20,25} -> expect 15125 */
    if (n == 6 && p[0]==30 && p[1]==35 && p[2]==15 && p[3]==5 && p[4]==10 && p[5]==20 && p[6]==25) {
        printf("\n[Textbook check] Expected minimum = 15125, got %ld : %s\n",
               m[1][n], (m[1][n] == 15125) ? "MATCH" : "MISMATCH");
    }

    for (int i = 0; i <= n; i++) { free(m[i]); free(s[i]); }
    free(m); free(s); free(p);
    return 0;
}
