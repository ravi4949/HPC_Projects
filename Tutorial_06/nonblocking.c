// NON-BLOCKING: Isend/Irecv start the transfer and return immediately,
// so we can do other work, then MPI_Wait to finish.
// Run: mpirun -np 2 ./3_nonblocking
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void do_work() {                                  // work that does NOT use the message
    volatile double x = 0;
    for (long i = 0; i < 50000000; i++) x += i * 0.5;
}

int main(int argc, char **argv) {
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sizes_mb[3] = {1, 10, 50};
    for (int s = 0; s < 3; s++) {
        int n = sizes_mb[s] * 250000;
        int *data = malloc(n * sizeof(int));
        int ack = 0;
        MPI_Request req;
        double t_block, t_nonblock, start;

        // ---- (A) blocking: send/receive first, THEN work ----
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();
        if (rank == 0) {
            for (int i = 0; i < n; i++) data[i] = i;
            MPI_Send(data, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
            do_work();
            MPI_Recv(&ack, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            t_block = MPI_Wtime() - start;
        } else {
            MPI_Recv(data, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            do_work();
            MPI_Send(&ack, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }

        // ---- (B) non-blocking: start transfer, work, THEN wait ----
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();
        if (rank == 0) {
            MPI_Isend(data, n, MPI_INT, 1, 0, MPI_COMM_WORLD, &req);  // returns at once
            do_work();                                                // work meanwhile
            MPI_Wait(&req, MPI_STATUS_IGNORE);                        // now send is done
            MPI_Recv(&ack, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            t_nonblock = MPI_Wtime() - start;
            printf("%d MB: blocking = %f s, non-blocking = %f s\n",
                   sizes_mb[s], t_block, t_nonblock);
        } else {
            MPI_Irecv(data, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
            do_work();
            MPI_Wait(&req, MPI_STATUS_IGNORE);                        // data valid only after this
            MPI_Send(&ack, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
        free(data);
    }
    MPI_Finalize();
    return 0;
}