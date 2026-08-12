#include <stdio.h>
/*Pseudocode:

SELECTION-SORT(A, n)
  for i = 1 to n-1
      min_idx = i
      for j = i+1 to n
          if A[j] < A[min_idx]: min_idx = j
      exchange A[i] with A[min_idx]

Loop invariant (at the start of each outer iteration i): A[1..i−1] contains the (i−1) smallest elements of the array, in sorted order.

Initialization: before i=1, A[1..0] is empty — trivially true.
Maintenance: the inner loop finds the true minimum of the remaining A[i..n] and swaps it into position i, so A[1..i] now holds the i smallest elements sorted.
Termination: the loop stops after i=n−1, so A[1..n−1] holds the n−1 smallest elements sorted — the single leftover element A[n] is necessarily the largest, so it's already correctly placed. That's why the loop only needs to run n−1 times, not n: there's nothing left to compare the last element against.

Running time: the inner loop always scans the entire remaining subarray regardless of whether it's already sorted — there's no early exit. So both worst and best case are Θ(n²); best case is not better.*/

long comparisons = 0;

void selectionSort(int a[], int n)
{
  for (int i = 0; i < n - 1; i++)
  {
    int minIdx = i;
    for (int j = i + 1; j < n; j++)
    {
      comparisons++;
      if (a[j] < a[minIdx])
        minIdx = j;
    }
    int tmp = a[i];
    a[i] = a[minIdx];
    a[minIdx] = tmp;
  }
}

int main(void)
{
  int a1[] = {5, 3, 8, 1, 9, 2};
  int n = 6;
  selectionSort(a1, n);
  printf("Comparisons (random input): %ld\n", comparisons); // 15 = n(n-1)/2

  int a2[] = {1, 2, 3, 4, 5, 6};
  comparisons = 0;
  selectionSort(a2, n);
  printf("Comparisons (sorted input): %ld\n", comparisons); // also 15
  return 0;
}