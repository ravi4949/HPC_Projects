#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

typedef struct {
    long long num_points;
    long long count_inside;
    unsigned int seed;
} thread_data_t;

void *worker(void *arg) {
    thread_data_t *data = (thread_data_t *) arg;
    long long local_count = 0;
    unsigned int seed = data->seed;

    for (long long i = 0; i < data->num_points; i++) {
        double x = ((double) rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;
        double y = ((double) rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;
        if (x * x + y * y <= 1.0) {
            local_count++;
        }
    }

    data->count_inside = local_count;
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_points> <num_threads>\n", argv[0]);
        return 1;
    }

    long long num_points = atoll(argv[1]);
    int num_threads = atoi(argv[2]);

    if (num_points <= 0 || num_threads <= 0) {
        fprintf(stderr, "num_points and num_threads must be positive\n");
        return 1;
    }

    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    thread_data_t *tdata = malloc(sizeof(thread_data_t) * num_threads);

    long long base = num_points / num_threads;
    long long remainder = num_points % num_threads;

    unsigned int master_seed = (unsigned int) time(NULL);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int t = 0; t < num_threads; t++) {
        tdata[t].num_points = base + (t < remainder ? 1 : 0);
        tdata[t].count_inside = 0;
        tdata[t].seed = master_seed + (unsigned int)(t * 7919 + 1);
        pthread_create(&threads[t], NULL, worker, &tdata[t]);
    }

    long long total_inside = 0;
    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
        total_inside += tdata[t].count_inside;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) +
                      (end.tv_nsec - start.tv_nsec) / 1e9;

    double pi_estimate = 4.0 * (double) total_inside / (double) num_points;
    double error = fabs(pi_estimate - M_PI);

    printf("%lld,%d,%.10f,%.10f,%.6f\n",
           num_points, num_threads, pi_estimate, error, elapsed);

    free(threads);
    free(tdata);
    return 0;
}