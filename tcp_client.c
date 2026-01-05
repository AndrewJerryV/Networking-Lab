// TCP Client - gcc -o tcp_client tcp_client.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int c, char **v) {
    if (c < 2) {
        puts("Usage: ./tcp_client <ip>"); 
        return 1; 
    }
    
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in a = {AF_INET, htons(12345)};
    inet_pton(AF_INET, v[1], &a.sin_addr);
    connect(s, (void*)&a, sizeof(a));
    printf("Connected to %s:12345\n", v[1]);
    
    fd_set f; char b[1024]; int n;
    while(1) {
        FD_ZERO(&f); FD_SET(0, &f); FD_SET(s, &f);
        select(s+1, &f, 0, 0, 0);
        if (FD_ISSET(s, &f) && (n = recv(s, b, 1023, 0)) > 0) { 
            b[n] = 0; fputs(b, stdout); 
        }
        else if 
            (FD_ISSET(s, &f)) 
            break;
        if (FD_ISSET(0, &f) && fgets(b, 1024, stdin)) 
            send(s, b, strlen(b), 0);
    }
    close(s);
}
