#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

int Gx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
int Gy[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};

int n;
unsigned char *image;
unsigned char *output;

typedef struct {
    int start_row;
    int end_row;
} thread_data_t;

void *worker(void *arg) {
    thread_data_t *data = (thread_data_t *) arg;
    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 1; j < n - 1; j++) {
            int gx = 0, gy = 0;
            for (int ki = -1; ki <= 1; ki++) {
                for (int kj = -1; kj <= 1; kj++) {
                    int pixel = image[(i + ki) * n + (j + kj)];
                    gx += pixel * Gx[ki + 1][kj + 1];
                    gy += pixel * Gy[ki + 1][kj + 1];
                }
            }
            int magnitude = (int) sqrt((double)(gx * gx + gy * gy));
            if (magnitude > 255) magnitude = 255;
            output[i * n + j] = (unsigned char) magnitude;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <n> <num_threads>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    if (n < 3 || num_threads < 1) {
        fprintf(stderr, "invalid arguments\n");
        return 1;
    }

    image = malloc(sizeof(unsigned char) * n * n);
    output = calloc(n * n, sizeof(unsigned char));

    unsigned int seed = 42;
    for (int i = 0; i < n * n; i++) {
        image[i] = rand_r(&seed) % 256;
    }

    pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
    thread_data_t *tdata = malloc(sizeof(thread_data_t) * num_threads);

    int interior_rows = n - 2;
    int base = interior_rows / num_threads;
    int remainder = interior_rows % num_threads;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int current_row = 1;
    for (int t = 0; t < num_threads; t++) {
        int rows_for_this_thread = base + (t < remainder ? 1 : 0);
        tdata[t].start_row = current_row;
        tdata[t].end_row = current_row + rows_for_this_thread;
        current_row += rows_for_this_thread;
        pthread_create(&threads[t], NULL, worker, &tdata[t]);
    }

    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    long long checksum = 0;
    for (int i = 0; i < n * n; i++) checksum += output[i];

    printf("%d,%d,%.6f,%lld\n", n, num_threads, elapsed, checksum);

    free(image);
    free(output);
    free(threads);
    free(tdata);
    return 0;
}