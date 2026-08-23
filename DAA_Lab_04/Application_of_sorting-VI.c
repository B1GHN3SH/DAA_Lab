/*
 * DAA Lab-04, Q6: Application of sorting-VI
 * -------------------------------------------
 * Problem : Given n intervals [l_i, r_i] on a line (endpoints count as
 *           being IN the interval), find a point p covered by the
 *           largest number of intervals.
 *
 * Approach: Sweep line, same skeleton as Q4, but with a crucial twist:
 *           because endpoints are inclusive, if one interval ENDS at
 *           coordinate c and another STARTS at c, both cover point c.
 *           So when sorting events, ties at the same coordinate must
 *           process all "+1" (starts) BEFORE any "-1" (ends) -- otherwise
 *           we'd undercount the point where a closing and an opening
 *           interval meet.
 *
 *           1. Build 2n events: (l_i, +1), (r_i, -1)
 *           2. Sort by time; break ties with +1 before -1  -> O(n log n)
 *           3. Sweep, track running count and record the coordinate
 *              where the count peaks                        -> O(n)
 *
 * Complexity: O(n log n) time, O(n) extra space.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int time;
    int delta; /* +1 = interval starts (l_i), -1 = interval ends (r_i) */
} Event;

int cmpEvent(const void *a, const void *b) {
    const Event *ea = (const Event *)a;
    const Event *eb = (const Event *)b;
    if (ea->time != eb->time) return ea->time - eb->time;
    /* Tie-break: starts (+1) before ends (-1) so a shared endpoint
       is counted as covered by both intervals. Descending delta puts
       +1 before -1. */
    return eb->delta - ea->delta;
}

/*
 * l[], r[] : arrays of size n with left/right endpoints (inclusive).
 * Returns a point of maximum coverage and writes the coverage count
 * to *maxCount.
 */
int findMaxCoveragePoint(const int *l, const int *r, int n, int *maxCount) {
    Event *events = (Event *)malloc(2 * n * sizeof(Event));
    for (int i = 0; i < n; i++) {
        events[2 * i]     = (Event){l[i], +1};
        events[2 * i + 1] = (Event){r[i], -1};
    }

    qsort(events, 2 * n, sizeof(Event), cmpEvent);

    int count = 0, best = 0, bestPoint = events[0].time;
    for (int i = 0; i < 2 * n; i++) {
        count += events[i].delta;
        if (count > best) {
            best = count;
            bestPoint = events[i].time;
        }
    }

    free(events);
    *maxCount = best;
    return bestPoint;
}

void runTest(const char *label, const int *l, const int *r, int n) {
    printf("--- %s ---\n", label);
    printf("Intervals: ");
    for (int i = 0; i < n; i++) printf("(%d,%d) ", l[i], r[i]);
    printf("\n");

    int maxCount;
    int point = findMaxCoveragePoint(l, r, n, &maxCount);
    printf("Max coverage point: p = %d, covered by %d interval(s)\n\n",
           point, maxCount);
}

int main(void) {
    /* Test 1: exact example from the lab sheet.
       Expected: max coverage = 3 (e.g. p = 50 covers (20,60),(50,90),(15,70)) */
    int l1[] = {10, 20, 50, 15};
    int r1[] = {40, 60, 90, 70};
    runTest("Test 1: lab sheet example (expect coverage 3)", l1, r1, 4);

    /* Test 2: disjoint intervals -> max coverage is always 1 */
    int l2[] = {1, 10, 20};
    int r2[] = {5, 15, 25};
    runTest("Test 2: disjoint intervals", l2, r2, 3);

    /* Test 3: all intervals identical -> full overlap everywhere inside */
    int l3[] = {5, 5, 5};
    int r3[] = {10, 10, 10};
    runTest("Test 3: identical intervals", l3, r3, 3);

    /* Test 4: touching endpoints (one ends where next starts) should
       count as covered by BOTH at that shared point. */
    int l4[] = {1, 5, 10};
    int r4[] = {5, 10, 15};
    runTest("Test 4: touching endpoints (expect coverage 2 at p=5 or p=10)", l4, r4, 3);

    /* Test 5: single interval */
    int l5[] = {100};
    int r5[] = {200};
    runTest("Test 5: single interval", l5, r5, 1);

    return 0;
}
