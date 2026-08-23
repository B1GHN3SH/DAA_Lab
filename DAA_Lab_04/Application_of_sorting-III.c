/*
 * DAA Lab-04, Q3: Application of sorting-III
 * -------------------------------------------
 * Problem : Given a set S of n integers and integer T, test whether some
 *           k distinct elements of S sum to T.
 *
 * Approach: 1. Sort S                                       -> O(n log n)
 *           2. Fix the first (k-1) elements using (k-1) nested loops,
 *              implemented as recursion so k is a runtime parameter
 *              -> O(n^(k-1)) combinations
 *           3. For the LAST element, instead of one more loop, binary
 *              search the remaining sorted suffix for the exact value
 *              (T - partial_sum) -> O(log n) per combination
 *
 *           To avoid re-using an index and avoid duplicate combinations,
 *           each recursive pick only considers indices >= current start,
 *           and the binary search for the last element only searches
 *           indices strictly after the last fixed index.
 *
 * Complexity: O(n^(k-1) * log n) time, O(k) extra space (recursion depth).
 */

#include <stdio.h>
#include <stdlib.h>

int cmpInt(const void *a, const void *b) {
    return (*(const int *)a) - (*(const int *)b);
}

/* Binary search for `target` within arr[lo..hi] inclusive. Returns index or -1. */
int binarySearchRange(const int *arr, int lo, int hi, int target) {
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

/*
 * arr        : sorted array
 * n          : size of arr
 * kLeft      : how many more elements we still need to pick (>=1)
 * start      : smallest index we're allowed to pick from
 * target     : remaining sum needed
 * chosen     : output buffer recording the chosen values (size k)
 * chosenLen  : how many have been chosen so far (index into chosen[])
 *
 * Returns 1 if a valid combination is found (and fills `chosen`), else 0.
 */
int pickK(const int *arr, int n, int kLeft, int start, int target,
          int *chosen, int chosenLen) {
    if (kLeft == 1) {
        /* Last element: binary search the remaining suffix. */
        int idx = binarySearchRange(arr, start, n - 1, target);
        if (idx == -1) return 0;
        chosen[chosenLen] = arr[idx];
        return 1;
    }

    /* Need to leave room for the remaining (kLeft - 1) picks after this one. */
    int lastPossible = n - kLeft; /* inclusive upper bound for this pick's index */
    for (int i = start; i <= lastPossible; i++) {
        chosen[chosenLen] = arr[i];
        if (pickK(arr, n, kLeft - 1, i + 1, target - arr[i], chosen, chosenLen + 1))
            return 1;
    }
    return 0;
}

/*
 * Tests whether k elements of S (n elements) sum to T.
 * If found, fills `chosen` (must have room for k ints) with the combination.
 */
int kSumExists(const int *S, int n, int k, int T, int *chosen) {
    if (k <= 0 || k > n) return 0;

    int *sorted = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) sorted[i] = S[i];
    qsort(sorted, n, sizeof(int), cmpInt);

    int result = pickK(sorted, n, k, 0, T, chosen, 0);

    free(sorted);
    return result;
}

void printArr(const char *label, const int *arr, int n) {
    printf("%s: [", label);
    for (int i = 0; i < n; i++) printf("%d%s", arr[i], (i + 1 < n) ? ", " : "");
    printf("]\n");
}

void runTest(const char *label, const int *S, int n, int k, int T) {
    printf("--- %s ---\n", label);
    printArr("S", S, n);
    printf("k = %d, T = %d\n", k, T);

    int *chosen = (int *)malloc(k * sizeof(int));
    if (kSumExists(S, n, k, T, chosen)) {
        int sum = 0;
        printArr("Result: FOUND, combination", chosen, k);
        for (int i = 0; i < k; i++) sum += chosen[i];
        printf("Sum check: %d\n\n", sum);
    } else {
        printf("Result: NOT FOUND\n\n");
    }
    free(chosen);
}

int main(void) {
    int S1[] = {2, 7, 4, 1, 9, 5};
    runTest("Test 1: k=3 exists (2+4+9=15 or others)", S1, 6, 3, 15);

    int S2[] = {1, 2, 3, 4, 5};
    runTest("Test 2: k=2 exists (classic pair sum)", S2, 5, 2, 9);

    int S3[] = {1, 2, 3, 4, 5};
    runTest("Test 3: k=2 does not exist", S3, 5, 2, 100);

    int S4[] = {10, -3, 7, 2, -8, 5, 1};
    runTest("Test 4: k=4 with negatives", S4, 7, 4, 6);

    int S5[] = {3, 3, 3};
    runTest("Test 5: k=3, all same value must use distinct indices", S5, 3, 3, 9);

    int S6[] = {5, 10, 15};
    runTest("Test 6: k=1 (degenerate, just membership test)", S6, 3, 1, 10);

    return 0;
}
