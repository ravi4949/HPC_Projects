#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sizes_mb[3] = {1, 10, 50};
    for (int s = 0; s < 3; s++) {
        int n = sizes_mb[s] * 250000;            // 1 MB = 250,000 ints (4 bytes each)
        int *data = malloc(n * sizeof(int));
        int ack = 0;

        MPI_Barrier(MPI_COMM_WORLD);             // start together
        double start = MPI_Wtime();

        if (rank == 0) {
            for (int i = 0; i < n; i++) data[i] = i;
            MPI_Send(data, n, MPI_INT, 1, 0, MPI_COMM_WORLD);        // waits here
            MPI_Recv(&ack, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%d MB: %f seconds\n", sizes_mb[s], MPI_Wtime() - start);
        } else {
            MPI_Recv(data, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process 1 got first=%d last=%d\n", data[0], data[n - 1]);
            MPI_Send(&ack, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);        // tell 0 "I got it"
        }
        free(data);
    }
    MPI_Finalize();
    return 0;
}