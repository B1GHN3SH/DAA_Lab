#include <stdio.h>

/*All n coins are equal weight except possibly one, which is lighter (never heavier). Split the coins into two equal-size groups and weigh their totals:

Unequal → the lighter pan contains the defective coin (recurse only into that half — the other half is now proven genuine and discarded).
Equal → no defective coin exists in this range at all.

If the count is odd, you can't form two equal groups from everything, so set one coin aside, compare the remaining even split. If that's balanced, both halves are genuine — only the set-aside coin is still a suspect, so do one final single-coin-vs-single-coin weighing against a now-proven-genuine coin. Since the range strictly shrinks by roughly half each weighing, this takes ⌊log₂n⌋ + c weighings.*/

#define GENUINE 10.0
#define LIGHT_DELTA 1.0
long weighings = 0;

double rangeSum(double coins[], int l, int r)
{
  double s = 0;
  for (int i = l; i <= r; i++)
    s += coins[i];
  return s;
}

int weigh(double coins[], int l1, int r1, int l2, int r2)
{
  weighings++;
  double s1 = rangeSum(coins, l1, r1), s2 = rangeSum(coins, l2, r2);
  if (s1 < s2)
    return -1;
  if (s1 > s2)
    return 1;
  return 0;
}

int findDefective(double coins[], int low, int high)
{
  int n = high - low + 1;
  if (n == 1)
    return low; // reached only via a strictly-lighter split -> this is it

  int half = n / 2;
  if (n % 2 == 0)
  {
    int result = weigh(coins, low, low + half - 1, low + half, high);
    if (result == 0)
      return -1;
    if (result < 0)
      return findDefective(coins, low, low + half - 1);
    else
      return findDefective(coins, low + half, high);
  }
  else
  {
    int setAside = high;
    int leftStart = low, leftEnd = low + half - 1;
    int rightStart = low + half, rightEnd = low + 2 * half - 1;
    int result = weigh(coins, leftStart, leftEnd, rightStart, rightEnd);
    if (result < 0)
      return findDefective(coins, leftStart, leftEnd);
    if (result > 0)
      return findDefective(coins, rightStart, rightEnd);
    weighings++;
    if (coins[setAside] < coins[leftStart])
      return setAside;
    return -1;
  }
}

int main(void)
{
  int n = 13;
  double coins[13];
  for (int i = 0; i < n; i++)
    coins[i] = GENUINE;
  coins[7] -= LIGHT_DELTA;

  weighings = 0;
  int idx = findDefective(coins, 0, n - 1);
  printf("Defective coin index: %d, weighings used: %ld\n", idx, weighings);
  return 0;
}