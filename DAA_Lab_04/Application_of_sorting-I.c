/*
 * DAA Lab-04, Q1: Application of sorting-I
 * -----------------------------------------
 * Problem : Given n (number, colour) pairs, already sorted by number,
 *           re-arrange them so all reds come before all blues before
 *           all yellows, WITHOUT disturbing the relative (sorted) order
 *           of numbers within each colour group.
 *
 * Approach: Comparison sorting is overkill (and can't beat O(n log n))
 *           since colour only takes 3 distinct values. Use a STABLE
 *           counting sort on the colour field:
 *
 *              1. Count occurrences of each colour            -> O(n)
 *              2. Prefix-sum the counts to get starting slots -> O(1) (3 buckets)
 *              3. Walk the ORIGINAL array left to right and place
 *                 each item into its bucket's next free slot   -> O(n)
 *
 *           Walking the original (already number-sorted) array in order
 *           and appending into buckets preserves relative order within
 *           a colour group => stability, which is exactly what keeps
 *           numbers sorted inside each colour.
 *
 * Complexity: O(n) time, O(n) extra space.
 */

#include <stdio.h>
#include <stdlib.h>

typedef enum { RED = 0, BLUE = 1, YELLOW = 2 } Colour;

typedef struct {
    int    num;
    Colour colour;
} Item;

const char *colourName(Colour c) {
    switch (c) {
        case RED:    return "Red";
        case BLUE:   return "Blue";
        case YELLOW: return "Yellow";
    }
    return "?";
}

/* Stable O(n) sort of arr[0..n-1] by colour (Red < Blue < Yellow). */
void sortByColour(Item *arr, int n) {
    int count[3] = {0, 0, 0};
    for (int i = 0; i < n; i++) count[arr[i].colour]++;

    int start[3];
    start[0] = 0;
    start[1] = start[0] + count[0];
    start[2] = start[1] + count[1];

    Item *output = (Item *)malloc(n * sizeof(Item));
    int next[3] = {start[0], start[1], start[2]};

    for (int i = 0; i < n; i++) {
        Colour c = arr[i].colour;
        output[next[c]++] = arr[i];
    }

    for (int i = 0; i < n; i++) arr[i] = output[i];
    free(output);
}

void printItems(const Item *arr, int n) {
    for (int i = 0; i < n; i++)
        printf("(%d, %s) ", arr[i].num, colourName(arr[i].colour));
    printf("\n");
}

void runTest(const char *label, Item *arr, int n) {
    printf("--- %s ---\n", label);
    printf("Input  (sorted by number): ");
    printItems(arr, n);

    sortByColour(arr, n);

    printf("Output (grouped by colour, numbers stable within group): ");
    printItems(arr, n);

    /* check colour grouping */
    int ok = 1;
    for (int i = 1; i < n; i++)
        if (arr[i - 1].colour > arr[i].colour) ok = 0;

    /* check numbers strictly increasing within each colour run */
    for (int i = 1; i < n; i++)
        if (arr[i - 1].colour == arr[i].colour && arr[i - 1].num > arr[i].num)
            ok = 0;

    printf("Validation: %s\n\n", ok ? "PASS" : "FAIL");
}

int main(void) {
    /* Test 1: example with mixed colours, numbers already sorted */
    Item t1[] = {
        {1, RED}, {2, BLUE}, {3, YELLOW}, {4, RED},
        {5, BLUE}, {6, YELLOW}, {7, RED}
    };
    runTest("Test 1: mixed", t1, sizeof(t1) / sizeof(t1[0]));

    /* Test 2: already grouped */
    Item t2[] = {
        {1, RED}, {2, RED}, {3, BLUE}, {4, BLUE}, {5, YELLOW}
    };
    runTest("Test 2: already grouped", t2, sizeof(t2) / sizeof(t2[0]));

    /* Test 3: single colour */
    Item t3[] = { {10, BLUE}, {20, BLUE}, {30, BLUE} };
    runTest("Test 3: single colour", t3, sizeof(t3) / sizeof(t3[0]));

    /* Test 4: reverse colour order in input (Yellow, Blue, Red interleaved) */
    Item t4[] = {
        {1, YELLOW}, {2, YELLOW}, {3, BLUE}, {4, RED}, {5, RED}, {6, BLUE}
    };
    runTest("Test 4: interleaved", t4, sizeof(t4) / sizeof(t4[0]));

    /* Test 5: n = 0 edge case */
    Item t5[1];
    runTest("Test 5: empty input", t5, 0);

    return 0;
}
