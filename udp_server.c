// gcc -o udp_server udp_server.c
// ./udp_server
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 100

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in srv = {AF_INET, htons(54321), {INADDR_ANY}};
    bind(sock, (void*)&srv, sizeof(srv));
    printf("Server started on port 54321\n");

    struct sockaddr_in clients[MAX_CLIENTS];
    int client_count = 0;
    char buf[1024];
    fd_set fds;

    while (1) {
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        FD_SET(sock, &fds);
        select(sock + 1, &fds, 0, 0, 0);

        if (FD_ISSET(sock, &fds)) {
            struct sockaddr_in cli;
            socklen_t sl = sizeof(cli);
            int n = recvfrom(sock, buf, 1023, 0, (void*)&cli, &sl);
            if (n <= 0) continue;
            buf[n] = 0;
            printf("From %s:%d -> %s", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port), buf);

            // Remember new client
            int found = 0;
            for (int i = 0; i < client_count; i++)
                if (clients[i].sin_port == cli.sin_port && clients[i].sin_addr.s_addr == cli.sin_addr.s_addr) {
                    found = 1;
                    break;
                }
            if (!found && client_count < MAX_CLIENTS) {
                clients[client_count++] = cli;
                printf("New client registered: %s:%d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
            }

            // Echo back with prefix
            char echo[1040];
            int elen = snprintf(echo, sizeof(echo), "[ECHO] %s", buf);
            sendto(sock, echo, elen, 0, (void*)&cli, sl);
        }

        if (FD_ISSET(0, &fds)) {
            if (!fgets(buf, 1024, stdin)) break;
            int len = strlen(buf);
            // Broadcast to all clients
            for (int i = 0; i < client_count; i++)
                sendto(sock, buf, len, 0, (void*)&clients[i], sizeof(clients[i]));
            printf("Broadcast sent to %d clients\n", client_count);
        }
    }

    close(sock);
}
