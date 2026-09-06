/*
 * DAA LAB-5 : Question 1
 * ---------------------------------------------------------------------
 * Find the median of a list of N numbers WITHOUT sorting the list.
 *
 * Approach : Randomized Quickselect
 *   - The median is just the (n/2)-th order statistic (0-indexed).
 *   - Quickselect finds the k-th smallest element of an unsorted array
 *     in expected linear time by using the same partition step as
 *     Quicksort, but recursing into only ONE side of the partition
 *     (the side that must contain the k-th element) instead of both.
 *   - For even n, the median is the average of the (n/2 - 1)-th and
 *     (n/2)-th order statistics, so quickselect is invoked twice
 *     (on independent copies, so the first call's partial reordering
 *     does not corrupt the array needed for the second call... in
 *     practice we simply reuse the array since after the first
 *     quickselect the (n/2-1)-th smallest can be found in the
 *     already-partitioned left sub-array, done here as two calls on
 *     the same array for clarity).
 *
 * Complexity Analysis
 *   Let n = number of elements.
 *   - Partition step (Lomuto scheme): O(size of current sub-array)
 *   - Expected case : T(n) = T(n/2) + O(n)  =>  O(n)      [geometric series]
 *   - Worst case    : T(n) = T(n-1) + O(n)  =>  O(n^2)     (bad pivot every time,
 *                     e.g. already sorted input with last-element pivot)
 *   - Randomizing the pivot makes the worst case astronomically
 *     unlikely; expected running time over all inputs is O(n).
 *   - Space Complexity: O(1) extra (in-place partitioning, ignoring
 *                        the O(log n) recursion stack in the expected
 *                        case).
 * ---------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Swap two integers */
static void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

/* Lomuto partition around a[high], returns final index of pivot */
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

/* Randomized partition: pick a random pivot, swap it to the end, then
 * do a normal Lomuto partition. This is what keeps expected time O(n). */
static int randomizedPartition(int a[], int low, int high) {
    int randIndex = low + rand() % (high - low + 1);
    swap(&a[randIndex], &a[high]);
    return partition(a, low, high);
}

/*
 * quickSelect(a, low, high, k)
 * Returns the k-th smallest element (0-indexed) of a[low..high].
 * Iterative-tail-call style to avoid unnecessary recursion depth.
 */
static int quickSelect(int a[], int low, int high, int k) {
    while (low <= high) {
        if (low == high) return a[low];

        int pivotIndex = randomizedPartition(a, low, high);

        if (k == pivotIndex) {
            return a[pivotIndex];
        } else if (k < pivotIndex) {
            high = pivotIndex - 1;
        } else {
            low = pivotIndex + 1;
        }
    }
    /* unreachable if k is valid */
    return -1;
}

/* Finds the median of array a[0..n-1]. Uses two quickSelect calls on
 * two independent copies for even n, so neither call disturbs data
 * the other one needs. */
static double findMedian(int a[], int n) {
    if (n % 2 == 1) {
        int *copy = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) copy[i] = a[i];
        int mid = quickSelect(copy, 0, n - 1, n / 2);
        free(copy);
        return (double)mid;
    } else {
        int *copy1 = malloc(n * sizeof(int));
        int *copy2 = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) { copy1[i] = a[i]; copy2[i] = a[i]; }
        int lower = quickSelect(copy1, 0, n - 1, n / 2 - 1);
        int upper = quickSelect(copy2, 0, n - 1, n / 2);
        free(copy1); free(copy2);
        return (lower + upper) / 2.0;
    }
}

/* ---------------- reference / brute-force check (uses a sort) ---------------- */
static int cmpInt(const void *x, const void *y) {
    return (*(int *)x - *(int *)y);
}
static double bruteMedian(int a[], int n) {
    int *copy = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) copy[i] = a[i];
    qsort(copy, n, sizeof(int), cmpInt);
    double result;
    if (n % 2 == 1) result = copy[n / 2];
    else result = (copy[n / 2 - 1] + copy[n / 2]) / 2.0;
    free(copy);
    return result;
}

/* ---------------- self-test harness ---------------- */
static void runSelfTests(void) {
    printf("=== Running self-tests for median (quickselect vs brute-force) ===\n");
    srand((unsigned int)time(NULL));
    int trials = 200, maxN = 500, failures = 0;

    for (int t = 0; t < trials; t++) {
        int n = 1 + rand() % maxN;
        int *arr = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) arr[i] = rand() % 10000 - 5000;

        double got = findMedian(arr, n);
        double expected = bruteMedian(arr, n);

        if (got != expected) {
            failures++;
            printf("  FAIL (n=%d): got %.1f expected %.1f\n", n, got, expected);
        }
        free(arr);
    }

    if (failures == 0)
        printf("All %d randomized trials PASSED.\n", trials);
    else
        printf("%d / %d trials FAILED.\n", failures, trials);
    printf("=====================================================\n\n");
}

int main(void) {
    runSelfTests();

    int n;
    printf("Enter N (number of elements): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid N.\n");
        return 1;
    }

    int *a = malloc(n * sizeof(int));
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);

    double median = findMedian(a, n);
    printf("Median = %.1f\n", median);

    free(a);
    return 0;
}
