#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd, newsock, n;
    struct sockaddr_in addr = {AF_INET, htons(8080), INADDR_ANY};
    socklen_t addr_len = sizeof(addr);
    char buf[4096], fname[256], content[2048];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) exit(1);
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) exit(1);
    listen(sockfd, 5);

    printf("[Main Server PID: %d] Listening on port 8080...\n", getpid());

    while (1) {
        if ((newsock = accept(sockfd, (struct sockaddr*)&addr, &addr_len)) < 0) continue;
        printf("[Main Server] Connection accepted.\n");

        if (fork() == 0) { // Child Process
            close(sockfd);
            n = recv(newsock, fname, 255, 0); fname[n] = 0;
            printf("[Child PID: %d] Requested file: %s\n", getpid(), fname);

            FILE *fp = fopen(fname, "r");
            if (fp) {
                n = fread(content, 1, 2047, fp); content[n] = 0; fclose(fp);
                snprintf(buf, sizeof(buf), "\n[Server PID: %d] File Content:\n%s\n", getpid(), content);
            } else {
                snprintf(buf, sizeof(buf), "\n[Server PID: %d] Error: File '%s' not found.\n", getpid(), fname);
            }
            send(newsock, buf, strlen(buf), 0);
            printf("[Child PID: %d] Response sent. Closing connection.\n", getpid());
            close(newsock);
            exit(0);
        }
        close(newsock);
    }
    return 0;
}