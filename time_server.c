// gcc -o time_server time_server.c
// ./time_server
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in srv = {AF_INET, htons(13000), {INADDR_ANY}};
    bind(sock, (void*)&srv, sizeof(srv));
    printf("Time Server started on port 13000\n");

    char buf[256];

    while (1) {
        struct sockaddr_in cli;
        socklen_t sl = sizeof(cli);
        int n = recvfrom(sock, buf, 255, 0, (void*)&cli, &sl);
        if (n <= 0) continue;
        buf[n] = 0;

        printf("Time request from %s:%d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));

        // Get current time
        time_t now = time(NULL);
        char *timestr = ctime(&now);
        int len = strlen(timestr);

        // Send time back to client
        sendto(sock, timestr, len, 0, (void*)&cli, sl);
        printf("Sent: %s", timestr);
    }

    close(sock);
}
