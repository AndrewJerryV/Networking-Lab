#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server = {AF_INET, htons(PORT), INADDR_ANY}, client;
    socklen_t len = sizeof(client);
    bind(sock, (struct sockaddr*)&server, sizeof(server));

    int frame;
    printf("Receiver ready...\n");
    while (1) {
        recvfrom(sock, &frame, sizeof(int), 0, (struct sockaddr*)&client, &len);
        if (rand() % 3 == 0) { // Simulate 33% packet loss
            printf("Frame %d dropped!\n", frame);
            continue;
        }
        printf("Frame %d received. Sending ACK.\n", frame);
        sendto(sock, &frame, sizeof(int), 0, (struct sockaddr*)&client, len);
    }
    return 0;
}