#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define NODES 3
#define INF 999

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server = {AF_INET, htons(8080), INADDR_ANY}, client;
    socklen_t len = sizeof(client);
    bind(sock, (struct sockaddr*)&server, sizeof(server));

    // Initially: R0 knows R1 (dist 2), but doesn't know R2 (INF)
    int r0_vec[NODES] = {0, 2, INF};
    int r1_vec[NODES];

    printf("Router 0: Initial Vector [%d, %d, %d]\n", r0_vec[0], r0_vec[1], r0_vec[2]);
    
    // Receive Router 1's vector
    recvfrom(sock, r1_vec, sizeof(r1_vec), 0, (struct sockaddr*)&client, &len);
    printf("Received update from Router 1...\n");

    // Update logic: Check if going through R1 is cheaper to reach R2
    for (int i = 0; i < NODES; i++) {
        if (r0_vec[i] > r0_vec[1] + r1_vec[i]) {
            r0_vec[i] = r0_vec[1] + r1_vec[i];
        }
    }

    printf("Router 0: Updated Vector [%d, %d, %d]\n", r0_vec[0], r0_vec[1], r0_vec[2]);
    sendto(sock, r0_vec, sizeof(r0_vec), 0, (struct sockaddr*)&client, len);
    
    return 0;
}