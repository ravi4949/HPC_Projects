// 2x2 grid:   0 1
//             2 3
// Run: mpirun -np 4 ./5_grid
#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int C = 2;                       // columns
    int R = size / C;                // rows
    int row = rank / C, col = rank % C;

    // MPI_PROC_NULL = "nobody": sending/receiving with it does nothing
    int up    = (row > 0)     ? rank - C : MPI_PROC_NULL;
    int down  = (row < R - 1) ? rank + C : MPI_PROC_NULL;
    int left  = (col > 0)     ? rank - 1 : MPI_PROC_NULL;
    int right = (col < C - 1) ? rank + 1 : MPI_PROC_NULL;

    int from_up = -1, from_down = -1, from_left = -1, from_right = -1;
    MPI_Sendrecv(&rank, 1, MPI_INT, up,    0, &from_down,  1, MPI_INT, down,  0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&rank, 1, MPI_INT, down,  0, &from_up,    1, MPI_INT, up,    0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&rank, 1, MPI_INT, left,  0, &from_right, 1, MPI_INT, right, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&rank, 1, MPI_INT, right, 0, &from_left,  1, MPI_INT, left,  0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Process %d (row %d, col %d): up=%d down=%d left=%d right=%d  (-1 = none)\n",
           rank, row, col, from_up, from_down, from_left, from_right);
    MPI_Finalize();
    return 0;
}