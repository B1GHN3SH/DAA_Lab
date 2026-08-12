#include <stdio.h>
#include <stdlib.h>
/*Comparing elements pairwise first, then only comparing winners-to-winners and losers-to-losers, avoids redundant work. Recurrence: split into halves, recursively get (max,min) of each, then just 2 more comparisons combine them: T(n) = 2T(n/2) + 2. This solves to exactly 3n/2 − 2 comparisons when n is a power of 2 (vs. the naive 2n − 2)*/
long comparisons = 0;

void maxMin(int a[], int lo, int hi, int *mx, int *mn)
{
  if (lo == hi)
  {
    *mx = *mn = a[lo];
    return;
  }
  if (hi - lo == 1)
  {
    comparisons++;
    if (a[lo] < a[hi])
    {
      *mn = a[lo];
      *mx = a[hi];
    }
    else
    {
      *mn = a[hi];
      *mx = a[lo];
    }
    return;
  }
  int mid = (lo + hi) / 2, lmax, lmin, rmax, rmin;
  maxMin(a, lo, mid, &lmax, &lmin);
  maxMin(a, mid + 1, hi, &rmax, &rmin);
  comparisons++;
  *mx = (lmax > rmax) ? lmax : rmax;
  comparisons++;
  *mn = (lmin < rmin) ? lmin : rmin;
}

int main(void)
{
  int n = 1024;
  int *a = malloc(n * sizeof(int));
  srand(1);
  for (int i = 0; i < n; i++)
    a[i] = rand() % 1000000;
  int mx, mn;
  maxMin(a, 0, n - 1, &mx, &mn);
  printf("max=%d min=%d comparisons=%ld (bound 3n/2-2=%d)\n", mx, mn, comparisons, 3 * n / 2 - 2);
  return 0;
}