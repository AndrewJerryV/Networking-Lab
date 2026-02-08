#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#define PORT 8080
#define MAX_FRAMES 100

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    int totalFrames, windowSize;
    int acked[MAX_FRAMES] = {0};
    int base = 1;
    char buf[1024];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("[CLIENT] Socket created\n");

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    sendto(sockfd, "HI", 2, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    addr_size = sizeof(serverAddr);
    recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&serverAddr, &addr_size);
    printf("[CLIENT] Connected to server\n\n");

    printf("Enter the number of frames to be sent: ");
    scanf("%d", &totalFrames);
    printf("Enter the window size: ");
    scanf("%d", &windowSize);
    printf("\n");

    struct timeval tv;
    tv.tv_sec = 2;  
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    while (base <= totalFrames) {
        for (int i = 0; i < windowSize && (base + i) <= totalFrames; i++) {
            int frame = base + i;
            sendto(sockfd, &frame, sizeof(int), 0, (struct sockaddr*)&serverAddr, addr_size);
            usleep(100000);
        }

        int responses_needed = (base + windowSize <= totalFrames) ? windowSize : (totalFrames - base + 1);

        for (int i = 0; i < responses_needed; i++) {
            int ack;
            int recv_len = recvfrom(sockfd, &ack, sizeof(int), 0, (struct sockaddr*)&serverAddr, &addr_size);

            if (recv_len < 0) {
                printf("  Ack not received for %d\n", base);
                printf("  RESENDING ...\n");
                sendto(sockfd, &base, sizeof(int), 0, (struct sockaddr*)&serverAddr, addr_size);
                recvfrom(sockfd, &ack, sizeof(int), 0, (struct sockaddr*)&serverAddr, &addr_size);
                if (ack > 0) {
                    printf("  Ack received %d\n", ack);
                    acked[ack] = 1;
                    if(ack == base) base++;
                }
            } else {
                if (ack > 0) {
                    printf("  Ack received %d\n", ack);
                    acked[ack] = 1;
                    while (acked[base] && base <= totalFrames) {
                        base++;
                    }
                } else {
                    printf("  Ack not received for %d\n", -ack);
                    printf("  RESENDING ...\n");
                    int resendFrame = abs(ack);
                    sendto(sockfd, &resendFrame, sizeof(int), 0, (struct sockaddr*)&serverAddr, addr_size);
                }
            }
        }
        if (base > totalFrames) break;
    }
    printf("\n[CLIENT] EXIT\n");
    close(sockfd);
    return 0;
}
