
#include <stdio.h>
#include <omp.h>

#define N 6

int graph[N][N] = {
    {0,1,1,0,0,0},
    {1,0,0,1,1,0},
    {1,0,0,0,0,1},
    {0,1,0,0,0,0},
    {0,1,0,0,0,1},
    {0,0,1,0,1,0}
};

int main() {
    int visited[N] = {0};
    int frontier[N], next[N];
    int count = 1;

    frontier[0] = 0;
    visited[0] = 1;

    printf("BFS: ");

    while (count > 0) {
        int nextCount = 0;

        #pragma omp parallel for schedule(static)
        for (int i = 0; i < count; i++) {
            int u = frontier[i];

            #pragma omp critical
            printf("%d ", u);

            for (int v = 0; v < N; v++) {
                if (graph[u][v]) {
                    #pragma omp critical
                    {
                        if (!visited[v]) {
                            visited[v] = 1;
                            next[nextCount++] = v;
                        }
                    }
                }
            }
        }

        count = nextCount;

        for (int i = 0; i < count; i++)
            frontier[i] = next[i];
    }

    printf("\n");
    return 0;
}
