/*
 * DAA Lab-02, Q3: Merging k sorted arrays, each of size n
 * ----------------------------------------------------------
 * Method 1 (sequential): merge array1+array2, then merge that result
 *   with array3, then array4, ... -> O(n k^2)
 * Method 2 (pairwise/tournament): merge k arrays in pairs, repeat on
 *   the k/2 results, etc. (like merge sort's combine step) -> O(nk log k)
 *
 * Two experiments, both written to one CSV:
 *   experiment=A: fix n, vary k
 *   experiment=B: fix k, vary n
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int* merge_two(int *a, int na, int *b, int nb){
    int *out = malloc(sizeof(int) * (na + nb));
    int i=0, j=0, k=0;
    while (i < na && j < nb) out[k++] = (a[i] <= b[j]) ? a[i++] : b[j++];
    while (i < na) out[k++] = a[i++];
    while (j < nb) out[k++] = b[j++];
    return out;
}

/* Method 1: sequential merging */
int* method1_sequential(int **arrays, int k, int n, int *out_len){
    int *result = malloc(sizeof(int) * n);
    memcpy(result, arrays[0], sizeof(int) * n);
    int result_len = n;
    for (int i = 1; i < k; i++){
        int *merged = merge_two(result, result_len, arrays[i], n);
        free(result);
        result = merged;
        result_len += n;
    }
    *out_len = result_len;
    return result;
}

/* Method 2: pairwise (tournament) merging */
int* method2_pairwise(int **arrays, int k, int n, int *out_len){
    /* copy pointers/lengths into working lists we can halve each round */
    int **cur = malloc(sizeof(int*) * k);
    int *lens = malloc(sizeof(int) * k);
    for (int i = 0; i < k; i++){
        cur[i] = malloc(sizeof(int) * n);
        memcpy(cur[i], arrays[i], sizeof(int) * n);
        lens[i] = n;
    }
    int count = k;
    while (count > 1){
        int newCount = (count + 1) / 2;
        int **next = malloc(sizeof(int*) * newCount);
        int *nextLens = malloc(sizeof(int) * newCount);
        int idx = 0;
        for (int i = 0; i < count; i += 2){
            if (i + 1 < count){
                next[idx] = merge_two(cur[i], lens[i], cur[i+1], lens[i+1]);
                nextLens[idx] = lens[i] + lens[i+1];
                free(cur[i]); free(cur[i+1]);
            } else {
                next[idx] = cur[i];       /* odd one out carries forward */
                nextLens[idx] = lens[i];
            }
            idx++;
        }
        free(cur); free(lens);
        cur = next; lens = nextLens; count = newCount;
    }
    int *result = cur[0];
    *out_len = lens[0];
    free(cur); free(lens);
    return result;
}

int** make_sorted_arrays(int k, int n, unsigned seed){
    srand(seed);
    int **arrays = malloc(sizeof(int*) * k);
    for (int i = 0; i < k; i++){
        int *a = malloc(sizeof(int) * n);
        for (int j = 0; j < n; j++) a[j] = rand() % 1000000;
        /* sort each array (simple insertion sort fine for small n, else qsort) */
        for (int x = 1; x < n; x++){
            int key = a[x], y = x-1;
            while (y >= 0 && a[y] > key){ a[y+1] = a[y]; y--; }
            a[y+1] = key;
        }
        arrays[i] = a;
    }
    return arrays;
}

void free_arrays(int **arrays, int k){
    for (int i = 0; i < k; i++) free(arrays[i]);
    free(arrays);
}

int is_sorted(int *a, int n){
    for (int i = 1; i < n; i++) if (a[i-1] > a[i]) return 0;
    return 1;
}

int main(void){
    FILE *fp = fopen("q3_kway_merge_benchmark.csv", "w");
    fprintf(fp, "experiment,n,k,method,time_us\n");

    /* ---------- Experiment A: fix n, vary k ---------- */
    {
        int n = 1000;
        int ks[] = {2, 4, 8, 16, 32, 64, 128, 256};
        int nk = sizeof(ks)/sizeof(ks[0]);
        for (int idx = 0; idx < nk; idx++){
            int k = ks[idx];
            int **arrays = make_sorted_arrays(k, n, 777 + k);

            int len1;
            clock_t c0 = clock();
            int *r1 = method1_sequential(arrays, k, n, &len1);
            clock_t c1 = clock();
            double t1 = (double)(c1-c0)/CLOCKS_PER_SEC*1e6;
            if (!is_sorted(r1, len1)) fprintf(stderr, "ERROR method1 A k=%d\n", k);
            free(r1);

            int len2;
            clock_t c2 = clock();
            int *r2 = method2_pairwise(arrays, k, n, &len2);
            clock_t c3 = clock();
            double t2 = (double)(c3-c2)/CLOCKS_PER_SEC*1e6;
            if (!is_sorted(r2, len2)) fprintf(stderr, "ERROR method2 A k=%d\n", k);
            free(r2);

            fprintf(fp, "A_vary_k,%d,%d,Sequential,%.2f\n", n, k, t1);
            fprintf(fp, "A_vary_k,%d,%d,Pairwise,%.2f\n", n, k, t2);
            printf("[A] n=%d k=%d  sequential=%.1fus  pairwise=%.1fus\n", n, k, t1, t2);

            free_arrays(arrays, k);
        }
    }

    /* ---------- Experiment B: fix k, vary n ---------- */
    {
        int k = 32;
        int ns[] = {100, 500, 1000, 5000, 10000, 50000, 100000};
        int nn = sizeof(ns)/sizeof(ns[0]);
        for (int idx = 0; idx < nn; idx++){
            int n = ns[idx];
            int **arrays = make_sorted_arrays(k, n, 555 + n);

            int len1;
            clock_t c0 = clock();
            int *r1 = method1_sequential(arrays, k, n, &len1);
            clock_t c1 = clock();
            double t1 = (double)(c1-c0)/CLOCKS_PER_SEC*1e6;
            if (!is_sorted(r1, len1)) fprintf(stderr, "ERROR method1 B n=%d\n", n);
            free(r1);

            int len2;
            clock_t c2 = clock();
            int *r2 = method2_pairwise(arrays, k, n, &len2);
            clock_t c3 = clock();
            double t2 = (double)(c3-c2)/CLOCKS_PER_SEC*1e6;
            if (!is_sorted(r2, len2)) fprintf(stderr, "ERROR method2 B n=%d\n", n);
            free(r2);

            fprintf(fp, "B_vary_n,%d,%d,Sequential,%.2f\n", n, k, t1);
            fprintf(fp, "B_vary_n,%d,%d,Pairwise,%.2f\n", n, k, t2);
            printf("[B] n=%d k=%d  sequential=%.1fus  pairwise=%.1fus\n", n, k, t1, t2);

            free_arrays(arrays, k);
        }
    }

    fclose(fp);
    printf("Done. Wrote q3_kway_merge_benchmark.csv\n");
    return 0;
}
