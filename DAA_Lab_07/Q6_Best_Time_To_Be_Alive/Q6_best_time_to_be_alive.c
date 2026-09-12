/*
 * Q6. The best time to be alive
 * --------------------------------
 * Given the birth and death year of every prominent (dead) scientist
 * in the book's index, find the year (or year-range) in which the
 * largest number of them were simultaneously alive.
 * Tie rule: if person A died the same year person B was born, A's
 * death is considered to occur BEFORE B's birth that year (so they
 * are NOT counted as alive together at that instant).
 *
 * ALGORITHM (classic sweep line / max point-coverage of intervals)
 * -------------------------------------------------------------------
 * Each scientist with (birth b, death d) contributes:
 *      a "+1" event at year b   (alive count increases)
 *      a "-1" event at year d+1 (alive count decreases -- one year
 *                                 AFTER death, since they were alive
 *                                 during the death year itself)
 * OR, equivalently and more directly matching the problem's tie rule
 * (death-before-birth on ties), we can use events at exactly (b,+1)
 * and (d,-1) and, when sorting, process ALL "-1" (death) events at a
 * given year BEFORE any "+1" (birth) events at that same year. This
 * directly encodes "if A died the same year B was born, A's death
 * happens first" -- so at the instant B is counted alive, A is
 * already not counted.
 *
 * Steps:
 *   1. Build 2n events: (year=b_i, type=+1) and (year=d_i, type=-1).
 *   2. Sort events by year; break ties by processing -1 before +1.
 *   3. Sweep through events in order, maintaining a running count of
 *      "currently alive"; apply +1 events by incrementing count, and
 *      after each +1 event, compare the running count against the
 *      best-seen maximum (deaths never increase the count, so the
 *      maximum can only occur right after a birth event is applied).
 *   4. Track the year at which the maximum first occurs.
 *
 * COMPLEXITY
 * ----------
 *  Building events   : O(n)
 *  Sorting events     : O(n log n)
 *  Sweep               : O(n)
 *  Total               : O(n log n) time, O(n) space
 *  (This dominates the naive O(n * range-of-years) approach, which
 *   would be O(n * R) where R is the span of years involved.)
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int year;
    int type; /* -1 = death event, +1 = birth event */
} Event;

/* Comparator: sort by year; on a tie, death(-1) events come before
 * birth(+1) events, per the problem's stated tie-breaking rule.      */
static int cmp_event(const void *a, const void *b) {
    const Event *ea = a, *eb = b;
    if (ea->year != eb->year) return ea->year - eb->year;
    return ea->type - eb->type; /* -1 < +1, so deaths sort first */
}

int main(int argc, char *argv[]) {
    int n;
    int *birth, *death;

    if (argc > 1) {
        /* read from a file: "birth death" per line */
        FILE *f = fopen(argv[1], "r");
        if (!f) { perror("fopen"); return 1; }
        int cap = 16; n = 0;
        birth = malloc(cap * sizeof(int));
        death = malloc(cap * sizeof(int));
        int b, d;
        while (fscanf(f, "%d %d", &b, &d) == 2) {
            if (n == cap) { cap *= 2; birth = realloc(birth, cap*sizeof(int)); death = realloc(death, cap*sizeof(int)); }
            birth[n] = b; death[n] = d; n++;
        }
        fclose(f);
    } else {
        /* Demo data set: (birth, death) pairs of well known scientists
         * (approximate, illustrative only) */
        int demoB[] = {1473, 1564, 1571, 1546, 1643, 1564, 1642, 1596, 1707, 1643, 1867, 1856, 1879};
        int demoD[] = {1543, 1642, 1630, 1601, 1727, 1642, 1727, 1650, 1783, 1727, 1934, 1943, 1955};
        n = sizeof(demoB) / sizeof(demoB[0]);
        birth = malloc(n * sizeof(int));
        death = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) { birth[i] = demoB[i]; death[i] = demoD[i]; }
        printf("(no input file given -- using a small built-in demo data set of %d scientists)\n", n);
    }

    Event *ev = malloc(2 * n * sizeof(Event));
    for (int i = 0; i < n; i++) {
        ev[2*i]   = (Event){ birth[i], +1 };
        ev[2*i+1] = (Event){ death[i], -1 };
    }
    qsort(ev, 2 * n, sizeof(Event), cmp_event);

    int count = 0, best = 0, bestYear = 0;
    for (int i = 0; i < 2 * n; i++) {
        count += ev[i].type;
        if (ev[i].type == +1 && count > best) {
            best = count;
            bestYear = ev[i].year;
        }
    }

    printf("Number of scientists (intervals): %d\n", n);
    printf("Maximum number alive at once     : %d\n", best);
    printf("Achieved starting in year         : %d\n", bestYear);

    printf("\nEvent trace (year, delta, running count):\n");
    count = 0;
    for (int i = 0; i < 2 * n; i++) {
        count += ev[i].type;
        printf("  year %5d  %+d  -> alive=%d%s\n", ev[i].year, ev[i].type, count,
               (ev[i].type == +1 && count == best) ? "   <-- max" : "");
    }

    free(birth); free(death); free(ev);
    return 0;
}
