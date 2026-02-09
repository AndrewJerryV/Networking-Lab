#include <stdio.h>
#define INF 999
#define N 10

void dijkstra(int G[N][N], int n, int start) {
    int cost[N], vis[N] = {0}, next[N], i, count, min, u;
    for (i = 0; i < n; i++) { 
        cost[i] = INF; 
        next[i] = -1; 
    }
    cost[start] = 0;
    for (count = 0; count < n - 1; count++) {
        min = INF; u = -1;
        // Find min distance node
        for (i = 0; i < n; i++) if (!vis[i] && cost[i] <= min) { min = cost[i]; u = i; }
        if (u == -1) break; // Disconnected graph check
        vis[u] = 1;
        for (i = 0; i < n; i++)
            if (!vis[i] && G[u][i] && cost[u] != INF && cost[u] + G[u][i] < cost[i]) {
                cost[i] = cost[u] + G[u][i];
                next[i] = (u == start) ? i : next[u];
            }
    }
    printf("\nRouter %d:\nDestination\tCost\tNext Hop\n", start);
    for (i = 0; i < n; i++) if (i != start)
        printf("%d\t\t%d\t%d\n", i, cost[i], (cost[i] >= INF) ? -1 : next[i]);
}

int main() {
    int n, i, j, G[N][N];
    printf("Enter the number of routers: "); scanf("%d", &n);
    printf("Enter the Cost Matrix (0 for self, 999 for no link):\n");
    for (i = 0; i < n; i++) for (j = 0; j < n; j++) {
        scanf("%d", &G[i][j]);
        if (G[i][j] == 999) 
            G[i][j] = INF;
    }
    printf("\n--- Routing Tables ---\n");
    for (i = 0; i < n; i++) 
        dijkstra(G, n, i);
    return 0;
}