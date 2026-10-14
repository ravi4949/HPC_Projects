
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

int visited[N] = {0};

void dfs(int u) {
    #pragma omp critical
    {
        printf("%d ", u);
    }

    for (int v = 0; v < N; v++) {
        if (graph[u][v]) {
            int take = 0;

            #pragma omp critical
            {
                if (!visited[v]) {
                    visited[v] = 1;
                    take = 1;
                }
            }

            if (take) {
                #pragma omp task firstprivate(v)
                dfs(v);
            }
        }
    }

    #pragma omp taskwait
}

int main() {
    #pragma omp parallel
    {
        #pragma omp single
        {
            visited[0] = 1;
            dfs(0);
        }
    }

    printf("\n");
    return 0;
}
