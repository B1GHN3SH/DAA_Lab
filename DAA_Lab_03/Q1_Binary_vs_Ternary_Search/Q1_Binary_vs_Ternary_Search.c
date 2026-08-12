#include <stdio.h>
#include <stdlib.h>
// Binary search makes 1 comparison per level and halves the range, so its recurrence is T(n) = T(n/2) + O(1) → O(log₂n). Ternary search makes up to 2 comparisons per level but only cuts the range to a third, giving T(n) = T(n/3) + O(1) → the total work is proportional to 2·log₃n. Converting to the same base: 2·log₃n = (2/log₂3)·log₂n ≈ 1.26·log₂n. So ternary search does more comparisons overall

int binarySearch(int a[], int lo, int hi, int x, long *cmp)
{
  while (lo <= hi)
  {
    int mid = lo + (hi - lo) / 2;
    (*cmp)++;
    if (a[mid] == x)
      return mid;
    (*cmp)++;
    if (a[mid] < x)
      lo = mid + 1;
    else
      hi = mid - 1;
  }
  return -1;
}

int ternarySearch(int a[], int lo, int hi, int x, long *cmp)
{
  while (lo <= hi)
  {
    int m1 = lo + (hi - lo) / 3;
    int m2 = hi - (hi - lo) / 3;
    (*cmp)++;
    if (a[m1] == x)
      return m1;
    (*cmp)++;
    if (a[m2] == x)
      return m2;
    (*cmp)++;
    if (x < a[m1])
      hi = m1 - 1;
    else
    {
      (*cmp)++;
      if (x > a[m2])
        lo = m2 + 1;
      else
      {
        lo = m1 + 1;
        hi = m2 - 1;
      }
    }
  }
  return -1;
}

int main(void)
{
  int n = 100000;
  static int a[100000];
  for (int i = 0; i < n; i++)
    a[i] = i * 2;

  long totalB = 0, totalT = 0;
  srand(42);
  for (int i = 0; i < 5000; i++)
  {
    int x = a[rand() % n];
    long cb = 0, ct = 0;
    binarySearch(a, 0, n - 1, x, &cb);
    ternarySearch(a, 0, n - 1, x, &ct);
    totalB += cb;
    totalT += ct;
  }
  printf("Average binary comparisons  = %.3f\n", (double)totalB / 5000);
  printf("Average ternary comparisons = %.3f\n", (double)totalT / 5000);
  return 0;
}