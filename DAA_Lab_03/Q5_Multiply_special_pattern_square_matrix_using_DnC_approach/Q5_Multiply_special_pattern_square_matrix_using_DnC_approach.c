#include <stdio.h>
#include <stdlib.h>
/*The matrix M = [[M1,M2],[M2,M1]] is recursive — each block M1, M2 has the same pattern all the way down to single elements. That's a strong constraint: it means the entire n×n matrix is completely determined by only n numbers, not n². (Count check: f(n) = 2f(n/2), f(1)=1 ⟹ f(n) = n.)

Key algebraic fact: if A = [[A1,A2],[A2,A1]] and B = [[B1,B2],[B2,B1]], then

A·B = [[A1B1+A2B2,  A1B2+A2B1],
      [A2B1+A1B2,  A2B2+A1B1]]

Both off-diagonal blocks equal (A1B2+A2B1), both diagonal blocks equal (A1B1+A2B2) — so the product is again the same pattern. That means we can represent every matrix in this problem just as a flat array of n numbers, and multiplication only needs to work on those n numbers, never expanding to n² entries.

Recurrence: 4 recursive multiplications of half-size + O(n/2) additions → T(n) = 4T(n/2) + O(n). By the master theorem (f(n)=O(n) is polynomially smaller than n^log₂4 = n²) this solves to T(n) = Θ(n²) — the compact representation is what saves us from the usual Θ(n² log n) or Θ(n³).*/

// leaves[0..n/2) describes block M1 (recursively), leaves[n/2..n) describes M2
void addLeaves(long long *A, long long *B, long long *C, int n)
{
  for (int i = 0; i < n; i++)
    C[i] = A[i] + B[i];
}

long long *multiplyStructured(long long *A, long long *B, int n)
{
  long long *C = malloc(n * sizeof(long long));
  if (n == 1)
  {
    C[0] = A[0] * B[0];
    return C;
  }
  int h = n / 2;
  long long *A1 = A, *A2 = A + h, *B1 = B, *B2 = B + h;

  long long *P11 = multiplyStructured(A1, B1, h);
  long long *P22 = multiplyStructured(A2, B2, h);
  long long *P12 = multiplyStructured(A1, B2, h);
  long long *P21 = multiplyStructured(A2, B1, h);

  addLeaves(P11, P22, C, h);     // diagonal blocks of result
  addLeaves(P12, P21, C + h, h); // off-diagonal blocks of result

  free(P11);
  free(P22);
  free(P12);
  free(P21);
  return C;
}

int main(void)
{
  int n = 8;
  long long leavesA[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  long long leavesB[8] = {2, 0, 1, 3, 1, 1, 0, 2};
  long long *C = multiplyStructured(leavesA, leavesB, n);
  printf("Result leaves: ");
  for (int i = 0; i < n; i++)
    printf("%lld ", C[i]);
  printf("\n");
  free(C);
  return 0;
}