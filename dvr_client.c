#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define NODES 3

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr = {AF_INET, htons(8080), INADDR_ANY};

    // R1 knows R0 (dist 2) and R2 (dist 5)
    int r1_vec[NODES] = {2, 0, 5};
    int updated_r0[NODES];

    printf("Router 1: Sending Vector [%d, %d, %d] to Router 0\n", r1_vec[0], r1_vec[1], r1_vec[2]);
    sendto(sock, r1_vec, sizeof(r1_vec), 0, (struct sockaddr*)&addr, sizeof(addr));

    recvfrom(sock, updated_r0, sizeof(updated_r0), 0, NULL, NULL);
    printf("Router 1: Router 0 has updated its path to R2 through us!\n");

    return 0;
}