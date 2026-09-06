/*
 * DAA LAB-5 : Question 2
 * ---------------------------------------------------------------------
 * Find the K'th smallest element in a given list of N numbers WITHOUT
 * sorting the list.
 *
 * Approach : Randomized Quickselect (Hoare's Selection Algorithm)
 *   - Same partition/recursion idea as Question 1, generalised to an
 *     arbitrary rank k (1-indexed here, so k=1 -> smallest element).
 *   - Only recurse into the ONE partition that must contain the
 *     k-th smallest element; the other side is discarded, which is
 *     what gives the expected linear running time (unlike quicksort,
 *     which recurses into both sides).
 *
 * Complexity Analysis
 *   - Expected case : O(n)     (each partition step roughly halves
 *                                the remaining search space, giving
 *                                the series n + n/2 + n/4 + ... = O(n))
 *   - Worst case    : O(n^2)   (adversarial input + unlucky pivot
 *                                choices every time, e.g. always
 *                                picking the max/min as pivot)
 *   - Worst case can be brought down to guaranteed O(n) using the
 *     deterministic "median of medians" pivot selection strategy,
 *     at the cost of a larger constant factor; not required here
 *     since randomized pivoting already gives O(n) expected time.
 *   - Space Complexity: O(1) extra / O(log n) expected recursion depth.
 * ---------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

static int partition(int a[], int low, int high) {
    int pivot = a[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (a[j] <= pivot) {
            i++;
            swap(&a[i], &a[j]);
        }
    }
    swap(&a[i + 1], &a[high]);
    return i + 1;
}

static int randomizedPartition(int a[], int low, int high) {
    int randIndex = low + rand() % (high - low + 1);
    swap(&a[randIndex], &a[high]);
    return partition(a, low, high);
}

/* Returns the k-th smallest element (0-indexed rank) of a[low..high]. */
static int quickSelect(int a[], int low, int high, int k) {
    while (low <= high) {
        if (low == high) return a[low];
        int pivotIndex = randomizedPartition(a, low, high);
        if (k == pivotIndex) return a[pivotIndex];
        else if (k < pivotIndex) high = pivotIndex - 1;
        else low = pivotIndex + 1;
    }
    return -1;
}

/* 1-indexed convenience wrapper: kthSmallest(a, n, 1) = minimum */
static int kthSmallest(int a[], int n, int k) {
    return quickSelect(a, 0, n - 1, k - 1);
}

/* ---------------- reference / brute-force check ---------------- */
static int cmpInt(const void *x, const void *y) {
    return (*(int *)x - *(int *)y);
}
static int bruteKthSmallest(int a[], int n, int k) {
    int *copy = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) copy[i] = a[i];
    qsort(copy, n, sizeof(int), cmpInt);
    int result = copy[k - 1];
    free(copy);
    return result;
}

/* ---------------- self-test harness ---------------- */
static void runSelfTests(void) {
    printf("=== Running self-tests for kth-smallest (quickselect vs brute-force) ===\n");
    srand((unsigned int)time(NULL));
    int trials = 200, maxN = 500, failures = 0;

    for (int t = 0; t < trials; t++) {
        int n = 1 + rand() % maxN;
        int k = 1 + rand() % n;
        int *arr = malloc(n * sizeof(int));
        int *copy = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) { arr[i] = rand() % 10000 - 5000; copy[i] = arr[i]; }

        int got = kthSmallest(copy, n, k);
        int expected = bruteKthSmallest(arr, n, k);

        if (got != expected) {
            failures++;
            printf("  FAIL (n=%d, k=%d): got %d expected %d\n", n, k, got, expected);
        }
        free(arr); free(copy);
    }

    if (failures == 0)
        printf("All %d randomized trials PASSED.\n", trials);
    else
        printf("%d / %d trials FAILED.\n", failures, trials);
    printf("=====================================================\n\n");
}

int main(void) {
    runSelfTests();

    int n, k;
    printf("Enter N (number of elements): ");
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid N.\n"); return 1; }

    int *a = malloc(n * sizeof(int));
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);

    printf("Enter K (1 <= K <= %d): ", n);
    if (scanf("%d", &k) != 1 || k < 1 || k > n) {
        printf("Invalid K.\n");
        free(a);
        return 1;
    }

    int result = kthSmallest(a, n, k);
    printf("%d-th smallest element = %d\n", k, result);

    free(a);
    return 0;
}
