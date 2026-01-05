// gcc -o udp_client udp_client.c
// ./udp_client 127.0.0.1
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int c, char **v) {
    if (c < 2) {
        puts("Usage: ./udp_client <ip>");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in srv = {AF_INET, htons(54321)};
    inet_pton(AF_INET, v[1], &srv.sin_addr);
    printf("Ready to send to %s:54321\n", v[1]);

    char buf[1024];
    fd_set fds;

    while (1) {
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        FD_SET(sock, &fds);
        select(sock + 1, &fds, 0, 0, 0);

        if (FD_ISSET(0, &fds)) {
            if (!fgets(buf, 1024, stdin)) break;
            sendto(sock, buf, strlen(buf), 0, (void*)&srv, sizeof(srv));
        }

        if (FD_ISSET(sock, &fds)) {
            struct sockaddr_in from;
            socklen_t fl = sizeof(from);
            int n = recvfrom(sock, buf, 1023, 0, (void*)&from, &fl);
            if (n <= 0) continue;
            buf[n] = 0;
            printf("From %s:%d -> %s", inet_ntoa(from.sin_addr), ntohs(from.sin_port), buf);
        }
    }

    close(sock);
}
