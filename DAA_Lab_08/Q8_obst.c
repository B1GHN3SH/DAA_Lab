/* Q8: Optimal BST (CLRS). Input: p[1..n] key probs, q[0..n] dummy-key probs (sum = 1).
 * e[i][j] = min_r e[i][r-1] + e[r+1][j] + w[i][j],  w[i][j] = sum p[i..j] + sum q[i-1..j].
 * e[i][i-1] = q[i-1]. root[i][j] stores the best r.
 * Time O(n^3) (three nested loops), Space O(n^2). */
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define N 32
static double e[N + 2][N + 2], w[N + 2][N + 2];
static int root[N + 2][N + 2];

double obst(const double *p, const double *q, int n) {
    for (int i = 1; i <= n + 1; i++) { e[i][i-1] = q[i-1]; w[i][i-1] = q[i-1]; }
    for (int len = 1; len <= n; len++)
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;
            e[i][j] = DBL_MAX; w[i][j] = w[i][j-1] + p[j] + q[j];
            for (int r = i; r <= j; r++) {
                double t = e[i][r-1] + e[r+1][j] + w[i][j];
                if (t < e[i][j]) { e[i][j] = t; root[i][j] = r; }
            }
        }
    return e[1][n];
}
static void print_tree(int i, int j, int parent, const char *side) {
    if (i > j) { printf("  d%d is the %s child of k%d\n", j, side, parent); return; }
    int r = root[i][j];
    if (parent == 0) printf("  k%d is the root\n", r);
    else printf("  k%d is the %s child of k%d\n", r, side, parent);
    print_tree(i, r - 1, r, "left"); print_tree(r + 1, j, r, "right");
}
int main(void) {
    double p1[] = {0, .15, .10, .05, .10, .20}, q1[] = {.05, .10, .05, .05, .05, .10};
    printf("CLRS n=5: expected cost = %.2f (expect 2.75)\n", obst(p1, q1, 5));
    print_tree(1, 5, 0, "");
    double p2[] = {0, .04, .06, .08, .02, .10, .12, .14}, q2[] = {.06, .06, .06, .06, .05, .05, .05, .05};
    printf("CLRS n=7: expected cost = %.2f (expect 3.12)\n", obst(p2, q2, 7));
    double p3[] = {0, 1.0}, q3[] = {0, 0};
    printf("n=1, p=1: cost = %.2f (expect 1.00)\n", obst(p3, q3, 1));
    return 0;
}
