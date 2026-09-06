/*
 * DAA Lab-06, Question 1
 * 1D array operations and their worst-case complexities.
 *
 * Input representation: a statically/dynamically allocated int array
 * arr[0..n-1], plus its size n (an "unsorted array" ADT).
 *
 * Each function is instrumented with a global operation counter so that
 * the empirically measured op-count can be compared against the derived
 * theoretical worst-case complexity (printed after every run).
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

static long long ops = 0;   /* operation counter, reset before every call */
#define TICK() (ops++)

/* ---------- (i) Maximum element ----------------------------------- O(n) */
int find_max(int *a, int n) {
    ops = 0;
    int max = a[0]; TICK();
    for (int i = 1; i < n; i++) { TICK(); if (a[i] > max) max = a[i]; }
    return max;
}

/* ---------- (ii) First and second largest -------------------------- O(n) */
void first_second_largest(int *a, int n, int *first, int *second) {
    ops = 0;
    if (a[0] > a[1]) { *first = a[0]; *second = a[1]; } else { *first = a[1]; *second = a[0]; }
    TICK();
    for (int i = 2; i < n; i++) {
        TICK();
        if (a[i] > *first) { *second = *first; *first = a[i]; }
        else if (a[i] > *second) { *second = a[i]; }
    }
}

/* ---------- (iii) Mean ---------------------------------------------- O(n) */
double find_mean(int *a, int n) {
    ops = 0;
    long long sum = 0;
    for (int i = 0; i < n; i++) { TICK(); sum += a[i]; }
    return (double) sum / n;
}

/* ---------- (iv) Median ----------------------------------------- O(n) avg
 *   Using quickselect (randomized): expected O(n), worst case O(n^2).
 *   (A worst-case-O(n) median-of-medians selection also exists.)      */
static int partition_lomuto(int *a, int lo, int hi) {
    int pivot_idx = lo + rand() % (hi - lo + 1);
    int pivot = a[pivot_idx];
    int tmp = a[pivot_idx]; a[pivot_idx] = a[hi]; a[hi] = tmp;
    int store = lo;
    for (int i = lo; i < hi; i++) {
        TICK();
        if (a[i] < pivot) { tmp = a[i]; a[i] = a[store]; a[store] = tmp; store++; }
    }
    tmp = a[store]; a[store] = a[hi]; a[hi] = tmp;
    return store;
}
static int quickselect(int *a, int lo, int hi, int k) {
    if (lo == hi) return a[lo];
    int p = partition_lomuto(a, lo, hi);
    if (k == p) return a[p];
    else if (k < p) return quickselect(a, lo, p - 1, k);
    else return quickselect(a, p + 1, hi, k);
}
double find_median(int *a, int n) {
    ops = 0;
    int *tmp = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) tmp[i] = a[i];
    double result;
    if (n % 2 == 1) {
        result = quickselect(tmp, 0, n - 1, n / 2);
    } else {
        int lo = quickselect(tmp, 0, n - 1, n / 2 - 1);
        for (int i = 0; i < n; i++) tmp[i] = a[i];
        int hi = quickselect(tmp, 0, n - 1, n / 2);
        result = (lo + hi) / 2.0;
    }
    free(tmp);
    return result;
}

/* ---------- (v) Standard deviation ---------------------------------- O(n) */
double find_stddev(int *a, int n) {
    ops = 0;
    double mean = find_mean(a, n);
    double sumsq = 0;
    for (int i = 0; i < n; i++) { TICK(); sumsq += (a[i] - mean) * (a[i] - mean); }
    return sqrt(sumsq / n);
}

/* ---------- (vi) Mode --------------------------------------- O(n^2) worst
 *  Without hashing / without a known value range, only pairwise counting
 *  is possible on an *unsorted* array -> O(n^2).
 *  (If a bounded range or a hash table is allowed, this drops to O(n).)  */
int find_mode(int *a, int n) {
    ops = 0;
    int best_val = a[0], best_count = 0;
    for (int i = 0; i < n; i++) {
        int count = 0;
        for (int j = 0; j < n; j++) { TICK(); if (a[j] == a[i]) count++; }
        if (count > best_count) { best_count = count; best_val = a[i]; }
    }
    return best_val;
}

