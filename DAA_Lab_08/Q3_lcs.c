/* Q3: LCS length + reconstruction. Input: two C strings X (len m), Y (len n).
 * L[i][j] = L[i-1][j-1]+1 if X[i-1]==Y[j-1] else max(L[i-1][j], L[i][j-1]).
 * Reconstruct by walking back from L[m][n].
 * Time O(mn), Space O(mn). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lcs(const char *X, const char *Y, char **out) {
    int m = strlen(X), n = strlen(Y);
    int *L = calloc((m + 1) * (n + 1), sizeof(int));
#define T(i, j) L[(i) * (n + 1) + (j)]
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            T(i, j) = X[i-1] == Y[j-1] ? T(i-1, j-1) + 1 : (T(i-1, j) >= T(i, j-1) ? T(i-1, j) : T(i, j-1));
    int len = T(m, n);
    char *s = malloc(len + 1); s[len] = 0;
    int i = m, j = n, k = len;
    while (i > 0 && j > 0) {
        if (X[i-1] == Y[j-1]) { s[--k] = X[i-1]; i--; j--; }
        else if (T(i-1, j) >= T(i, j-1)) i--;
        else j--;
    }
    free(L); *out = s; return len;
}
static int is_subseq(const char *s, const char *t) {
    while (*s && *t) if (*s == *t++) s++;
    return *s == 0;
}
int main(void) {
    const char *tests[][3] = {
        {"ABCBDAB", "BDCABA", "4"}, {"AGGTAB", "GXTXAYB", "4"},
        {"abc", "def", "0"}, {"", "abc", "0"}, {"AAAA", "AA", "2"}};
    for (int t = 0; t < 5; t++) {
        char *s; int len = lcs(tests[t][0], tests[t][1], &s);
        printf("X=\"%s\" Y=\"%s\" -> len=%d (expect %s) LCS=\"%s\" valid=%s\n", tests[t][0], tests[t][1],
               len, tests[t][2], s, is_subseq(s, tests[t][0]) && is_subseq(s, tests[t][1]) ? "yes" : "NO");
        free(s);
    }
    return 0;
}
