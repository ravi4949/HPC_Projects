// Ring: send to next, receive from previous.
// Run: mpirun -np 4 ./4_ring
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    int rank, size, recv_val;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int previous = (rank - 1 + size) % size;

    // Sendrecv does send AND receive together -> no deadlock.
    // (If everyone did Send first then Recv, all could get stuck waiting.)
    MPI_Sendrecv(&rank, 1, MPI_INT, next, 0,
                 &recv_val, 1, MPI_INT, previous, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Process %d sent %d to %d, received %d from %d\n",
           rank, rank, next, recv_val, previous);
    MPI_Finalize();
    return 0;
}