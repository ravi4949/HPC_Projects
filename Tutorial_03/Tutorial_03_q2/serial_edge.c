#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int Gx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
int Gy[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n < 3) {
        fprintf(stderr, "n must be >= 3\n");
        return 1;
    }

    unsigned char *image = malloc(sizeof(unsigned char) * n * n);
    unsigned char *output = calloc(n * n, sizeof(unsigned char));

    unsigned int seed = 42;
    for (int i = 0; i < n * n; i++) {
        image[i] = rand_r(&seed) % 256;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 1; i < n - 1; i++) {
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

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    long long checksum = 0;
    for (int i = 0; i < n * n; i++) checksum += output[i];

    printf("%d,%.6f,%lld\n", n, elapsed, checksum);

    free(image);
    free(output);
    return 0;
}