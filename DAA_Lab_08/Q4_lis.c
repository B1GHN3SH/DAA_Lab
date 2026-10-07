/* Q4: Longest strictly Increasing Subsequence. Input: int array A[0..n-1].
 * (a) DP: dp[i] = 1 + max dp[j] (j<i, A[j]<A[i]).        Time O(n^2), Space O(n).
 * (b) Patience/binary search: tails[k] = smallest tail of an increasing
 *     subsequence of length k+1.                          Time O(n log n), Space O(n). */
#include <stdio.h>
#include <stdlib.h>

int lis_dp(const int *a, int n) {
    if (n == 0) return 0;
    int *dp = malloc(n * sizeof(int)), best = 1;
    for (int i = 0; i < n; i++) {
        dp[i] = 1;
        for (int j = 0; j < i; j++) if (a[j] < a[i] && dp[j] + 1 > dp[i]) dp[i] = dp[j] + 1;
        if (dp[i] > best) best = dp[i];
    }
    free(dp); return best;
}
int lis_fast(const int *a, int n) {
    int *t = malloc((n + 1) * sizeof(int)), len = 0;
    for (int i = 0; i < n; i++) {
        int lo = 0, hi = len;                    /* first tail >= a[i] (strict increase) */
        while (lo < hi) { int mid = (lo + hi) / 2; if (t[mid] < a[i]) lo = mid + 1; else hi = mid; }
        t[lo] = a[i]; if (lo == len) len++;
    }
    free(t); return len;
}
static int brute(const int *a, int n) {         /* all subsets, n <= 16 */
    int best = 0;
    for (int m = 0; m < (1 << n); m++) {
        int last = -1000000, ok = 1, cnt = 0;
        for (int i = 0; i < n && ok; i++) if (m >> i & 1) { if (a[i] <= last) ok = 0; last = a[i]; cnt++; }
        if (ok && cnt > best) best = cnt;
    }
    return best;
}
int main(void) {
    int a[] = {10, 9, 2, 5, 3, 7, 101, 18}, b[] = {0, 1, 0, 3, 2, 3}, c[] = {7, 7, 7, 7}, d[] = {5, 4, 3, 2, 1};
    printf("[10,9,2,5,3,7,101,18] -> %d / %d (expect 4)\n", lis_dp(a, 8), lis_fast(a, 8));
    printf("[0,1,0,3,2,3]         -> %d / %d (expect 4)\n", lis_dp(b, 6), lis_fast(b, 6));
    printf("[7,7,7,7]             -> %d / %d (expect 1)\n", lis_dp(c, 4), lis_fast(c, 4));
    printf("[5,4,3,2,1]           -> %d / %d (expect 1)\n", lis_dp(d, 5), lis_fast(d, 5));
    srand(2); int bad = 0;
    for (int t = 0; t < 300; t++) {
        int n = 1 + rand() % 14, x[16];
        for (int i = 0; i < n; i++) x[i] = rand() % 10;
        int r = brute(x, n);
        if (lis_dp(x, n) != r || lis_fast(x, n) != r) bad++;
    }
    printf("Random check vs brute force: %s\n", bad ? "FAIL" : "all 300 passed");
    return 0;
}
