#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

typedef struct { 
    int seq_no, ack_no; 
    char data[1024]; 
} Frame;

int main() {
    int sockfd, n, len, exp_seq = 0;
    struct sockaddr_in servaddr, cliaddr;
    Frame frame;
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
        { perror("Socket"); exit(EXIT_FAILURE); }
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
        { perror("Bind"); exit(EXIT_FAILURE); }
    
    printf("Receiver running on port %d...\n", PORT);

    while (1) {
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, &frame, sizeof(Frame), 0, (struct sockaddr *)&cliaddr, &len);
        
        if (rand() % 10 < 3) {
            printf("[Packet Loss] Dropped Frame %d\n", frame.seq_no);
            continue;
        }

        if (n > 0) {
            printf("Received Frame: %d | Data: %s\n", frame.seq_no, frame.data);
            
            if (frame.seq_no == exp_seq) {
                printf("  -> Frame accepted. Sending ACK %d\n", exp_seq);
                frame.ack_no = exp_seq;
                frame.seq_no = 0;
                exp_seq = 1 - exp_seq;
            } else {
                printf("  -> Duplicate! Resending ACK %d\n", frame.seq_no);
                frame.ack_no = frame.seq_no;
            }
            sendto(sockfd, &frame, sizeof(Frame), 0, (struct sockaddr *)&cliaddr, len);
        }
    }
    close(sockfd);
    return 0;
}