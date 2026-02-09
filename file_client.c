#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in addr = {AF_INET, htons(8080), inet_addr("127.0.0.1")};
    char fname[256], buf[2048];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) exit(1);
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) exit(1);

    printf("Enter filename to request: ");
    scanf("%s", fname);
    send(sock, fname, strlen(fname), 0);

    if (recv(sock, buf, 2048, 0) > 0)
        printf("Server Response:\n%s\n", buf);
    else
        printf("Receive failed\n");

    close(sock);
    return 0;
}