#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0), n;
    struct sockaddr_in a = {AF_INET, htons(8080), INADDR_ANY};
    char b[1024] = {0};

    int opt=1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bind(s, (struct sockaddr *)&a, sizeof(a));
    listen(s, 3);
    printf("Server listening on port 8080...\n");

    n = accept(s, 0, 0);
    read(n, b, 1024);
    printf("Client: %s\n", b);

    close(n); 
    close(s);
    return 0;
}