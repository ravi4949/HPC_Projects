
#include <stdio.h>
#include <omp.h>

#define N 16

int main() {
    omp_set_dynamic(0);

    printf("Number of threads: ");

    #pragma omp parallel
    {
        #pragma omp single
        printf("%d\n", omp_get_num_threads());
    }

    printf("\nDefault schedule assignment:\n");

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        printf("Iteration %2d executed by thread %d\n",
               i, omp_get_thread_num());
    }

    return 0;
}
