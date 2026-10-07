/* Q5: Maximum Sum Increasing Subsequence. Input: int array A[0..n-1] (positive).
 * msis[i] = A[i] + max(0, max msis[j] for j<i, A[j]<A[i]); prev[] rebuilds the subsequence.
 * Time O(n^2), Space O(n). */
#include <stdio.h>
#include <stdlib.h>

long long msis(const int *a, int n, int *seq, int *len) {
    long long *dp = malloc(n * sizeof(long long)), best = 0;
    int *prev = malloc(n * sizeof(int)), bi = -1;
    for (int i = 0; i < n; i++) {
        dp[i] = a[i]; prev[i] = -1;
        for (int j = 0; j < i; j++)
            if (a[j] < a[i] && dp[j] + a[i] > dp[i]) { dp[i] = dp[j] + a[i]; prev[i] = j; }
        if (dp[i] > best) { best = dp[i]; bi = i; }
    }
    int k = 0, tmp[1024];
    for (int i = bi; i != -1; i = prev[i]) tmp[k++] = a[i];
    for (int i = 0; i < k; i++) seq[i] = tmp[k - 1 - i];
    *len = k; free(dp); free(prev); return best;
}
static long long brute(const int *a, int n) {
    long long best = 0;
    for (int m = 1; m < (1 << n); m++) {
        int last = 0, ok = 1; long long s = 0;
        for (int i = 0; i < n && ok; i++) if (m >> i & 1) { if (a[i] <= last) ok = 0; last = a[i]; s += a[i]; }
        if (ok && s > best) best = s;
    }
    return best;
}
static void run(const int *a, int n, long long expect) {
    int seq[1024], len; long long r = msis(a, n, seq, &len);
    printf("n=%d -> sum=%lld (expect %lld) seq=[", n, r, expect);
    for (int i = 0; i < len; i++) printf("%d%s", seq[i], i + 1 < len ? "," : "");
    printf("]\n");
}
int main(void) {
    int a[] = {1, 101, 2, 3, 100, 4, 5}, b[] = {3, 4, 5, 10}, c[] = {10, 5, 4, 3}, d[] = {1, 101, 2, 3, 100};
    run(a, 7, 106); run(b, 4, 22); run(c, 4, 10); run(d, 5, 106);
    srand(3); int bad = 0;
    for (int t = 0; t < 300; t++) {
        int n = 1 + rand() % 14, x[16], seq[64], len;
        for (int i = 0; i < n; i++) x[i] = 1 + rand() % 20;
        if (msis(x, n, seq, &len) != brute(x, n)) bad++;
    }
    printf("Random check vs brute force: %s\n", bad ? "FAIL" : "all 300 passed");
    return 0;
}
