#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int s = socket(2, 1, 0), n, o = 1;
    struct sockaddr_in a = {2, htons(8081), 0};
    char b[1024];

    setsockopt(s, 1, 2, &o, 4);
    bind(s, (struct sockaddr *)&a, 16);
    listen(s, 3);
    printf("Server live. Type 'exit' to quit.\n");

    n = accept(s, 0, 0);
    while (1) {
        memset(b, 0, 1024);
        if (read(n, b, 1024) <= 0 || !strncmp(b, "exit", 4)) 
            break;
        printf("Client: %sServer: ", b);
        fgets(b, 1024, stdin);
        send(n, b, strlen(b), 0);
        if (!strncmp(b, "exit", 4)) 
            break;
    }

    close(n); 
    close(s);
    return 0;
}