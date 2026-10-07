/* Q2: Number of combinations (order ignored). Input: int array C + target V.
 * dp[v] += dp[v-c] with coins in the OUTER loop => each multiset counted once.
 * Time O(nV), Space O(V). */
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long u64;

u64 count_ways(const int *c, int n, int V) {
    u64 *dp = calloc(V + 1, sizeof(u64));
    dp[0] = 1;
    for (int i = 0; i < n; i++)
        for (int v = c[i]; v <= V; v++) dp[v] += dp[v - c[i]];
    u64 r = dp[V]; free(dp); return r;
}
static u64 brute(const int *c, int n, int idx, int V) {  /* include/exclude recursion */
    if (V == 0) return 1;
    if (V < 0 || idx == n) return 0;
    return brute(c, n, idx, V - c[idx]) + brute(c, n, idx + 1, V);
}
int main(void) {
    int a[] = {1, 2, 3}, b[] = {2, 5, 3, 6}, d[] = {2}, e[] = {1, 5, 10, 25};
    printf("{1,2,3}, V=4     -> %llu (expect 4)\n", count_ways(a, 3, 4));
    printf("{2,5,3,6}, V=10  -> %llu (expect 5)\n", count_ways(b, 4, 10));
    printf("{2}, V=3         -> %llu (expect 0)\n", count_ways(d, 1, 3));
    printf("{1,5,10,25}, V=100 -> %llu (expect 242)\n", count_ways(e, 4, 100));
    printf("V=0              -> %llu (expect 1)\n", count_ways(a, 3, 0));
    int bad = 0;
    for (int V = 0; V <= 20; V++)
        if (count_ways(b, 4, V) != brute(b, 4, 0, V)) bad++;
    printf("Check vs brute force (V=0..20): %s\n", bad ? "FAIL" : "passed");
    return 0;
}
