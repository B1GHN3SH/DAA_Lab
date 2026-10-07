/* Q7: Rod cutting with reconstruction. Input: n and price array P[1..n] (P[0] unused).
 * r[j] = max_{1<=i<=j} P[i] + r[j-i]; s[j] = the i achieving the max (first piece).
 * Time O(n^2), Space O(n). */
#include <stdio.h>
#include <stdlib.h>

int rod_cut(const int *P, int n, int *pieces, int *cnt) {
    int *r = calloc(n + 1, sizeof(int)), *s = calloc(n + 1, sizeof(int));
    for (int j = 1; j <= n; j++) {
        r[j] = -1;
        for (int i = 1; i <= j; i++) if (P[i] + r[j - i] > r[j]) { r[j] = P[i] + r[j - i]; s[j] = i; }
    }
    int k = 0;
    for (int j = n; j > 0; j -= s[j]) pieces[k++] = s[j];
    *cnt = k; int rev = r[n]; free(r); free(s); return rev;
}
static int brute(const int *P, int n) {
    if (n == 0) return 0;
    int best = 0;
    for (int i = 1; i <= n; i++) { int v = P[i] + brute(P, n - i); if (v > best) best = v; }
    return best;
}
static void run(const int *P, int n, int expect) {
    int pc[64], k, rev = rod_cut(P, n, pc, &k);
    printf("n=%d -> revenue=%d (expect %d), pieces:", n, rev, expect);
    for (int i = 0; i < k; i++) printf(" %d", pc[i]);
    printf("\n");
}
int main(void) {
    int A[] = {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};    /* CLRS */
    run(A, 4, 10); run(A, 7, 18); run(A, 10, 30);
    int B[] = {0, 3, 5, 8, 9, 10, 17, 17, 20};
    run(B, 8, 24);
    int bad = 0;
    for (int n = 1; n <= 10; n++) { int pc[64], k; if (rod_cut(A, n, pc, &k) != brute(A, n)) bad++; }
    printf("Check vs brute force: %s\n", bad ? "FAIL" : "passed");
    return 0;
}
