/*
 * DAA Lab-01, Q6: Element uniqueness
 * ---------------------------------------
 * Given n random numbers, check if there are any duplicates.
 *
 * Three approaches, increasing in efficiency:
 *   (i)   Brute force: compare every pair -> O(n^2)
 *   (ii)  Sort then scan adjacent elements -> O(n log n)
 *   (iii) Hash set (bucket-based) -> O(n) average time, O(n) space
 *
 * We implement all three, time them, and compare.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// (i) Brute force O(n^2)
int has_duplicate_bruteforce(int *arr, int n) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] == arr[j]) return 1;
    return 0;
}

// (ii) Sort + linear scan O(n log n)
int cmp_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}
int has_duplicate_sorting(int *arr, int n) {
    int *tmp = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) tmp[i] = arr[i];
    qsort(tmp, n, sizeof(int), cmp_int);
    int found = 0;
    for (int i = 0; i < n - 1; i++) {
        if (tmp[i] == tmp[i + 1]) { found = 1; break; }
    }
    free(tmp);
    return found;
}

// (iii) Hash set O(n) average -- simple chaining hash table
typedef struct Node { int value; struct Node *next; } Node;

int has_duplicate_hashing(int *arr, int n, int range) {
    int table_size = n * 2 + 1; // keep load factor reasonable
    Node **table = calloc(table_size, sizeof(Node *));
    int found = 0;

    for (int i = 0; i < n && !found; i++) {
        int v = arr[i];
        unsigned idx = ((unsigned)v) % table_size;
        Node *cur = table[idx];
        while (cur) {
            if (cur->value == v) { found = 1; break; }
            cur = cur->next;
        }
        if (!found) {
            Node *node = malloc(sizeof(Node));
            node->value = v;
            node->next = table[idx];
            table[idx] = node;
        }
    }

    // free table
    for (int i = 0; i < table_size; i++) {
        Node *cur = table[i];
        while (cur) { Node *next = cur->next; free(cur); cur = next; }
    }
    free(table);
    return found;
}

void fill_unique_random(int *arr, int n, int range) {
    // fill with a random permutation-like set (guaranteed unique)
    for (int i = 0; i < n; i++) arr[i] = i;
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
    }
}

int main(void) {
    srand((unsigned)time(NULL));

    int sizes[] = {100, 1000, 5000, 10000, 20000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("%-8s %-12s %-12s %-12s %-10s %-10s %-10s\n",
           "n", "BruteForce", "Sorting", "Hashing",
           "t_brute(s)", "t_sort(s)", "t_hash(s)");

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        int *arr = malloc(n * sizeof(int));
        fill_unique_random(arr, n, n); // all-unique array (worst case: must check everything)

        clock_t t0 = clock();
        int r1 = has_duplicate_bruteforce(arr, n);
        clock_t t1 = clock();
        int r2 = has_duplicate_sorting(arr, n);
        clock_t t2 = clock();
        int r3 = has_duplicate_hashing(arr, n, n);
        clock_t t3 = clock();

        double time_brute = (double)(t1 - t0) / CLOCKS_PER_SEC;
        double time_sort  = (double)(t2 - t1) / CLOCKS_PER_SEC;
        double time_hash  = (double)(t3 - t2) / CLOCKS_PER_SEC;

        printf("%-8d %-12d %-12d %-12d %-10.5f %-10.5f %-10.5f\n",
               n, r1, r2, r3, time_brute, time_sort, time_hash);

        free(arr);
    }

    printf("\nConclusion: for small n, brute force O(n^2) is fine, but its\n");
    printf("runtime blows up quickly as n grows. Sorting-based O(n log n)\n");
    printf("scales far better, and hashing O(n) average is fastest for\n");
    printf("large n at the cost of extra memory for the hash table. For\n");
    printf("sufficiently large n, brute force becomes impractical while\n");
    printf("sorting and hashing remain feasible.\n");

    return 0;
}
