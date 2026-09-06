/*
 * DAA Lab-06, Question 4
 * Sorting a permutation using reverse(p, i, j).
 *
 * Input representation: the permutation is stored as an int array
 * p[0..n-1] holding a permutation of {1,...,n}.
 *
 * ----------------------------------------------------------------------
 * PART A: O(n) reversals suffice  (see reversal_count_sort)
 * ----------------------------------------------------------------------
 * Claim: any permutation of size n can be sorted with at most n-1 calls
 * to reverse().
 *
 * Proof (constructive, "selection sort with reversal"):
 *   For i = 0 .. n-2:
 *     Let j = index of value (i+1) within the currently unsorted
 *     suffix p[i .. n-1]  (it exists by the pigeonhole principle, since
 *     p is a permutation of 1..n and values 1..i already sit correctly
 *     in positions 0..i-1 and are never touched again).
 *     Call reverse(p, i, j).
 *
 * Effect of reverse(p, i, j): the element that was at position j (namely
 * value i+1) moves to position i; the elements originally at
 * p[i..j-1] are shifted to p[i+1..j] in reverse order.
 *
 * Invariant: after the i-th call, p[0..i] = [1, 2, ..., i+1] and this
 * prefix is never modified by any subsequent call, because every later
 * call only touches indices >= i+1.
 *
 * After n-1 calls (i = 0 .. n-2), p[0..n-2] = [1,...,n-1], and since p is
 * a permutation, p[n-1] must equal n. Hence p is fully sorted.
 * Total reversal count = n-1 = O(n).  QED.
 *
 * (Note: this bound is on the *number* of reversals, not their summed
 * length/cost - a single reversal can itself have length O(n), so the
 * total *cost* of this simple algorithm can be Theta(n^2) in the worst
 * case. Part B below fixes the cost.)
 *
 * ----------------------------------------------------------------------
 * PART B: O(n log^2 n) total COST algorithm  (see cost_bounded_sort)
 * ----------------------------------------------------------------------
 * Cost(reverse(p,i,j)) = j - i + 1 (its length). We want total cost
 * (sum of lengths of all reversals used) to be O(n log^2 n).
 *
 * Strategy: Merge-sort structure, T(n) = 2*T(n/2) + Merge(n), where the
 * MERGE step itself is implemented using reversal-based ROTATIONS with
 * total cost O(n log n) (not O(n^2)):
 *
 *   rotate(p, i, mid, j):  cyclically rotate p[i..j] so that the block
 *   p[mid..j] comes before p[i..mid-1]. This is achieved with exactly
 *   THREE reversals:
 *        reverse(p, i, mid-1);  reverse(p, mid, j);  reverse(p, i, j);
 *   Cost of one rotate = (mid-i) + (j-mid+1) + (j-i+1) = O(2*(j-i+1)),
 *   i.e. linear in the touched range - each reversal call is legal
 *   (reverse(p,a,b) as given by the problem).
 *
 *   merge(p, lo, mid, hi):  merges two already-sorted runs
 *   p[lo..mid-1] and p[mid..hi] using the classic recursive
 *   "rotate + binary search" in-place merge (the same technique used by
 *   std::inplace_merge):
 *     - Base case: if either run is empty, or the run sizes total <= 1,
 *       return; if the two runs have total size 2, reverse if needed.
 *     - Otherwise pick the larger run, split it at its midpoint m1;
 *       binary-search the other (sorted) run for the position m2 where
 *       p[m1] would be inserted (this costs O(log n) comparisons, no
 *       array cost).
 *     - rotate(p, m1, mid, m2-1) to interleave the two blocks
 *       correctly - cost O(m2 - m1) <= O(hi-lo+1).
 *     - Recurse on the two now-independent halves.
 *   Because each level of this merge recursion touches every element
 *   at most O(1) times via a rotate, and recursion depth is O(log n),
 *   ONE merge of size s costs O(s log s).
 *
 * Overall recurrence for full sort:
 *   T(n) = 2 T(n/2) + O(n log n)        [merge-sort with O(n log n) merge]
 *        = O(n log^2 n)                  by the Akra-Bazzi / repeated
 *                                          substitution (same pattern as
 *                                          "T(n)=2T(n/2)+n log n").
 * This matches the required O(n log^2 n) TOTAL COST bound (sum of all
 * reversal lengths across the whole sort), while still using only
 * reverse() as the primitive operation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static long long reversal_count = 0;
static long long total_cost = 0;

/* the only primitive we're allowed: reverse p[i..j] inclusive */
void reverse(int *p, int i, int j) {
    reversal_count++;
    total_cost += (j - i + 1);
    while (i < j) { int t = p[i]; p[i] = p[j]; p[j] = t; i++; j--; }
}

int is_sorted(int *p, int n) {
    for (int i = 0; i < n - 1; i++) if (p[i] > p[i + 1]) return 0;
    return 1;
}

/* ---------------- PART A: O(n) reversals (selection-by-reversal) ------ */
void reversal_count_sort(int *p, int n) {
    for (int i = 0; i < n - 1; i++) {
        int j = i;
        for (int k = i; k < n; k++) if (p[k] == i + 1) { j = k; break; }
        if (j != i) reverse(p, i, j);
    }
}

