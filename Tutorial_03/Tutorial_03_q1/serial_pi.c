#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_points>\n", argv[0]);
        return 1;
    }

    long long num_points = atoll(argv[1]);
    if (num_points <= 0) {
        fprintf(stderr, "num_points must be a positive integer\n");
        return 1;
    }

    unsigned int seed = (unsigned int) time(NULL);
    long long count_inside = 0;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (long long i = 0; i < num_points; i++) {
        double x = ((double) rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;
        double y = ((double) rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            count_inside++;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) +
                      (end.tv_nsec - start.tv_nsec) / 1e9;

    double pi_estimate = 4.0 * (double) count_inside / (double) num_points;
    double error = fabs(pi_estimate - M_PI);

    printf("%lld,%.10f,%.10f,%.6f\n", num_points, pi_estimate, error, elapsed);

    return 0;
}