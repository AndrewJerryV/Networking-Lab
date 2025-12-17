#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in a = {AF_INET, htons(8080)};
    char *m = "Hello from Client!";

    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    
    if (connect(s, (struct sockaddr *)&a, sizeof(a)) == 0) {
        send(s, m, 18, 0);
        printf("Message sent.\n");
    }

    close(s);
    return 0;
}