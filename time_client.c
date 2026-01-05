// gcc -o time_client time_client.c
// ./time_client 127.0.0.1
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int c, char **v) {
    if (c < 2) {
        puts("Usage: ./time_client <server-ip>");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in srv = {AF_INET, htons(13000)};
    inet_pton(AF_INET, v[1], &srv.sin_addr);

    // Send time request
    char *req = "TIME";
    sendto(sock, req, strlen(req), 0, (void*)&srv, sizeof(srv));
    printf("Time request sent to %s:13000\n", v[1]);

    // Receive time response
    char buf[256];
    struct sockaddr_in from;
    socklen_t fl = sizeof(from);
    int n = recvfrom(sock, buf, 255, 0, (void*)&from, &fl);
    if (n > 0) {
        buf[n] = 0;
        printf("Server Time: %s", buf);
    }

    close(sock);
}
