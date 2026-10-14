
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

int main() {
    long long samples[] = {
        10, 100, 10000, 1000000,
        10000000, 100000000, 1000000000
    };

    int count = sizeof(samples) / sizeof(samples[0]);

    printf("Points,Estimated Pi,Absolute Error\n");

    for (int j = 0; j < count; j++) {
        long long n = samples[j];
        long long inside = 0;

        #pragma omp parallel
        {
            unsigned int seed =
                1234u + (unsigned int)omp_get_thread_num() * 1000003u;
            long long local_inside = 0;

            #pragma omp for
            for (long long i = 0; i < n; i++) {
                double x = 2.0 * rand_r(&seed) / RAND_MAX - 1.0;
                double y = 2.0 * rand_r(&seed) / RAND_MAX - 1.0;

                if (x * x + y * y <= 1.0) {
                    local_inside++;
                }
            }

            #pragma omp atomic
            inside += local_inside;
        }

        double pi = 4.0 * inside / n;
        printf("%lld,%.10f,%.10f\n",
               n, pi, fabs(3.141592653589793 - pi));
    }

    return 0;
}
