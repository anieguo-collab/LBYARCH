#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

extern void dot_asm(size_t n, const double* A, const double* B, double* sdot);

void dot_c(size_t n, const double* A, const double* B, double* sdot) {
    double sum = 0.0;
    for (size_t i = 0; i < n; i++)
        sum += A[i] * B[i];
    *sdot = sum;
}

static double now_ms(void) {
    LARGE_INTEGER f, c;
    QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&c);
    return 1000.0 * c.QuadPart / f.QuadPart;
}

int main(void) {
    size_t sizes[] = { 1ull << 20, 1ull << 24, 1ull << 28 };
    const int RUNS = 20;

    for (int s = 0; s < 3; s++) {
        size_t n = sizes[s];
        double* A = malloc(n * sizeof(double));
        double* B = malloc(n * sizeof(double));
        if (!A || !B) { printf("malloc failed for n=%zu\n", n); return 1; }
        for (size_t i = 0; i < n; i++) {
            A[i] = (double)rand() / RAND_MAX;
            B[i] = (double)rand() / RAND_MAX;
        }

        double sdot_c = 0, sdot_a = 0, t_c = 0, t_a = 0;
        for (int r = 0; r < RUNS; r++) {
            double t0 = now_ms();
            dot_c(n, A, B, &sdot_c);
            t_c += now_ms() - t0;

            t0 = now_ms();
            dot_asm(n, A, B, &sdot_a);
            t_a += now_ms() - t0;
        }
        printf("n=%zu\n C   : %.6f  avg %.3f ms\n ASM : %.6f  avg %.3f ms\n %s\n",
            n, sdot_c, t_c / RUNS, sdot_a, t_a / RUNS,
            fabs(sdot_c - sdot_a) < 1e-6 * fabs(sdot_c) ? "Correct" : "MISMATCH");
        free(A); free(B);
    }
    return 0;
}