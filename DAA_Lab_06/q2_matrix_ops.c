/*
 * DAA Lab-06, Question 2
 * n x n square matrix operations and their worst-case complexities.
 *
 * Input representation: a matrix is stored as a dynamically allocated
 * n x n array of doubles, accessed as a single contiguous block
 * (double *M, element (i,j) = M[i*n+j]) for cache-friendly access.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

static long long ops = 0;
#define TICK() (ops++)
#define AT(M, n, i, j) (M[(i) * (n) + (j)])

double *alloc_mat(int n) { return malloc((size_t) n * n * sizeof(double)); }

void print_mat(const char *label, double *M, int n) {
    printf("%s:\n", label);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) printf("%8.2f ", AT(M, n, i, j));
        printf("\n");
    }
}

/* ---------- (i) Matrix addition -------------------------------- O(n^2) */
double *mat_add(double *A, double *B, int n) {
    ops = 0;
    double *C = alloc_mat(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) { TICK(); AT(C, n, i, j) = AT(A, n, i, j) + AT(B, n, i, j); }
    return C;
}

/* ---------- (ii) Matrix multiplication -------------------------- O(n^3)
 *  (Naive triple loop. Strassen's algorithm gives O(n^2.807), and the
 *  current best known bound is ~O(n^2.371) - noted, but not required for
 *  the naive worst-case answer.)                                       */
double *mat_mul(double *A, double *B, int n) {
    ops = 0;
    double *C = alloc_mat(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            double sum = 0;
            for (int k = 0; k < n; k++) { TICK(); sum += AT(A, n, i, k) * AT(B, n, k, j); }
            AT(C, n, i, j) = sum;
        }
    return C;
}

/* ---------- (iii) Is zero matrix? ------------------- O(n^2) worst case
 *  (Early-exits as soon as a non-zero entry is found.)                 */
int is_zero_matrix(double *M, int n) {
    ops = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) { TICK(); if (AT(M, n, i, j) != 0.0) return 0; }
    return 1;
}

/* ---------- (iv) Is symmetric? ----------------------- O(n^2) worst case
 *  (Only need to check the upper triangle against the lower triangle.)  */
int is_symmetric(double *M, int n) {
    ops = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) { TICK(); if (AT(M, n, i, j) != AT(M, n, j, i)) return 0; }
    return 1;
}

/* ---------- (v) Determinant --------------------------------------- O(n^3)
 *  Via Gaussian elimination (LU-style forward elimination) with partial
 *  pivoting; det = product of pivots (with sign flips for row swaps).
 *  (Cofactor expansion would cost O(n!) - exponential, so elimination
 *  is the practical worst-case-polynomial choice.)                     */
double determinant(double *M, int n) {
    ops = 0;
    double *A = alloc_mat(n);
    for (int i = 0; i < n * n; i++) A[i] = M[i];
    double det = 1.0;
    for (int col = 0; col < n; col++) {
        int piv = col;
        for (int r = col + 1; r < n; r++) { TICK(); if (fabs(AT(A, n, r, col)) > fabs(AT(A, n, piv, col))) piv = r; }
        if (fabs(AT(A, n, piv, col)) < 1e-12) { free(A); return 0.0; }
        if (piv != col) {
            for (int c = 0; c < n; c++) { double t = AT(A, n, piv, c); AT(A, n, piv, c) = AT(A, n, col, c); AT(A, n, col, c) = t; }
            det = -det;
        }
        det *= AT(A, n, col, col);
        for (int r = col + 1; r < n; r++) {
            double factor = AT(A, n, r, col) / AT(A, n, col, col);
            for (int c = col; c < n; c++) { TICK(); AT(A, n, r, c) -= factor * AT(A, n, col, c); }
        }
    }
    free(A);
    return det;
}

/* ---------- (vi) Transpose in place ------------------------------- O(n^2)
 *  (Swap across the diagonal; only the upper triangle needs visiting.)  */
void transpose_in_place(double *M, int n) {
    ops = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) { TICK(); double t = AT(M, n, i, j); AT(M, n, i, j) = AT(M, n, j, i); AT(M, n, j, i) = t; }
}

/* ---------- (vii) Dominant eigenvalue / eigenvector --------------- O(k*n^2)
 *  Exact eigen-decomposition of a general n x n matrix has no closed
 *  form for n >= 5 (Abel-Ruffini) and standard practical algorithms
 *  (QR algorithm) cost O(n^3) per iteration, O(k*n^3) overall.
 *  Here we use the Power Method to obtain the dominant eigenpair:
 *  each iteration is one mat-vec product, O(n^2), for k iterations
 *  until convergence -> O(k*n^2).                                      */
double power_method(double *M, int n, double *eigenvector_out, int max_iter, double tol) {
    ops = 0;
    double *v = malloc(n * sizeof(double));
    double *w = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) v[i] = 1.0;
    double lambda_old = 0.0, lambda = 0.0;
    for (int it = 0; it < max_iter; it++) {
        for (int i = 0; i < n; i++) {
            double sum = 0;
            for (int j = 0; j < n; j++) { TICK(); sum += AT(M, n, i, j) * v[j]; }
            w[i] = sum;
        }
        double norm = 0;
        for (int i = 0; i < n; i++) norm += w[i] * w[i];
        norm = sqrt(norm);
        if (norm < 1e-14) break;
        for (int i = 0; i < n; i++) v[i] = w[i] / norm;
        lambda = 0;
        for (int i = 0; i < n; i++) lambda += v[i] * w[i];
        if (fabs(lambda - lambda_old) < tol) { lambda_old = lambda; break; }
        lambda_old = lambda;
    }
    for (int i = 0; i < n; i++) eigenvector_out[i] = v[i];
    free(v); free(w);
    return lambda_old;
}

/* ------------------------------- driver ---------------------------- */
int main(void) {
    int n = 3;
    double A[9] = {2, 0, 0,  0, 3, 4,  0, 4, 9};   /* symmetric */
    double B[9] = {1, 2, 3,  4, 5, 6,  7, 8, 10};

    print_mat("A", A, n);
    print_mat("B", B, n);

    double *C = mat_add(A, B, n); long long ops_add = ops;
    print_mat("A+B", C, n); printf("[ops=%lld, theory O(n^2)]\n\n", ops_add);
    free(C);

    C = mat_mul(A, B, n); long long ops_mul = ops;
    print_mat("A*B", C, n); printf("[ops=%lld, theory O(n^3)]\n\n", ops_mul);
    free(C);

    printf("is_zero(A) = %d          [ops=%lld, theory O(n^2)]\n", is_zero_matrix(A, n), ops);
    printf("is_symmetric(A) = %d     [ops=%lld, theory O(n^2)]\n", is_symmetric(A, n), ops);
    printf("is_symmetric(B) = %d     [ops=%lld, theory O(n^2)]\n", is_symmetric(B, n), ops);
    printf("det(B) = %.4f           [ops=%lld, theory O(n^3)]\n", determinant(B, n), ops);

    double T[9]; for (int i = 0; i < 9; i++) T[i] = B[i];
    transpose_in_place(T, n);
    print_mat("B transposed (in place)", T, n); printf("[ops=%lld, theory O(n^2)]\n\n", ops);

    double eigvec[3];
    double eigval = power_method(A, n, eigvec, 1000, 1e-10);
    printf("Dominant eigenvalue of A ~= %.6f  [theory O(k*n^2) for k iterations]\n", eigval);
    printf("Corresponding eigenvector ~= [%.4f, %.4f, %.4f]\n", eigvec[0], eigvec[1], eigvec[2]);

    return 0;
}