/* ---------- (vii) Removing duplicates ------------------------------ O(n^2)
 *  In-place on an unsorted array without extra structures: for every
 *  element, scan the already-kept prefix -> O(n^2). (O(n) expected with
 *  a hash set, O(n log n) if sorting is allowed first.)                */
int remove_duplicates(int *a, int n) {
    ops = 0;
    int write = 1; /* a[0] is trivially unique */
    for (int i = 1; i < n; i++) {
        int is_dup = 0;
        for (int j = 0; j < write; j++) {
            TICK();
            if (a[j] == a[i]) { is_dup = 1; break; }
        }
        if (!is_dup) a[write++] = a[i];
    }
    return write; /* new length */
}

/* ---------- (viii) Reversing the array ------------------------------ O(n) */
void reverse_array(int *a, int n) {
    ops = 0;
    int lo = 0, hi = n - 1;
    while (lo < hi) { TICK(); int t = a[lo]; a[lo] = a[hi]; a[hi] = t; lo++; hi--; }
}

/* ---------- (ix) Partition around a random pivot -------------------- O(n)
 *  Rearranges the array in place so all elements >= pivot come first,
 *  followed by all elements < pivot (single linear Lomuto-style pass).  */
int partition_ge_pivot(int *a, int n) {
    ops = 0;
    int pivot_idx = rand() % n;
    int pivot = a[pivot_idx];
    int t = a[pivot_idx]; a[pivot_idx] = a[n - 1]; a[n - 1] = t; /* stash pivot at end */
    int store = 0;
    for (int i = 0; i < n - 1; i++) {
        TICK();
        if (a[i] >= pivot) { t = a[i]; a[i] = a[store]; a[store] = t; store++; }
    }
    t = a[store]; a[store] = a[n - 1]; a[n - 1] = t; /* drop pivot into place */
    return store; /* index boundary: [0,store) >= pivot, rest < pivot */
}

/* ------------------------------- driver ---------------------------- */
void print_arr(const char *label, int *a, int n) {
    printf("%s: [", label);
    for (int i = 0; i < n; i++) printf("%d%s", a[i], i + 1 < n ? ", " : "");
    printf("]\n");
}

int main(void) {
    srand((unsigned) time(NULL));
    int n = 10;
    int base[10] = {7, 2, 9, 4, 4, 1, 9, 5, 2, 8};
    int a[10];

    for (int i = 0; i < n; i++) a[i] = base[i];
    print_arr("Input", a, n);

    int maxv = find_max(a, n); long long ops_max = ops;
    printf("\n(i) Max = %d              [ops=%lld, theory O(n)]\n", maxv, ops_max);

    int f, s;
    first_second_largest(a, n, &f, &s); long long ops_fs = ops;
    printf("(ii) First=%d Second=%d   [ops=%lld, theory O(n)]\n", f, s, ops_fs);

    double meanv = find_mean(a, n); long long ops_mean = ops;
    printf("(iii) Mean = %.2f         [ops=%lld, theory O(n)]\n", meanv, ops_mean);
    printf("(iv) Median = %.2f        [theory O(n) expected via quickselect]\n", find_median(a, n));
    double sdv = find_stddev(a, n); long long ops_sd = ops;
    printf("(v) StdDev = %.4f        [ops=%lld, theory O(n)]\n", sdv, ops_sd);
    int modev = find_mode(a, n); long long ops_mode = ops;
    printf("(vi) Mode = %d            [ops=%lld, theory O(n^2)]\n", modev, ops_mode);

    for (int i = 0; i < n; i++) a[i] = base[i];
    int newlen = remove_duplicates(a, n); long long ops_dedup = ops;
    printf("(vii) After dedup (len=%d) [ops=%lld, theory O(n^2)]: ", newlen, ops_dedup);
    print_arr("", a, newlen);

    for (int i = 0; i < n; i++) a[i] = base[i];
    reverse_array(a, n); long long ops_rev = ops;
    printf("(viii) Reversed            [ops=%lld, theory O(n)]: ", ops_rev);
    print_arr("", a, n);

    for (int i = 0; i < n; i++) a[i] = base[i];
    int boundary = partition_ge_pivot(a, n); long long ops_part = ops;
    printf("(ix) Partitioned (>=pivot first, boundary=%d) [ops=%lld, theory O(n)]: ", boundary, ops_part);
    print_arr("", a, n);

    return 0;
}
