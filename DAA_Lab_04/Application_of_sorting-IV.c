/*
 * DAA Lab-04, Q4: Application of sorting-IV
 * -------------------------------------------
 * Problem : n people, each with entry time a_i and exit time b_i
 *           (b_i > a_i, all 2n times distinct). Find the time at which
 *           the most people are simultaneously present.
 *
 * Approach: Classic sweep line.
 *           1. Build 2n events: (a_i, +1) for each arrival,
 *                                (b_i, -1) for each departure.
 *           2. Sort all events by time                     -> O(n log n)
 *           3. Sweep left to right maintaining a running counter;
 *              record the time at which the counter peaks   -> O(n)
 *
 * Complexity: O(n log n) time, O(n) extra space.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int time;
    int delta; /* +1 = arrival, -1 = departure */
} Event;

int cmpEvent(const void *a, const void *b) {
    const Event *ea = (const Event *)a;
    const Event *eb = (const Event *)b;
    return ea->time - eb->time; /* times are guaranteed distinct */
}

/*
 * entry[], exit[] : arrays of size n with entry/exit times.
 * Returns the time of peak attendance and writes the peak count to *peakCount.
 */
int findPeakTime(const int *entry, const int *exit, int n, int *peakCount) {
    Event *events = (Event *)malloc(2 * n * sizeof(Event));
    for (int i = 0; i < n; i++) {
        events[2 * i]     = (Event){entry[i], +1};
        events[2 * i + 1] = (Event){exit[i],  -1};
    }

    qsort(events, 2 * n, sizeof(Event), cmpEvent);

    int count = 0, best = 0, bestTime = events[0].time;
    for (int i = 0; i < 2 * n; i++) {
        count += events[i].delta;
        if (count > best) {
            best = count;
            bestTime = events[i].time;
        }
    }

    free(events);
    *peakCount = best;
    return bestTime;
}

void runTest(const char *label, const int *entry, const int *exit, int n) {
    printf("--- %s ---\n", label);
    printf("Persons (entry, exit): ");
    for (int i = 0; i < n; i++) printf("(%d,%d) ", entry[i], exit[i]);
    printf("\n");

    int peakCount;
    int peakTime = findPeakTime(entry, exit, n, &peakCount);
    printf("Peak attendance: %d people, first reached at time %d\n\n",
           peakCount, peakTime);
}

int main(void) {
    /* Test 1: simple overlap */
    int e1[] = {1, 2, 4};
    int x1[] = {5, 3, 8};
    runTest("Test 1: 3 people, overlaps", e1, x1, 3);

    /* Test 2: no overlap at all -> peak is always 1 */
    int e2[] = {1, 10, 20};
    int x2[] = {5, 15, 25};
    runTest("Test 2: no overlaps", e2, x2, 3);

    /* Test 3: everyone overlaps at one instant */
    int e3[] = {1, 2, 3, 4};
    int x3[] = {100, 99, 98, 97};
    runTest("Test 3: everyone overlaps", e3, x3, 4);

    /* Test 4: single person */
    int e4[] = {5};
    int x4[] = {10};
    runTest("Test 4: single person", e4, x4, 1);

    /* Test 5: staggered arrivals/departures, peak in the middle */
    int e5[] = {1, 3, 5, 7, 9};
    int x5[] = {20, 4, 18, 8, 15};
    runTest("Test 5: staggered", e5, x5, 5);

    return 0;
}
