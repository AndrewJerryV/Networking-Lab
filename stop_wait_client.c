#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8080

typedef struct { 
    int seq_no, ack_no; 
    char data[1024]; 
} Frame;

int main() {
    int sockfd, i = 0, seq = 0, n, addr_len;
    struct sockaddr_in servaddr;
    Frame frame;
    struct timeval tv = {2, 0};

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
        { perror("Socket"); exit(EXIT_FAILURE); }
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (i < 5) {
        printf("------------------------------------------\n");
        frame.seq_no = seq;
        sprintf(frame.data, "Packet %d", i);
        printf("Sending Frame %d: %s\n", seq, frame.data);
        sendto(sockfd, &frame, sizeof(Frame), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        
        addr_len = sizeof(servaddr);
        n = recvfrom(sockfd, &frame, sizeof(Frame), 0, (struct sockaddr *)&servaddr, &addr_len);
        
        if (n < 0) 
            printf("!! TIMEOUT !! Retransmitting...\n");
        else if (frame.ack_no == seq) {
            printf("ACK %d received. Move to next packet.\n", seq);
            seq = 1 - seq;
            i++;
        }
        sleep(1);
    }
    close(sockfd);
    return 0;
}