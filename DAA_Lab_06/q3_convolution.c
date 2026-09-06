/*
 * DAA Lab-06, Question 3
 * Convolution C = A * B via the Fast Fourier Transform, O(n log n).
 *
 * Input representation: vectors A (length m) and B (length n), n >= m,
 * stored as arrays of complex numbers (real inputs, zero imaginary part).
 *
 * ----------------------------------------------------------------------
 * Why FFT gives O(n log n) divide-and-conquer convolution
 * ----------------------------------------------------------------------
 * Direct convolution: C[k] = sum_j A[j]*B[k-j] costs O(m*n) = O(n^2)
 * in the worst case (m up to n).
 *
 * The Convolution Theorem says pointwise multiplication in the
 * "frequency domain" equals convolution in the "time domain":
 *      C = A * B   <=>   FFT(C) = FFT(A) . FFT(B)   (pointwise product)
 * So:  C = IFFT( FFT(A) . FFT(B) )
 *
 * The FFT itself is a divide-and-conquer algorithm (Cooley-Tukey):
 * splitting a length-N sequence into even/odd indexed halves gives
 *      T(N) = 2T(N/2) + O(N)   =>   T(N) = O(N log N)
 *
 * Overall convolution algorithm:
 *   1. Pick N = smallest power of two with N >= m + n - 1 (to avoid
 *      wrap-around / "circular" aliasing -> "linear convolution").
 *   2. Zero-pad A and B to length N.                         O(N)
 *   3. FFT(A), FFT(B).                                        O(N log N)
 *   4. Pointwise multiply.                                    O(N)
 *   5. Inverse FFT.                                            O(N log N)
 *   Total: O(N log N) = O(n log n)  since N = Theta(n) when n >= m.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

typedef double complex cplx;
static const double PI = 3.14159265358979323846;

/* In-place iterative FFT (Cooley-Tukey, divide-and-conquer unrolled to
 * an iterative bit-reversal form). invert=1 computes the inverse FFT
 * (and the caller divides by N afterwards).                            */
void fft(cplx *a, int N, int invert) {
    /* bit-reversal permutation */
    for (int i = 1, j = 0; i < N; i++) {
        int bit = N >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) { cplx t = a[i]; a[i] = a[j]; a[j] = t; }
    }
    for (int len = 2; len <= N; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cplx wlen = cos(ang) + I * sin(ang);
        for (int i = 0; i < N; i += len) {
            cplx w = 1;
            for (int j = 0; j < len / 2; j++) {
                cplx u = a[i + j];
                cplx v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) for (int i = 0; i < N; i++) a[i] /= N;
}

int next_pow2(int x) { int p = 1; while (p < x) p <<= 1; return p; }

/* Linear convolution of real vectors A (len m) and B (len n), n >= m.
 * Returns a freshly allocated real array of length m+n-1.              */
double *convolve(double *A, int m, double *B, int n, int *out_len) {
    int result_len = m + n - 1;
    int N = next_pow2(result_len);

    cplx *fa = calloc(N, sizeof(cplx));
    cplx *fb = calloc(N, sizeof(cplx));
    for (int i = 0; i < m; i++) fa[i] = A[i];
    for (int i = 0; i < n; i++) fb[i] = B[i];

    fft(fa, N, 0);
    fft(fb, N, 0);
    for (int i = 0; i < N; i++) fa[i] *= fb[i];
    fft(fa, N, 1);

    double *C = malloc(result_len * sizeof(double));
    for (int i = 0; i < result_len; i++) C[i] = creal(fa[i]);

    free(fa); free(fb);
    *out_len = result_len;
    return C;
}

/* Brute-force O(m*n) convolution, used only to cross-check correctness. */
double *convolve_naive(double *A, int m, double *B, int n, int *out_len) {
    int result_len = m + n - 1;
    double *C = calloc(result_len, sizeof(double));
    for (int j = 0; j < m; j++)
        for (int k = 0; k < n; k++)
            C[j + k] += A[j] * B[k];
    *out_len = result_len;
    return C;
}

int main(void) {
    /* n >= m, as specified */
    double A[3] = {1, 2, 3};                 /* m = 3 */
    double B[6] = {4, 5, 6, 7, 8, 9};        /* n = 6 */
    int m = 3, n = 6;

    int len1, len2;
    double *fft_result = convolve(A, m, B, n, &len1);
    double *naive_result = convolve_naive(A, m, B, n, &len2);

    printf("A = ["); for (int i = 0; i < m; i++) printf("%.0f%s", A[i], i+1<m?", ":""); printf("]\n");
    printf("B = ["); for (int i = 0; i < n; i++) printf("%.0f%s", B[i], i+1<n?", ":""); printf("]\n\n");

    printf("FFT-based convolution   (O(n log n)): [");
    for (int i = 0; i < len1; i++) printf("%.1f%s", fft_result[i], i+1<len1?", ":"");
    printf("]\n");

    printf("Naive convolution       (O(m*n))    : [");
    for (int i = 0; i < len2; i++) printf("%.1f%s", naive_result[i], i+1<len2?", ":"");
    printf("]\n\n");

    double max_err = 0;
    for (int i = 0; i < len1; i++) {
        double err = fabs(fft_result[i] - naive_result[i]);
        if (err > max_err) max_err = err;
    }
    printf("Max abs error between FFT and naive result: %.2e  %s\n",
           max_err, max_err < 1e-6 ? "(MATCH - algorithm validated)" : "(MISMATCH!)");

    free(fft_result); free(naive_result);
    return 0;
}
