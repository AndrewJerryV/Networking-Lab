#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define MAX_BUFFER 100

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    int frame, expectedFrame = 1;
    int windowSize;
    int buffer[MAX_BUFFER] = {0}; 
    char buf[1024];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    printf("[SERVER] Socket created\n");

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    printf("[SERVER] Bind done\n");
    printf("[SERVER] Server listening...\n");

    addr_size = sizeof(clientAddr);
    recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&clientAddr, &addr_size);
    printf("[SERVER] Server accepted the client\n");
    sendto(sockfd, "OK", 2, 0, (struct sockaddr*)&clientAddr, addr_size);

    printf("Enter the window size of the server: ");
    scanf("%d", &windowSize);
    printf("\n");

    while (1) {
        int recv_frame;
        recvfrom(sockfd, &recv_frame, sizeof(int), 0, (struct sockaddr*)&clientAddr, &addr_size);
        static int dropped_2 = 0;
        static int dropped_5 = 0;

        if (recv_frame == 2 && !dropped_2) {
            dropped_2 = 1; 
            continue; 
        }
        if (recv_frame == 5 && !dropped_5) {
            dropped_5 = 1;
            continue;
        }

        if (recv_frame == expectedFrame) {
            printf("  Frame %d received, acknowledgement sent for %d\n", recv_frame, recv_frame);
            sendto(sockfd, &recv_frame, sizeof(int), 0, (struct sockaddr*)&clientAddr, addr_size);
            expectedFrame++;

            while (buffer[expectedFrame] == 1) {
                printf("  Frame %d received, acknowledgement sent for %d\n", expectedFrame, expectedFrame);
                sendto(sockfd, &expectedFrame, sizeof(int), 0, (struct sockaddr*)&clientAddr, addr_size);
                buffer[expectedFrame] = 0; // Clear buffer
                expectedFrame++;
            }
        } else if (recv_frame > expectedFrame) {
            printf("  Frame %d received, expected frame %d\n", recv_frame, expectedFrame);
            int nak = -expectedFrame;
            printf("  Sending NAK for %d\n", expectedFrame);
            sendto(sockfd, &nak, sizeof(int), 0, (struct sockaddr*)&clientAddr, addr_size);

            if (recv_frame < MAX_BUFFER) {
                buffer[recv_frame] = 1;
            }
        }
    }
    close(sockfd);
    return 0;
}
