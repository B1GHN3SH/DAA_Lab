/*
 * DAA Lab-02, Q2: Merge sort vs. Modified (3-way) merge sort
 * ------------------------------------------------------------
 * Standard merge sort:   T(n) = 2T(n/2) + O(n)  -> O(n log2 n)
 * 3-way merge sort:      T(n) = 3T(n/3) + O(n)  -> O(n log3 n) = O(n log n)
 *
 * Both are O(n log n) asymptotically; this benchmark compares actual
 * wall-clock constants.
 *
 * Writes CSV: n,method,time_us
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* ---------------- Standard 2-way merge sort ---------------- */
void merge2(int *arr, int l, int m, int r, int *tmp){
    int i = l, j = m+1, k = l;
    while (i <= m && j <= r) tmp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    while (i <= m) tmp[k++] = arr[i++];
    while (j <= r) tmp[k++] = arr[j++];
    for (int x = l; x <= r; x++) arr[x] = tmp[x];
}
void mergesort2(int *arr, int l, int r, int *tmp){
    if (l >= r) return;
    int m = l + (r-l)/2;
    mergesort2(arr, l, m, tmp);
    mergesort2(arr, m+1, r, tmp);
    merge2(arr, l, m, r, tmp);
}

/* ---------------- 3-way merge sort ---------------- */
void merge3(int *arr, int l, int m1, int m2, int r, int *tmp){
    int i = l, j = m1+1, k = m2+1, idx = l;
    while (i <= m1 && j <= m2 && k <= r){
        if (arr[i] <= arr[j] && arr[i] <= arr[k]) tmp[idx++] = arr[i++];
        else if (arr[j] <= arr[i] && arr[j] <= arr[k]) tmp[idx++] = arr[j++];
        else tmp[idx++] = arr[k++];
    }
    while (i <= m1 && j <= m2) tmp[idx++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    while (j <= m2 && k <= r)  tmp[idx++] = (arr[j] <= arr[k]) ? arr[j++] : arr[k++];
    while (i <= m1 && k <= r)  tmp[idx++] = (arr[i] <= arr[k]) ? arr[i++] : arr[k++];
    while (i <= m1) tmp[idx++] = arr[i++];
    while (j <= m2) tmp[idx++] = arr[j++];
    while (k <= r)  tmp[idx++] = arr[k++];
    for (int x = l; x <= r; x++) arr[x] = tmp[x];
}
void mergesort3(int *arr, int l, int r, int *tmp){
    if (l >= r) return;
    int len = r - l + 1;
    if (len < 3){ /* base case: fall back to 2-way merge for tiny ranges */
        if (len == 2 && arr[l] > arr[r]){ int t=arr[l]; arr[l]=arr[r]; arr[r]=t; }
        return;
    }
    int third = len / 3;
    int m1 = l + third - 1;
    int m2 = l + 2*third - 1;
    mergesort3(arr, l, m1, tmp);
    mergesort3(arr, m1+1, m2, tmp);
    mergesort3(arr, m2+1, r, tmp);
    merge3(arr, l, m1, m2, r, tmp);
}

int* make_random_array(int n, unsigned seed){
    int *a = malloc(sizeof(int) * n);
    srand(seed);
    for (int i = 0; i < n; i++) a[i] = rand() % 1000000;
    return a;
}

int is_sorted(int *a, int n){
    for (int i = 1; i < n; i++) if (a[i-1] > a[i]) return 0;
    return 1;
}

int main(void){
    int sizes[] = {1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000};
    int nsizes = sizeof(sizes)/sizeof(sizes[0]);
    const int TRIALS = 5;

    FILE *fp = fopen("q2_mergesort_benchmark.csv", "w");
    fprintf(fp, "n,method,avg_time_us\n");

    for (int s = 0; s < nsizes; s++){
        int n = sizes[s];
        int *tmp = malloc(sizeof(int) * n);
        double total2 = 0, total3 = 0;

        for (int t = 0; t < TRIALS; t++){
            int *a2 = make_random_array(n, 100 + t);
            int *a3 = malloc(sizeof(int) * n);
            memcpy(a3, a2, sizeof(int)*n);

            clock_t c0 = clock();
            mergesort2(a2, 0, n-1, tmp);
            clock_t c1 = clock();
            total2 += (double)(c1-c0) / CLOCKS_PER_SEC * 1e6;
            if (!is_sorted(a2, n)) fprintf(stderr, "ERROR: mergesort2 failed at n=%d\n", n);

            clock_t c2 = clock();
            mergesort3(a3, 0, n-1, tmp);
            clock_t c3 = clock();
            total3 += (double)(c3-c2) / CLOCKS_PER_SEC * 1e6;
            if (!is_sorted(a3, n)) fprintf(stderr, "ERROR: mergesort3 failed at n=%d\n", n);

            free(a2); free(a3);
        }

        fprintf(fp, "%d,MergeSort2Way,%.2f\n", n, total2/TRIALS);
        fprintf(fp, "%d,MergeSort3Way,%.2f\n", n, total3/TRIALS);
        printf("n=%d  2-way=%.1fus  3-way=%.1fus\n", n, total2/TRIALS, total3/TRIALS);

        free(tmp);
    }

    fclose(fp);
    printf("Done. Wrote q2_mergesort_benchmark.csv\n");
    return 0;
}
