#include <stdio.h>

#define INF 999

int main() {
    int n, i, j, k;
    int cost[10][10], dist[10][10], next[10][10];

    printf("Enter number of routers: ");
    scanf("%d", &n);

    printf("Enter cost matrix (999 for infinity):\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);
            dist[i][j] = cost[i][j];

            if (i == j)
                next[i][j] = i;
            else if (cost[i][j] != INF)
                next[i][j] = j;
            else
                next[i][j] = -1;
        }
    }

    /* Distance Vector Routing Algorithm */
    for (k = 0; k < n; k++) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k]; // first hop
                }
            }
        }
    }

    /* Print Routing Tables */
    for (i = 0; i < n; i++) {
        printf("\nRouter %d\n", i);
        printf("Destination Distance NextRouter\n");

        for (j = 0; j < n; j++) {
            printf("%5d", j);

            if (dist[i][j] >= INF)
                printf("%11d", -1);
            else
                printf("%11d", dist[i][j]);

            printf("%12d\n", next[i][j]);
        }
    }

    /* Print Final Cost Matrix */
    printf("Final Cost Matrix:\n ");
    for (i = 0; i < n; i++)
        printf("%4d", i);

    printf("\n---------------------------------\n");

    for (i = 0; i < n; i++) {
        printf("%2d |", i);
        for (j = 0; j < n; j++) {
            printf("%4d", dist[i][j]);
        }
        printf("\n");
    }

    return 0;
}
