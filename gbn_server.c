#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>

int main() {
    int sfd, cfd, win, tot, base = 0, next = 0, ack;
    struct sockaddr_in addr = {AF_INET, htons(8081), {INADDR_ANY}};
    time_t timer = time(NULL);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1; setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(sfd, 1);
    
    printf("[SERVER] Waiting...\n");
    cfd = accept(sfd, NULL, NULL);
    printf("Total Packets: "); scanf("%d", &tot);
    printf("Window Size: ");   scanf("%d", &win);
    send(cfd, &tot, sizeof(tot), 0);

    fcntl(cfd, F_SETFL, O_NONBLOCK);

    while (base < tot) {
        // A. SEND NEW PACKETS (Fill Window)
        while (next < base + win && next < tot) {
            printf("[SERVER] Sending Frame: %d\n", next);
            send(cfd, &next, sizeof(next), 0);
            if (base == next) timer = time(NULL);
            next++;
            usleep(10000);
        }
        // B. CHECK FOR ACKS (Non-blocking)
        if (recv(cfd, &ack, sizeof(ack), 0) > 0) {
            if (ack >= base) {
                printf("[SERVER] ACK %d received. Sliding window.\n", ack);
                base = ack + 1;
                timer = time(NULL);
            }
        }
        // C. HANDLE TIMEOUT
        if (time(NULL) - timer > 2) {
            printf("[SERVER] Timeout! Resending Window from %d\n", base);
            for (int i = base; i < next; i++) {
                printf("[SERVER] Re-sending: %d\n", i);
                send(cfd, &i, sizeof(i), 0);
            }
            timer = time(NULL);
        }
        usleep(10000);
    }

    printf("[SERVER] Done.\n");
    close(cfd); close(sfd);
    return 0;
}