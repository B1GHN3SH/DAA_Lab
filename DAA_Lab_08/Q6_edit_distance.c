/* Q6: Edit distance (insert/delete/substitute, unit cost) with traceback.
 * Input: strings A (len m), B (len n). D[i][j] = min(D[i-1][j-1]+(A[i-1]!=B[j-1]),
 * D[i-1][j]+1 (delete), D[i][j-1]+1 (insert)). Traceback from D[m][n] to D[0][0].
 * Time O(mn), Space O(mn) (traceback needs the full table). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

int edit_distance(const char *A, const char *B, int verbose) {
    int m = strlen(A), n = strlen(B);
    int *D = malloc((m + 1) * (n + 1) * sizeof(int));
#define T(i, j) D[(i) * (n + 1) + (j)]
    for (int i = 0; i <= m; i++) T(i, 0) = i;
    for (int j = 0; j <= n; j++) T(0, j) = j;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            T(i, j) = MIN3(T(i-1, j-1) + (A[i-1] != B[j-1]), T(i-1, j) + 1, T(i, j-1) + 1);
    int d = T(m, n);
    if (verbose) {
        char (*ops)[64] = malloc((m + n + 1) * 64); int k = 0, i = m, j = n, cost = 0;
        while (i > 0 || j > 0) {
            if (i > 0 && j > 0 && T(i, j) == T(i-1, j-1) + (A[i-1] != B[j-1])) {
                if (A[i-1] == B[j-1]) sprintf(ops[k++], "Match       '%c'", A[i-1]);
                else { sprintf(ops[k++], "Substitute  '%c' -> '%c'", A[i-1], B[j-1]); cost++; }
                i--; j--;
            } else if (i > 0 && T(i, j) == T(i-1, j) + 1) { sprintf(ops[k++], "Delete      '%c'", A[i-1]); i--; cost++; }
            else { sprintf(ops[k++], "Insert      '%c'", B[j-1]); j--; cost++; }
        }
        printf("\"%s\" -> \"%s\": distance = %d (traceback cost = %d)\n", A, B, d, cost);
        for (int x = k - 1; x >= 0; x--) printf("  %s\n", ops[x]);
        free(ops);
    }
    free(D); return d;
}
int main(void) {
    edit_distance("kitten", "sitting", 1);   /* expect 3 */
    edit_distance("horse", "ros", 1);        /* expect 3 */
    edit_distance("sunday", "saturday", 1);  /* expect 3 */
    printf("\"\" -> \"abc\": %d (expect 3)\n", edit_distance("", "abc", 0));
    printf("\"abc\" -> \"abc\": %d (expect 0)\n", edit_distance("abc", "abc", 0));
    return 0;
}
