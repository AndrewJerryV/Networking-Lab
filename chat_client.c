#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int s = socket(2, 1, 0);
    struct sockaddr_in a = {2, htons(8081)};
    char b[1024];

    inet_pton(2, "127.0.0.1", &a.sin_addr);
    if (connect(s, (struct sockaddr *)&a, 16) < 0) 
        return 1;

    printf("Connected. Type 'exit' to quit.\n");
    while (1) {
        printf("Client: ");
        fgets(b, 1024, stdin);
        send(s, b, strlen(b), 0);
        if (!strncmp(b, "exit", 4)) 
            break;
        
        memset(b, 0, 1024);
        if (read(s, b, 1024) <= 0 || !strncmp(b, "exit", 4)) 
            break;
        printf("Server: %s", b);
    }
    close(s);
    return 0;
}