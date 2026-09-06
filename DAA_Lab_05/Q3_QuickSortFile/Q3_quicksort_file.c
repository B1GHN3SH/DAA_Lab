/*
 * DAA LAB-5 : Question 3
 * ---------------------------------------------------------------------
 * Implement Quick Sort of N random elements stored in a file.
 *
 * Workflow
 *   1. Generate N random integers and write them to "input.txt"
 *      (simulates "N random elements stored in a file").
 *   2. Read the N integers back from "input.txt" into an array.
 *   3. Sort the array in-place using Quicksort (Lomuto partition,
 *      randomized pivot to avoid the O(n^2) worst case on adversarial
 *      / already-sorted input).
 *   4. Write the sorted array to "output.txt".
 *
 * Complexity Analysis
 *   - Best / Average case : O(n log n)   (balanced partitions each level,
 *                             log n levels of recursion, O(n) work per level)
 *   - Worst case          : O(n^2)       (always maximally unbalanced
 *                             partition, e.g. picking smallest/largest
 *                             element as pivot every time on sorted data)
 *   - Randomized pivot selection makes the worst case extremely
 *     unlikely in practice; expected time is O(n log n).
 *   - Space Complexity     : O(log n) expected recursion stack (in-place
 *                             partitioning, no auxiliary array needed).
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

static void quickSort(int a[], int low, int high) {
    while (low < high) {
        int p = randomizedPartition(a, low, high);
        /* Recurse into the smaller partition, iterate on the larger
         * one -- bounds worst-case stack depth to O(log n). */
        if (p - low < high - p) {
            quickSort(a, low, p - 1);
            low = p + 1;
        } else {
            quickSort(a, p + 1, high);
            high = p - 1;
        }
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

/* Reads n integers from filename into a freshly malloc'd array. */
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
    printf("=== Running self-tests for quicksort (vs qsort reference) ===\n");
    int trials = 100, maxN = 1000, failures = 0;
    for (int t = 0; t < trials; t++) {
        int n = 1 + rand() % maxN;
        int *a = malloc(n * sizeof(int));
        int *ref = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) { a[i] = rand() % 20000 - 10000; ref[i] = a[i]; }

        quickSort(a, 0, n - 1);
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

    quickSort(a, 0, n - 1);

    writeOutputFile(outFile, a, n);
    printf("Sorted elements written to '%s'.\n", outFile);

    /* Show a small preview if n is small */
    if (n <= 20) {
        printf("Sorted output: ");
        for (int i = 0; i < n; i++) printf("%d ", a[i]);
        printf("\n");
    }

    free(a);
    return 0;
}
