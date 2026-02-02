#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define WSIZE 4
#define TOTAL 8

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr = {AF_INET, htons(PORT), INADDR_ANY};
    int ack, base = 0, sent = 0;
    int acks[TOTAL] = {0};

    struct timeval tv = {2, 0}; // 2-second timeout
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (base < TOTAL) {
        // Send window
        for (int i = 0; i < WSIZE && (base + i) < TOTAL; i++) {
            if (!acks[base + i]) {
                int frame = base + i;
                printf("Sending Frame %d\n", frame);
                sendto(sock, &frame, sizeof(int), 0, (struct sockaddr*)&addr, sizeof(addr));
            }
        }

        // Wait for ACKs
        for (int i = 0; i < WSIZE; i++) {
            if (recvfrom(sock, &ack, sizeof(int), 0, NULL, NULL) > 0) {
                printf("Received ACK for Frame %d\n", ack);
                acks[ack] = 1;
            }
        }

        // Slide window
        while (acks[base] && base < TOTAL) base++;
    }
    printf("Transfer Complete.\n");
    close(sock);
    return 0;
}