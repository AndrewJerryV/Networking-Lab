#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
    int sock, val, tot, expected = 0;
    struct sockaddr_in addr = {AF_INET, htons(8081), {INADDR_ANY}};
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));

    recv(sock, &tot, sizeof(tot), 0);
    printf("[CLIENT] Connected. Expecting %d packets.\n", tot);
    srand(time(0)); 

    while (expected < tot) {
        if (recv(sock, &val, sizeof(val), 0) <= 0) break;

        if (rand() % 100 < 30) {
            printf("[CLIENT] Frame %d -> SIMULATED DROP\n", val);
            continue;
        }

        if (val == expected) {
            printf("[CLIENT] Frame %d -> ACCEPTED. Sending ACK %d\n", val, val);
            send(sock, &val, sizeof(val), 0);
            expected++;
        } else {
            printf("[CLIENT] Frame %d -> OUT OF ORDER (Expected %d). Sending ACK %d\n", val, expected, expected - 1);
            int last_ack = expected - 1;
            if (last_ack >= 0) send(sock, &last_ack, sizeof(last_ack), 0);
        }
    }
    printf("[CLIENT] Done.\n");
    close(sock);
    return 0;
}