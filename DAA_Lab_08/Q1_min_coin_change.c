/* Q1: Minimum Coin Change.  Input: int array C[0..n-1] of denominations + target V.
 * dp[v] = min coins for amount v = 1 + min_i dp[v-C[i]].
 * Time O(nV), Space O(V). */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int min_coins(const int *c, int n, int V) {
    if (V < 0) return -1;
    int *dp = malloc((V + 1) * sizeof(int));
    dp[0] = 0;
    for (int v = 1; v <= V; v++) {
        dp[v] = INT_MAX;
        for (int i = 0; i < n; i++)
            if (c[i] > 0 && c[i] <= v && dp[v - c[i]] != INT_MAX && dp[v - c[i]] + 1 < dp[v])
                dp[v] = dp[v - c[i]] + 1;
    }
    int r = dp[V] == INT_MAX ? -1 : dp[V];
    free(dp);
    return r;
}
static int brute(const int *c, int n, int V) {          /* exponential reference */
    if (V == 0) return 0;
    int best = -1;
    for (int i = 0; i < n; i++)
        if (c[i] <= V) { int r = brute(c, n, V - c[i]); if (r >= 0 && (best < 0 || r + 1 < best)) best = r + 1; }
    return best;
}
int main(void) {
    int c1[] = {1, 2, 5}, c2[] = {2}, c3[] = {1, 3, 4}, c4[] = {186, 419, 83, 408};
    printf("{1,2,5}, V=11  -> %d (expect 3)\n", min_coins(c1, 3, 11));
    printf("{2}, V=3       -> %d (expect -1)\n", min_coins(c2, 1, 3));
    printf("{1,3,4}, V=6   -> %d (expect 2)\n", min_coins(c3, 3, 6));
    printf("{186,419,83,408}, V=6249 -> %d (expect 20)\n", min_coins(c4, 4, 6249));
    printf("V=0            -> %d (expect 0)\n", min_coins(c1, 3, 0));
    srand(1); int bad = 0;
    for (int t = 0; t < 300; t++) {
        int n = 1 + rand() % 4, c[4];
        for (int i = 0; i < n; i++) c[i] = 1 + rand() % 9;
        int V = rand() % 22;
        if (min_coins(c, n, V) != brute(c, n, V)) bad++;
    }
    printf("Random check vs brute force: %s\n", bad ? "FAIL" : "all 300 passed");
    return 0;
}