/* ---------------- PART B: O(n log^2 n) cost (rotate + merge sort) -----
 * NOTE: this block uses HALF-OPEN intervals [first, last) throughout,
 * which is the standard, off-by-one-safe convention for rotate/merge.
 * reverse_range(p,first,last) reverses p[first..last-1] by calling the
 * mandated inclusive primitive reverse(p, first, last-1).               */
void reverse_range(int *p, int first, int last) {
    if (first < last) reverse(p, first, last - 1);
}

/* rotate [first,last) so that block [mid,last) ends up before
 * [first,mid) -- the classic "three reversals" trick.                   */
void rotate3(int *p, int first, int mid, int last) {
    if (first >= mid || mid >= last) return;
    reverse_range(p, first, mid);
    reverse_range(p, mid, last);
    reverse_range(p, first, last);
}

/* first index in half-open [first,last) (sorted ascending) with p[idx] >= key */
int lower_bound(int *p, int first, int last, int key) {
    int lo = first, hi = last;
    while (lo < hi) { int mid = lo + (hi - lo) / 2; if (p[mid] >= key) hi = mid; else lo = mid + 1; }
    return lo;
}
/* first index in half-open [first,last) (sorted ascending) with p[idx] > key */
int upper_bound(int *p, int first, int last, int key) {
    int lo = first, hi = last;
    while (lo < hi) { int mid = lo + (hi - lo) / 2; if (p[mid] > key) hi = mid; else lo = mid + 1; }
    return lo;
}

/* recursively merge sorted runs p[lo,mid) and p[mid,hi) in place using
 * rotate3() -- classic O((hi-lo) log(hi-lo)) rotate+binary-search merge
 * (the same idea used by std::inplace_merge).                           */
void merge_inplace(int *p, int lo, int mid, int hi) {
    int n1 = mid - lo, n2 = hi - mid;
    if (n1 == 0 || n2 == 0) return;
    if (n1 + n2 == 2) { if (p[lo] > p[mid]) reverse_range(p, lo, hi); return; }

    int m1, m2;
    if (n1 > n2) {
        m1 = lo + n1 / 2;
        m2 = lower_bound(p, mid, hi, p[m1]);
    } else {
        m2 = mid + n2 / 2;
        m1 = upper_bound(p, lo, mid, p[m2]);
    }
    rotate3(p, m1, mid, m2);
    int new_mid = m1 + (m2 - mid);
    merge_inplace(p, lo, m1, new_mid);
    merge_inplace(p, new_mid, m2, hi);
}

void cost_bounded_sort(int *p, int lo, int hi) { /* half-open [lo,hi) */
    if (hi - lo <= 1) return;
    int mid = lo + (hi - lo) / 2;
    cost_bounded_sort(p, lo, mid);
    cost_bounded_sort(p, mid, hi);
    merge_inplace(p, lo, mid, hi);
}

/* ------------------------------- driver ---------------------------- */
void print_arr(const char *label, int *a, int n) {
    printf("%s: [", label);
    for (int i = 0; i < n; i++) printf("%d%s", a[i], i + 1 < n ? ", " : "");
    printf("]\n");
}

void shuffle(int *a, int n) {
    for (int i = n - 1; i > 0; i--) { int j = rand() % (i + 1); int t = a[i]; a[i] = a[j]; a[j] = t; }
}

int main(void) {
    srand(42);
    int n = 16;

    /* ---- Part A demo ---- */
    int *p1 = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) p1[i] = i + 1;
    shuffle(p1, n);
    print_arr("Part A - input ", p1, n);
    reversal_count = 0; total_cost = 0;
    reversal_count_sort(p1, n);
    print_arr("Part A - output", p1, n);
    printf("Part A: sorted=%d, reversals used=%lld (bound n-1=%d), total cost=%lld\n\n",
           is_sorted(p1, n), reversal_count, n - 1, total_cost);
    free(p1);

    /* ---- Part B demo + cost scaling check across sizes ---- */
    int *p2 = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) p2[i] = i + 1;
    shuffle(p2, n);
    print_arr("Part B - input ", p2, n);
    reversal_count = 0; total_cost = 0;
    cost_bounded_sort(p2, 0, n);
    print_arr("Part B - output", p2, n);
    printf("Part B: sorted=%d, reversals used=%lld, total cost=%lld\n\n",
           is_sorted(p2, n), reversal_count, total_cost);
    free(p2);

    printf("Cost scaling of Part B algorithm across sizes (expect ~ n*log2(n)^2 growth):\n");
    printf("%8s %12s %16s %20s\n", "n", "total_cost", "n*log2(n)^2", "cost / (n log2n^2)");
    for (int size = 64; size <= 4096; size *= 2) {
        int *p = malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) p[i] = i + 1;
        shuffle(p, size);
        reversal_count = 0; total_cost = 0;
        cost_bounded_sort(p, 0, size);
        double lg = 0; int t = size; while (t > 1) { lg++; t >>= 1; }
        double bound = size * lg * lg;
        printf("%8d %12lld %16.0f %20.3f  sorted=%d\n",
               size, total_cost, bound, total_cost / bound, is_sorted(p, size));
        free(p);
    }
    return 0;
}
