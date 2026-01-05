// TCP Server - gcc -o tcp_server tcp_server.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int lsn = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1, fdmax = lsn;
    setsockopt(lsn, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in addr = {AF_INET, htons(12345), {INADDR_ANY}};
    struct sockaddr_in cli[FD_SETSIZE];
    bind(lsn, (void*)&addr, sizeof(addr));
    listen(lsn, 5);
    printf("Server started on port 12345\n");
    
    fd_set master, fds;
    FD_ZERO(&master);
    FD_SET(lsn, &master);
    char buf[1024], out[1088];
    
    while(1) {
        fds = master;
        select(fdmax + 1, &fds, 0, 0, 0);
        
        for (int i = 0; i <= fdmax; i++) {
            if (!FD_ISSET(i, &fds)) continue;
            
            if (i == lsn) {
                // New client connection
                socklen_t l = sizeof(addr);
                int fd = accept(lsn, (void*)&addr, &l);
                FD_SET(fd, &master);
                if (fd > fdmax) fdmax = fd;
                cli[fd] = addr;
                
                printf("Client connected: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
                int len = snprintf(out, sizeof(out), "Client %s:%d joined\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
                for (int j = 0; j <= fdmax; j++)
                    if (FD_ISSET(j, &master) && j != lsn)
                        send(j, out, len, 0);
            } else {
                // Message from existing client
                int n = recv(i, buf, 1023, 0);
                if (n <= 0) {
                    printf("Client disconnected: %s:%d\n", inet_ntoa(cli[i].sin_addr), ntohs(cli[i].sin_port));
                    close(i);
                    FD_CLR(i, &master);
                    continue;
                }
                buf[n] = 0;
                printf("Message from %s:%d: %s", inet_ntoa(cli[i].sin_addr), ntohs(cli[i].sin_port), buf);
                
                int len = snprintf(out, sizeof(out), "[%s:%d] %s", inet_ntoa(cli[i].sin_addr), ntohs(cli[i].sin_port), buf);
                for (int j = 0; j <= fdmax; j++)
                    if (FD_ISSET(j, &master) && j != lsn && j != i)
                        send(j, out, len, 0);
            }
        }
    }
}
