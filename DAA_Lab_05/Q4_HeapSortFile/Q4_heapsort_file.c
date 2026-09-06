/*
 * DAA LAB-5 : Question 4
 * ---------------------------------------------------------------------
 * Implement Heap Sort to sort N randomly generated elements stored in
 * a file. Do the complexity analysis of your algorithm.
 *
 * Workflow
 *   1. Generate N random integers and write them to "input.txt".
 *   2. Read the N integers back from "input.txt" into an array.
 *   3. Build a max-heap in-place: O(n) using the bottom-up
 *      "heapify" build (NOT n inserts, which would cost O(n log n)).
 *   4. Repeatedly swap the heap root (max element) with the last
 *      element of the heap, shrink the heap by one, and sift the new
 *      root down to restore the heap property -- this places elements
 *      in sorted order from the back of the array forward.
 *   5. Write the sorted array to "output.txt".
 *
 * Complexity Analysis
 *   - Build-heap phase : O(n)        (tight bound via amortized analysis
 *                          of sift-down costs across all levels of the
 *                          heap -- NOT O(n log n), a common misconception)
 *   - Extraction phase : n calls to "sift down", each O(log n)
 *                          => O(n log n)
 *   - Total             : O(n log n) in ALL cases (best, average, AND
 *                          worst) -- heap sort has no bad input, unlike
 *                          quicksort, because the heap shape is always
 *                          a complete binary tree regardless of the
 *                          input's initial order.
 *   - Space Complexity  : O(1) extra -- entirely in-place, no recursion
 *                          needed if sift-down is written iteratively
 *                          (done here iteratively for that reason).
 * ---------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

/* Iterative sift-down (max-heapify) for the sub-tree rooted at index i,
 * within a heap of the given size (heap occupies a[0..size-1]). */
static void siftDown(int a[], int size, int i) {
    while (1) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && a[left] > a[largest]) largest = left;
        if (right < size && a[right] > a[largest]) largest = right;

        if (largest == i) break; /* heap property already satisfied */

        swap(&a[i], &a[largest]);
        i = largest;
    }
}

/* Builds a max-heap in-place in O(n) time by sifting down every
 * non-leaf node, starting from the last non-leaf and moving to the root. */
static void buildMaxHeap(int a[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        siftDown(a, n, i);
    }
}

static void heapSort(int a[], int n) {
    buildMaxHeap(a, n);
    for (int end = n - 1; end > 0; end--) {
        swap(&a[0], &a[end]);   /* move current max to its final sorted slot */
        siftDown(a, end, 0);    /* restore heap property on the shrunk heap */
    }
}

/* Generates n random integers into the given file, one per line. */
static void generateInputFile(const char *filename, int n, int range) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { perror("fopen"); exit(1); }
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d\n", rand() % (2 * range + 1) - range);
    }
    fclose(fp);
}

static int *readInputFile(const char *filename, int n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("fopen"); exit(1); }
    int *a = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        if (fscanf(fp, "%d", &a[i]) != 1) {
            fprintf(stderr, "Error reading element %d from %s\n", i, filename);
            exit(1);
        }
    }
    fclose(fp);
    return a;
}

static void writeOutputFile(const char *filename, int a[], int n) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { perror("fopen"); exit(1); }
    for (int i = 0; i < n; i++) fprintf(fp, "%d\n", a[i]);
    fclose(fp);
}

/* ---------------- correctness self-test ---------------- */
static int cmpInt(const void *x, const void *y) { return (*(int *)x - *(int *)y); }

static void runSelfTests(void) {
    printf("=== Running self-tests for heapsort (vs qsort reference) ===\n");
    int trials = 100, maxN = 1000, failures = 0;
    for (int t = 0; t < trials; t++) {
        int n = 1 + rand() % maxN;
        int *a = malloc(n * sizeof(int));
        int *ref = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) { a[i] = rand() % 20000 - 10000; ref[i] = a[i]; }

        heapSort(a, n);
        qsort(ref, n, sizeof(int), cmpInt);

        int ok = 1;
        for (int i = 0; i < n; i++) if (a[i] != ref[i]) { ok = 0; break; }
        if (!ok) { failures++; printf("  FAIL at n=%d\n", n); }
        free(a); free(ref);
    }
    if (failures == 0) printf("All %d randomized trials PASSED.\n", trials);
    else printf("%d / %d trials FAILED.\n", failures, trials);
    printf("=====================================================\n\n");
}

int main(void) {
    srand((unsigned int)time(NULL));
    runSelfTests();

    int n;
    printf("Enter N (number of random elements to generate): ");
    if (scanf("%d", &n) != 1 || n <= 0) { printf("Invalid N.\n"); return 1; }

    const char *inFile = "input.txt";
    const char *outFile = "output.txt";

    generateInputFile(inFile, n, 100000);
    printf("Generated %d random elements into '%s'.\n", n, inFile);

    int *a = readInputFile(inFile, n);

    heapSort(a, n);

    writeOutputFile(outFile, a, n);
    printf("Sorted elements written to '%s'.\n", outFile);

    if (n <= 20) {
        printf("Sorted output: ");
        for (int i = 0; i < n; i++) printf("%d ", a[i]);
        printf("\n");
    }

    free(a);
    return 0;
}
